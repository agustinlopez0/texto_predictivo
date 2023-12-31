#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define BUFFER_SIZE 255
#define CMD_SIZE 200

// Función de prueba para la función sanitizar_string
void test_sanitizar_string(); 

// Recibe el nombre de un archivo de texto y el nombre de la persona y
// agrega el contenido sanitizado al archivo Entrada/<nombre_persona>.txt 
void agregar_entrada(char nombre_archivo[], char nombre_persona[]); 

// Devuelve la cadena sanitizada
char* sanitizar_string(char linea[]); 

int main(int argc, char *argv[]) {
    test_sanitizar_string();
    if (argc != 2) {
        printf("Uso: %s <nombre_persona>\n", argv[0]);
        return 1;
    }

    char nombre_persona[strlen(argv[1]) + 1];
    strcpy(nombre_persona, argv[1]);

    char cmd[CMD_SIZE];

    system("mkdir -p Entradas Salidas");
    
    
    // Lista los archivos en la carpeta Textos/<nombre_persona> y guarda la lista en archivo.txt
    sprintf(cmd, "ls Textos/%s > archivo.txt", nombre_persona);
    if(system(cmd)){
        printf("Error: no existe la persona\n");
        return 1;
    }

    // Vacía el archivo si existe, de lo contrario, crea un archivo nuevo
    sprintf(cmd, "truncate -s 0 Entradas/%s.txt", nombre_persona);
    system(cmd);

    // Abre el archivo que contiene los nombres de los textos
    FILE *nombres_textos = fopen("archivo.txt", "r");

    char nombre_texto[BUFFER_SIZE];

    // Lee el nombre de los textos uno por uno desde el archivo
    while (fgets(nombre_texto, BUFFER_SIZE, nombres_textos)) {
        nombre_texto[strlen(nombre_texto) - 1] = '\0'; // Elimina el '\n' leído por fgets
        agregar_entrada(nombre_texto, nombre_persona); // Agrega la entrada al archivo correspondiente
    }

    fclose(nombres_textos);
    system("rm archivo.txt");

    sprintf(cmd, "python3 main.py %s", nombre_persona);
    system(cmd);

    printf("Ejecucion terminada: %s\n", nombre_persona);
    return 0;
}

void agregar_entrada(char nombre_archivo[], char nombre_persona[]) {
    char ruta_textos[100], ruta_entradas[100];

    sprintf(ruta_textos, "Textos/%s/%s", nombre_persona, nombre_archivo);
    sprintf(ruta_entradas, "Entradas/%s.txt", nombre_persona);

    FILE *archivo_textos = fopen(ruta_textos, "r"), *archivo_entradas = fopen(ruta_entradas, "a");
    if (!archivo_textos || !archivo_entradas) {
        perror("Error al abrir el archivo");
        return;
    }

    char buff[BUFFER_SIZE];
    // Lee el contenido del archivo de texto y lo limpia usando la función sanitizar_string
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
    for (int i = 0; i < strlen(linea); i++) {
        if (isalpha(linea[i])) {
            salida[cont++] = tolower(linea[i]); 
        } else if (linea[i] == ' ' || linea[i] == '\n') {
            if (cont >= 1 && isalpha(salida[cont - 1])) {
                salida[cont++] = ' '; 
            }
        } else if (linea[i] == '.') {
            if(cont >= 1 && salida[cont - 1] != '\n'){
                salida[cont++] = '\n';
            }
        }
    }
    salida[cont] = '\0'; 
    return salida;
}

void test_sanitizar_string() {
    char* cadena_prueba1 = sanitizar_string("esta es una cadena sin caracteres especiales");
    char* cadena_prueba2 = sanitizar_string("Este es un texto.\n\nCon    saltos de\nlinea.");
    char* cadena_prueba3 = sanitizar_string("");
    char* cadena_prueba4 = sanitizar_string("TEXTO CON LETRAS MAYUSCULAS");
    char* cadena_prueba5 = sanitizar_string("Este es un texto.\n Con saltos de \n linea.");

    assert(!strcmp(cadena_prueba1, cadena_prueba1));
    assert(!strcmp(cadena_prueba2, "este es un texto\ncon saltos de linea\n"));
    assert(!strcmp(cadena_prueba3, ""));
    assert(!strcmp(cadena_prueba4, "texto con letras mayusculas"));
    assert(!strcmp(cadena_prueba5, "este es un texto\ncon saltos de linea\n"));

    free(cadena_prueba1);
    free(cadena_prueba2);
    free(cadena_prueba3);
    free(cadena_prueba4);
    free(cadena_prueba5);
}
