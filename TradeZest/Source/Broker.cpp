#include <iostream>

#include "Broker.hpp"

namespace http = boost::beast::http;

Broker::Broker(const std::string& api_key)
{
    m_Headers.insert({ "Authorization", "Bearer " + api_key });
    m_Headers.insert({ "Content-Type", "application/json" });

    GetAccountId(); // Not the best place to init this
}

std::string Broker::FetchAccountId() const
{
    boost::json::object response{};
    const boost::json::object body{};

    http::status result = HTTPRequest::Instance().Make(
        boost::urls::url{ m_Endpoint + "/accounts" },
        http::verb::get,
        m_Headers,
        body,
        response
    );

    if (result != http::status::ok)
    {
        return "";
    }

    boost::json::object account = response["accounts"].as_array().at(0).as_object();
    return account["id"].get_string().c_str();
}

std::string Broker::OrderToString(const OrderType order) const noexcept
{
    return m_OrderNames.at(static_cast<size_t>(order));
}

std::string Broker::GranularityToString(const CandleGranularity granularity) const noexcept
{
    return m_CandleGranularityNames.at(static_cast<size_t>(granularity));
}

boost::json::object Broker::GenerateBodyRequest(const OrderRequest& order) const noexcept
{
    boost::json::object orderBody{};

    orderBody["type"] = OrderToString(order.type);
    orderBody["instrument"] = order.instrument;
    orderBody["units"] = order.units;
    orderBody["positionFill"] = "DEFAULT";
    orderBody["timeInForce"] = "FOK";

    if (order.type != OrderType::Market)
    {
        orderBody["priceBound"] = order.price;
        orderBody["timeInForce"] = "GTC";
    }

    if (!order.takeProfitPrice.empty())
    {
        orderBody["takeProfitOnFill"] = { "price", order.takeProfitPrice };
    }

    if (!order.stopLossPrice.empty())
    {
        orderBody["stopLossOnFill"] = { "price", order.stopLossPrice };
    }

    if (order.type == OrderType::TakeProfit || order.type == OrderType::Stop)
    {
        orderBody["tradeID"] = order.tradeID;
    }

    return orderBody;
}

std::string Broker::GetAccountId()
{
    if (m_AccountId.empty())
    {
        m_AccountId = FetchAccountId();
        m_EndpointWithID = m_Endpoint + "/accounts/" + m_AccountId;
    }

    return m_AccountId;
}

boost::json::object Broker::GetAccountDetails() const
{
    boost::json::object response{};
    const boost::json::object body{};

    http::status result = HTTPRequest::Instance().Make(
        boost::urls::url{ m_EndpointWithID },
        http::verb::get,
        m_Headers,
        body,
        response
    );

    if (result != http::status::ok)
    {
        return {};
    }

    return response["account"].as_object();
}

boost::json::array Broker::GetOrders() const
{
    boost::json::object response{};
    const boost::json::object body{};

    http::status result = HTTPRequest::Instance().Make(
        boost::urls::url{ m_EndpointWithID + "/orders" },
        http::verb::get,
        m_Headers,
        body,
        response
    );

    if (result != http::status::ok)
    {
        return  boost::json::array{};
    }

    return response["orders"].as_array();
}

boost::json::array Broker::GetPendingOrders() const
{
    boost::json::object response{};
    const boost::json::object body{};

    http::status result = HTTPRequest::Instance().Make(
        boost::urls::url{ m_EndpointWithID + "/pendingOrders" },
        http::verb::get,
        m_Headers,
        body,
        response
    );

    if (result != http::status::ok)
    {
        return boost::json::array{};
    }

    return response["orders"].as_array();
}

boost::json::object Broker::GetOrder(const std::string& orderId) const
{
    boost::json::object response{};
    const boost::json::object body{};

    http::status result = HTTPRequest::Instance().Make(
        boost::urls::url{ m_EndpointWithID + "/orders/" + orderId },
        http::verb::get,
        m_Headers,
        body,
        response
    );

    if (result != http::status::ok)
    {
        return {};
    }

    return response["order"].as_object();
}

std::string Broker::CreateOrder(const OrderRequest& order) const
{
    boost::json::object body{};
    body["order"] = GenerateBodyRequest(order);

    boost::json::object response{};
    http::status result = HTTPRequest::Instance().Make(
        boost::urls::url{ m_EndpointWithID + "/orders" },
        http::verb::post,
        m_Headers,
        body,
        response
    );

    if (result != http::status::created)
    {
        return "-1";
    }

    auto& transaction = response["orderCreateTransaction"].as_object();
    return transaction["id"].get_string().c_str();
}

bool Broker::CancelOrder(const std::string& orderId) const
{
    boost::json::object response{};
    const boost::json::object body{};

    http::status result = HTTPRequest::Instance().Make(
        boost::urls::url{ m_EndpointWithID + "/orders/" + orderId + "/cancel"},
        http::verb::put,
        m_Headers,
        body,
        response
    );

    if (result != http::status::ok)
    {
        return false;
    }

    return true;
}

boost::json::object Broker::GetPosition(const std::string& instrument) const
{
    boost::json::object response{};
    const boost::json::object body{};

    http::status result = HTTPRequest::Instance().Make(
        boost::urls::url{ m_EndpointWithID + "/positions/" + instrument },
        http::verb::get,
        m_Headers,
        body,
        response
    );

    if (result != http::status::ok)
    {
        return {};
    }

    return response["position"].as_object();
}

boost::json::array Broker::GetOpenPositions() const
{
    boost::json::object response{};
    const boost::json::object body{};

    http::status result = HTTPRequest::Instance().Make(
        boost::urls::url{ m_EndpointWithID + "/openPositions" },
        http::verb::get,
        m_Headers,
        body,
        response
    );

    if (result != http::status::ok)
    {
        return boost::json::array{};
    }

    return response["positions"].as_array();
}

bool Broker::ClosePosition(const std::string& instrument) const
{
    boost::json::object response{};
    const boost::json::object body{};

    http::status result = HTTPRequest::Instance().Make(
        boost::urls::url{ m_EndpointWithID + "/positions/" + instrument + "/close" },
        http::verb::put,
        m_Headers,
        body,
        response
    );

    if (result != http::status::ok)
    {
        return false;
    }

    return true;
}

boost::json::object Broker::GetPricing(const std::string& instrument) const
{
    boost::json::object response{};
    const boost::json::object body{};

    boost::urls::url url{ m_EndpointWithID + "/pricing" };
    url.params().set("instruments", instrument);

    http::status result = HTTPRequest::Instance().Make(
        url,
        http::verb::get,
        m_Headers,
        body,
        response
    );

    if (result != http::status::ok)
    {
        return {};
    }

    return response;
}

boost::json::array Broker::GetCandles(
    const std::string& instrument,
    CandleGranularity granularity,
    int count,
    std::string from,
    std::string to) const
{
    boost::urls::url url{ m_EndpointWithID + "/instruments/" + instrument + "/candles" };
    url.params().set("granularity", GranularityToString(granularity));
    url.params().set("count", std::to_string(count));
 
    if (!from.empty())
    {
        url.params().set("from", from);
    }

    if (!to.empty())
    {
        url.params().set("to", to);
    }

    boost::json::object response{};
    const boost::json::object body{};

    http::status result = HTTPRequest::Instance().Make(
        url,
        http::verb::get,
        m_Headers,
        body,
        response
    );

    if (result != http::status::ok)
    {
        return boost::json::array{};
    }

    return response["candles"].as_array();
}
