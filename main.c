#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define BUFFER_SIZE 255

// Función de prueba para la función sanitizar_string
void test_sanitizar_string(); 

// Agrega entrada a un archivo
void agregar_entrada(char nombre_archivo[], char nombre_persona[]); 

// Función para limpiar y formatear una cadena de texto
char* sanitizar_string(char linea[]); 

int main(int argc, char *argv[]) {
    test_sanitizar_string();
    if (argc != 2) {
        printf("Uso: %s <nombre_persona>\n", argv[0]);
        return 1;
    }

    char nombre_persona[strlen(argv[1]) + 1];
    strcpy(nombre_persona, argv[1]);

    char cmd[100];

    system("mkdir -p Entradas Salidas");

    // Vacía el archivo si existe, de lo contrario, crea un archivo nuevo
    sprintf(cmd, "truncate -s 0 Entradas/%s.txt", nombre_persona);
    system(cmd);
    
    // Lista los archivos en la carpeta Textos/<nombre_persona> y guarda la lista en archivo.txt
    sprintf(cmd, "ls Textos/%s > archivo.txt", nombre_persona);
    if(system(cmd)){
        printf("Error: no existe la persona\n");
        return 1;
    }

    // Abre el archivo que contiene los nombres de los textos
    FILE *nombres_textos = fopen("archivo.txt", "r");
    if (nombres_textos == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    char nombre_texto[BUFFER_SIZE];

    // Lee el nombre de los textos uno por uno desde el archivo
    while (fgets(nombre_texto, BUFFER_SIZE, nombres_textos)) {
        nombre_texto[strlen(nombre_texto) - 1] = '\0'; // Elimina el '\n' leído por fgets
        agregar_entrada(nombre_texto, nombre_persona); // Agrega la entrada al archivo correspondiente
    }

    fclose(nombres_textos);
    system("rm archivo.txt");

    // Ejecuta un script Python con el nombre de la persona como argumento
    sprintf(cmd, "python3 main.py %s", nombre_persona);
    system(cmd);

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
        if (isalnum(linea[i])) {
            salida[cont++] = tolower(linea[i]); 
        } else if (linea[i] == ' ' || linea[i] == '\n') {
            if (cont >= 1 && isalpha(salida[cont - 1])) {
                salida[cont++] = ' '; 
            }
        } else if (linea[i] == '.') {
            salida[cont++] = '\n'; 
        }
    }
    salida[cont] = '\0'; 
    return salida;
}

void test_sanitizar_string() {
    char cadena_prueba1[] = "esta es una cadena sin caracteres especiales";
    char cadena_prueba2[] = "Este es un texto.\nCon saltos de\nlinea.";
    char cadena_prueba3[] = "";
    char cadena_prueba4[] = "TEXTO CON LETRAS MAYUSCULAS";
    char cadena_prueba5[] = "Este es un texto.\n Con saltos de \n linea.";

    assert(!strcmp(sanitizar_string(cadena_prueba1), cadena_prueba1));
    assert(!strcmp(sanitizar_string(cadena_prueba2), "este es un texto\ncon saltos de linea\n"));
    assert(!strcmp(sanitizar_string(cadena_prueba3), ""));
    assert(!strcmp(sanitizar_string(cadena_prueba4), "texto con letras mayusculas"));
    assert(!strcmp(sanitizar_string(cadena_prueba5), "este es un texto\ncon saltos de linea\n"));

}
