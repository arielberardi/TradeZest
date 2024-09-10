#include <iostream>
#include <string>

//#include "Broker/RobbinHood.hpp"
#include "HTTPRequest.hpp"

int main()
{
    try
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

        std::cout << boost::json::serialize(response) << std::endl;
        std::cout << response["parsedBody"].as_object()["message"] << std::endl;
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}

