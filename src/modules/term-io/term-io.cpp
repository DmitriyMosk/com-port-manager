#include <com-api.hpp> 
#include <term-io.hpp>

#include <iostream>
#include <limits>

namespace modules::term_io { 
    void PortsInfo(const std::vector<com_api::PortData>& comPorts) { 
        std::cout << "Available COM ports:" << std::endl;

        for (const auto& port : comPorts) {
            std::cout << port.systemId << " - " << port.friendlyName << std::endl;
        }
    }

    bool SelectPort(const std::vector<com_api::PortData>& comPorts, int *selectedPort) {
        std::cout << "Select port: ";
        
        int portId;
        bool validPort = false;
        do {
            if (!(std::cin >> portId)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number: ";
                continue;
            }

            for (const auto& port : comPorts) {
                if (port.systemId == portId) {
                    validPort = true;
                    break;
                }
            }

            if (!validPort) {
                std::cout << "Port not found. Please select a valid port: ";
            }
        } while (!validPort);

        *selectedPort = portId;

        return TERM_IO_CORRECT_INPUT;
    }

    bool SelectBaudrate(const std::vector<com_api::PortData>& comPorts, int* baudRate) { 
        std::cout << "Select baudrate: ";

        int baudrate;
        while (!(std::cin >> baudrate)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number: ";
        }

        *baudRate = baudrate;

        return TERM_IO_CORRECT_INPUT;
    }

    void PortInfo(com_api::PortData port, std::string message) {
        
    }
}