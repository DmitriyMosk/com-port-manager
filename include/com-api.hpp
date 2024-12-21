#include <windows.h>

#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <iostream>
#include <vector>
#include <string>

#ifndef MODULES_COM_API_HPP
#define MODULES_COM_API_HPP 
    
    namespace modules::com_api {

        struct ComPortInfo {
            std::string portName;
            std::string friendlyName;
        };

        std::vector<ComPortInfo> scanPorts();

        // HANDLE openPort(const std::string& portName, DWORD baudRate);

        // void closePort(HANDLE hCom);

        // bool readFromPort(HANDLE hCom, char* buffer, DWORD bufferSize, DWORD& bytesRead);

        // bool writeToPort(HANDLE hCom, const char* buffer, DWORD bufferSize, DWORD& bytesWritten);
    }

#endif 