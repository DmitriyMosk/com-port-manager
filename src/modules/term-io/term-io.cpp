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
            std::cout << "Baudrate: "   << ((portInfo._shortly_state_baudRate != 0) ? portInfo._shortly_state_baudRate : -1) << "\tSelected: " << portInfo.port.attr_baud_rate << std::endl;
            std::cout << "Byte size: "  << ((portInfo._shortly_state_byteSize != 0) ? portInfo._shortly_state_byteSize : -1) << std::endl;
            std::cout << "Stop bits: "  << ((portInfo._shortly_state_stopBits != 0) ? portInfo._shortly_state_stopBits : -1) << std::endl;
            std::cout << "Parity: "     << ((portInfo._shortly_state_parity != 0) ? portInfo._shortly_state_parity : -1) << std::endl;
        } else if (portInfo._qtype == QueryInfoType::FULLY) {
            std::cout << "In queue: "   << ((portInfo._fully_errors_cbInQue != 0) ? portInfo._fully_errors_cbInQue : -1) << std::endl;
        }
    }

    // void PortInfo(com_api::PortData port, std::string message) {
    //     std::cout << "\n=== Port Information ===\n";
    //     std::cout << message << "\n";
    //     std::cout << "System ID: " << port.systemId << "\n";
    //     std::cout << "Friendly Name: " << port.friendlyName << "\n";

    //     // Basic port information that's typically available
    //     std::cout << "Port Number: COM" << port.systemId << "\n";
    //     std::cout << "Status: " << (port.isAvailable ? "Available" : "In Use") << "\n";

    //     HANDLE hCOM = CreateFileA(
    //         com_api::ConvertIDToPortName(port.systemId).c_str(),
    //         GENERIC_READ | GENERIC_WRITE,
    //         0,
    //         NULL,
    //         OPEN_EXISTING,
    //         FILE_ATTRIBUTE_NORMAL,
    //         NULL
    //     );

    //     if (hCOM != INVALID_HANDLE_VALUE) {
    //         DCB dcbSerialParams = { 0 };
    //         dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    //         if (GetCommState(hCOM, &dcbSerialParams)) {
    //             std::cout << "Current Settings:\n";
    //             std::cout << "Baud Rate: " << dcbSerialParams.BaudRate << "\n";
    //             std::cout << "Byte Size: " << (int)dcbSerialParams.ByteSize << "\n";
    //             std::cout << "Parity: " << (int)dcbSerialParams.Parity << "\n";
    //             std::cout << "Stop Bits: " << (int)dcbSerialParams.StopBits << "\n";
    //         }

    //         COMMPROP commProp;
    //         if (GetCommProperties(hCOM, &commProp)) {
    //             std::cout << "\nPort Capabilities:\n";
    //             std::cout << "Maximum Baud Rate: " << commProp.dwMaxBaud << "\n";
    //             std::cout << "Maximum TX Queue: " << commProp.dwMaxTxQueue << "\n";
    //             std::cout << "Maximum RX Queue: " << commProp.dwMaxRxQueue << "\n";
    //         }

    //         CloseHandle(hCOM);
    //     } else {
    //         std::cout << "Could not open port for detailed information.\n";
    //     }
    // }
}