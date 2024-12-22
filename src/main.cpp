#include "com-api.hpp" 
#include "term-io.hpp"
#include <cassert>

using namespace std;
using namespace modules;

int main() {
    cout << "----------[PORT LIST]------------" << endl;

    com_api::PortCollection comPorts = com_api::ScanPorts();
    
    // // Print port info 
    term_io::PortsInfo(comPorts);

    // cout << "---------[USER REQUEST]----------" << endl;
    
    // // Select port 
    // int portId;
    // term_io::SelectPort(comPorts, &portId);
    
    // com_api::PortData port = com_api::(portId);

    // // Insert Baudrate
    // int baudRate;
    // bool isBaudrateSelected = term_io::SelectBaudrate(comPorts, &baudRate);

    // if (!isBaudrateSelected) {
    //     assert(isBaudrateSelected);
    // }

    // cout << "----------[USER SETTINGS]--------" << endl;

    // cout << "Selected port: " << portId << endl;    
    // cout << "Selected baudrate: " << baudRate << endl;

    // cout << "---------------------------------" << endl;

    // com_api::PortData port = com_api::GetPortBySystemId(portId);

    // term_io::PortInfo(port, COM_API_INFO_SHORTLY);

    // cout << "---------------------------------" << endl;\

    // // port = com_api::openPort(port.friendlyName, baudRate);
    
    // // if (port.accept) { 

    // // }

    return EXIT_SUCCESS;
}