#include <bitset>
#include <iostream>
#include <cstdint>

// How to define a bit mask?
constexpr uint8_t enablePS{1 << 2};

// Lets define our Bitmasks
// #define ALS_TURN_ON (1 << 0)
#define ALS_100MS_INTEGRATION_TIME (1 << 6)
// #define ALS_200MS_INTEGRATION_TIME (1 << 7)

// Using Bitset: Define bitmasks
std::bitset<8> ALS_200MS_INTEGRATION_TIME = (1 << 7);
std::bitset<8> ALS_TURN_ON = (1 << 0);
std::uint8_t test;

#define enablePSS 0b00000000
#define doesThisAlsoWork (1 << 2)

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
    // Testing for XOR operator:
    uint8_t ichbinxor = {1 << 3};
    uint8_t mask = 0b00000010;
    std::cout << static_cast<int>(mask ^= ichbinxor) << " => \n";

    test = 0b01001101;

    // std::cout << (0b11111111);

    // std::bitset<4> bits1{0b0001};
    // std::cout << rotl(bits1) << '\n';

    // std::bitset<4> bits2{0b1001};
    // std::cout << rotl(bits2) << '\n';

    std::cout << (ALS_TURN_ON | ALS_200MS_INTEGRATION_TIME) << "\n";

    // if (std::bitset<8>(10000001) == (ALS_TURN_ON | ALS_200MS_INTEGRATION_TIME))
    // {
    //     std::cout << "10000001 is part";
    // }

    std::cout << ((std::bitset<8>(10000000) == (ALS_TURN_ON | ALS_200MS_INTEGRATION_TIME)) ? "ALS turned on & 200ms\n" : "idk not a valid value!\n");

    // std::cout
    //     << (static_cast<bool>(doesThisAlsoWork & test) ? "an" : "aus")
    //     << "\n";
    // std::cout << 'Ist die 3. Stelle 1 or 0?' << (static_cast<bool>(test & enablePSS)) ? '3. Stelle = 1' : '3. Stelle = 0';

    return 0;
}