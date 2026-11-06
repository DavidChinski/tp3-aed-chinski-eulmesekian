#include <iostream>
#include <string>

#include "tabla_hash.h"

// Programa para explorar funciones de hash: carga en una TablaHash todos los
// jugadores (distintos) de la entrada estandar y muestra cuantas colisiones
// hubo. La funcion que se prueba es la funcion_hash de funcion_hash.cpp.
//
// Uso: ./explorar_hash < datos/goles.txt

// Prueben tambien con otras capacidades iniciales.
const size_t CAPACIDAD_INICIAL = 67;

int main() {
    
    TablaHash<std::string, int> tabla(CAPACIDAD_INICIAL);
    std::string jugador;
    while (std::getline(std::cin, jugador)) {
        if (jugador != "") {
           if (tabla.buscar(jugador) == nullptr){
                tabla.insertar(jugador, 1);
           }
        }
    }

    std::cout << "Claves: " << tabla.cantidad() << std::endl;
    std::cout << "Capacidad: " << tabla.capacidad() << std::endl;
    std::cout << "Colisiones: " << tabla.colisiones() << std::endl;

    return 0;
}
