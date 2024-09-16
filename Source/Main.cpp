#include <iostream>
#include <string>

#include "Broker.hpp"
#include "HTTPRequest.hpp"

int main() {
  Broker broker{API_KEY};
  std::cout << broker.GetAccountId() << std::endl;
}
