#ifndef DICCIONARIO_H
#define DICCIONARIO_H

#include <cstddef>

#include "tabla_hash.h"

// TAD Diccionario<K, V>: asocia claves a valores, con cada clave apareciendo
// a lo sumo una vez. Es la interfaz vista en la teorica (DiccionarioVacio,
// Insertar, Pertenece, Obtener, Eliminar), esta vez representada sobre una tabla de hash.
//
// Invariante de representacion:
//   - tabla es una TablaHash<K, V> que cumple su propio invariante, y sus
//     pares <clave, valor> son exactamente las asociaciones del diccionario.
//
// Complejidades: n es la cantidad de claves del diccionario y c la capacidad
// de la tabla; valen las mismas hipotesis que en TablaHash.
template <typename K, typename V>
class Diccionario {
  private:
    static const size_t CAPACIDAD_INICIAL = 8;

    TablaHash<K, V> tabla;

  public:
    // Pre:  -
    // Post: crea un diccionario vacio.
    // Costo: O(1).
    Diccionario();

    // Pre:  -
    // Post: el diccionario asocia clave -> valor (si la clave ya estaba, se
    //       reemplaza su valor anterior).
    // Costo: O(1) amortizado en promedio, O(n) en peor caso.
    void insertar(const K& clave, const V& valor);

    // Pre:  -
    // Post: devuelve si la clave tiene un valor asociado en el diccionario.
    // Costo: O(1) en promedio, O(n) en peor caso.
    bool pertenece(const K& clave);

    // Pre:  pertenece(clave).
    // Post: devuelve el valor asociado a la clave.
    // Costo: O(1) en promedio, O(n) en peor caso.
    V obtener(const K& clave);

    // Pre:  pertenece(clave).
    // Post: el diccionario ya no tiene la clave (ni su valor).
    // Costo: O(1) en promedio, O(n) en peor caso.
    void eliminar(const K& clave);

    // Post: devuelve la cantidad de claves. Costo: O(1).
    size_t cantidad();

    // Iterador del diccionario: recorre todas las asociaciones clave -> valor,
    // cada una exactamente una vez, en algun orden (ver TAD Iterador en el
    // enunciado). Deja de ser valido si se inserta o se elimina.
    //
    // Invariante de representacion:
    //   - de_la_tabla es un iterador de la tabla del diccionario, y su par
    //     actual es la asociacion actual de este iterador.
    class Iterador {
      private:
        typename TablaHash<K, V>::Iterador de_la_tabla;

        // Para que crear_iterador() pueda armar el iterador.
        friend class Diccionario;

      public:
        // Pre:  -
        // Post: indica si quedan asociaciones por recorrer.
        // Costo: O(1).
        bool hay_mas() {
            // COMPLETAR
            return false;
        }

        // Pre:  hay_mas().
        // Post: devuelve la clave actual, sin modificar el iterador.
        // Costo: O(1).
        K clave() {
            // COMPLETAR (reemplazar la linea de abajo)
            return K();
        }

        // Pre:  hay_mas().
        // Post: devuelve el valor asociado a la clave actual, sin modificar
        //       el iterador.
        // Costo: O(1).
        V valor() {
            // COMPLETAR (reemplazar la linea de abajo)
            return V();
        }

        // Pre:  hay_mas().
        // Post: el iterador pasa a la asociacion siguiente (o al final).
        // Costo: O(c) en peor caso. Recorrer el diccionario entero cuesta
        //        O(n + c).
        void avanzar() {
            // COMPLETAR
        }
    };

    // Pre:  -
    // Post: devuelve un iterador a la primera asociacion del diccionario (si
    //       esta vacio, el iterador ya esta al final).
    // Costo: O(c) en peor caso.
    Iterador crear_iterador();
};

// El constructor ya esta resuelto: crea la tabla con la capacidad inicial.
template <typename K, typename V>
Diccionario<K, V>::Diccionario() : tabla(CAPACIDAD_INICIAL) {
}

template <typename K, typename V>
void Diccionario<K, V>::insertar(const K& clave, const V& valor) {
    // COMPLETAR
}

template <typename K, typename V>
bool Diccionario<K, V>::pertenece(const K& clave) {
    // COMPLETAR
    return false;
}

template <typename K, typename V>
V Diccionario<K, V>::obtener(const K& clave) {
    // COMPLETAR
    return V();
}

template <typename K, typename V>
void Diccionario<K, V>::eliminar(const K& clave) {
    // COMPLETAR
}

template <typename K, typename V>
size_t Diccionario<K, V>::cantidad() {
    // COMPLETAR
    return 0;
}

// El "typename" le avisa al compilador que Diccionario<K, V>::Iterador es un
// tipo.
template <typename K, typename V>
typename Diccionario<K, V>::Iterador Diccionario<K, V>::crear_iterador() {
    Iterador it;
    // COMPLETAR
    return it;
}

#endif
