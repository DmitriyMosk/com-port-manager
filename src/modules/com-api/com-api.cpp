#include "com-api.hpp"

#include <sstream>
#include <iostream>

namespace modules::com_api {
    IOCode Port::QueryPort() {
        std::stringstream portName("");

        portName << COM_API_SYSTEM_PREFIX << this->attr_system_id;

        this->attr_system_name = portName.str();

        HANDLE hCom = CreateFileA(
            wr::stringToLpFileName(this->attr_system_name), 
            wr::dwDesireAccess(this->attr_desired_access),
            wr::dwShareAccess(this->attr_sharing_access), NULL,
            wr::dwCreationDisposition(this->attr_flags_creation),
            wr::dwFlagsAndAttributes(this->attr_file_flags), NULL
        );

        if (hCom == INVALID_HANDLE_VALUE) {
            DWORD error_code = GetLastError();

            switch (error_code) {
                case ERROR_FILE_NOT_FOUND:
                    return IOCode::ERROR_INVALID_PORT;
                    break;
                case ERROR_ACCESS_DENIED:
                    return IOCode::ERROR_PORT_ACCESS_DENIED;
                    break;
                case ERROR_INVALID_PARAMETER:
                    return IOCode::ERROR_INVALID_PARAM;
                    break;
                default:
                    return IOCode::ERROR_INVALID_VALUE;
                    break;
            }
        } else {
            this->hCom = hCom;
            this->attr_is_available = PortStatus::PORT_AVAILABLE;

            return IOCode::QUERY_SUCCESS;
        }
    }

    IOCode Port::Close() {
        if (this->hCom != nullptr) {
            return CloseHandle(this->hCom) ? IOCode::QUERY_SUCCESS : IOCode::ERROR_INVALID_VALUE;
        } else {
            return IOCode::ERROR_INVALID_VALUE;
        }
    }

    int Port::GetSystemID() {
        return attr_system_id;
    }

    bool Port::IsAvailable() {
        return attr_is_available == PortStatus::PORT_AVAILABLE;
    }

    PortCollection ScanPorts() {
        PortCollection ports; 
        
        for (int i = 1; i < 256; i++) { 
            Port port(i);

            port.attr_desired_access    = COM_API_GENERIC_R;
            port.attr_sharing_access    = FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE;
            port.attr_flags_creation    = OPEN_EXISTING;
            port.attr_file_flags        = FILE_ATTRIBUTE_NORMAL;

            IOCode status = port.QueryPort();

            if (status == IOCode::QUERY_SUCCESS) {
                char pathBuff[5000]; 

                QueryDosDeviceA(&(port.attr_system_name)[4], pathBuff, sizeof(pathBuff));
                port.attr_friendly_name = std::string(pathBuff);

                ports.push_back(port);
        
                if (port.Close() != IOCode::QUERY_SUCCESS) {
                    DWORD error = GetLastError();
                    std::cerr << "Error closing handle: " << error << std::endl;
                }
            }
        }
       
        return ports;
    }

    // PortData GetPortBySystemId(int systemId) {
    //     PortData port; 

    //     std::string portName = ConvertIDToPortName(systemId); 

    //     HANDLE hComm = CreateFileA(portName.c_str(), 
    //         GENERIC_READ | GENERIC_WRITE,
    //         0,
    //         0,
    //         OPEN_EXISTING,
    //         0,
    //         0);

    //     port.systemId = systemId;
    //     port.isAvailable = hComm != INVALID_HANDLE_VALUE; 
    //     port.descriptor = hComm;
    //     port.friendlyName = port.isAvailable ? ConvertIDToPortName(systemId) : "Port not found";

    //     return port;
    // }

    //PortData GetPortFD
}