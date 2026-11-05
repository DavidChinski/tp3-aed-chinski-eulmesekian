#include "goleadores.h"

#include <iostream>

Goleadores::Goleadores() {
    // COMPLETAR (si hace falta)
}

void Goleadores::anotar_gol(const std::string& jugador) {

    goles_por_jugador.insertar(jugador, goles(jugador) + 1); //si el jugador no estbaa, ahora lo mete con 1 gol, y si ya estaba simplemente lo guarda ocn un gol mas.

    if(goles(jugador) > goles(maximo)){
        maximo = jugador;
    }
}   // El maximo solo cambia si lo pasa estrictamente de goles, ya que si hay empate el jugador maximo llego a la cantidad de goles antes que el jugador qu ele sumamos un gol. (contempla el caso de cuando recien insertas el primer jugador xq goles devuelve 0 si no esta)


int Goleadores::goles(const std::string& jugador) {
    
    if (goles_por_jugador.pertenece(jugador)){
        return goles_por_jugador.obtener(jugador);
    }
    else{
        return 0;
    }
    
}

size_t Goleadores::cantidad_de_jugadores() {
    
    return goles_por_jugador.cantidad();
}

std::string Goleadores::maximo_goleador() {
    // COMPLETAR
    return maximo;
}

void Goleadores::imprimir_por_nombre() {
    // OPTATIVO
}

void Goleadores::imprimir_por_goles() {
    // OPTATIVO
}
