#include <boost/test/unit_test.hpp>
#include "../Source/HTTPRequest.hpp"

BOOST_AUTO_TEST_CASE(HTTPRequest_request)
{
    const boost::urls::url api("https://echo.free.beeceptor.com/sample-request?author=Pedro&juan=Pedro");
    const std::unordered_map<std::string, std::string> headers{ { "x-api-key", "Bearer mytoken" } };
    const boost::json::object body{ { "message", "Hello world" } };
    boost::json::object response{};

    auto status = HTTPRequest::Instance().make(
        api,
        boost::beast::http::verb::post,
        headers,
        body,
        response
    );

    BOOST_ASSERT(status == boost::beast::http::status::ok);
    BOOST_ASSERT(response["method"].as_string() == "POST");
    BOOST_ASSERT(response["headers"].as_object()["X-Api-Key"] == "Bearer mytoken");
    BOOST_ASSERT(response["parsedBody"].as_object()["message"] == "Hello world");
};

