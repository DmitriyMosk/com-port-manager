#include "com-api.hpp"

#include <sstream>
#include <iostream>

namespace modules::com_api {
    std::string ConvertIDToPortName(int id) {
        std::stringstream portName("\\\\.\\COM");

        portName << id; 

        return portName.str();
    }

    std::vector<PortData> ScanPorts() {
        std::vector<PortData> ports;
        char targetPath[5000];
        
        for (int i = 1; i < 256; i++) {
            std::string portName = ConvertIDToPortName(i); 

            HANDLE hComm = CreateFileA(portName.c_str(), 
                GENERIC_READ | GENERIC_WRITE,
                0,
                0,
                OPEN_EXISTING,
                0,
                0);

            if (hComm != INVALID_HANDLE_VALUE) {
                PortData port;
                port.descriptor = hComm;
                port.systemId = i;
                
                QueryDosDeviceA(&portName[4], targetPath, 5000);
                port.friendlyName = std::string(targetPath);
                
                ports.push_back(port);
                CloseHandle(hComm);
            } 
        }
        return ports;
    }

    PortData GetPortBySystemId(int systemId) {
        PortData port; 

        std::string portName = ConvertIDToPortName(systemId); 

        HANDLE hComm = CreateFileA(portName.c_str(), 
            GENERIC_READ | GENERIC_WRITE,
            0,
            0,
            OPEN_EXISTING,
            0,
            0);

        port.systemId = systemId;
        port.accept = hComm != INVALID_HANDLE_VALUE; 
        port.descriptor = hComm;
        port.friendlyName = port.accept ? ConvertIDToPortName(systemId) : "Port not found";
        
        return port;
    }

    //PortData GetPortFD
}