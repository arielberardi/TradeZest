#include <boost/beast.hpp>
#include <boost/url.hpp>
#include <boost/json.hpp>

#include "RobbinHood.hpp"

double RobbinHood::getAccountBalance() const
{
}

boost::json::object RobbinHood::getAccountHoldings() const
{
}
    
double RobbinHood::getAssetPrice(const std::string& symbol) const
{
}

boost::json::object RobbinHood::getTradingPair(const std::string& symbol) const
{
}

boost::json::object RobbinHood::getCurrentOrders() const
{
}

std::string RobbinHood::placeOrder(const std::string& symbol, OrderSide side, OrderType type, long double quantity)
{
}

bool RobbinHood::cancelOrder(const std::string& orderId)
{
}