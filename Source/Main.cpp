#include <iostream>
#include <string>

#include "Broker.hpp"
#include "HTTPRequest.hpp"
#include "Secrets.hpp"

int main()
{
    HTTPRequest httpClient{};

    Broker<HTTPRequest> broker{API_KEY, httpClient};

    std::cout << broker.GetAccountId() << std::endl;
    std::cout << broker.GetAccountDetails() << std::endl;
}
