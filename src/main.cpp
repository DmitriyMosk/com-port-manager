#include "com-api.hpp" 
#include "term-io.hpp"
#include <cassert>

using namespace std;
using namespace modules;

int main() {
    cout << "---------------------------------" << endl;

    vector<com_api::PortData> comPorts = com_api::ScanPorts();
    
    // Print port info 
    term_io::PortsInfo(comPorts);

    cout << "---------------------------------" << endl;
    
    // Select port 
    int portId;
    bool isPortSelected = term_io::SelectPort(comPorts, &portId);

    // Insert Baudrate
    int baudRate;
    bool isBaudrateSelected = term_io::SelectBaudrate(comPorts, &baudRate);

    if (!isPortSelected || !isBaudrateSelected) {
        assert(isPortSelected); 
        assert(isBaudrateSelected);
    }

    cout << "---------------------------------" << endl;

    cout << "Selected port: " << portId << endl;    
    cout << "Selected baudrate: " << baudRate << endl;

    cout << "---------------------------------" << endl;

    com_api::PortData port = com_api::GetPortBySystemId(portId);

    term_io::PortInfo(port, "shortly");

    cout << "---------------------------------" << endl;\

    port = com_api::openPort(port.friendlyName, baudRate);
    
    if (port.accept) { 

    }
    
    return EXIT_SUCCESS;
}