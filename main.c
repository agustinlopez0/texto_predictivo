#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include <string.h>

#define BUFFER_SIZE 255

// Recibe el nombre de un archivo de texto y el nombre de la persona con la que
//   se esta trabajando, en Entradas/<nombre_persona>.txt escribe
//   el texto del archivo sanitizado, solamente con caracteres alfabetici
void agregar_entrada(char nombre_archivo[], char nombre_persona[]);

void sanitizar(char linea[], FILE *archivo_entradas);


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <nombre_persona>\n", argv[0]);
        return 1;
    }

    char nombre_persona[strlen(argv[1]) + 1];
    strcpy(nombre_persona, argv[1]);

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

    char buff[BUFFER_SIZE];
    while (fgets(buff, BUFFER_SIZE, fp) != NULL) {
        // Eliminar el carácter de nueva línea
        buff[strcspn(buff, "\n")] = '\0';
        agregar_entrada(buff, nombre_persona);
    }

    fclose(fp);


   
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
        sanitizar(buff, archivo_entradas);
    }
    fclose(archivo_textos); 
    fclose(archivo_entradas);
}


void sanitizar(char linea[], FILE *archivo_entradas) {
    for(int i = 0; i < strlen(linea); i++){
        if(isalpha(linea[i]) || linea[i] == ' '){
            fputc(tolower(linea[i]), archivo_entradas);
        } else if(linea[i] == '\n' && linea[i - 1] != '.'){
            fputc(' ', archivo_entradas);
        } else if(linea[i] == '.'){
            fputc('\n', archivo_entradas);
        }
    }
}