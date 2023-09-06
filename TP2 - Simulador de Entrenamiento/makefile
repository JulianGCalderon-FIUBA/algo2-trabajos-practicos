CFLAGS=-ggdb -O0 -std=c99 -Wall -Wconversion -Wtype-limits -Werror -I src
VFLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2

valgrind-main: main
	valgrind $(VFLAGS) ./main 2>&1

main: main.c src/*.c src/*.h
	gcc $(CFLAGS) -o main main.c src/*.c 2>&1

valgrind-pruebas-hospital: pruebas-hospital
	valgrind $(VFLAGS) ./pruebas_hospital 2>&1

valgrind-pruebas-heap: pruebas-heap
	valgrind $(VFLAGS) ./pruebas_heap 2>&1

valgrind-pruebas-simulador: pruebas-simulador
	valgrind $(VFLAGS) ./pruebas_simulador 2>&1

pruebas-hospital: pruebas_hospital.c src/*.c src/*.h
	gcc $(CFLAGS) -o pruebas_hospital pruebas_hospital.c src/*.c 2>&1

pruebas-heap: pruebas_heap.c src/*.c src/*.h
	gcc $(CFLAGS) -o pruebas_heap pruebas_heap.c src/*.c 2>&1

pruebas-simulador: pruebas_simulador.c src/*.c src/*.h
	gcc $(CFLAGS) -o pruebas_simulador pruebas_simulador.c src/*.c 2>&1

clean:
	rm -f pruebas_hospital pruebas_heap pruebas_simulador main 2>&1
