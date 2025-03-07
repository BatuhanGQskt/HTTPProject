#define ASIO_STANDALONE
#define BOOST_ASIO_HEADER_ONLY
#define BOOST_SYSTEM_NO_LIB
#define BOOST_ERROR_CODE_HEADER_ONLY
#include <cstdlib>
#include <iostream>
#include "./src/libmylib.hpp"
#include <boost/asio.hpp>

#include <vector>
#include <thread>

using namespace boost::asio;
using namespace boost::asio::ip;
using boost::system::error_code;

std::vector<char> vBuffer(1 * 1024);

void GrabSomeData(tcp::socket& socket) {
    socket.async_read_some(buffer(vBuffer.data(), vBuffer.size()),
        [&](std::error_code ec, std::size_t length) {
            if (!ec) {
                std::cout << "\n\nRead " << length << " bytes\n\n";
                std::cout.write(vBuffer.data(), length);
                std::cout << std::endl;

                // Continue reading asynchronously
                GrabSomeData(socket);
            }
            else {
                std::cerr << "Read error: " << ec.message() << std::endl;
            }
        }
    );
}

int main() {
    boost::system::error_code ec;
    io_context context;

    // Keep the context running in a separate thread
    io_context::work idleWork(context);
        std::thread thrContext([&]() {
        context.run();
    });

    // Resolver to get IP address for example.com
    tcp::resolver resolver(context);
    auto results = resolver.resolve("example.com", "80", ec);  // Port 80 for HTTP

    if (ec) {
        std::cerr << "Failed to resolve example.com: " << ec.message() << std::endl;
        return -1;
    }

    // Establish connection
    tcp::socket socket(context);
    connect(socket, results, ec);
    if (ec) {
        std::cerr << "Failed to connect: " << ec.message() << std::endl;
        return -1;
    }

    std::cout << "Connected to example.com!" << std::endl;

    if (socket.is_open()) {
        // Start asynchronous reading
        GrabSomeData(socket);

        // Send HTTP request asynchronously
        std::string sRequest =
            "GET /index.html HTTP/1.1\r\n"
            "Host: example.com\r\n"
            "Connection: close\r\n\r\n";

        async_write(socket, buffer(sRequest.data(), sRequest.size()),
            [&](std::error_code ec, std::size_t length) {
                if (ec) {
                    std::cerr << "Send failed: " << ec.message() << std::endl;
                }
            });

        // Keep the program running until the response is fully read
        thrContext.join();
    }

    return 0;
}
