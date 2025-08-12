#pragma once
#include <string>
#include <vector>

struct SedeGrafo {
    std::string codigoIATA;
    std::string pais;
    std::string ciudad;
    std::string estadio;
    int capacidad;
    int anyoInauguracion;
};

class Grafo {
public:
    Grafo();
    bool cargarSedes(const std::string& archivoSedes);
    bool cargarAristas(const std::string& archivoAristas);
    int buscarIndicePorIATA(const std::string& codigoIATA) const;
    void mostrarSede(const std::string& codigoIATA) const;
    void mostrarTodasSedes() const;
    std::string rutaMasCorta(const std::string& origen, const std::string& destino, bool porDistancia) const;
    std::string sedeMasLejana(const std::string& origen, bool porDistancia) const;
    std::string redTransporteEficiente(bool porDistancia) const;
    void mostrarSedeInauguracion() const;
private:
    std::vector<SedeGrafo> sedes;
    double matrizDistancia[16][16];
    double matrizTiempo[16][16];
    void dijkstra(int origen, const double matriz[16][16], std::vector<double>& dist, std::vector<int>& prev) const;
    void prim(const double matriz[16][16], std::vector<int>& padre) const;
};