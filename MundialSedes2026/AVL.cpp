#include "AVL.h"
#include <iostream>
#include <algorithm>

NodoAVL::NodoAVL(const Sede& s) : sede(s), izq(nullptr), der(nullptr), altura(1) {}

int altura(NodoAVL* n) { return n ? n->altura : 0; }
int balance(NodoAVL* n) { return n ? altura(n->izq) - altura(n->der) : 0; }
int max(int a, int b) { return (a > b) ? a : b; }

NodoAVL* rotarDerecha(NodoAVL* y) {
    NodoAVL* x = y->izq;
    NodoAVL* T2 = x->der;
    x->der = y;
    y->izq = T2;
    y->altura = max(altura(y->izq), altura(y->der)) + 1;
    x->altura = max(altura(x->izq), altura(x->der)) + 1;
    return x;
}

NodoAVL* rotarIzquierda(NodoAVL* x) {
    NodoAVL* y = x->der;
    NodoAVL* T2 = y->izq;
    y->izq = x;
    x->der = T2;
    x->altura = max(altura(x->izq), altura(x->der)) + 1;
    y->altura = max(altura(y->izq), altura(y->der)) + 1;
    return y;
}

NodoAVL* insertarIATA(NodoAVL* nodo, const Sede& sede) {
    if (!nodo) return new NodoAVL(sede);
    if (sede.codigoIATA < nodo->sede.codigoIATA)
        nodo->izq = insertarIATA(nodo->izq, sede);
    else if (sede.codigoIATA > nodo->sede.codigoIATA)
        nodo->der = insertarIATA(nodo->der, sede);
    else
        return nodo;
    nodo->altura = 1 + max(altura(nodo->izq), altura(nodo->der));
    int bal = balance(nodo);
    if (bal > 1 && sede.codigoIATA < nodo->izq->sede.codigoIATA)
        return rotarDerecha(nodo);
    if (bal < -1 && sede.codigoIATA > nodo->der->sede.codigoIATA)
        return rotarIzquierda(nodo);
    if (bal > 1 && sede.codigoIATA > nodo->izq->sede.codigoIATA) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }
    if (bal < -1 && sede.codigoIATA < nodo->der->sede.codigoIATA) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }
    return nodo;
}

NodoAVL* insertarAnyo(NodoAVL* nodo, const Sede& sede) {
    if (!nodo) return new NodoAVL(sede);
    if (sede.anyoInauguracion < nodo->sede.anyoInauguracion)
        nodo->izq = insertarAnyo(nodo->izq, sede);
    else if (sede.anyoInauguracion > nodo->sede.anyoInauguracion)
        nodo->der = insertarAnyo(nodo->der, sede);
    else
        return nodo;
    nodo->altura = 1 + max(altura(nodo->izq), altura(nodo->der));
    int bal = balance(nodo);
    if (bal > 1 && sede.anyoInauguracion < nodo->izq->sede.anyoInauguracion)
        return rotarDerecha(nodo);
    if (bal < -1 && sede.anyoInauguracion > nodo->der->sede.anyoInauguracion)
        return rotarIzquierda(nodo);
    if (bal > 1 && sede.anyoInauguracion > nodo->izq->sede.anyoInauguracion) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }
    if (bal < -1 && sede.anyoInauguracion < nodo->der->sede.anyoInauguracion) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }
    return nodo;
}

NodoAVL* buscarIATA(NodoAVL* nodo, const std::string& codigo) {
    if (!nodo) return nullptr;
    if (codigo == nodo->sede.codigoIATA) return nodo;
    if (codigo < nodo->sede.codigoIATA) return buscarIATA(nodo->izq, codigo);
    return buscarIATA(nodo->der, codigo);
}

NodoAVL* minNodo(NodoAVL* nodo) {
    NodoAVL* actual = nodo;
    while (actual && actual->izq) actual = actual->izq;
    return actual;
}

NodoAVL* eliminarIATA(NodoAVL* nodo, const std::string& codigo) {
    if (!nodo) return nodo;
    if (codigo < nodo->sede.codigoIATA)
        nodo->izq = eliminarIATA(nodo->izq, codigo);
    else if (codigo > nodo->sede.codigoIATA)
        nodo->der = eliminarIATA(nodo->der, codigo);
    else {
        if (!nodo->izq || !nodo->der) {
            NodoAVL* temp = nodo->izq ? nodo->izq : nodo->der;
            if (!temp) {
                temp = nodo;
                nodo = nullptr;
            }
            else
                *nodo = *temp;
            delete temp;
        }
        else {
            NodoAVL* temp = minNodo(nodo->der);
            nodo->sede = temp->sede;
            nodo->der = eliminarIATA(nodo->der, temp->sede.codigoIATA);
        }
    }
    if (!nodo) return nodo;
    nodo->altura = 1 + max(altura(nodo->izq), altura(nodo->der));
    int bal = balance(nodo);
    if (bal > 1 && balance(nodo->izq) >= 0)
        return rotarDerecha(nodo);
    if (bal > 1 && balance(nodo->izq) < 0) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }
    if (bal < -1 && balance(nodo->der) <= 0)
        return rotarIzquierda(nodo);
    if (bal < -1 && balance(nodo->der) > 0) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }
    return nodo;
}

void mostrarSede(const Sede& s) {
    std::cout << "IATA: " << s.codigoIATA << "\nPais: " << s.pais << "\nCiudad: " << s.ciudad
        << "\nEstadio: " << s.estadio << "\nCapacidad: " << s.capacidad
        << "\nAño inauguracion: " << s.anyoInauguracion << "\n----------------------\n";
}

void inOrderIATA(NodoAVL* nodo) {
    if (!nodo) return;
    inOrderIATA(nodo->izq);
    mostrarSede(nodo->sede);
    inOrderIATA(nodo->der);
}

void inOrderAnyo(NodoAVL* nodo) {
    if (!nodo) return;
    inOrderAnyo(nodo->izq);
    mostrarSede(nodo->sede);
    inOrderAnyo(nodo->der);
}

void mostrarPorPais(NodoAVL* nodo, const std::string& pais) {
    if (!nodo) return;
    mostrarPorPais(nodo->izq, pais);
    // Comparar en mayúsculas
    std::string paisNodo = nodo->sede.pais;
    std::transform(paisNodo.begin(), paisNodo.end(), paisNodo.begin(), ::toupper);
    if (paisNodo == pais)
        mostrarSede(nodo->sede);
    mostrarPorPais(nodo->der, pais);
}

void mostrarPorCapacidadRec(NodoAVL* nodo, int capacidad, int& contador) {
    if (!nodo) return;
    mostrarPorCapacidadRec(nodo->izq, capacidad, contador);
    if (nodo->sede.capacidad >= capacidad) {
        mostrarSede(nodo->sede);
        contador++;
    }
    mostrarPorCapacidadRec(nodo->der, capacidad, contador);
}

void mostrarPorCapacidad(NodoAVL* nodo, int capacidad) {
    int contador = 0;
    mostrarPorCapacidadRec(nodo, capacidad, contador);
    if (contador == 0) {
        std::cout << "no hay sedes con el filtro ingresado\n";
    }
}

void modificarSede(NodoAVL* nodo) {
    if (!nodo) return;
    std::cout << "Modificar sede:\n";
    std::cout << "Pais actual: " << nodo->sede.pais << "\nNuevo pais: ";
    std::getline(std::cin, nodo->sede.pais);
    std::cout << "Ciudad actual: " << nodo->sede.ciudad << "\nNueva ciudad: ";
    std::getline(std::cin, nodo->sede.ciudad);
    std::cout << "Estadio actual: " << nodo->sede.estadio << "\nNuevo estadio: ";
    std::getline(std::cin, nodo->sede.estadio);
    std::cout << "Capacidad actual: " << nodo->sede.capacidad << "\nNueva capacidad: ";
    std::cin >> nodo->sede.capacidad;
    std::cout << "Año inauguracion actual: " << nodo->sede.anyoInauguracion << "\nNuevo año: ";
    std::cin >> nodo->sede.anyoInauguracion;
    std::cin.ignore();
}

void insertarSede(NodoAVL*& raizIATA, NodoAVL*& raizAnyo, const Sede& sede) {
    raizIATA = insertarIATA(raizIATA, sede);
    raizAnyo = insertarAnyo(raizAnyo, sede);
}

void eliminarSede(NodoAVL*& raizIATA, NodoAVL*& raizAnyo, const std::string& codigo) {
    NodoAVL* nodo = buscarIATA(raizIATA, codigo);
    if (!nodo) {
        std::cout << "No existe sede con ese código IATA.\n";
        return;
    }
    raizIATA = eliminarIATA(raizIATA, codigo);
    raizAnyo = eliminarIATA(raizAnyo, codigo);
}

void inicializarSedes(NodoAVL*& raizIATA, NodoAVL*& raizAnyo) {
    insertarSede(raizIATA, raizAnyo, {"ATL", "Estados Unidos", "Atlanta", "Mercedes-Benz Stadium", 71000, 2017});
    insertarSede(raizIATA, raizAnyo, {"BOS", "Estados Unidos", "Boston", "Gillette Stadium", 65878, 2002});
    insertarSede(raizIATA, raizAnyo, {"DAL", "Estados Unidos", "Dallas", "AT&T Stadium", 80000, 2009});
    insertarSede(raizIATA, raizAnyo, {"GUAD", "Mexico", "Guadalajara", "Estadio Akron", 48407, 2010});
    insertarSede(raizIATA, raizAnyo, {"HOU", "Estados Unidos", "Houston", "NRG Stadium", 72220, 2002});
    insertarSede(raizIATA, raizAnyo, {"KC", "Estados Unidos", "Kansas City", "Arrowhead Stadium", 76416, 1972});
    insertarSede(raizIATA, raizAnyo, {"LA", "Estados Unidos", "Los Angeles", "SoFi Stadium", 70000, 2020});
    insertarSede(raizIATA, raizAnyo, {"MIA", "Estados Unidos", "Miami", "Hard Rock Stadium", 65326, 1987});
    insertarSede(raizIATA, raizAnyo, {"MON", "Canada", "Montreal", "Olympic Stadium", 66108, 1976});
    insertarSede(raizIATA, raizAnyo, {"MTY", "Mexico", "Monterrey", "Estadio BBVA", 53500, 2015});
    insertarSede(raizIATA, raizAnyo, {"NY", "Estados Unidos", "New York/New Jersey", "MetLife Stadium", 82500, 2010});
    insertarSede(raizIATA, raizAnyo, {"PHI", "Estados Unidos", "Philadelphia", "Lincoln Financial Field", 69596, 2003});
    insertarSede(raizIATA, raizAnyo, {"SEA", "Estados Unidos", "Seattle", "Lumen Field", 72000, 2002});
    insertarSede(raizIATA, raizAnyo, {"SF", "Estados Unidos", "San Francisco Bay Area", "Levi's Stadium", 68500, 2014});
    insertarSede(raizIATA, raizAnyo, {"TOR", "Canada", "Toronto", "BMO Field", 30000, 2007});
    insertarSede(raizIATA, raizAnyo, {"CDMX", "Mexico", "Ciudad de Mexico", "Estadio Azteca", 87000, 1966});
}