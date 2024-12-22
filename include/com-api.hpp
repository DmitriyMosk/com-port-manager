#include <windows.h>
#include <string>
#include <stdexcept>
#include <tchar.h>
#include <vector>

#ifndef MODULES_COM_API_HPP
#define MODULES_COM_API_HPP
    #define COM_API_SYSTEM_PREFIX "\\\\.\\COM"

    enum IOCode {
        ERROR_INVALID_VALUE         = static_cast<intptr_t>(-1),  // INVALID_HANDLE_VALUE is -1
        ERROR_INVALID_PORT          = ERROR_FILE_NOT_FOUND,
        ERROR_PORT_ACCESS_DENIED    = ERROR_ACCESS_DENIED,
        ERROR_INVALID_PARAM         = ERROR_INVALID_PARAMETER,
        QUERY_SUCCESS               = 0
    };

    enum PortStatus {
        PORT_AVAILABLE,
        PORT_NOT_AVAILABLE 
    };
    
    // Generic access rights
    #define COM_API_GENERIC_RWRD    (GENERIC_READ | GENERIC_WRITE)
    #define COM_API_GENERIC_R       (GENERIC_READ)
    #define COM_API_GENERIC_W       (GENERIC_WRITE)

    // for port information
    #define COM_API_PORT_INFO_SHORTLY   "shortly" 
    #define COM_API_PORT_INFO_FULLY     "fully"

    // Forward declaration
    typedef std::string port_name_t;  
    typedef DWORD       COM_DW;

    namespace modules::com_api {
        namespace wr { 
            LPCSTR stringToLpFileName(port_name_t name); 

            DWORD dwDesireAccess(COM_DW accessRight); 
            DWORD dwShareAccess(COM_DW sharingAccess);
            DWORD dwCreationDisposition(COM_DW creationDisposition);
            DWORD dwFlagsAndAttributes(COM_DW fileFlags);
        }

        struct Port {
            int attr_system_id;
            int attr_baud_rate;
            PortStatus attr_is_available; 

            COM_DW attr_desired_access;
            COM_DW attr_sharing_access; 
            COM_DW attr_flags_creation;
            COM_DW attr_file_flags;

            HANDLE hCom;

            port_name_t attr_friendly_name;
            port_name_t attr_system_name;
            //

            Port(int id) : 
                attr_system_id      (0), 
                attr_baud_rate      (0),
                attr_desired_access (0),
                attr_sharing_access (0),
                attr_flags_creation (0),
                attr_file_flags     (0),
                attr_is_available   (PortStatus::PORT_NOT_AVAILABLE), 
                attr_system_name    (""),
                attr_friendly_name  (""),
                // hCom - handle to the port
                hCom                (nullptr)
            {
                attr_system_id = id;
            }

            // Queries the hardware for port information and updates the port attributes.
            IOCode QueryPort();

            IOCode Close(); 
            // Returns the system ID of the port
            int GetSystemID(); 
            // Check port availability
            bool IsAvailable();
        };

        typedef std::vector<Port> PortCollection;

        // Scans and returns a list of available COM ports.
        PortCollection ScanPorts();        


        // Retrieves the port data for the given ID
        // Port RetreivePortData(int id);
        
        // HANDLE openPort(const std::string& portName, DWORD baudRate);
        
        // void closePort(HANDLE hCom);

        // bool readFromPort(HANDLE hCom, char* buffer, DWORD bufferSize, DWORD& bytesRead);

        // bool writeToPort(HANDLE hCom, const char* buffer, DWORD bufferSize, DWORD& bytesWritten);
    }

#endif 