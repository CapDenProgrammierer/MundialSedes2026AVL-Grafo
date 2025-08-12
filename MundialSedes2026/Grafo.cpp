#include "Grafo.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

Grafo::Grafo() {
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            matrizDistancia[i][j] = matrizTiempo[i][j] = (i == j ? 0 : std::numeric_limits<double>::max());
}

bool Grafo::cargarSedes(const std::string& archivoSedes) {
    std::ifstream file(archivoSedes);
    if (!file.is_open()) return false;
    sedes.clear();
    std::string linea;
    while (std::getline(file, linea)) {
        std::stringstream ss(linea);
        SedeGrafo s;
        std::getline(ss, s.codigoIATA, ',');
        std::getline(ss, s.pais, ',');
        std::getline(ss, s.ciudad, ',');
        std::getline(ss, s.estadio, ',');
        std::string capacidad, anyo;
        std::getline(ss, capacidad, ',');
        std::getline(ss, anyo, ',');
        s.capacidad = std::stoi(capacidad);
        s.anyoInauguracion = std::stoi(anyo);
        sedes.push_back(s);
    }
    return sedes.size() == 16;
}

bool Grafo::cargarAristas(const std::string& archivoAristas) {
    std::ifstream file(archivoAristas);
    if (!file.is_open()) return false;
    std::string linea;
    while (std::getline(file, linea)) {
        std::stringstream ss(linea);
        std::string iata1, iata2, dist, tiempo;
        std::getline(ss, iata1, ',');
        std::getline(ss, iata2, ',');
        std::getline(ss, dist, ',');
        std::getline(ss, tiempo, ',');
        int idx1 = buscarIndicePorIATA(iata1);
        int idx2 = buscarIndicePorIATA(iata2);
        if (idx1 != -1 && idx2 != -1) {
            matrizDistancia[idx1][idx2] = matrizDistancia[idx2][idx1] = std::stod(dist);
            matrizTiempo[idx1][idx2] = matrizTiempo[idx2][idx1] = std::stod(tiempo);
        }
    }
    return true;
}

int Grafo::buscarIndicePorIATA(const std::string& codigoIATA) const {
    for (int i = 0; i < sedes.size(); ++i)
        if (sedes[i].codigoIATA == codigoIATA)
            return i;
    return -1;
}

void Grafo::mostrarSede(const std::string& codigoIATA) const {
    int idx = buscarIndicePorIATA(codigoIATA);
    if (idx == -1) {
        std::cout << "No existe sede con ese codigo IATA\n";
        return;
    }
    const SedeGrafo& s = sedes[idx];
    std::cout << "IATA: " << s.codigoIATA << "\nPais: " << s.pais << "\nCiudad: " << s.ciudad
        << "\nEstadio: " << s.estadio << "\nCapacidad: " << s.capacidad
        << "\nAnno inauguracion: " << s.anyoInauguracion << "\n----------------------\n";
}

void Grafo::dijkstra(int origen, const double matriz[16][16], std::vector<double>& dist, std::vector<int>& prev) const {
    dist.assign(16, std::numeric_limits<double>::max());
    prev.assign(16, -1);
    std::vector<bool> visitado(16, false);
    dist[origen] = 0;
    for (int i = 0; i < 16; ++i) {
        int u = -1;
        for (int j = 0; j < 16; ++j)
            if (!visitado[j] && (u == -1 || dist[j] < dist[u]))
                u = j;
        if (dist[u] == std::numeric_limits<double>::max()) break;
        visitado[u] = true;
        for (int v = 0; v < 16; ++v) {
            if (matriz[u][v] < std::numeric_limits<double>::max() && dist[u] + matriz[u][v] < dist[v]) {
                dist[v] = dist[u] + matriz[u][v];
                prev[v] = u;
            }
        }
    }
}

void Grafo::prim(const double matriz[16][16], std::vector<int>& padre) const {
    std::vector<double> key(16, std::numeric_limits<double>::max());
    std::vector<bool> mstSet(16, false);
    padre.assign(16, -1);
    key[0] = 0;
    for (int count = 0; count < 16 - 1; ++count) {
        double min = std::numeric_limits<double>::max();
        int u = -1;
        for (int v = 0; v < 16; ++v)
            if (!mstSet[v] && key[v] < min)
                min = key[v], u = v;
        mstSet[u] = true;
        for (int v = 0; v < 16; ++v)
            if (matriz[u][v] < key[v] && !mstSet[v]) {
                key[v] = matriz[u][v];
                padre[v] = u;
            }
    }
}

std::string Grafo::rutaMasCorta(const std::string& origen, const std::string& destino, bool porDistancia) const {
    int idxOrigen = buscarIndicePorIATA(origen);
    int idxDestino = buscarIndicePorIATA(destino);
    if (idxOrigen == -1 || idxDestino == -1) {
        return "Alguno de los códigos IATA no existe.";
    }
    const double (*matriz)[16] = porDistancia ? matrizDistancia : matrizTiempo;
    std::vector<double> dist;
    std::vector<int> prev;
    dijkstra(idxOrigen, matriz, dist, prev);

    if (dist[idxDestino] == std::numeric_limits<double>::max()) {
        return "No hay ruta disponible entre las sedes.";
    }

    // Reconstruir el camino
    std::vector<int> camino;
    for (int at = idxDestino; at != -1; at = prev[at])
        camino.push_back(at);
    std::reverse(camino.begin(), camino.end());

    std::ostringstream oss;
    oss << "Ruta más corta (" << (porDistancia ? "Distancia" : "Tiempo") << "):\n";
    for (size_t i = 0; i < camino.size(); ++i) {
        oss << sedes[camino[i]].codigoIATA;
        if (i < camino.size() - 1) oss << " -> ";
    }
    oss << "\nTotal " << (porDistancia ? "distancia" : "tiempo") << ": " << dist[idxDestino];
    return oss.str();
}

std::string Grafo::sedeMasLejana(const std::string& origen, bool porDistancia) const {
    int idxOrigen = buscarIndicePorIATA(origen);
    if (idxOrigen == -1) {
        return "El código IATA no existe.";
    }
    const double (*matriz)[16] = porDistancia ? matrizDistancia : matrizTiempo;
    std::vector<double> dist;
    std::vector<int> prev;
    dijkstra(idxOrigen, matriz, dist, prev);

    double maxDist = -1;
    int idxLejana = -1;
    for (int i = 0; i < 16; ++i) {
        if (i != idxOrigen && dist[i] != std::numeric_limits<double>::max() && dist[i] > maxDist) {
            maxDist = dist[i];
            idxLejana = i;
        }
    }
    if (idxLejana == -1) {
        return "No se encontró ninguna sede lejana.";
    }
    std::ostringstream oss;
    oss << "Sede más lejana desde " << sedes[idxOrigen].codigoIATA << " (" << (porDistancia ? "Distancia" : "Tiempo") << "):\n";
    oss << sedes[idxLejana].codigoIATA << " - " << sedes[idxLejana].ciudad << "\n";
    oss << "Valor: " << maxDist;
    return oss.str();
}

std::string Grafo::redTransporteEficiente(bool porDistancia) const {
    const double (*matriz)[16] = porDistancia ? matrizDistancia : matrizTiempo;
    std::vector<int> padre;
    prim(matriz, padre);

    std::ostringstream oss;
    oss << "Red de transporte eficiente (" << (porDistancia ? "Distancia" : "Tiempo") << "):\n";
    double total = 0;
    for (int i = 1; i < 16; ++i) {
        if (padre[i] != -1) {
            oss << sedes[padre[i]].codigoIATA << " <-> " << sedes[i].codigoIATA
                << " : " << matriz[padre[i]][i] << "\n";
            total += matriz[padre[i]][i];
        }
    }
    oss << "Total " << (porDistancia ? "distancia" : "tiempo") << ": " << total;
    return oss.str();
}

void Grafo::mostrarSedeInauguracion() const {
    if (sedes.empty()) {
        std::cout << "No hay sedes cargadas\n";
        return;
    }
    const SedeGrafo* antigua = &sedes[0];
    for (const auto& s : sedes) {
        if (s.anyoInauguracion < antigua->anyoInauguracion)
            antigua = &s;
    }
    std::cout << "Sede con inauguración más antigua:\n";
    std::cout << "IATA: " << antigua->codigoIATA << "\nPais: " << antigua->pais << "\nCiudad: " << antigua->ciudad
        << "\nEstadio: " << antigua->estadio << "\nCapacidad: " << antigua->capacidad
        << "\nAño inauguración: " << antigua->anyoInauguracion << "\n";
}

void Grafo::mostrarTodasSedes() const {
    // Implementación básica, solo para evitar el error de linker
    for (const auto& s : sedes) {
        std::cout << s.codigoIATA << " - " << s.ciudad << "\n";
    }
}