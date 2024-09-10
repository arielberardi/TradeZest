#pragma once

#include <string>
#include <boost/json.hpp>

enum class OrderType
{
    Limit = 0,
    Market,
    StopLimit,
    StopLoss
};

enum class OrderSide
{
    Sell = 0,
    Buy,
};

class BrokerInterface
{
public:
    virtual double getAccountBalance() const = 0;
    virtual boost::json::object getAccountHoldings() const = 0;

    virtual double getAssetPrice(const std::string& symbol) const = 0;
    virtual boost::json::object getTradingPair(const std::string& symbol) const = 0;

    virtual boost::json::object getCurrentOrders() const = 0;
    virtual std::string placeOrder(const std::string& symbol, OrderSide side, OrderType type, long double quantity) = 0;
    virtual bool cancelOrder(const std::string& orderId) = 0;
};