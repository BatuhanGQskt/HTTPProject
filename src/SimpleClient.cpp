#include <iostream>
#include <olc_net.hpp>

enum class CustomMsgTypes : uint32_t {
    FireBullet,
    MovePlayer
};

int main() {
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::FireBullet;

    int a = 1;
    bool b = true;
    float c = 3.14159f;

    struct {
        float x;
        float y;
    } d[5];

    msg << a << b << c << d;

    std::cout << "Before: " << msg << std::endl;

    a = 99;
    b = false;
    c = 2.71828f;

    msg >> d >> c >> b >> a;
    std::cout << "After: " << msg << std::endl;
    std::cout << "Variables: " << a << " " << b << " " << c << std::endl;

    return 0;
}