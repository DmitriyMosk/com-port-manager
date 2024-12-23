#ifndef MODULES_TERM_IO_HPP
#define MODULES_TERM_IO_HPP

#include "com-api.hpp"
#include "hardware.hpp"

#include <iostream>
#include <vector>

#define TERM_IO_CORRECT_INPUT true 
#define TERM_IO_INCORRECT_INPUT false

namespace modules::term_io {

    // Функция для вывода информации о портах
    void PortsInfo(const com_api::PortCollection& comPorts);

    // Функция для вывода заголовка 
    void Title(std::string title);

    // Функция для выбора порта
    void SelectPort(const com_api::PortCollection& comPorts, int* selectedPort);

    // Функция для выбора скорости передачи данных
    void SelectBaudrate(int* baudRate);

    void SelectConnectionType(HW::ConnectionType* connectionType);
    
    void PortInfo(const com_api::PortInfo&);


    template<typename T>
    std::string InterpretAttribute(T attribute);
} // namespace modules::term_io

#endif // MODULES_TERM_IO_HPP