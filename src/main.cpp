#include "com-api.hpp" 
#include "term-io.hpp"
#include "hardware.hpp"

#include <cassert>

using namespace std;
using namespace modules;

int main() {
    term_io::Title("COM Port Scanner");

    com_api::PortCollection comPorts = com_api::ScanPorts();
    // Print port info 
    term_io::PortsInfo(comPorts);
    
    //
    term_io::Title("Request user settings");
    
    // Select port 
    int portId;
    term_io::SelectPort(comPorts, &portId);

    // Insert Baudrate
    int baudRate;
    term_io::SelectBaudrate(&baudRate);
    //

    HW::ConnectionType connectionType;
    term_io::SelectConnectionType(&connectionType); 
    // 
    term_io::Title("User settings");
    cout << "Port ID: " << portId << endl;
    cout << "Baudrate: " << baudRate << endl;
    cout << "Connection type: " << (connectionType == HW::ConnectionType::ASYNCHRONOUS ? "ASYNCHRONOUS" : "SYNCHRONOUS") << endl;

    term_io::Title("Checking port availability");
    assert(com_api::QueryPortById(comPorts, portId).IsAvailable() == true);

    term_io::Title("Port info (if exists)");
    com_api::QueryPortInfo(comPorts, portId).PrintPortInfo();

    HW::WirePort(com_api::QueryPortById(comPorts, portId), baudRate, connectionType);

    return EXIT_SUCCESS;
}