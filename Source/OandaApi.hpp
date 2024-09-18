#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/json.hpp>
#include <boost/url/url.hpp>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>

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

template <typename HttpClient> class OandaApi
{
  public:
    OandaApi(const std::string& api_key, const std::string& endpoint, HttpClient& httpClient)
        : m_HttpClient(httpClient), m_Endpoint(endpoint)
    {
        m_Headers.insert({"Authorization", "Bearer " + api_key});
        m_Headers.insert({"Content-Type", "application/json"});
    }

    std::string GetAccountId()
    {
        const boost::urls::url url{m_Endpoint + "/accounts"};
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(url, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return "";
        }

        boost::json::object account = response["accounts"].as_array().at(0).as_object();
        return account["id"].get_string().c_str();
    }

    boost::json::object GetAccountDetails(const std::string& accountId) const
    {
        const boost::urls::url url{GetAccountEndpoint(accountId)};
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(url, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return boost::json::object{};
        }

        return response["account"].as_object();
    }

    boost::json::array GetOrders(const std::string& accountId) const
    {
        const boost::urls::url url{GetAccountEndpoint(accountId) + "/orders"};
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(url, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return boost::json::array{};
        }

        return response["orders"].as_array();
    }

    boost::json::array GetPendingOrders(const std::string& accountId) const
    {
        const boost::urls::url url{GetAccountEndpoint(accountId) + "/pendingOrders"};
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(url, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return boost::json::array{};
        }

        return response["orders"].as_array();
    }

    boost::json::object GetOrder(const std::string& accountId, const std::string& orderId) const
    {
        const boost::urls::url url{GetAccountEndpoint(accountId) + "/orders/" + orderId};
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(url, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return boost::json::object{};
        }

        return response["order"].as_object();
    }

    std::string CreateOrder(const std::string& accountId, const OrderRequest& order) const
    {
        boost::json::object body{};
        body["order"] = GenerateBodyRequest(order);

        const boost::urls::url url{GetAccountEndpoint(accountId) + "/orders"};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(url, boost::beast::http::verb::post, m_Headers, body, response);

        if (result != boost::beast::http::status::created)
        {
            return "";
        }

        auto& transaction = response["orderCreateTransaction"].as_object();
        return transaction["id"].get_string().c_str();
    }

    bool CancelOrder(const std::string& accountId, const std::string& orderId) const
    {
        const boost::urls::url url{GetAccountEndpoint(accountId) + "/orders/" + orderId + "/cancel"};
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(url, boost::beast::http::verb::put, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return false;
        }

        return true;
    }

    boost::json::object GetPosition(const std::string& accountId, const std::string& instrument) const
    {
        const boost::urls::url url{GetAccountEndpoint(accountId) + "/positions/" + instrument};
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(url, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return boost::json::object{};
        }

        return response["position"].as_object();
    }

    boost::json::array GetOpenPositions(const std::string& accountId) const
    {
        const boost::urls::url url{GetAccountEndpoint(accountId) + "/openPositions"};
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(url, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return boost::json::array{};
        }

        return response["positions"].as_array();
    }

    bool ClosePosition(const std::string& accountId, const std::string& instrument) const
    {
        const boost::urls::url url{GetAccountEndpoint(accountId) + "/positions/" + instrument + "/close"};
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(url, boost::beast::http::verb::put, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return false;
        }

        return true;
    }

    boost::json::object GetPricing(const std::string& accountId, const std::string& instrument) const
    {
        boost::json::object response{};
        const boost::json::object body{};

        boost::urls::url url{GetAccountEndpoint(accountId) + "/pricing"};
        url.params().set("instruments", instrument);

        boost::beast::http::status result =
            m_HttpClient.Make(url, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return boost::json::object{};
        }

        return response;
    }

    boost::json::array GetCandles(const std::string& accountId,
                                  const std::string& instrument,
                                  CandleGranularity granularity = CandleGranularity::S5,
                                  int count = 500,
                                  std::string from = "",
                                  std::string to = "") const
    {
        boost::urls::url url{GetAccountEndpoint(accountId) + "/instruments/" + instrument + "/candles"};
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

        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(url, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return boost::json::array{};
        }

        return response["candles"].as_array();
    }

  private:
    std::unordered_map<std::string, std::string> m_Headers{};
    std::string m_Endpoint{};

    HttpClient& m_HttpClient;
    const std::array<std::string, 4> m_OrderNames{"MARKET", "LIMIT", "TAKE_PROFIT", "STOP"};
    const std::array<std::string, 21> m_CandleGranularityNames{"S5", "S10", "S15", "S30", "M1", "M2", "M4",
                                                               "M5", "M10", "M15", "M30", "H1", "H2", "H3",
                                                               "H4", "H6",  "H8",  "H12", "D",  "W",  "A"};

    constexpr std::string OrderToString(const OrderType order) const noexcept
    {
        return m_OrderNames.at(static_cast<size_t>(order));
    }

    constexpr std::string GranularityToString(const CandleGranularity granularity) const noexcept
    {
        return m_CandleGranularityNames.at(static_cast<size_t>(granularity));
    }

    std::string GetAccountEndpoint(const std::string& id) const noexcept
    {
        return m_Endpoint + "/accounts/" + id;
    }

    boost::json::object GenerateBodyRequest(const OrderRequest& order) const noexcept
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
            orderBody["takeProfitOnFill"] = {"price", order.takeProfitPrice};
        }

        if (!order.stopLossPrice.empty())
        {
            orderBody["stopLossOnFill"] = {"price", order.stopLossPrice};
        }

        if (order.type == OrderType::TakeProfit || order.type == OrderType::Stop)
        {
            orderBody["tradeID"] = order.tradeID;
        }

        return orderBody;
    }
};
