// Pruebas del TP 3. Se corren con "make test". La parte optativa (los
// listados de Goleadores) se prueba con "make ejemplo-optativo".
//
// Son un punto de partida: que pasen no garantiza que el TP este bien.
// Agreguen las suyas.

#include <iostream>
#include <string>

#include "diccionario.h"
#include "goleadores.h"
#include "tabla_hash.h"

int pruebas_fallidas = 0;

void verificar(bool condicion, const std::string& descripcion) {
    if (condicion) {
        std::cout << "  OK    " << descripcion << std::endl;
    } else {
        std::cout << "  ERROR " << descripcion << std::endl;
        pruebas_fallidas++;
    }
}

// Un tipo de clave cuyo hash es siempre el mismo: todas las claves caen en el
// mismo bucket. Sirve para probar que las colisiones se resuelven bien.
class ClaveQueColisiona {
  public:
    int numero;
};

bool operator==(const ClaveQueColisiona& a, const ClaveQueColisiona& b) {
    return a.numero == b.numero;
}

size_t funcion_hash(const ClaveQueColisiona&) {
    return 7;
}

void pruebas_funcion_hash() {
    std::cout << "funcion_hash" << std::endl;
    verificar(funcion_hash(std::string("Messi")) ==
                  funcion_hash(std::string("Messi")),
              "dos strings iguales tienen el mismo hash");
    verificar(funcion_hash(10) == funcion_hash(10),
              "dos enteros iguales tienen el mismo hash");
}

void pruebas_tabla_hash() {
    std::cout << "TablaHash" << std::endl;
    TablaHash<std::string, int> tabla(16);
    verificar(tabla.cantidad() == 0, "una tabla nueva esta vacia");
    verificar(tabla.capacidad() == 16,
              "una tabla nueva tiene la capacidad inicial pedida");
    verificar(tabla.colisiones() == 0, "una tabla nueva no tiene colisiones");
    verificar(tabla.buscar("Messi") == nullptr,
              "buscar en una tabla vacia devuelve nullptr");

    tabla.insertar("Messi", 7);
    tabla.insertar("Mbappe", 8);
    verificar(tabla.cantidad() == 2, "insertar aumenta la cantidad");
    verificar(tabla.buscar("Messi") != nullptr && *tabla.buscar("Messi") == 7,
              "buscar encuentra el valor insertado");
    verificar(tabla.buscar("Giroud") == nullptr,
              "buscar una clave que no esta devuelve nullptr");

    int* goles = tabla.buscar("Messi");
    if (goles != nullptr) {
        *goles = 10;
    }
    verificar(tabla.buscar("Messi") != nullptr && *tabla.buscar("Messi") == 10,
              "el puntero de buscar permite modificar el valor");

    tabla.eliminar("Messi");
    verificar(tabla.buscar("Messi") == nullptr, "eliminar quita la clave");
    verificar(tabla.buscar("Mbappe") != nullptr,
              "eliminar no toca las demas claves");
    verificar(tabla.cantidad() == 1, "eliminar disminuye la cantidad");
}

void pruebas_colisiones() {
    std::cout << "TablaHash con colisiones" << std::endl;
    TablaHash<ClaveQueColisiona, int> tabla(8);
    for (int i = 0; i < 10; i++) {
        tabla.insertar(ClaveQueColisiona{i}, i * i);
    }
    verificar(tabla.colisiones() == 9,
              "10 claves en un mismo bucket son 9 colisiones");
    bool estan_todas = true;
    for (int i = 0; i < 10; i++) {
        int* valor = tabla.buscar(ClaveQueColisiona{i});
        if (valor == nullptr || *valor != i * i) {
            estan_todas = false;
        }
    }
    verificar(estan_todas, "se encuentran todas las claves del mismo bucket");
    verificar(tabla.buscar(ClaveQueColisiona{10}) == nullptr,
              "una clave que no esta no se confunde con las del bucket");

    // Se elimina del medio, del principio y del final de la lista.
    tabla.eliminar(ClaveQueColisiona{5});
    tabla.eliminar(ClaveQueColisiona{0});
    tabla.eliminar(ClaveQueColisiona{9});
    bool quedaron_bien = tabla.cantidad() == 7;
    for (int i = 0; i < 10; i++) {
        bool deberia_estar = i != 5 && i != 0 && i != 9;
        bool esta = tabla.buscar(ClaveQueColisiona{i}) != nullptr;
        if (esta != deberia_estar) {
            quedaron_bien = false;
        }
    }
    verificar(quedaron_bien, "eliminar funciona en cualquier posicion");
    verificar(tabla.colisiones() == 6,
              "al eliminar bajan las colisiones");
}

void pruebas_redimension() {
    std::cout << "TablaHash que crece" << std::endl;
    const int N = 10000;
    TablaHash<int, int> tabla(1);
    bool carga_acotada = true;
    for (int i = 0; i < N; i++) {
        tabla.insertar(i, 2 * i);
        if (tabla.cantidad() >
            tabla.FACTOR_DE_CARGA_MAXIMO * tabla.capacidad()) {
            carga_acotada = false;
        }
    }
    verificar(tabla.cantidad() == (size_t)N, "se insertaron todas las claves");
    verificar(carga_acotada,
              "el factor de carga nunca supera el maximo permitido");
    verificar(tabla.capacidad() > 1, "la tabla crecio");
    verificar(tabla.colisiones() < tabla.cantidad(),
              "hay menos colisiones que claves");

    bool estan_todas = true;
    for (int i = 0; i < N; i++) {
        int* valor = tabla.buscar(i);
        if (valor == nullptr || *valor != 2 * i) {
            estan_todas = false;
        }
    }
    verificar(estan_todas, "despues de crecer se encuentran todas las claves");

    for (int i = 0; i < N; i++) {
        tabla.eliminar(i);
    }
    verificar(tabla.cantidad() == 0, "se pueden eliminar todas las claves");
}

// Indica si vista[0..n-1] es todo true.
bool todas_vistas(bool* vista, int n) {
    for (int i = 0; i < n; i++) {
        if (!vista[i]) {
            return false;
        }
    }
    return true;
}

void pruebas_iterador_tabla() {
    std::cout << "TablaHash: iterador" << std::endl;
    TablaHash<int, int> vacia(16);
    verificar(!vacia.crear_iterador().hay_mas(),
              "el iterador de una tabla vacia no tiene mas pares");

    // Muchas claves en una tabla que crece.
    const int N = 1000;
    TablaHash<int, int> tabla(4);
    for (int i = 0; i < N; i++) {
        tabla.insertar(i, 2 * i);
    }
    bool* vista = new bool[N];
    for (int i = 0; i < N; i++) {
        vista[i] = false;
    }
    int recorridos = 0;
    bool sin_repetir = true;
    bool valores_bien = true;
    TablaHash<int, int>::Iterador it = tabla.crear_iterador();
    while (it.hay_mas() && recorridos <= N) {
        int clave = it.clave();
        if (clave < 0 || clave >= N || vista[clave]) {
            sin_repetir = false;
        } else {
            vista[clave] = true;
        }
        if (it.valor() != 2 * clave) {
            valores_bien = false;
        }
        recorridos++;
        it.avanzar();
    }
    verificar(recorridos == N, "el iterador recorre tantos pares como claves");
    verificar(sin_repetir && todas_vistas(vista, N),
              "el iterador pasa por cada clave exactamente una vez");
    verificar(valores_bien, "el iterador da el valor de cada clave");
    delete[] vista;

    // Pocas claves en una tabla grande: hay que saltear los buckets vacios.
    TablaHash<int, int> rala(64);
    rala.insertar(3, 30);
    rala.insertar(40, 400);
    rala.insertar(63, 630);
    rala.eliminar(40);
    int suma = 0;
    recorridos = 0;
    TablaHash<int, int>::Iterador it_rala = rala.crear_iterador();
    while (it_rala.hay_mas() && recorridos <= 3) {
        suma = suma + it_rala.clave() + it_rala.valor();
        recorridos++;
        it_rala.avanzar();
    }
    verificar(recorridos == 2 && suma == 3 + 30 + 63 + 630,
              "el iterador saltea los buckets vacios");

    // Todas las claves en el mismo bucket.
    TablaHash<ClaveQueColisiona, int> misma(8);
    for (int i = 0; i < 10; i++) {
        misma.insertar(ClaveQueColisiona{i}, i);
    }
    recorridos = 0;
    TablaHash<ClaveQueColisiona, int>::Iterador it_misma =
        misma.crear_iterador();
    while (it_misma.hay_mas() && recorridos <= 10) {
        recorridos++;
        it_misma.avanzar();
    }
    verificar(recorridos == 10,
              "el iterador recorre todos los pares de un mismo bucket");
}

void pruebas_iterador_diccionario() {
    std::cout << "Diccionario: iterador" << std::endl;
    Diccionario<int, int> vacio;
    verificar(!vacio.crear_iterador().hay_mas(),
              "el iterador de un diccionario vacio no tiene mas asociaciones");

    const int N = 1000;
    Diccionario<int, int> diccionario;
    for (int i = 0; i < N; i++) {
        diccionario.insertar(i, i);
        diccionario.insertar(i, i + 1);
    }
    bool* vista = new bool[N];
    for (int i = 0; i < N; i++) {
        vista[i] = false;
    }
    int recorridos = 0;
    bool sin_repetir = true;
    bool valores_bien = true;
    Diccionario<int, int>::Iterador it = diccionario.crear_iterador();
    while (it.hay_mas() && recorridos <= N) {
        int clave = it.clave();
        if (clave < 0 || clave >= N || vista[clave]) {
            sin_repetir = false;
        } else {
            vista[clave] = true;
        }
        if (it.valor() != clave + 1) {
            valores_bien = false;
        }
        recorridos++;
        it.avanzar();
    }
    verificar(recorridos == N && sin_repetir && todas_vistas(vista, N),
              "el iterador pasa por cada clave exactamente una vez");
    verificar(valores_bien, "el iterador da el ultimo valor de cada clave");
    delete[] vista;
}

void pruebas_diccionario() {
    std::cout << "Diccionario" << std::endl;
    Diccionario<std::string, std::string> capitales;
    verificar(capitales.cantidad() == 0, "un diccionario nuevo esta vacio");
    verificar(!capitales.pertenece("Argentina"),
              "en un diccionario vacio no pertenece ninguna clave");

    capitales.insertar("Argentina", "Buenos Aires");
    capitales.insertar("Francia", "Paris");
    verificar(capitales.pertenece("Argentina"),
              "una clave insertada pertenece");
    verificar(capitales.obtener("Argentina") == "Buenos Aires",
              "obtener devuelve el valor insertado");
    verificar(!capitales.pertenece("Brasil"),
              "una clave que no se inserto no pertenece");
    verificar(capitales.cantidad() == 2, "insertar aumenta la cantidad");

    capitales.insertar("Argentina", "CABA");
    verificar(capitales.obtener("Argentina") == "CABA",
              "insertar una clave que ya estaba reemplaza el valor");
    verificar(capitales.cantidad() == 2,
              "insertar una clave que ya estaba no cambia la cantidad");

    capitales.eliminar("Argentina");
    verificar(!capitales.pertenece("Argentina"), "eliminar quita la clave");
    verificar(capitales.obtener("Francia") == "Paris",
              "eliminar no toca las demas claves");
    verificar(capitales.cantidad() == 1, "eliminar disminuye la cantidad");
}

void pruebas_goleadores() {
    std::cout << "Goleadores" << std::endl;
    Goleadores goleadores;
    verificar(goleadores.cantidad_de_jugadores() == 0,
              "al empezar no hay goleadores");
    verificar(goleadores.goles("Messi") == 0,
              "un jugador que no anoto tiene 0 goles");
    verificar(goleadores.cantidad_de_jugadores() == 0,
              "consultar los goles no agrega al jugador");

    goleadores.anotar_gol("Messi");
    goleadores.anotar_gol("Mbappe");
    goleadores.anotar_gol("Messi");
    verificar(goleadores.goles("Messi") == 2, "anotar_gol suma de a un gol");
    verificar(goleadores.goles("Mbappe") == 1,
              "los goles de cada jugador se cuentan por separado");
    verificar(goleadores.cantidad_de_jugadores() == 2,
              "cada jugador se cuenta una sola vez");
    verificar(goleadores.maximo_goleador() == "Messi",
              "el maximo goleador es el que mas goles tiene");

    goleadores.anotar_gol("Mbappe");
    verificar(goleadores.maximo_goleador() == "Messi",
              "en un empate sigue siendo maximo el que llego primero");
    goleadores.anotar_gol("Mbappe");
    verificar(goleadores.maximo_goleador() == "Mbappe",
              "el maximo goleador cambia cuando lo superan");

    Goleadores otros;
    otros.anotar_gol("Di Maria");
    verificar(otros.maximo_goleador() == "Di Maria",
              "con un solo gol, el maximo goleador es quien lo hizo");
}

int main() {
    pruebas_funcion_hash();
    pruebas_tabla_hash();
    pruebas_colisiones();
    pruebas_redimension();
    pruebas_iterador_tabla();
    pruebas_diccionario();
    pruebas_iterador_diccionario();
    pruebas_goleadores();

    std::cout << std::endl;
    if (pruebas_fallidas == 0) {
        std::cout << "Todas las pruebas pasaron." << std::endl;
        return 0;
    }
    std::cout << "Fallaron " << pruebas_fallidas << " pruebas." << std::endl;
    return 1;
}
