#ifndef GOLEADORES_H
#define GOLEADORES_H

#include <cstddef>
#include <string>

#include "diccionario.h"

// TAD Goleadores: la tabla de goleadores de un torneo. A diferencia de los
// dos TADs de abajo, no es generico: las claves son nombres de jugadores
// (string) y los valores son cantidades de goles (int).
//
// Invariante de representacion:
//   - goles_por_jugador tiene como claves exactamente a los jugadores que
//     anotaron al menos un gol, y el valor de cada uno es su cantidad de
//     goles (siempre >= 1).
//   - Si hay al menos un jugador, maximo es el nombre de uno de los que mas
//     goles tiene (solo el nombre: sus goles se consultan en el diccionario).
//
// Complejidades: n es la cantidad de jugadores distintos y m la longitud
// del nombre del jugador (calcular el hash de un string cuesta O(m)).
class Goleadores {
  private:
    Diccionario<std::string, int> goles_por_jugador;
    std::string maximo;

  public:
    // Pre:  -
    // Post: crea una tabla de goleadores sin goles anotados.
    Goleadores();

    // Pre:  -
    // Post: el jugador tiene un gol mas que antes (si no tenia ninguno, pasa
    //       a tener 1).
    // Costo: O(m) amortizado en promedio.
    void anotar_gol(const std::string& jugador);

    // Pre:  -
    // Post: devuelve la cantidad de goles del jugador (0 si no anoto ninguno).
    // Costo: O(m) en promedio.
    int goles(const std::string& jugador);

    // Post: devuelve la cantidad de jugadores con al menos un gol. Costo: O(1).
    size_t cantidad_de_jugadores();

    // Pre:  cantidad_de_jugadores() > 0.
    // Post: devuelve el jugador con mas goles. Si hay varios empatados,
    //       devuelve el que llego primero a esa cantidad de goles.
    // Costo: O(m) en peor caso (no depende de n).
    std::string maximo_goleador();

    // OPTATIVO.
    // Post: imprime una linea "<jugador>: <goles>" por cada jugador, en orden
    //       alfabetico (el orden de < entre strings).
    // Costo: O(n log n) comparaciones de nombres en peor caso.
    void imprimir_por_nombre();

    // OPTATIVO.
    // Post: imprime una linea "<jugador>: <goles>" por cada jugador, de mayor
    //       a menor cantidad de goles. Los jugadores con la misma cantidad de
    //       goles aparecen en orden alfabetico.
    // Costo: O(n log n) comparaciones en peor caso.
    void imprimir_por_goles();
};

#endif
