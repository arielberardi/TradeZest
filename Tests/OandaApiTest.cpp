#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Source/OandaApi.hpp"
#include "Tests/Mocks/MockHttpRequest.hpp"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;

static constexpr char API_KEY[] = "SOMEKEY";
static constexpr char ACCOUNT_ID[] = "1011-13251-11";
static constexpr char API_ENDPOINT[] = "http://myendpoint.sample/v3";
static constexpr char API_ENDPOINT_ID[] = "http://myendpoint.sample/v3/accounts/1011-13251-11";
static constexpr char ORDER_ID[] = "6375";
static constexpr char INSTRUMENT[] = "INSTRUMENT";

TEST(OandaApiTest, GetAccountIdSuccess)
{
    boost::json::object response = {
        {
            "accounts",
            boost::json::array({
                boost::json::object({
                    {"id", ACCOUNT_ID} //
                })                     //
            })                         //
        } //
    };

    boost::urls::url url{std::string(API_ENDPOINT) + "/accounts"};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(SetArgReferee<4>(response), Return(boost::beast::http::status::ok)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    EXPECT_EQ(broker.GetAccountId(), ACCOUNT_ID);
}

TEST(OandaApiTest, GetAccountIdFailure)
{
    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make).WillOnce(Return(boost::beast::http::status::bad_request));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    EXPECT_TRUE(broker.GetAccountId().empty());
}

TEST(OandaApiTest, GetAccountDetailsSuccess)
{
    boost::json::object response{
        {
            "account",
            boost::json::object{
                {"balance", "1200.0"},
                {"id", ACCOUNT_ID},
                {"alias", "FAKE ALIAS"} //
            } //
        } //
    };

    boost::urls::url url{std::string(API_ENDPOINT_ID)};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(SetArgReferee<4>(response), Return(boost::beast::http::status::ok)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::object expectedResponse{
        {"balance", "1200.0"},
        {"id", ACCOUNT_ID},
        {"alias", "FAKE ALIAS"} //
    };

    EXPECT_EQ(broker.GetAccountDetails(ACCOUNT_ID), expectedResponse);
}

TEST(OandaApiTest, GetAccountDetailsFailure)
{
    boost::urls::url url{std::string(API_ENDPOINT_ID)};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(Return(boost::beast::http::status::bad_request)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::object expectedResponse{};
    EXPECT_EQ(broker.GetAccountDetails(ACCOUNT_ID), expectedResponse);
}

TEST(OandaApiTest, GetOrdersSuccess)
{
    boost::json::object response{
        {"lastTransactionID", "6375"},
        {"orders",
         boost::json::array{
             boost::json::object{
                 {"createTime", "2016-06-22T18:41:29.294265338Z"},
                 {"id", "6375"} //
             } //
         }} //
    };

    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/orders"};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(SetArgReferee<4>(response), Return(boost::beast::http::status::ok)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::array expectedResponse{
        boost::json::object{
            {"createTime", "2016-06-22T18:41:29.294265338Z"},
            {"id", "6375"} //
        } //
    };
    EXPECT_EQ(broker.GetOrders(ACCOUNT_ID), expectedResponse);
}

TEST(OandaApiTest, GetOrdersFailure)
{
    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/orders"};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(Return(boost::beast::http::status::bad_request)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::array expectedResponse{};
    EXPECT_EQ(broker.GetOrders(ACCOUNT_ID), expectedResponse);
}

TEST(OandaApiTest, GetPendingOrdersSuccess)
{
    boost::json::object response{
        {"lastTransactionID", "6375"},
        {"orders",
         boost::json::array{
             boost::json::object{
                 {"createTime", "2016-06-22T18:41:29.294265338Z"},
                 {"id", "6375"} //
             } //
         }} //
    };

    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/pendingOrders"};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(SetArgReferee<4>(response), Return(boost::beast::http::status::ok)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::array expectedResponse{
        boost::json::object{
            {"createTime", "2016-06-22T18:41:29.294265338Z"},
            {"id", "6375"} //
        } //
    };
    EXPECT_EQ(broker.GetPendingOrders(ACCOUNT_ID), expectedResponse);
}

TEST(OandaApiTest, GetPendingOrdersFailure)
{
    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/pendingOrders"};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(Return(boost::beast::http::status::bad_request)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::array expectedResponse{};
    EXPECT_EQ(broker.GetPendingOrders(ACCOUNT_ID), expectedResponse);
}

TEST(OandaApiTest, GetOrderSuccess)
{
    boost::json::object response{
        {"lastTransactionID", "6375"},
        {
            "order",
            boost::json::object{
                {"createTime", "2016-06-22T18:41:29.294265338Z"},
                {"id", "6375"} //
            } //
        } //
    };

    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/orders/" + ORDER_ID};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(SetArgReferee<4>(response), Return(boost::beast::http::status::ok)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::object expectedResponse{
        {"createTime", "2016-06-22T18:41:29.294265338Z"},
        {"id", "6375"} //
    };

    EXPECT_EQ(broker.GetOrder(ACCOUNT_ID, ORDER_ID), expectedResponse);
}

TEST(OandaApiTest, GetOrderFailure)
{
    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/orders/" + ORDER_ID};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(Return(boost::beast::http::status::bad_request)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::object expectedResponse{};
    EXPECT_EQ(broker.GetOrder(ACCOUNT_ID, ORDER_ID), expectedResponse);
}

TEST(OandaApiTest, CreateOrderSuccess)
{
    OrderRequest order{
        .type{OrderType::Market},
        .instrument{"EUR_USD"},
        .units{"100"} //
    };

    boost::json::object response{
        {"lastTransactionID", "6374"},
        {
            "orderCreateTransaction",
            boost::json::object{
                {"time", "2016-06-22T18:41:29.294265338Z"},
                {"id", ORDER_ID} //
            } //
        } //
    };

    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/orders"};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::post, _, _, _))
        .WillOnce(DoAll(SetArgReferee<4>(response), Return(boost::beast::http::status::created)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    EXPECT_EQ(broker.CreateOrder(ACCOUNT_ID, order), ORDER_ID);
}

TEST(OandaApiTest, CreateOrderFailure)
{
    OrderRequest order{
        .type{OrderType::Market},
        .instrument{"EUR_USD"},
        .units{"100"} //
    };

    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/orders"};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::post, _, _, _))
        .WillOnce(DoAll(Return(boost::beast::http::status::bad_request)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    EXPECT_TRUE(broker.CreateOrder(ACCOUNT_ID, order).empty());
}

TEST(OandaApiTest, CancelOrderSuccess)
{
    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/orders/" + std::string(ORDER_ID) + "/cancel"};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::put, _, _, _))
        .WillOnce(DoAll(Return(boost::beast::http::status::ok)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    EXPECT_TRUE(broker.CancelOrder(ACCOUNT_ID, ORDER_ID));
}

TEST(OandaApiTest, CancelOrderFailure)
{
    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/orders/" + std::string(ORDER_ID) + "/cancel"};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::put, _, _, _))
        .WillOnce(DoAll(Return(boost::beast::http::status::bad_request)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    EXPECT_FALSE(broker.CancelOrder(ACCOUNT_ID, ORDER_ID));
}

TEST(OandaApiTest, GetPositionSuccess)
{
    boost::json::object response{
        {"lastTransactionID", "6375"},
        {
            "position",
            boost::json::object{
                {"instrument", INSTRUMENT},
                {"p1", "-54200"} //
            } //
        } //
    };

    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/positions/" + std::string(INSTRUMENT)};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(SetArgReferee<4>(response), Return(boost::beast::http::status::ok)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::object expectedResponse{
        {"instrument", INSTRUMENT},
        {"p1", "-54200"} //
    };

    EXPECT_EQ(broker.GetPosition(ACCOUNT_ID, INSTRUMENT), expectedResponse);
}

TEST(OandaApiTest, GetPositionFailure)
{
    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/positions/" + std::string(INSTRUMENT)};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(Return(boost::beast::http::status::bad_request)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::object expectedResponse{};
    EXPECT_EQ(broker.GetPosition(ACCOUNT_ID, INSTRUMENT), expectedResponse);
}

TEST(OandaApiTest, GetOpenPositionsSuccess)
{
    boost::json::object response{
        {"lastTransactionID", "6375"},
        {
            "positions",
            boost::json::array{
                boost::json::object{
                    {"instrument", INSTRUMENT},
                    {"p1", "-54200"} //
                } //
            } //
        } //
    };

    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/openPositions"};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(SetArgReferee<4>(response), Return(boost::beast::http::status::ok)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::array expectedResponse{
        boost::json::object{
            {"instrument", INSTRUMENT},
            {"p1", "-54200"} //
        } //
    };

    EXPECT_EQ(broker.GetOpenPositions(ACCOUNT_ID), expectedResponse);
}

TEST(OandaApiTest, GetOpenPositionsFailure)
{
    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/openPositions"};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(Return(boost::beast::http::status::bad_request)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::array expectedResponse{};
    EXPECT_EQ(broker.GetOpenPositions(ACCOUNT_ID), expectedResponse);
}

TEST(OandaApiTest, ClosePositionSuccess)
{
    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/positions/" + std::string(INSTRUMENT) + "/close"};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::put, _, _, _))
        .WillOnce(DoAll(Return(boost::beast::http::status::ok)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    EXPECT_TRUE(broker.ClosePosition(ACCOUNT_ID, INSTRUMENT));
}

TEST(OandaApiTest, ClosePositionFailure)
{
    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/positions/" + std::string(INSTRUMENT) + "/close"};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::put, _, _, _))
        .WillOnce(DoAll(Return(boost::beast::http::status::bad_request)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    EXPECT_FALSE(broker.ClosePosition(ACCOUNT_ID, INSTRUMENT));
}

TEST(OandaApiTest, GetPricingSuccess)
{
    boost::json::object response{
        {
            "prices",
            boost::json::array{
                boost::json::object{
                    {"instrument", INSTRUMENT},
                    {"status", "tradeable"} //
                } //
            } //
        } //
    };

    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/pricing?instruments=" + std::string(INSTRUMENT)};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(SetArgReferee<4>(response), Return(boost::beast::http::status::ok)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    EXPECT_EQ(broker.GetPricing(ACCOUNT_ID, INSTRUMENT), response);
}

TEST(OandaApiTest, GetPricingFailure)
{
    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/pricing?instruments=" + std::string(INSTRUMENT)};

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(Return(boost::beast::http::status::bad_request)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::object expectedResponse{};
    EXPECT_EQ(broker.GetPricing(ACCOUNT_ID, INSTRUMENT), expectedResponse);
}

TEST(OandaApiTest, GetCandleSuccess)
{
    boost::json::object response{
        {"instrument", INSTRUMENT},
        {"gramularity", "S5"},
        {
            "candles",
            boost::json::array{
                boost::json::object{
                    {"complete", true},
                    {"volume", 29} //
                } //
            } //
        } //
    };

    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/instruments/" + std::string(INSTRUMENT) + "/candles"};
    url.params().set("granularity", "S5");
    url.params().set("count", "500");

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(SetArgReferee<4>(response), Return(boost::beast::http::status::ok)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::array expectedReponse{
        boost::json::object{
            {"complete", true},
            {"volume", 29} //
        } //
    };

    EXPECT_EQ(broker.GetCandles(ACCOUNT_ID, INSTRUMENT), expectedReponse);
}

TEST(OandaApiTest, GetCandleSuccessWithCustomParams)
{
    boost::json::object response{
        {"instrument", INSTRUMENT},
        {"gramularity", "S5"},
        {
            "candles",
            boost::json::array{
                boost::json::object{
                    {"complete", true},
                    {"volume", 29} //
                } //
            } //
        } //
    };

    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/instruments/" + std::string(INSTRUMENT) + "/candles"};
    url.params().set("granularity", "S10");
    url.params().set("count", "100");
    url.params().set("from", "1000");
    url.params().set("to", "1001");

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(SetArgReferee<4>(response), Return(boost::beast::http::status::ok)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::array expectedReponse{
        boost::json::object{
            {"complete", true},
            {"volume", 29} //
        } //
    };

    auto result = broker.GetCandles(ACCOUNT_ID, INSTRUMENT, CandleGranularity::S10, 100, "1000", "1001");
    EXPECT_EQ(result, expectedReponse);
}

TEST(OandaApiTest, GetCandleSuccessWithJustFrom)
{
    boost::json::object response{
        {"instrument", INSTRUMENT},
        {"gramularity", "S5"},
        {
            "candles",
            boost::json::array{
                boost::json::object{
                    {"complete", true},
                    {"volume", 29} //
                } //
            } //
        } //
    };

    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/instruments/" + std::string(INSTRUMENT) + "/candles"};
    url.params().set("granularity", "S10");
    url.params().set("count", "100");
    url.params().set("from", "1000");

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(SetArgReferee<4>(response), Return(boost::beast::http::status::ok)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::array expectedReponse{
        boost::json::object{
            {"complete", true},
            {"volume", 29} //
        } //
    };

    auto result = broker.GetCandles(ACCOUNT_ID, INSTRUMENT, CandleGranularity::S10, 100, "1000");
    EXPECT_EQ(result, expectedReponse);
}

TEST(OandaApiTest, GetCandleSuccessWithJustTo)
{
    boost::json::object response{
        {"instrument", INSTRUMENT},
        {"gramularity", "S5"},
        {
            "candles",
            boost::json::array{
                boost::json::object{
                    {"complete", true},
                    {"volume", 29} //
                } //
            } //
        } //
    };

    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/instruments/" + std::string(INSTRUMENT) + "/candles"};
    url.params().set("granularity", "S10");
    url.params().set("count", "100");
    url.params().set("to", "1001");

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(SetArgReferee<4>(response), Return(boost::beast::http::status::ok)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::array expectedReponse{
        boost::json::object{
            {"complete", true},
            {"volume", 29} //
        } //
    };

    auto result = broker.GetCandles(ACCOUNT_ID, INSTRUMENT, CandleGranularity::S10, 100, "", "1001");
    EXPECT_EQ(result, expectedReponse);
}

TEST(OandaApiTest, GetCandlesFailure)
{
    boost::urls::url url{std::string(API_ENDPOINT_ID) + "/instruments/" + std::string(INSTRUMENT) + "/candles"};
    url.params().set("granularity", "S5");
    url.params().set("count", "500");

    MockHttpRequest mockHttpRequest{};
    EXPECT_CALL(mockHttpRequest, Make(url, boost::beast::http::verb::get, _, _, _))
        .WillOnce(DoAll(Return(boost::beast::http::status::bad_request)));

    OandaApi<MockHttpRequest> broker{API_KEY, API_ENDPOINT, mockHttpRequest};

    boost::json::array expectedResponse{};
    EXPECT_EQ(broker.GetCandles(ACCOUNT_ID, INSTRUMENT), expectedResponse);
}
