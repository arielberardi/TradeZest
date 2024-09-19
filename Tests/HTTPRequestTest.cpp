#include <gtest/gtest.h>

#include "Source/HTTPRequest.hpp"

TEST(HTTPRequest, MakeAJsonRequest)
{
    const boost::urls::url api("https://echo.free.beeceptor.com/sample-request?author=Pedro&juan=Pedro");
    const std::unordered_map<std::string, std::string> headers{{"x-api-key", "Bearer mytoken"}};
    const boost::json::object body{};
    boost::json::object response{};

    HTTPRequest httpRequest{};
    auto status = httpRequest.Make(api, boost::beast::http::verb::post, headers, body, response);

    EXPECT_EQ(status, boost::beast::http::status::ok);
}
