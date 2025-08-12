#pragma once
#include "Sede.h"

struct NodoAVL {
    Sede sede;
    NodoAVL* izq;
    NodoAVL* der;
    int altura;
    NodoAVL(const Sede& s);
};

// Funciones AVL
int altura(NodoAVL* n);
int balance(NodoAVL* n);
NodoAVL* insertarIATA(NodoAVL* nodo, const Sede& sede);
NodoAVL* insertarAnyo(NodoAVL* nodo, const Sede& sede);
NodoAVL* buscarIATA(NodoAVL* nodo, const std::string& codigo);
NodoAVL* eliminarIATA(NodoAVL* nodo, const std::string& codigo);
void mostrarSede(const Sede& s);
void inOrderIATA(NodoAVL* nodo);
void inOrderAnyo(NodoAVL* nodo);
void mostrarPorPais(NodoAVL* nodo, const std::string& pais);
void mostrarPorCapacidad(NodoAVL* nodo, int capacidad);
void modificarSede(NodoAVL* nodo);
void insertarSede(NodoAVL*& raizIATA, NodoAVL*& raizAnyo, const Sede& sede);
void eliminarSede(NodoAVL*& raizIATA, NodoAVL*& raizAnyo, const std::string& codigo);
void inicializarSedes(NodoAVL*& raizIATA, NodoAVL*& raizAnyo);