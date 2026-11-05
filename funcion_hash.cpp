#include "funcion_hash.h"

size_t funcion_hash(const std::string& clave) {
    
    size_t resultado = 0;

    for (size_t i = 0; i < clave.length(); i++) {
        resultado = resultado * 37 + clave[i];
    }

    return resultado;

}

size_t funcion_hash(int clave) {
    return clave; //como devolvemos size_t automaticamente lo convierte
}
