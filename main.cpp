#define ASIO_STANDALONE
#define BOOST_ASIO_HEADER_ONLY
#define BOOST_SYSTEM_NO_LIB
#define BOOST_ERROR_CODE_HEADER_ONLY
#include <cstdlib>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>

#include <vector>
#include <thread>
#include <atomic>

using namespace boost::asio;
using namespace boost::asio::ip;
using boost::system::error_code;

std::vector<char> vBuffer(1 * 1024);
std::atomic<bool> dataTransferComplete = false;

void GrabSomeData(tcp::socket& socket) {
    socket.async_read_some(boost::asio::buffer(vBuffer.data(), vBuffer.size()),
        [&](boost::system::error_code ec, std::size_t length) {
            if (!ec) {
                std::cout << "\n\nRead " << length << " bytes\n\n";

                for (int i = 0; i < length; i++) {
                    std::cout << vBuffer[i];
                }
                
                // Continue reading asynchronously
                GrabSomeData(socket);
            }
            else if (boost::asio::error::eof == ec || ec == boost::asio::error::connection_reset) {
                // Normal closure - server closed the connection after sending all data
                std::cout << "\n\nEOF - Server closed connection normally\n";
                dataTransferComplete = true;
            }
            else {
                std::cerr << "Read error: " << ec.message() << std::endl;
                dataTransferComplete = true;
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
    // tcp::resolver resolver(context);
    // auto results = resolver.resolve("example.com", "80", ec);  // Port 80 for HTTP
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("23.209.46.91"), 80);
    if (ec) {
        std::cerr << "Failed to resolve example.com: " << ec.message() << std::endl;
        return -1;
    }

    // Establish connection
    tcp::socket socket(context);
    socket.connect(endpoint, ec);
    // connect(socket, results, ec);
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
                    dataTransferComplete = true;
                }
            });
        
        // Wait for data transfer to complete instead of fixed sleep
        using namespace std::chrono_literals;
        while (!dataTransferComplete) {
            std::this_thread::sleep_for(100ms);
        }
        
        std::cout << "\nTransfer complete!\n";
        
        // Clean shutdown
        socket.close(ec);
        context.stop();
        
        // Wait for the context thread to finish
        if(thrContext.joinable()) thrContext.join();
    }

    return 0;
}
