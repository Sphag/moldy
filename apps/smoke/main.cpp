#include <iostream>

import moldy.core;

int main()
{
    std::cout << "Core smoke check\n";
    std::cout << "Compiler: " << core::compiler_id() << '\n';
    std::cout << "Build configuration: " << core::build_configuration() << '\n';
    std::cout << "C++ language standard: " << core::language_standard() << '\n';

    return 0;
}
