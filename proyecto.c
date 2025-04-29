#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct directorio { 
    int id;
    char name[50];// 50 para evitar desbordamiento
    char apellido[50];
    char apellidoM[50];
    char tel[10];
};

int menu(){
    int option = 0;
    printf(
        "\nBienvenido!!\n"
        "Por favor seleccione una opcion\n"
        "1.- Agregar contacto\n"
        "2.- Ver datos\n"
        "3.- Buscar alumno\n:"
        "4.- Modificar alumno\n:"
        "5.- Eliminar contacto\n:"
        "6.- Ordenar\n:"
    );
    scanf("%d", &option);
    return option;
}

void verificar_datos(char *cadena, char *destino){
    int validar;
    do{
        validar = 1;
        printf("%s: ", cadena);
        scanf("%39s", destino);
        while(getchar() != '\n');// Limpiar buffer de entrada

        for (int i = 0; destino[i] != '\0' ; i++){
            if (!isalpha(destino[i]) && destino[i] != ' ') {//Si el carácter NO es una letra Y tampoco es un espacio", entonces hay un error      
                printf("Datos invalidos\n"
                "Solo se aceptan letras, ingrese nuevamente los datos por favor.\n");//regresa que encontor un caracter invalido
                validar = 0;
                break;
            }
        }
    } while (!validar);
}

void validar_numero(char *cadena, char *destino){
    int validar;
    do{
        validar = 1;
        printf("%s: ", cadena);
        scanf("%10s", destino);
        while(getchar() != '\n');// Limpiar buffer de entrada

        for (int i = 0; destino[i] != '\0' ; i++){
            if (!isdigit(destino[i])) {//Si el carácter NO es un numero, entonces hay un error      
                printf("Datos invalidos\n"
                "Solo se aceptan numeros, ingrese nuevamente los datos por favor.\n");//regresa que enconto un caracter invalido
                validar = 0;
                break;
            }
        }

        if(validar && strlen(destino) < 10){//validamos su longitud minima
            printf("Minimo 10 digitos requeridos\n");
            validar = 0;
        }

    } while (!validar);
}


void ingresar_datos(struct directorio *alumnos, int id){
    alumnos->id = id;// punteros a estructuras

    verificar_datos("Nombre Completo", alumnos->name);
    verificar_datos("Apellido paterno", alumnos->apellido);
    verificar_datos("Apellido materno", alumnos->apellidoM);
    validar_numero("Numero Telefonico", alumnos->tel);
    //printf("Direccion: ");
    //scanf("%d", &alumnos->tel); // & necesario para el entero
    printf("\n");

}

int encontrar_id(){//esto es para saber el ultimo id ingresado
    FILE *archivo = fopen("contactos.txt", "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

    char linea[100];
    int ultimo_id = 1; 
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        // Eliminar salto de línea
        linea[strcspn(linea, "\n")] = '\0';

        // Buscar si la línea empieza con "id:"
        if (strstr(linea, "ID: ") == linea) {
            int id = atoi(linea + 4); // Convertir a entero el caracter
            ultimo_id = id + 1; // Guardamos el último id
        }
    }

    fclose(archivo);
    return ultimo_id;
}


int main(){
    int option = 0, contactos = 0, conta = 0, capacidad = 10, ultimo_id;
    struct directorio *alumnos = malloc(sizeof(struct directorio) * capacidad); //memoria dinamica
    //struct directorio alumnos[10]; para memoria estatica

    do{
        option = menu();
        FILE *archivo = fopen("contactos.txt", "a"); // Modo append (agregar al final)
        if (archivo == NULL) {
            printf("Error al abrir el archivo.\n");
            break;
        }

       switch (option){
            case 1:
                printf("Cuantos contactos deseas agregar?:");
                scanf("%d", &contactos);

                ultimo_id = encontrar_id();

                for (int i = 0; i < contactos; i++){
                    ingresar_datos(&alumnos[conta], ultimo_id);
                    ultimo_id ++;
                    conta++;
                }

                for (int i = 0; i < conta; i++) {
                    fprintf(archivo, "ID: %d\n", alumnos[i].id);
                    fprintf(archivo, "Nombre: %s\n", alumnos[i].name);
                    fprintf(archivo, "Apellido Paterno: %s\n", alumnos[i].apellido);
                    fprintf(archivo, "Apellido Materno: %s\n", alumnos[i].apellidoM);
                    fprintf(archivo, "Telefono: %s\n", alumnos[i].tel); // Asumiendo que tel es char[]
                    fprintf(archivo, "------------------------\n");
                }

                fclose(archivo);

                printf("%d contactos guardados exitosamente en 'contactos.txt'.\n", contactos);
                break;

            case 2:
                {
                    FILE *archivo = fopen("contactos.txt", "r");
                    if (archivo == NULL) {
                        printf("No hay contactos registrados aun.\n");
                        break;
                    }

                    printf("\n=== LISTA DE CONTACTOS ===\n\n"); 
                    int contacto_actual = 0;
                    char linea[50];

                    while (fgets(linea, sizeof(linea), archivo) != NULL) {
                        // Si la línea es un separador, incrementar contador
                        if (strstr(linea, "---") != NULL) {
                            contacto_actual++;
                            printf("\n");
                        } else {
                            // Eliminar el salto de línea si existe
                            linea[strcspn(linea, "\n")] = '\0';
                            printf("%s\n", linea);
                        }
                    }
                    
                    fclose(archivo);
                    printf("\nTotal: %d contactos\n", contacto_actual);
                    printf("=========================\n");
                    break;
                }
       }

    } while (option != 2);


    return 0;
}
