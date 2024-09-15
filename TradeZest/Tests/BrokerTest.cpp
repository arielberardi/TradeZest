#include <boost/test/unit_test.hpp>
#include "../Source/HTTPRequest.hpp"
#include "../Source/Broker.hpp"
#include "../Source/Secrets.hpp"

BOOST_AUTO_TEST_CASE(Broker_GetAccountId)
{
    Broker broker{ API_KEY };
    std::string knowId{ ACCOUNT_ID };

    BOOST_ASSERT(broker.GetAccountId() == knowId);
};

BOOST_AUTO_TEST_CASE(Broker_GetAccountDetails)
{
    Broker broker{ API_KEY };

    boost::json::object response = broker.GetAccountDetails();

    BOOST_ASSERT(response.empty() == false);
    BOOST_ASSERT(response["id"] == ACCOUNT_ID);
};

