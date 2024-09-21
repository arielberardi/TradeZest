#pragma once

#include <cassert>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/json.hpp>
#include <boost/url/url.hpp>

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
    OandaApi(std::string_view api_key, std::string_view endpoint, HttpClient& httpClient)
        : m_HttpClient{httpClient}, m_Endpoint{endpoint}
    {
        std::string token = std::format("Bearer {}", api_key);
        m_Headers.insert({"Authorization", token});
        m_Headers.insert({"Content-Type", "application/json"});
    }

    std::string GetAccountId()
    {
        const std::string url = std::format("{}/accounts", m_Endpoint);
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(boost::urls::url{url}, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return "";
        }

        boost::json::object account = response["accounts"].as_array().at(0).as_object();
        return account["id"].get_string().c_str();
    }

    boost::json::object GetAccountDetails(std::string_view accountId) const
    {
        const std::string url = std::format("{}/accounts/{}", m_Endpoint, accountId);
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(boost::urls::url{url}, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return boost::json::object{};
        }

        return response["account"].as_object();
    }

    boost::json::array GetOrders(std::string_view accountId) const
    {
        const std::string url = std::format("{}/accounts/{}/orders", m_Endpoint, accountId);
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(boost::urls::url{url}, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return boost::json::array{};
        }

        return response["orders"].as_array();
    }

    boost::json::array GetPendingOrders(std::string_view accountId) const
    {
        const std::string url = std::format("{}/accounts/{}/pendingOrders", m_Endpoint, accountId);
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(boost::urls::url{url}, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return boost::json::array{};
        }

        return response["orders"].as_array();
    }

    boost::json::object GetOrder(std::string_view accountId, std::string_view orderId) const
    {
        const std::string url = std::format("{}/accounts/{}/orders/{}", m_Endpoint, accountId, orderId);
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(boost::urls::url{url}, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return boost::json::object{};
        }

        return response["order"].as_object();
    }

    std::string CreateOrder(std::string_view accountId, OrderRequest& order) const
    {
        boost::json::object body{};
        body["order"] = GenerateBodyRequest(order);

        const std::string url = std::format("{}/accounts/{}/orders", m_Endpoint, accountId);
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(boost::urls::url{url}, boost::beast::http::verb::post, m_Headers, body, response);

        if (result != boost::beast::http::status::created)
        {
            return "";
        }

        auto& transaction = response["orderCreateTransaction"].as_object();
        return transaction["id"].get_string().c_str();
    }

    bool CancelOrder(std::string_view accountId, std::string_view orderId) const
    {
        const std::string url = std::format("{}/accounts/{}/orders/{}/cancel", m_Endpoint, accountId, orderId);
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(boost::urls::url{url}, boost::beast::http::verb::put, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return false;
        }

        return true;
    }

    boost::json::object GetPosition(std::string_view accountId, std::string_view instrument) const
    {
        const std::string url = std::format("{}/accounts/{}/positions/{}", m_Endpoint, accountId, instrument);
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(boost::urls::url{url}, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return boost::json::object{};
        }

        return response["position"].as_object();
    }

    boost::json::array GetOpenPositions(std::string_view accountId) const
    {
        const std::string url = std::format("{}/accounts/{}/openPositions", m_Endpoint, accountId);
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(boost::urls::url{url}, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return boost::json::array{};
        }

        return response["positions"].as_array();
    }

    bool ClosePosition(std::string_view accountId, std::string_view instrument) const
    {
        const std::string url = std::format("{}/accounts/{}/positions/{}/close", m_Endpoint, accountId, instrument);
        const boost::json::object body{};
        boost::json::object response{};

        boost::beast::http::status result =
            m_HttpClient.Make(boost::urls::url{url}, boost::beast::http::verb::put, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return false;
        }

        return true;
    }

    boost::json::object GetPricing(std::string_view accountId, std::string_view instrument) const
    {
        boost::json::object response{};
        const boost::json::object body{};

        boost::urls::url url{std::format("{}/accounts/{}/pricing", m_Endpoint, accountId)};
        url.params().set("instruments", instrument);

        boost::beast::http::status result =
            m_HttpClient.Make(boost::urls::url{url}, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return boost::json::object{};
        }

        return response;
    }

    boost::json::array GetCandles(std::string_view accountId,
                                  std::string_view instrument,
                                  CandleGranularity granularity = CandleGranularity::S5,
                                  int count = 500,
                                  std::string from = "",
                                  std::string to = "") const
    {
        boost::urls::url url{std::format("{}/accounts/{}/instruments/{}/candles", m_Endpoint, accountId, instrument)};
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
            m_HttpClient.Make(boost::urls::url{url}, boost::beast::http::verb::get, m_Headers, body, response);

        if (result != boost::beast::http::status::ok)
        {
            return boost::json::array{};
        }

        return response["candles"].as_array();
    }

  private:
    std::unordered_map<std::string, std::string> m_Headers{};
    std::string m_Endpoint;
    HttpClient& m_HttpClient;

    constexpr std::string_view OrderToString(const OrderType order) const noexcept
    {
        switch (order)
        {
            case OrderType::Market: return "MARKET";
            case OrderType::Limit: return "LIMIT";
            case OrderType::TakeProfit: return "TAKE_PROFIT";
            case OrderType::Stop: return "STOP";
        }

        return "";
    }

    constexpr std::string_view GranularityToString(const CandleGranularity granularity) const noexcept
    {
        switch (granularity)
        {
            case CandleGranularity::S5: return "S5";
            case CandleGranularity::S10: return "S10";
            case CandleGranularity::S15: return "S15";
            case CandleGranularity::S30: return "S30";
            case CandleGranularity::M1: return "M1";
            case CandleGranularity::M2: return "M2";
            case CandleGranularity::M4: return "M4";
            case CandleGranularity::M5: return "M5";
            case CandleGranularity::M10: return "M10";
            case CandleGranularity::M15: return "M15";
            case CandleGranularity::M30: return "M30";
            case CandleGranularity::H1: return "H1";
            case CandleGranularity::H2: return "H2";
            case CandleGranularity::H3: return "H3";
            case CandleGranularity::H4: return "H4";
            case CandleGranularity::H6: return "H6";
            case CandleGranularity::H8: return "H8";
            case CandleGranularity::H12: return "H12";
            case CandleGranularity::D: return "D";
            case CandleGranularity::W: return "W";
            case CandleGranularity::A: return "A";
        }

        return "";
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
