#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/json.hpp>
#include <boost/url/url.hpp>

class HTTPRequest
{
  public:
    HTTPRequest();

    boost::beast::http::status Make(const boost::urls::url& url,
                                    const boost::beast::http::verb method,
                                    const std::unordered_map<std::string, std::string>& headers,
                                    const boost::json::object& body,
                                    boost::json::object& jsonResponse);

  private:
    auto Connect(boost::asio::ssl::stream<boost::beast::tcp_stream>& stream, const boost::urls::url& endpoint);
    void Disconnect(boost::asio::ssl::stream<boost::beast::tcp_stream>& stream);

    boost::asio::io_context m_IOContext;
    boost::asio::ssl::context m_SSLContext;
    boost::asio::ip::tcp::resolver m_Resolver;
};
