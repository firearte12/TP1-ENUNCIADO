CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g

.PHONY: all compilar pruebas valgrind clean

compilar:
	$(CC) $(CFLAGS) -Isrc main.c src/tp1.c -o tp1

pruebas:
	$(CC) $(CFLAGS) -Ipruebas -Isrc pruebas/pa2m.c pruebas/pruebas_alumno.c src/tp1.c -o test_tp1
	./test_tp1

valgrind: pruebas
	valgrind --leak-check=full --show-leak-kinds=all ./test_tp1

clean:
	rm -f tp1 test_tp1