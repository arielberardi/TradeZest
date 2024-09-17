#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Source/Broker.hpp"
#include "Tests/Mocks/MockHttpRequest.hpp"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;

TEST(BrokerTest, BasicAssertions)
{
    constexpr char API_KEY[] = "SOMEKEY";
    MockHttpRequest mockHttpRequest{};

    boost::json::object response = {{"accounts", boost::json::array({boost::json::object({{"id", "SAMPLE-ID"}})})}};

    EXPECT_CALL(mockHttpRequest, Make)
        .WillOnce(DoAll(SetArgReferee<4>(response), Return(boost::beast::http::status::ok)));

    Broker<MockHttpRequest> broker{API_KEY, mockHttpRequest};

    std::cout << broker.GetAccountId() << std::endl;

    EXPECT_EQ(broker.GetAccountId(), "SAMPLE-ID");
}
