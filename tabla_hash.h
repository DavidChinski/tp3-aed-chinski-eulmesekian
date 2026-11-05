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



// Invariante de representacion:
//El vector Buckets tiene al menos una posicion. La clave no se repite en el vector.
//La clave cae en la en el int que devuelve la funcion hash mod M, con M = buckets.size   
// n es cantidad total de nodos
// n <= FACTOR_DE_CARGA_MAXIMO * buckets.size();


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

    // Duplica la cantidad de buckets y reubica todos los nodos en el bucket
    // que les corresponde con la nueva capacidad. La usa insertar.
    void redimensionar();

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
    

    //----------------------------------------------
    // Invariante de representacion:
    // si actual = nullptr no hay mas por recorrer
    // si actual != nullptr, entonces indice < buckets.size()
    // el atributo tabla apunta a la tabla que queremos recorrer
    class Iterador {
      private:
        // COMPLETAR: la representacion la eligen ustedes. Desde aca se puede
        // usar la parte privada de la tabla.
        TablaHash* tabla; 
        size_t indice;     
        Nodo* actual;      

        // Para que crear_iterador() pueda armar el iterador.
        friend class TablaHash;

      public:
        // Pre:  -
        // Post: indica si quedan pares por recorrer.
        // Costo: O(1).
        bool hay_mas() {
            return actual != nullptr;
        }

        // Pre:  hay_mas().
        // Post: devuelve la clave del par actual, sin modificar el iterador.
        // Costo: O(1).
        K clave() {
            return actual->clave;
        }

        // Pre:  hay_mas().
        // Post: devuelve el valor del par actual, sin modificar el iterador.
        // Costo: O(1).
        V valor() {
            return actual->valor;
        }

        // Pre:  hay_mas().
        // Post: el iterador pasa al par siguiente (o al final, si no hay mas).
        // Costo: O(c) en peor caso. Recorrer la tabla entera (crear el
        //        iterador y avanzar hasta el final) cuesta O(n + c).
        void avanzar() {
            actual = actual->sig;
            if (actual== nullptr){
                indice++;
                for (size_t i = indice; i < tabla->buckets.size(); i++)
                {
                    if (tabla->buckets[i] != nullptr){
                        indice = i;
                        actual = tabla->buckets[i];
                        break;
                    }
                }
            }  
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
// lista. No chequea si la clave ya esta por la precondicion. Suma 1 a n.
// Si con la clave nueva se superaria el factor de carga maximo, primero
// redimensiona (antes de calcular el indice, que depende de la capacidad) 
template <typename K, typename V>
void TablaHash<K, V>::insertar(const K& clave, const V& valor) {
    // condicion para mantener el mismo factor de carga y redistribuir la sobrecarga
    if (n + 1 > FACTOR_DE_CARGA_MAXIMO * buckets.size()) {
        redimensionar();
    }

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

// Cuenta los buckets no vacios y devuelve n menos esa cantidad
// (claves que comparten bucket con otra que llego antes).
template <typename K, typename V>
size_t TablaHash<K, V>::colisiones() {
    size_t cont = 0;
    for (size_t i = 0; i < buckets.size(); i++){
        if (buckets[i] != nullptr) cont++;
    }
    
    return n - cont;
}

// Crea un vector con el doble de buckets y mueve cada nodo (sin copiarlo) al
// bucket funcion_hash(clave) % nueva capacidad, enganchandolo al principio.
// Al final reemplaza buckets por el vector nuevo. n no cambia.
template <typename K, typename V>
void TablaHash<K, V>::redimensionar() {

    std::vector<Nodo*> nuevo;
    for (size_t i = 0; i < 2*buckets.size(); i++) {
        nuevo.push_back(nullptr);
    }

    for (size_t i = 0; i < buckets.size(); i++){
        Nodo* actual = buckets[i];
        while (actual != nullptr){
            Nodo* siguiente = actual->sig;
            size_t indice = funcion_hash(actual->clave) % nuevo.size();
            actual->sig = nuevo[indice];
            nuevo[indice] = actual;
            actual=siguiente;
        }     
    }

    buckets = nuevo; 
    
}

// Crea un iterador sobre esta tabla, parado en el primer nodo del primer
// bucket no vacio (o al final si la tabla esta vacia).
// El "typename" le avisa al compilador que TablaHash<K, V>::Iterador es un tipo.
template <typename K, typename V>
typename TablaHash<K, V>::Iterador TablaHash<K, V>::crear_iterador() {
    Iterador it;
    it.tabla = this;
    it.indice = 0;
    it.actual = nullptr;
    for (size_t i = 0; i < buckets.size(); i++){
        if (buckets[i] != nullptr){
            it.indice = i;
            it.actual = buckets[i];
            break;
        }
    }
    return it;

}

#endif
