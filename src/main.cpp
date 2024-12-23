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
    cout << "Connection type: " << term_io::InterpretAttribute(static_cast<HW::ConnectionType>(connectionType)) << endl;

    com_api::Port port = com_api::QueryPortById(comPorts, portId); 
    port.attr_baud_rate = baudRate; 
    
    cout << "###Check port availability.###" << endl;
    assert(port.CheckPort() == PortStatus::PORT_AVAILABLE);
    cout << "###[isAvailable -> OK].###" << endl;

    term_io::Title("Port info (if exists)");
    term_io::PortInfo(com_api::QueryPortInfo(port, QueryInfoType::FULLY));

    HW::WirePort(port, baudRate, connectionType);

    return EXIT_SUCCESS;
}