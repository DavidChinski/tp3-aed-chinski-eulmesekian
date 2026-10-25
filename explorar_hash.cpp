#include <iostream>
#include <string>

#include "tabla_hash.h"

// Programa para explorar funciones de hash: carga en una TablaHash todos los
// jugadores (distintos) de la entrada estandar y muestra cuantas colisiones
// hubo. La funcion que se prueba es la funcion_hash de funcion_hash.cpp.
//
// Uso: ./explorar_hash < datos/goles.txt

// Prueben tambien con otras capacidades iniciales.
const size_t CAPACIDAD_INICIAL = 8;

int main() {
    // COMPLETAR: crear una TablaHash<std::string, int> con CAPACIDAD_INICIAL.

    std::string jugador;
    while (std::getline(std::cin, jugador)) {
        if (jugador != "") {
            // En este punto, jugador tiene el nombre de quien hizo el gol.
            // COMPLETAR: si el jugador todavia no esta en la tabla, insertarlo
            // (el valor no importa).
        }
    }

    // COMPLETAR: imprimir la cantidad de claves, la capacidad final y la
    // cantidad de colisiones de la tabla.

    return 0;
}
