#pragma once

#include <string>
#include <unordered_map>

#include <boost/url.hpp>
#include <boost/json.hpp>

#include "HTTPRequest.hpp"
#include "Secrets.hpp"

enum class OrderType
{
    Market,
    Limit,
    TakeProfit,
    Stop
};

enum class CandleGranularity
{
    S5,
    S10,
    S15,
    S30,
    M1,
    M2,
    M4,
    M5,
    M10,
    M15,
    M30,
    H1,
    H2,
    H3,
    H4,
    H6,
    H8,
    H12,
    D,
    W,
    A
};

struct OrderRequest
{
    OrderType type;
    std::string instrument;
    std::string units;
    std::string price;
    std::string takeProfitPrice;
    std::string stopLossPrice;
    std::string tradeID;
};

class Broker
{
public:
    Broker(const std::string& api_key);

    std::string GetAccountId();
    boost::json::object GetAccountDetails() const;
    
    boost::json::array GetOrders() const;
    boost::json::array GetPendingOrders() const;
    boost::json::object GetOrder(const std::string& orderId) const;
    std::string CreateOrder(const OrderRequest& order) const;
    bool CancelOrder(const std::string& orderId) const;
    
    boost::json::object GetPosition(const std::string& instrument) const;
    boost::json::array GetOpenPositions() const;
    bool ClosePosition(const std::string& instrument) const;

    boost::json::object GetPricing(const std::string& instrument) const;
    boost::json::array GetCandles(const std::string& instrument, 
                                  CandleGranularity granularity,
                                  int count = 500,
                                  std::string from = "",
                                  std::string to = "") const;
private:    
    std::unordered_map<std::string, std::string> m_Headers{};
    std::string m_Endpoint{ API_ENDPOINT };
    std::string m_AccountId{};
    std::string m_EndpointWithID{};

    const std::array<std::string, 4> m_OrderNames{
        "MARKET", "LIMIT", "TAKE_PROFIT", "STOP"
    };

    const std::array<std::string, 21> m_CandleGranularityNames{
        "S5", "S10", "S15", "S30", "M1", "M2", "M4", "M5", "M10", "M15", "M30",
        "H1", "H2", "H3", "H4", "H6", "H8", "H12", "D", "W", "A"
    };

    std::string FetchAccountId() const;
    std::string OrderToString(const OrderType order) const noexcept;
    std::string GranularityToString(const CandleGranularity granularity) const noexcept;
    boost::json::object GenerateBodyRequest(const OrderRequest& order) const noexcept;
};
