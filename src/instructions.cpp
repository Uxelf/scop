#include <iostream>

void print_instructions(){
    std::cout << std::endl << "\e[96m";
    std::cout << "Instructions:" << std::endl;
    std::cout << "WASD: move" << std::endl;
    std::cout << "Mouse: rotate camera" << std::endl;
    std::cout << "Mouse scroll: change FOV" << std::endl;
    std::cout << "Key 1: Texture transition" << std::endl;
    std::cout << "Key 2: Light stop / move" << std::endl;
    std::cout << "\e[0m" << std::endl;
}