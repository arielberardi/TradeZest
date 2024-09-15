#include "pch.h"
#include "../TradeZest/Source/HTTPRequest.hpp"
#include "../TradeZest/Source/Broker.hpp"
#include "../TradeZest/Source/Secrets.hpp"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(BrokerTest, GetAccountId)
{
    Broker broker{ API_KEY };
    std::string knowId{ ACCOUNT_ID };

    EXPECT_TRUE(broker.GetAccountId() == knowId);
}

TEST(HTTPRequestTest, MakeRequest)
{
    const boost::urls::url api("https://echo.free.beeceptor.com/sample-request?author=Pedro&juan=Pedro");
    const std::unordered_map<std::string, std::string> headers{ { "x-api-key", "Bearer mytoken" } };
    const boost::json::object body{ { "message", "Hello world" } };
    boost::json::object response{};

    auto status = HTTPRequest::Instance().Make(
        api,
        boost::beast::http::verb::post,
        headers,
        body,
        response
    );

    EXPECT_TRUE(status == boost::beast::http::status::ok);
    EXPECT_TRUE(response["method"].as_string() == "POST");
    EXPECT_TRUE(response["headers"].as_object()["X-Api-Key"] == "Bearer mytoken");
    EXPECT_TRUE(response["parsedBody"].as_object()["message"] == "Hello world");
}
