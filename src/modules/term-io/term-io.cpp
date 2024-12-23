#include "com-api.hpp"
#include "term-io.hpp"
#include "hardware.hpp"

#include <iostream>
#include <limits>
#include <string>

namespace modules::term_io { 
    void Title(std::string title) { 
        std::cout << std::endl << "######################### - " << title << std::endl << std::endl;
    }

    void PortsInfo(const com_api::PortCollection& comPorts) { 
        for (const auto& port : comPorts) {
            std::cout << "ID: "     << port.attr_system_id 
                << "\tDevName: "    << port.attr_system_name  
                << "\tTargetName: " << port.attr_friendly_name << std::endl;
        }
    }

    void SelectPort(const com_api::PortCollection& comPorts, int* selectedPort) {
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
                if (port.attr_system_id == portId) {
                    validPort = true;
                    break;
                }
            }

            if (!validPort) {
                std::cout << "Port not found. Please select a valid port: ";
            }
        } while (!validPort);

        *selectedPort = portId;
    }

    void SelectBaudrate(int* baudRate) { 
        std::cout << "Select baudrate: ";

        int baudrate{0};
        while (!(std::cin >> baudrate)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number: ";
        }

        *baudRate = baudrate;
    }

    void SelectConnectionType(HW::ConnectionType* connectionType) {
        std::cout << "Select connection type (0 - sync, 1 - async): ";

        int choice;
        while (!(std::cin >> choice) || (choice != 0 && choice != 1)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number: ";
        }

        *connectionType = (choice == 0) ? HW::SYNCHRONOUS : HW::ASYNCHRONOUS;
    }

    void PortInfo(const com_api::PortInfo &portInfo) {
        if (portInfo._qtype == QueryInfoType::SHORTLY || portInfo._qtype == QueryInfoType::FULLY) {
            // Базовые параметры
            std::cout << "=== Basic Port Parameters ===\n";
            std::cout << "Baudrate: \t" << portInfo._shortly_state_baudRate << " bps" 
                    << "\tSelected: " << portInfo.port.attr_baud_rate << " bps\n";
            
            std::cout << "Byte size: \t" << static_cast<int>(portInfo._shortly_state_byteSize) << " bits\n";
            
            std::cout << "Stop bits: \t";
            switch(portInfo._shortly_state_stopBits) {
                case ONESTOPBIT: std::cout << "1"; break;
                case ONE5STOPBITS: std::cout << "1.5"; break;
                case TWOSTOPBITS: std::cout << "2"; break;
                default: std::cout << "Unknown"; break;
            }
            std::cout << "\n";
            
            std::cout << "Parity: \t";
            switch(portInfo._shortly_state_parity) {
                case NOPARITY: std::cout << "None"; break;
                case ODDPARITY: std::cout << "Odd"; break;
                case EVENPARITY: std::cout << "Even"; break;
                case MARKPARITY: std::cout << "Mark"; break;
                case SPACEPARITY: std::cout << "Space"; break;
                default: std::cout << "Unknown"; break;
            }
            std::cout << "\n";
        }

        if (portInfo._qtype == QueryInfoType::FULLY) {
            std::cout << "\n=== Extended Port Parameters ===\n";
            std::cout << "Queue Status:\n";
            std::cout << "  Input queue: \t" << portInfo._fully_errors_cbInQue << " bytes\n";
            std::cout << "  Output queue: \t" << portInfo._fully_errors_cbOutQue << " bytes\n";
            
            std::cout << "\nTimeout Settings (in milliseconds):\n";
            std::cout << "  Read interval timeout: \t\t" 
                    << portInfo._fully_timeouts_readIntervalTimeout << " ms\n";
            std::cout << "  Read total timeout = " 
                    << portInfo._fully_timeouts_readTotalTimeoutMultiplier << " x bytes + " 
                    << portInfo._fully_timeouts_readTotalTimeoutConstant << " ms\n";
            std::cout << "  Write total timeout = "
                    << portInfo._fully_timeouts_writeTotalTimeoutMultiplier << " x bytes + "
                    << portInfo._fully_timeouts_writeTotalTimeoutConstant << " ms\n";
        }
    }
}