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

void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

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

void validar_numero(char *cadena, char *destino){
    int validar;
    do{
        validar = 1;
        printf("%s", cadena);
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

void verificar_datos(const char *mensaje, char *destino, size_t tamaño) {
    char buffer[100];
    int valido;
    
    do {
        valido = 1;
        printf("%s", mensaje);
        fgets(buffer, sizeof(buffer), stdin);
        
        // Eliminar salto de línea si existe
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len-1] == '\n') {
            buffer[len-1] = '\0';
        } else if (len == sizeof(buffer) - 1) { // Manejar líneas muy largas
            limpiar_buffer();
        }
        
        // Validar que no esté vacío
        if (strlen(buffer) == 0) {
            printf("Error: No se ingresó ningún dato.\n");
            valido = 0;
            continue;
        }
        
        // Validar caracteres
        for (size_t i = 0; buffer[i] != '\0'; i++) {
            if (!isalpha((unsigned char)buffer[i]) && buffer[i] != ' ') {
                printf("Error: Solo se permiten letras y espacios.\n");
                valido = 0;
                break;
            }
        }
        
        if (valido) {
            strncpy(destino, buffer, tamaño-1);
            destino[tamaño-1] = '\0';
        }
    } while (!valido);
}

void ingresar_datos(struct directorio *alumnos, int id){
    alumnos->id = id;// punteros a estructuras

    verificar_datos("Nombre Completo: ", alumnos->name, sizeof(alumnos->name));
    verificar_datos("Apellido paterno: ", alumnos->apellido, sizeof(alumnos->apellido));
    verificar_datos("Apellido materno: ", alumnos->apellidoM, sizeof(alumnos->apellidoM));
    validar_numero("Numero Telefonico: ", alumnos->tel);
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

void busqueda_datos(const char *valor_buscado) {
    char linea[256];//para guardar la linea en el archivo
    char busqueda[50];// guarda el valor a buscar
    int num_linea = 0;
    int encontrados = 0;
    int contacto_actual = 0;
    char contacto_completo[5][256]; // Para almacenar todas las líneas del contacto actual
    int lineas_contacto = 0;

    FILE *archivo = fopen("contactos.txt", "r");
    if (!archivo) {
        perror("Error al abrir el archivo");
        return;
    }

    printf("Ingresa el %s que desea buscar: ", valor_buscado);
    fgets(busqueda, sizeof(busqueda), stdin);
    busqueda[strcspn(busqueda, "\n")] = 0;

    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = '\0';
        num_linea++;

        // Guardamos cada línea del contacto actual
        if (lineas_contacto < 5) {
            strcpy(contacto_completo[lineas_contacto], linea);
            lineas_contacto++;
        }

        // Buscamos coincidencia en el campo correspondiente
        if (strstr(linea, valor_buscado) != NULL && strstr(linea, busqueda) != NULL) {
            //printf("\nContacto encontrado:\n");
            for (int i = 0; i < 5; i++) {
                printf("%s\n", contacto_completo[i]);
            }
            encontrados++;
        }

        // Reiniciamos para el próximo contacto
        if (strstr(linea, "------------------------") != NULL) {
            lineas_contacto = 0;
            contacto_actual++;
        }
    }

    fclose(archivo);

    if (encontrados == 0) {
        printf("No se encontraron contactos con ese %s.\n", valor_buscado);
    } else {
        printf("\nAlumnos encontrados: %d\n", encontrados);
    }
}

void busqueda_alumno(){
    int opcion = 0;
    char condicion;

    printf("Ingrese el numero de la busqueda que desa realizar:\n"
        "1.- Nombre\n"
        "2.- Apellido Paterno\n"
        "3.- Apellido Materno\n"
        "4.- Telefono\n"
        "5.- ID\n"
    );
    scanf("%d", &opcion);
    while (getchar() != '\n'); // Limpiar buffer


    switch (opcion){
        case 1: busqueda_datos("Nombre"); break;
        case 2: busqueda_datos("Apellido Paterno"); break;
        case 3: busqueda_datos("Apellido Materno"); break;
        case 4: busqueda_datos("Telefono"); break;
        case 5: busqueda_datos("ID"); break;
        default: printf("Opción inválida.\n");
    }
    printf("Desea hacer otra busqueda? (S/N)");
    scanf(" %c", &condicion);  // El espacio antes de %c limpia el espacion en blanco
    while(getchar() != '\n');  // Limpiar el buffer de entrada

    if (tolower(condicion) == 's'){//Convierte a minusculas y compara
        system("cls");
        busqueda_alumno();  
    }    
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
        system("cls"); 

        switch (option){
            case 1:
                printf("Cuantos contactos deseas agregar?:");
                scanf("%d", &contactos);
                limpiar_buffer();

                ultimo_id = encontrar_id();

                for (int i = 0; i < contactos; i++){
                    if (conta >= capacidad) {
                        capacidad *= 2;
                        struct directorio *temp = realloc(alumnos, sizeof(struct directorio) * capacidad);
                        if (!temp) {
                            printf("Error: No se pudo expandir la memoria.\n");
                            break;
                        }
                        alumnos = temp;
                    }
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
                getchar();
                system("cls"); 
                break;

            case 2:
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
                    printf("\nPresione Enter para regresar al menu...");
                    while (getchar() != '\n'); // Limpia el buffer y espera Enter
                    getchar();
                    
                    system("cls"); 
                    break;
            case 3:
                busqueda_alumno();
                getchar();
                system("cls"); 
       }

    } while (option != 10);


    return 0;
}