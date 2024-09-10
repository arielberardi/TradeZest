#pragma once

#include <string>
#include <boost/json.hpp>

#include "BrokerInterface.hpp" 

class RobbinHood : public BrokerInterface
{
public:
    explicit RobbinHood(std::string& apiKey) : m_ApiKey(apiKey), m_Url("http://localhost:3001") {}

    double getAccountBalance() const override;
    boost::json::object getAccountHoldings() const override;

    double getAssetPrice(const std::string& symbol) const override;
    boost::json::object getTradingPair(const std::string& symbol) const override;

    boost::json::object getCurrentOrders() const override;
    std::string placeOrder(const std::string& symbol, OrderSide side, OrderType type, long double quantity) override;
    bool cancelOrder(const std::string& orderId) override;

private:
    std::string m_ApiKey{};
    boost::urls::url m_Url{};
};