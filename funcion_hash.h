#ifndef FUNCION_HASH_H
#define FUNCION_HASH_H

#include <cstddef>
#include <string>

// Funciones de hash: a cada clave le asignan un numero natural.
//
// La TablaHash<K, V> llama a funcion_hash(clave) y despues se queda con el
// resto de dividir por su capacidad. Por eso, para poder usar un tipo K como
// clave de la tabla, tiene que existir una funcion_hash que lo reciba.
//
// Pre:  -
// Post: devuelve un numero que depende unicamente del contenido de la clave
//       (dos claves iguales tienen siempre el mismo hash).
// Costo: O(m), con m el largo del string.
size_t funcion_hash(const std::string& clave);

// Costo: O(1).
size_t funcion_hash(int clave);

#endif
