#ifndef TABLA_HASH_H
#define TABLA_HASH_H

#include <cstddef>
#include "funcion_hash.h"

#include <vector>

// TAD TablaHash<K, V>: tabla de hash con direccionamiento cerrado.
//
// Guarda pares <clave, valor> repartidos en buckets: el par de una clave esta
// en el bucket numero funcion_hash(clave) % capacidad().
//
// Requisitos sobre los tipos:
//   - K: se puede copiar, comparar con == y existe funcion_hash(K).
//   - V: se puede copiar.
//
// Invariante de representacion:
//   COMPLETAR: escriban aca el invariante de la representacion que elijan.

//El vector Buckets tiene al menos una posicion. La clave no es repite en el vector.
//La clave cae en la en el int que devuelve la funcion hash mod N, con N = buckets.size   


//
// Complejidades: n es la cantidad de claves guardadas y c la capacidad.
// "Promedio" supone que la funcion de hash reparte las claves de manera
// uniforme y que calcularla (y comparar dos claves) cuesta O(1).
template <typename K, typename V>
class TablaHash {
  private:
    // COMPLETAR: la representacion la eligen ustedes (que guardan, y que hay
    // en cada bucket). Pueden agregar tambien funciones auxiliares privadas.
    // Lo que el Iterador necesite de aca tiene que estar declarado antes que el.
    struct Nodo {
        K clave;
        V valor;
        Nodo* sig;
        Nodo(const K& c, const V& v, Nodo* s) : clave(c), valor(v), sig(s) {}
    };
    std::vector<Nodo*> buckets;
    size_t n;    

  public:
    static const size_t FACTOR_DE_CARGA_MAXIMO = 2;

    // Pre:  capacidad_inicial >= 1.
    // Post: crea una tabla vacia, con capacidad_inicial buckets.
    // Costo: O(capacidad_inicial).
    TablaHash(size_t capacidad_inicial);

    // Post: libera toda la memoria pedida por la tabla.
    // Costo: O(n + c) en peor caso.
    ~TablaHash();

    // Copiar una tabla copiaria los punteros y no su contenido: lo prohibimos.
    TablaHash(const TablaHash&) = delete;
    TablaHash& operator=(const TablaHash&) = delete;

    // Pre:  la clave NO esta en la tabla.
    // Post: la tabla tiene ademas el par <clave, valor>, y
    //       cantidad() <= FACTOR_DE_CARGA_MAXIMO * capacidad().
    // Costo: O(1) amortizado (en peor caso O(n), cuando toca redimensionar).
    void insertar(const K& clave, const V& valor);

    // Pre:  la clave esta en la tabla.
    // Post: la tabla ya no tiene la clave (ni su valor).
    // Costo: O(1) en promedio, O(n) en peor caso.
    void eliminar(const K& clave);

    // Pre:  -
    // Post: si la clave esta, devuelve un puntero a su valor (que se puede
    //       usar para leerlo o modificarlo); si no, devuelve nullptr.
    //       El puntero deja de ser valido al insertar o eliminar.
    // Costo: O(1) en promedio, O(n) en peor caso.
    V* buscar(const K& clave);

    // Post: devuelve la cantidad de claves guardadas. Costo: O(1).
    size_t cantidad();

    // Post: devuelve la cantidad de buckets. Costo: O(1).
    size_t capacidad();

    // Post: devuelve la cantidad de claves que comparten bucket con alguna
    //       clave que llego antes, es decir:
    //       cantidad() - (cantidad de buckets que no estan vacios).
    // Costo: O(c) en peor caso.
    size_t colisiones();

    // Iterador de la tabla: recorre todos los pares <clave, valor>, cada uno
    // exactamente una vez, en algun orden (ver TAD Iterador en el enunciado).
    // Deja de ser valido si se inserta o se elimina en la tabla.
    //
    // Invariante de representacion:
    //   COMPLETAR: escriban aca el invariante de la representacion que elijan.
    class Iterador {
      private:
        // COMPLETAR: la representacion la eligen ustedes. Desde aca se puede
        // usar la parte privada de la tabla.

        // Para que crear_iterador() pueda armar el iterador.
        friend class TablaHash;

      public:
        // Pre:  -
        // Post: indica si quedan pares por recorrer.
        // Costo: O(1).
        bool hay_mas() {
            // COMPLETAR
            return false;
        }

        // Pre:  hay_mas().
        // Post: devuelve la clave del par actual, sin modificar el iterador.
        // Costo: O(1).
        K clave() {
            // COMPLETAR (reemplazar la linea de abajo)
            return K();
        }

        // Pre:  hay_mas().
        // Post: devuelve el valor del par actual, sin modificar el iterador.
        // Costo: O(1).
        V valor() {
            // COMPLETAR (reemplazar la linea de abajo)
            return V();
        }

        // Pre:  hay_mas().
        // Post: el iterador pasa al par siguiente (o al final, si no hay mas).
        // Costo: O(c) en peor caso. Recorrer la tabla entera (crear el
        //        iterador y avanzar hasta el final) cuesta O(n + c).
        void avanzar() {
            // COMPLETAR
        }
    };

    // Pre:  -
    // Post: devuelve un iterador al primer par de la tabla (si la tabla esta
    //       vacia, el iterador ya esta al final).
    // Costo: O(c) en peor caso.
    Iterador crear_iterador();
};

// ---------------------------------------------------------------------------
// Implementacion. Al ser una clase template, va en el mismo .h.
// ---------------------------------------------------------------------------

// Crea la tabla vacia: n = 0 y capacidad_inicial buckets, todos en nullptr.
template <typename K, typename V>
TablaHash<K, V>::TablaHash(size_t capacidad_inicial) {
    n = 0;
    for (size_t i = 0; i < capacidad_inicial; i++) {
        buckets.push_back(nullptr);
    }
}

// Recorre cada bucket y borra (delete) todos los nodos de su lista,
// guardando el siguiente antes de borrar el actual.
template <typename K, typename V>
TablaHash<K, V>::~TablaHash() {
    for (size_t i = 0; i < buckets.size(); i++) {
        Nodo* actual = buckets[i];
        while (actual != nullptr) {
            Nodo* siguiente = actual->sig;
            delete actual;
            actual = siguiente;
        }
    }
}

// Calcula el bucket de la clave y agrega un nodo nuevo al principio de su
// lista. No chequea si la clave ya esta (pre: no esta). Suma 1 a n.
// FALTA: redimensionar cuando se supera el factor de carga.
template <typename K, typename V>
void TablaHash<K, V>::insertar(const K& clave, const V& valor) {
    
    size_t indice = funcion_hash(clave) % buckets.size();
    Nodo* actual = buckets[indice];
    Nodo* a_insertar = new Nodo(clave, valor, nullptr);
    
    if (actual == nullptr){
        buckets[indice] = a_insertar;
        n++;
        return;
    }
    a_insertar->sig = buckets[indice];
    buckets[indice] = a_insertar;
    n++;

    return;
    
}

// Calcula el bucket de la clave, la busca en la lista y desengancha y borra
// su nodo (caso aparte si es la cabeza). Resta 1 a n. Pre: la clave esta.
template <typename K, typename V>
void TablaHash<K, V>::eliminar(const K& clave) {
    
    size_t indice = funcion_hash(clave) % buckets.size();

    Nodo* actual = buckets[indice];
    if (actual == nullptr) return;
    if (actual->sig == nullptr && actual->clave == clave){
        buckets[indice] = nullptr;
        delete actual;
        n--;
        return;
    }
    if (actual->clave == clave){
        buckets[indice] = actual->sig;
        delete actual;
        n--;
        return;
    }
    
    Nodo* anterior = nullptr;
    while (actual != nullptr){
        if (actual->clave == clave){
            anterior->sig = actual->sig;
            delete actual;
            n--;
            return;
        }
        anterior = actual;
        actual = actual->sig;
    }

}


// Calcula el bucket de la clave y recorre su lista. Si encuentra la clave,
// devuelve un puntero a su valor (para leerlo o modificarlo); si no, nullptr.
template <typename K, typename V>
V* TablaHash<K, V>::buscar(const K& clave) {
    
    size_t indice = funcion_hash(clave) % buckets.size();
    Nodo* actual = buckets[indice];
    while (actual != nullptr){
        if (actual->clave == clave){
            return &actual->valor;
        }
        actual = actual->sig; 
    }
    return nullptr;
}

// Devuelve n, la cantidad de claves guardadas.
template <typename K, typename V>
size_t TablaHash<K, V>::cantidad() {
    return n;
}

// Devuelve la cantidad de buckets (el tamanio del vector).
template <typename K, typename V>
size_t TablaHash<K, V>::capacidad() {
    return buckets.size();
}

// Devuelve n menos la cantidad de buckets no vacios. FALTA implementar.
template <typename K, typename V>
size_t TablaHash<K, V>::colisiones() {
    // COMPLETAR
    return 0;
}

// El "typename" le avisa al compilador que TablaHash<K, V>::Iterador es un tipo.
template <typename K, typename V>
typename TablaHash<K, V>::Iterador TablaHash<K, V>::crear_iterador() {
    Iterador it;
    // COMPLETAR
    return it;
}

#endif
