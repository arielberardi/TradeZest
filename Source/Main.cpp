#include <iostream>
#include <string>

#include "ConfigManager.hpp"
#include "HTTPRequest.hpp"
#include "OandaApi.hpp"

int main()
{
    ConfigManager::Instance().Load("config.txt");

    std::string apiKey = ConfigManager::Instance().Get("API_KEY", "default");
    std::string apiEndpoint = ConfigManager::Instance().Get("API_ENDPOINT", "defaultEndpoint");

    HTTPRequest httpClient{};
    OandaApi<HTTPRequest> broker{apiKey, apiEndpoint, httpClient};
    std::string accountId{broker.GetAccountId()};

    std::cout << accountId << std::endl;
    std::cout << broker.GetAccountDetails(accountId) << std::endl;
}
