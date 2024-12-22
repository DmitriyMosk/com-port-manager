#ifndef MODULES_TERM_IO_HPP
#define MODULES_TERM_IO_HPP

#include "com-api.hpp"
#include <iostream>
#include <vector>

#define TERM_IO_CORRECT_INPUT true 
#define TERM_IO_INCORRECT_INPUT false

namespace modules::term_io {

    // Функция для вывода информации о портах
    void PortsInfo(const com_api::PortCollection& comPorts);

    // // Функция для вывода информации о порте
    // void PortInfo(com_api::PortData, std::string);

    // // Функция для выбора порта
    // void SelectPort(const std::vector<com_api::PortData>& comPorts, int* selectedPort);

    // // Функция для выбора скорости передачи данных
    // bool SelectBaudrate(const std::vector<com_api::PortData>& comPorts, int* baudRate);

} // namespace modules::term_io

#endif // MODULES_TERM_IO_HPP