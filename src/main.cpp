#include <iostream>
#include "com-api.hpp" 

int main() {
    auto comPorts = modules::com_api::scanPorts();
    std::cout << "Available COM ports:" << std::endl;
    for (const auto& port : comPorts) {
        std::cout << port.portName << " - " << port.friendlyName << std::endl;
    }
}