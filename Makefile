CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

HEADERS = funcion_hash.h tabla_hash.h diccionario.h goleadores.h
FUENTES = funcion_hash.cpp goleadores.cpp
PROGRAMAS = goleadores explorar_hash tests

.PHONY: all test ejemplo ejemplo-optativo valgrind clean

all: $(PROGRAMAS)

goleadores: main.cpp $(FUENTES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ main.cpp $(FUENTES)

%: %.cpp $(FUENTES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $< $(FUENTES)

# Corre las pruebas.
test: tests
	./tests

# Corre el programa con el ejemplo y compara el resumen con el esperado.
ejemplo: goleadores
	./goleadores < datos/ejemplo.txt | head -n 2 | diff - datos/ejemplo_esperado.txt && echo "Salida correcta."

# Idem, incluyendo los dos listados (necesita CON_LISTADOS = true en main.cpp).
ejemplo-optativo: goleadores
	./goleadores < datos/ejemplo.txt | diff - datos/ejemplo_esperado_optativo.txt && echo "Salida correcta."

# Corre las pruebas y el ejemplo revisando el uso de memoria.
valgrind: tests goleadores
	valgrind --leak-check=full --error-exitcode=1 ./tests
	valgrind --leak-check=full --error-exitcode=1 ./goleadores < datos/ejemplo.txt

clean:
	rm -f $(PROGRAMAS)
