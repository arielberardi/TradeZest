#pragma once

#include <SDKDDKVer.h>
#include <cstdlib>
#include <unordered_map>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/url/url.hpp>
#include <boost/json.hpp>

class HTTPRequest
{
public:
    static HTTPRequest& Instance() noexcept {
        static HTTPRequest instance;
        return instance;
    }

    HTTPRequest(const HTTPRequest&) = delete;
    HTTPRequest& operator=(const HTTPRequest&) = delete;

    HTTPRequest(const HTTPRequest&&) = delete;
    HTTPRequest& operator=(const HTTPRequest&&) = delete;

    boost::beast::http::status make(
        const boost::urls::url& url,
        const boost::beast::http::verb method,
        const std::unordered_map<std::string, std::string>& headers, 
        const boost::json::object& body, 
        boost::json::object& jsonResponse);
private:
    HTTPRequest();

    auto connect(const boost::urls::url& endpoint);
    void disconnect();
       
    boost::asio::io_context m_IOContext;
    boost::asio::ssl::context m_SSLContext;
    boost::asio::ip::tcp::resolver m_Resolver;
    boost::asio::ssl::stream<boost::beast::tcp_stream> m_Stream;
};

// Need to create few test for this singleton