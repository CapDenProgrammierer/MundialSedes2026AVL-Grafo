#pragma once
#include <string>

struct Sede {
    std::string codigoIATA;
    std::string pais;
    std::string ciudad;
    std::string estadio;
    int capacidad = 0;
    int anyoInauguracion = 0;
};