#include <windows.h>
#include <string>
#include <stdexcept>
#include <tchar.h>
#include <vector>

#ifndef MODULES_COM_API_HPP
#define MODULES_COM_API_HPP
    
    namespace modules::com_api {
        struct PortData {
            HANDLE descriptor;
            std::string friendlyName;
            int systemId;
            bool accept;
        };

        std::string ConvertIDToPortName(int id);

        std::vector<PortData> ScanPorts();
        
        PortData GetPortBySystemId(int systemId);
        
        HANDLE openPort(const std::string& portName, DWORD baudRate);
        
        // void closePort(HANDLE hCom);

        // bool readFromPort(HANDLE hCom, char* buffer, DWORD bufferSize, DWORD& bytesRead);

        // bool writeToPort(HANDLE hCom, const char* buffer, DWORD bufferSize, DWORD& bytesWritten);
    }

#endif 