#include <climits>
#include <iostream>
#include <limits>

int main()
{
    bool passed = true;

    if (sizeof(float) * CHAR_BIT != 32)
    {
        std::cerr << "Expected float to occupy 32 bits, but it occupies " << (sizeof(float) * CHAR_BIT) << ".\n";
        passed = false;
    }

    if (!std::numeric_limits<float>::is_iec559)
    {
        std::cerr << "Expected float to provide IEC 60559 floating-point behavior.\n";
        passed = false;
    }

    if (!passed)
    {
        return 1;
    }

    std::cout << "Math policy toolchain assumptions are satisfied.\n";
    return 0;
}
