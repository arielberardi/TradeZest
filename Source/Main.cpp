#include <iostream>
#include <string>

#include "HTTPRequest.hpp"
#include "OandaApi.hpp"
#include "Secrets.hpp"

int main()
{
    HTTPRequest httpClient{};

    OandaApi<HTTPRequest> broker{API_KEY, API_ENDPOINT, httpClient};
    std::string accountId = broker.GetAccountId();

    std::cout << accountId << std::endl;
    std::cout << broker.GetAccountDetails(accountId) << std::endl;
}
