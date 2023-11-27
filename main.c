#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>
#include <assert.h>

#define BUFFER_SIZE 255

void test_sanitizar_string();

// Recibe el nombre de un archivo de texto y el nombre de la persona con la que
//   se esta trabajando, en Entradas/<nombre_persona>.txt escribe
//   el texto del archivo sanitizado, solamente con caracteres alfabetici
void agregar_entrada(char nombre_archivo[], char nombre_persona[]);

char* sanitizar_string(char linea[]);

int main(int argc, char *argv[]) {
    // test_sanitizar_string();
    if (argc != 2) {
        printf("Uso: %s <nombre_persona>\n", argv[0]);
        return 1;
    }

    char nombre_persona[strlen(argv[1]) + 1];
    strcpy(nombre_persona, argv[1]);

    // Si no existe Entradas/<nombre_persona>.txt lo crea y si existe
    //   lo deja en blanco
    char cmd[100];
    sprintf(cmd, "truncate -s 0 Entradas/%s.txt", nombre_persona);
    system(cmd);
    
    sprintf(cmd, "ls Textos/%s > archivo.txt", nombre_persona);
    system(cmd);

    FILE *fp = fopen("archivo.txt", "r");
    if (fp == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    char nombre_archivo[BUFFER_SIZE];
    while (fgets(nombre_archivo, BUFFER_SIZE, fp) != NULL) {
        // Elimina el '\n'
        nombre_archivo[strcspn(nombre_archivo, "\n")] = '\0';
        agregar_entrada(nombre_archivo, nombre_persona);
    }

    fclose(fp);

    sprintf(cmd, "python3 main.py %s", nombre_persona);
    system(cmd);
   
    return 0;
}

void agregar_entrada(char nombre_archivo[], char nombre_persona[]){
    char ruta_textos[100], ruta_entradas[100];
    sprintf(ruta_textos, "Textos/%s/%s", nombre_persona, nombre_archivo);
    sprintf(ruta_entradas, "Entradas/%s.txt", nombre_persona);

    FILE *archivo_textos = fopen(ruta_textos, "r"), *archivo_entradas = fopen(ruta_entradas, "a");
    if (!archivo_textos || !archivo_entradas) {
        perror("Error al abrir el archivo");
        return;
    }

    char buff[BUFFER_SIZE];
    while (fgets(buff, BUFFER_SIZE, archivo_textos) != NULL) {
        char* aux = sanitizar_string(buff);
        fputs(aux, archivo_entradas);
        free(aux);
    }
    fclose(archivo_textos); 
    fclose(archivo_entradas);
}


char* sanitizar_string(char linea[]) {
    char* salida = malloc(sizeof(char) * strlen(linea) + 1);
    int cont = 0;
    for(int i = 0; i < strlen(linea); i++){
        if(isalpha(linea[i]) || linea[i] == ' '){
            salida[cont++] = tolower(linea[i]);
        } else if(linea[i] == '\n' && linea[i - 1] != '.'){
            salida[cont++] = ' ';
        } else if(linea[i] == '.'){
            salida[cont++] = '\n';
        }
    }
    salida[cont] = '\0';
    return salida;
}

void test_sanitizar_string() {
    // Casos de prueba
    char cadena_prueba1[] = "esta es una cadena sin caracteres especiales";
    char cadena_prueba2[] = "Este es un texto.\nCon saltos de\nlinea.";
    char cadena_prueba3[] = "";
    char cadena_prueba4[] = "TEXTO CON LETRAS MAYUSCULAS";

    // Ejecución de las pruebas
    assert(strcmp(sanitizar_string(cadena_prueba1), cadena_prueba1) == 0);
    assert(strcmp(sanitizar_string(cadena_prueba2), "este es un texto\ncon saltos de linea\n") == 0);
    assert(strcmp(sanitizar_string(cadena_prueba3), "") == 0);
    assert(strcmp(sanitizar_string(cadena_prueba4), "texto con letras mayusculas") == 0);

    printf("Todas las pruebas han pasado correctamente.\n");
}
