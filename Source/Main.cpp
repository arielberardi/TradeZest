#include <iostream>
#include <string>

#include <boost/json.hpp>

#include "ConfigManager.hpp"
#include "HTTPRequest.hpp"
#include "OandaApi.hpp"

int main()
{
    // Load configuration
    ConfigManager::Instance().Load("config.txt");
    std::string apiKey = ConfigManager::Instance().Get("API_KEY", "defaultKey");
    std::string apiEndpoint = ConfigManager::Instance().Get("API_ENDPOINT", "defaultEndpoint");

    // Setup Broker API
    HTTPRequest httpClient{};
    OandaApi<HTTPRequest> broker{apiKey, apiEndpoint, httpClient};
    std::string accountId{broker.GetAccountId()};

    // Create and Limit Order with StopLoss and TakeProfit
    boost::json::object pricing = broker.GetPricing(accountId, "EUR_USD");
    double currentAsk = std::atof(pricing["asks"].as_array().at(0).as_object()["price"].as_string().c_str());
    double currentBid = std::atof(pricing["bids"].as_array().at(0).as_object()["price"].as_string().c_str());

    std::cout << std::to_string(currentAsk + 0.5) << std::endl;
    std::cout << std::to_string(currentBid - 0.5) << std::endl;

    OrderRequest order{
        .type{OrderType::Limit},                            //
        .instrument{"EUR_USD"},                             //
        .units{"100"},                                      //
        .price{std::to_string(currentAsk)},                 //
        .takeProfitPrice{std::to_string(currentAsk + 0.5)}, //
        .stopLossPrice{std::to_string(currentBid - 0.5)},   //
    };

    std::string orderId = broker.CreateOrder(accountId, order);

    if (orderId.empty())
    {
        std::cerr << "Error while creating the order" << std::endl;
        return -1;
    }

    std::cout << "Order created: " << orderId << std::endl;
}
