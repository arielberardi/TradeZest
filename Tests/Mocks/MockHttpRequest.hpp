#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/json.hpp>
#include <boost/url/url.hpp>
#include <unordered_map>

#include <gmock/gmock.h>

class MockHttpRequest
{
  public:
    MOCK_METHOD(boost::beast::http::status,
                Make,
                (const boost::urls::url& url,
                 const boost::beast::http::verb method,
                 (const std::unordered_map<std::string, std::string>& headers),
                 const boost::json::object& body,
                 boost::json::object& jsonResponse));
};
