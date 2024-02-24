#include <bitset>
#include <iostream>
#include <cstdint>

// How to define a bit mask?
constexpr uint8_t enablePS{111 << 0};

std::uint8_t test;

#define enablePSS 0b00000100

// "rotl" stands for "rotate left"
std::bitset<4>
rotl(std::bitset<4> bits)
{
    if (bits.test(3) == 1)
    {
        return (bits << 1) | std::bitset<4>{0b0001};
    }

    return (bits << 1);
}

int main()
{
    test = 0b01001001;
    std::bitset<4> bits1{0b0001};
    std::cout << rotl(bits1) << '\n';

    std::bitset<4> bits2{0b1001};
    std::cout << rotl(bits2) << '\n';

    std::cout << (static_cast<bool>(enablePSS & test));
    // std::cout << 'Ist die 3. Stelle 1 or 0?' << (static_cast<bool>(test & enablePSS)) ? '3. Stelle = 1' : '3. Stelle = 0';

    return 0;
}