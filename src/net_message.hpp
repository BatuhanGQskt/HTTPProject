#pragma once
#include <net_common.hpp>

namespace olc {
    namespace net {
        template <typename T>
        struct message_header {
            T id{};
            uint32_t size = 0;
        };

        template <typename T>
        class message {
        public:
            message_header<T> header{};
            std::vector<uint8_t> body; // Payload to be sent message over the network uint8_t is a byte
        
        public:
            // Returns the size of the message in bytes
            size_t size() const{
                return sizeof(message_header<T>) + body.size(); // Size of the header + size of the body(bytes)
            }

            // Overload the << operator to add stuff to the body
            friend std::ostream& operator << (std::ostream& os, const message<T>& msg) {
                os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
                return os;
            }

            // Pushes any POD-like data into the message buffer
            template <typename DataType>
            friend message<T>& operator << (message<T>& msg, const DataType& data) {
                // Check that the type of the data is trivially copyable
                static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

                // Cache the current size of the vector
                size_t i = msg.body.size();

                // Resize the vector by the size of the data being pushed
                msg.body.resize(msg.body.size() + sizeof(DataType));

                // Copy the data into the newly allocated vector space
                std::memcpy(msg.body.data() + i, &data, sizeof(DataType)); // First expression is the destination to copy to add our new data. +i is the offset to the end of the vector data.

                // Recalculate the message size
                msg.header.size = msg.size();

                // Return the target message so it can be chained
                return msg;
            }

        };
    }
}
