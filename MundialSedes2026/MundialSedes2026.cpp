#include <iostream>
#include <algorithm>
#include "AVL.h"
#include "Grafo.h"

void limpiarPantalla() {
    system("cls");
}


// Convierte una cadena a mayusculas
std::string aMayusculas(const std::string& str) {
    std::string resultado = str;
    std::transform(resultado.begin(), resultado.end(), resultado.begin(), ::toupper);
    return resultado;
}

int main() {
    NodoAVL* raizIATA = nullptr;
    NodoAVL* raizAnyo = nullptr;
    inicializarSedes(raizIATA, raizAnyo);

    Grafo grafo;
    grafo.cargarSedes("sedes_grafo.csv");
    grafo.cargarAristas("aristas_grafo.csv");

    int opcion;
    do {
        std::cout << "\n--- MENU SEDES MUNDIAL 2026 ---\n";
        std::cout << "1. Desplegar todas las sedes (IATA ascendente)\n";
        std::cout << "2. Buscar sede por codigo IATA\n";
        std::cout << "3. Insertar nueva sede\n";
        std::cout << "4. Modificar informacion de sede\n";
        std::cout << "5. Eliminar sede\n";
        std::cout << "6. Desplegar sedes de un pais especifico\n";
        std::cout << "7. Desplegar sedes con capacidad >= cantidad dada\n";
        std::cout << "8. Desplegar sedes ordenadas por anno de inauguracion\n";
        std::cout << "9. Ruta mas corta entre sedes (Dijkstra)\n";
        std::cout << "10. Sede mas lejana desde una sede (Dijkstra)\n";
        std::cout << "11. Red de transporte eficiente (Prim)\n";
        std::cout << "12. Mostrar datos de una sede del grafo\n";
        std::cout << "13. Mostrar sede que inaugura el mundial\n";
        std::cout << "0. Salir\nOpcion: ";
        std::cin >> opcion;
        std::cin.ignore();

        limpiarPantalla();

        if (opcion == 1) {
            inOrderIATA(raizIATA);
        }
        else if (opcion == 2) {
            std::string codigo;
            std::cout << "Ingrese codigo IATA: ";
            std::getline(std::cin, codigo);
            codigo = aMayusculas(codigo);
            NodoAVL* nodo = buscarIATA(raizIATA, codigo);
            if (nodo) mostrarSede(nodo->sede);
            else std::cout << "No existe sede con ese codigo.\n";
        }
        else if (opcion == 3) {
            Sede s;
            std::cout << "Ingrese codigo IATA: "; std::getline(std::cin, s.codigoIATA);
            s.codigoIATA = aMayusculas(s.codigoIATA);
            std::cout << "Ingrese pais: "; std::getline(std::cin, s.pais);
            s.pais = aMayusculas(s.pais);
            std::cout << "Ingrese ciudad: "; std::getline(std::cin, s.ciudad);
            std::cout << "Ingrese estadio: "; std::getline(std::cin, s.estadio);
            std::cout << "Ingrese capacidad: "; std::cin >> s.capacidad;
            std::cout << "Ingrese anno de inauguracion: "; std::cin >> s.anyoInauguracion;
            std::cin.ignore();
            insertarSede(raizIATA, raizAnyo, s);
        }
        else if (opcion == 4) {
            std::string codigo;
            std::cout << "Ingrese codigo IATA de la sede a modificar: ";
            std::getline(std::cin, codigo);
            codigo = aMayusculas(codigo);
            NodoAVL* nodo = buscarIATA(raizIATA, codigo);
            if (nodo) modificarSede(nodo);
            else std::cout << "No existe sede con ese codigo.\n";
        }
        else if (opcion == 5) {
            std::string codigo;
            std::cout << "Ingrese codigo IATA de la sede a eliminar: ";
            std::getline(std::cin, codigo);
            codigo = aMayusculas(codigo);
            eliminarSede(raizIATA, raizAnyo, codigo);
        }
        else if (opcion == 6) {
            std::string pais;
            std::cout << "Ingrese pais: ";
            std::getline(std::cin, pais);
            pais = aMayusculas(pais);
            mostrarPorPais(raizIATA, pais);
        }
        else if (opcion == 7) {
            int capacidad;
            std::cout << "Ingrese capacidad minima: ";
            std::cin >> capacidad;
            std::cin.ignore();
            mostrarPorCapacidad(raizIATA, capacidad);
        }
        else if (opcion == 8) {
            inOrderAnyo(raizAnyo);
        }else if (opcion == 9) {
            std::string origen, destino;
            std::cout << "Ingrese IATA origen: "; std::getline(std::cin, origen);
            std::cout << "Ingrese IATA destino: "; std::getline(std::cin, destino);
            std::cout << "Buscar por (1) distancia o (2) tiempo: ";
            int tipo; std::cin >> tipo; std::cin.ignore();
            std::cout << grafo.rutaMasCorta(aMayusculas(origen), aMayusculas(destino), tipo == 1) << "\n";
        }
        else if (opcion == 10) {
            std::string origen;
            std::cout << "Ingrese IATA origen: "; std::getline(std::cin, origen);
            std::cout << "Buscar por (1) distancia o (2) tiempo: ";
            int tipo; std::cin >> tipo; std::cin.ignore();
            std::cout << grafo.sedeMasLejana(aMayusculas(origen), tipo == 1) << "\n";
        }
        else if (opcion == 11) {
            std::cout << "Buscar red de transporte eficiente por (1) distancia o (2) tiempo: ";
            int tipo; std::cin >> tipo; std::cin.ignore();
            std::cout << grafo.redTransporteEficiente(tipo == 1) << "\n";
        }
        else if (opcion == 12) {
            std::string codigo;
            std::cout << "Ingrese codigo IATA: "; std::getline(std::cin, codigo);
            grafo.mostrarSede(aMayusculas(codigo));
        }
        else if (opcion == 13) {
            grafo.mostrarSedeInauguracion();
		}
        if (opcion != 0) {
            std::cout << "\nPresione ENTER para continuar...";
            std::cin.get();
            limpiarPantalla();
        }
    } while (opcion != 0);

    return 0;
}