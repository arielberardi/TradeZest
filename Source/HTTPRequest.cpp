#include "HTTPRequest.hpp"

#include <iostream>

#include "Secrets.hpp"

namespace http = boost::beast::http;
namespace net = boost::asio;
namespace ssl = net::ssl;
namespace json = boost::json;
using url = boost::urls::url;
using tcp = net::ip::tcp;

HTTPRequest::HTTPRequest()
    : m_IOContext(), m_SSLContext(ssl::context::sslv23), m_Resolver(m_IOContext) {
  m_SSLContext.set_default_verify_paths();
  m_SSLContext.set_verify_mode(boost::asio::ssl::verify_peer);
  m_SSLContext.load_verify_file(CERT_PATH);
}

auto HTTPRequest::Connect(ssl::stream<boost::beast::tcp_stream>& stream, const url& endpoint) {
  // This check needs to happen otherwise OpenSSL doesn't know the domain
  if (!SSL_set_tlsext_host_name(stream.native_handle(), endpoint.host().c_str())) {
    boost::beast::error_code ec{static_cast<int>(::ERR_get_error()),
                                net::error::get_ssl_category()};
    throw boost::beast::system_error{ec};
  }

  std::string port = endpoint.port();
  if (endpoint.port().empty()) {
    port = endpoint.scheme() == "https" ? "443" : "80";
  }

  auto hostIP = m_Resolver.resolve(endpoint.host(), port);
  boost::beast::get_lowest_layer(stream).connect(hostIP);

  stream.handshake(ssl::stream_base::client);
}

void HTTPRequest::Disconnect(ssl::stream<boost::beast::tcp_stream>& stream) {
  boost::beast::error_code errorCode;
  boost::beast::get_lowest_layer(stream).socket().shutdown(tcp::socket::shutdown_both, errorCode);

  if (errorCode && errorCode != ssl::error::stream_truncated) {
    throw boost::beast::system_error{errorCode};
  }
}

http::status HTTPRequest::Make(const url& endpoint,
                               const http::verb method,
                               const std::unordered_map<std::string, std::string>& headers,
                               const json::object& body,
                               json::object& jsonResponse) {
  boost::asio::ssl::stream<boost::beast::tcp_stream> stream(m_IOContext, m_SSLContext);

  Connect(stream, endpoint);

  // Create request
  http::request<http::string_body> request{method, endpoint.encoded_target(), 11};

  request.set(http::field::host, endpoint.host());
  request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
  request.set(http::field::content_type, "application/json");

  for (auto& header : headers) {
    request.set(header.first, header.second);
  }

  if (!body.empty()) {
    request.body() = json::serialize(body);
    request.prepare_payload();
  }

  http::write(stream, request);

  // Read response
  boost::beast::flat_buffer buffer;
  http::response<http::string_body> http_response;
  http::read(stream, buffer, http_response);

  // We expect to talk just with JSON APIs so we return empty if it fails to process it
  try {
    boost::json::value json_value = boost::json::parse(http_response.body());
    jsonResponse = json_value.as_object();
  } catch (boost::system::system_error e) {
    jsonResponse = {};
  }

  Disconnect(stream);
  return http_response.result();
}
