#include "src/tp1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void imprimir_pokemon(struct pokemon *p)
{
    const char *rareza_str;
    if (p->rareza == RAREZA_COMUN) {
        rareza_str = "Comun";
    } else if (p->rareza == RAREZA_RARO) {
        rareza_str = "Raro";
    } else {
        rareza_str = "Legendario";
    }

    printf("%s - Velocidad: %d - Peso: %.2f - Rareza: %s\n",
           p->nombre, p->velocidad, p->peso, rareza_str);
}

static bool callback_mostrar_todo(struct pokemon *p, void *extra)
{
    (void)extra;
    imprimir_pokemon(p);
    return true;
}

static bool callback_mostrar_por_rareza(struct pokemon *p, void *extra)
{
    enum rareza_pokemon rareza_buscada = *(enum rareza_pokemon *)extra;
    if (p->rareza == rareza_buscada) {
        imprimir_pokemon(p);
    }
    return true;
}

static void comando_buscar_nombre(tp1_t *tp1, const char *nombre_buscado)
{
    struct pokemon *encontrado = tp1_buscar_pokemon(tp1, nombre_buscado);
    if (encontrado == NULL) {
        printf("No se encontro el pokemon '%s'\n", nombre_buscado);
        return;
    }
    imprimir_pokemon(encontrado);
}

static void comando_mostrar_uno(tp1_t *tp1)
{
    size_t cantidad = tp1_cantidad(tp1);
    if (cantidad == 0) {
        printf("El diccionario esta vacio\n");
        return;
    }
    size_t indice_random = (size_t)(rand() % (int)cantidad);
    struct pokemon *elegido = tp1_buscar_orden(tp1, indice_random);
    imprimir_pokemon(elegido);
}

static void comando_mostrar_todo(tp1_t *tp1)
{
    tp1_iterar(tp1, callback_mostrar_todo, NULL);
}

static void comando_mostrar_rarezas(tp1_t *tp1)
{
    enum rareza_pokemon rarezas[] = {RAREZA_COMUN, RAREZA_RARO, RAREZA_LEGENDARIO};
    const char *titulos[] = {"Comunes", "Raros", "Legendarios"};

    for (int i = 0; i < 3; i++) {
        printf("== %s ==\n", titulos[i]);
        tp1_iterar(tp1, callback_mostrar_por_rareza, &rarezas[i]);
    }
}

int main(int argc, char const *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <archivo.csv> <comando> [argumento]\n", argv[0]);
        return 1;
    }

    const char *archivo = argv[1];
    const char *comando = argv[2];

    tp1_t *tp1 = tp1_leer_archivo(archivo);
    if (tp1 == NULL) {
        fprintf(stderr, "No se pudo leer el archivo '%s'\n", archivo);
        return 1;
    }

    if (strcmp(comando, "buscar-nombre") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Uso: %s <archivo.csv> buscar-nombre <nombre>\n", argv[0]);
            tp1_destruir(tp1);
            return 1;
        }
        comando_buscar_nombre(tp1, argv[3]);
    } else if (strcmp(comando, "mostrar-uno") == 0) {
        srand((unsigned int)time(NULL));
        comando_mostrar_uno(tp1);
    } else if (strcmp(comando, "mostrar-todo") == 0) {
        comando_mostrar_todo(tp1);
    } else if (strcmp(comando, "mostrar-rarezas") == 0) {
        comando_mostrar_rarezas(tp1);
    } else {
        fprintf(stderr, "Comando desconocido: %s\n", comando);
        tp1_destruir(tp1);
        return 1;
    }

    tp1_destruir(tp1);
    return 0;
}
