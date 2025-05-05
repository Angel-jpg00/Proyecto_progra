#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
 
#define maximo_contactos 500
 
typedef struct directorio {
    int matricula;
    char name[50];// 50 para evitar desbordamiento
    char apellido[50];
    char apellidoM[50];
    char tel[11];
    char domicilio[80];
    char licenciatura[50];
}directorio;
 
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
 
void validar_numero(char *cadena, char *destino) {
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
 
void verificar_datos(const char *mensaje, char *destino, size_t tamano) {
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
            strncpy(destino, buffer, tamano-1);
            destino[tamano-1] = '\0';
        }
    } while (!valido);
}
 
void ingresar_datos(struct directorio *alumnos) {
    printf("Matrícula: ");
    scanf("%d", &alumnos->matricula);
    limpiar_buffer();
    verificar_datos("Nombre Completo: ", alumnos->name, sizeof(alumnos->name));
    verificar_datos("Apellido paterno: ", alumnos->apellido, sizeof(alumnos->apellido));
    verificar_datos("Apellido materno: ", alumnos->apellidoM, sizeof(alumnos->apellidoM));
    validar_numero("Numero Telefonico: ", alumnos->tel);
    printf("Domicilio: ");
    fgets(alumnos->domicilio, sizeof(alumnos->domicilio), stdin);
    alumnos->domicilio[strcspn(alumnos->domicilio, "\n")] = '\0';  // Elimina el \n
 
    verificar_datos("Licenciatura: ", alumnos->licenciatura, sizeof(alumnos->licenciatura));
    printf("\n");
}
 
void busqueda_datos(const char *campo_buscado) {
    char linea[256];
    char busqueda[50];
    char contacto_completo[10][256];
    int lineas_contacto = 0;
    int encontrados = 0;
 
    FILE *archivo = fopen("contactos.txt", "r");
    if (!archivo) {
        perror("Error al abrir el archivo");
        return;
    }
 
    printf("Ingresa el %s que deseas buscar: ", campo_buscado);
    fgets(busqueda, sizeof(busqueda), stdin);
    busqueda[strcspn(busqueda, "\n")] = '\0';
 
    while (fgets(linea, sizeof(linea), archivo)) {
        linea[strcspn(linea, "\n")] = '\0';
 
        // Guardar la línea en el contacto actual
        if (lineas_contacto < 10) {
            strcpy(contacto_completo[lineas_contacto], linea);
            lineas_contacto++;
        }
 
        // Si llegamos al final de un contacto
        if (strstr(linea, "------------------------")) {
            int coincidencia = 0;
 
            // Buscar en las líneas si hay coincidencia en el campo y el valor
            for (int i = 0; i < lineas_contacto; i++) {
                if (strstr(contacto_completo[i], campo_buscado) && strstr(contacto_completo[i], busqueda)) {
                    coincidencia = 1;
                    break;
                }
            }
 
            // Si hubo coincidencia, imprimir todo el contacto
            if (coincidencia) {
                for (int i = 0; i < lineas_contacto; i++) {
                    printf("%s\n", contacto_completo[i]);
                }
                printf("\n");
                encontrados++;
            }
 
            // Reiniciar para siguiente contacto
            lineas_contacto = 0;
        }
    }
 
    fclose(archivo);
 
    if (encontrados == 0) {
        printf("No se encontraron contactos con ese %s.\n", campo_buscado);
    } else {
        printf("Alumnos encontrados: %d\n", encontrados);
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
        "5.- Matrícula\n"
    );
    scanf("%d", &opcion);
    while (getchar() != '\n'); // Limpiar buffer
 
 
    switch (opcion){
        case 1: busqueda_datos("Nombre:"); break;
        case 2: busqueda_datos("Apellido Paterno:"); break;
        case 3: busqueda_datos("Apellido Materno:"); break;
        case 4: busqueda_datos("Telefono:"); break;
        case 5: busqueda_datos("Matrícula:"); break;
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
 
void mostrar_datos(){
    FILE *archivo = fopen("contactos.txt", "r");
    if (archivo == NULL) {
        printf("No hay contactos registrados aun.\n");
    }
 
    printf("\n=== LISTA DE CONTACTOS ===\n\n");
    int contacto_actual = 0;
    char linea[100];
 
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
 
}
 
void ordenarDatos(struct directorio *alumnos){
    char again, linea[200];
    int orderOption = 0, conta = 0;
    // Primero se cargan todos los contactos para leerlos.
    FILE *archivo = fopen("contactos.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        free(alumnos);
        return;
    }
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        if (strstr(linea, "Matrícula: ") != NULL) {        //Detecta el campo requerido.
            alumnos[conta].matricula=atoi(linea + 11);    //Guarda el contacto en un array.
        }else if (strstr(linea, "Nombre: ") != NULL) {
            strcpy(alumnos[conta].name, linea + 8);
            alumnos[conta].name[strcspn(alumnos[conta].name, "\n")] = '\0';
        }else if (strstr(linea, "Apellido Paterno: ") != NULL) {
            strcpy(alumnos[conta].apellido, linea + 18);
            alumnos[conta].apellido[strcspn(alumnos[conta].apellido, "\n")] = '\0';
        }else if (strstr(linea, "Apellido Materno: ") != NULL) {
            strcpy(alumnos[conta].apellidoM, linea + 18);
            alumnos[conta].apellidoM[strcspn(alumnos[conta].apellidoM, "\n")] = '\0';
        }else if (strstr(linea, "Número telefónico: ") != NULL) {
            // Busca la posición después de los dos puntos y espacio
            char *inicio = strchr(linea, ':');
            if (inicio != NULL) {
                inicio += 2; // Avanza pasado el ": "
                strcpy(alumnos[conta].tel, inicio);
                alumnos[conta].tel[strcspn(alumnos[conta].tel, "\n")] = '\0';
            }
        }else if (strstr(linea, "Domicilio: ") != NULL) {
            char *inicio = strchr(linea, ':');
            if (inicio != NULL) {
                inicio += 2; // Avanza pasado el ": "
                strcpy(alumnos[conta].domicilio, inicio);
                alumnos[conta].domicilio[strcspn(alumnos[conta].domicilio, "\n")] = '\0';
            }
        }else if (strstr(linea, "Licenciatura: ") != NULL) {
            strcpy(alumnos[conta].licenciatura, linea + 14);
            alumnos[conta].licenciatura[strcspn(alumnos[conta].licenciatura, "\n")] = '\0';
        }else if (strstr(linea, "------------------------") != NULL){
            conta++;  // Incrementa el contador al final de cada contacto
        }
    }
    fclose(archivo);
    do {
        rewind(archivo);
        printf("\n¿Por qué campo desea ordenar?\n1. Nombre.\n2. Teléfono.\n:");
        scanf(" %d", &orderOption);
        //while (getchar() != '\n');  // Limpiar el buffer
        switch (orderOption) {
            case 1:  // Ordenar por nombre
                for (int i = 0; i < conta - 1; i++) {
                    for (int j = i + 1; j < conta; j++) {
                        if (strcmp(alumnos[i].name, alumnos[j].name) > 0){
                            struct directorio temp = alumnos[i];
                            alumnos[i] = alumnos[j];    //Algoritmo de ordenación de burbuja.
                            alumnos[j] = temp;          //Variable temporal para evitar pérdida de datos.
                        }
                    }
                }
                break;
            case 2:  // Ordenar por teléfono
                while (getchar() != '\n');
                for (int i = 0; i < conta - 1; i++) {
                    for (int j = i + 1; j < conta; j++) {
                        if (strcmp(alumnos[i].tel, alumnos[j].tel) > 0){
                            struct directorio temp = alumnos[i];
                            alumnos[i] = alumnos[j];
                            alumnos[j] = temp;
                        }
                    }
                }
                break;
            default:
                printf("Opción inválida.\n");
                continue;
        }
        archivo = fopen("contactos.txt", "w");  //Abre escritura para guardar los contactos ordenados previamente.
        for (int i = 0; i < conta; i++) {
            fprintf(archivo, "Matrícula: %d\n", alumnos[i].matricula);
            fprintf(archivo, "Nombre: %s\n", alumnos[i].name);
            fprintf(archivo, "Apellido Paterno: %s\n", alumnos[i].apellido);
            fprintf(archivo, "Apellido Materno: %s\n", alumnos[i].apellidoM);
            fprintf(archivo, "Número telefónico: %s\n", alumnos[i].tel);
            fprintf(archivo, "Domicilio: %s\n", alumnos[i].domicilio);
            fprintf(archivo, "Licenciatura: %s\n", alumnos[i].licenciatura);
            fprintf(archivo, "------------------------\n");  // Separador entre contactos
        }
        for (int i = 0; i < conta; i++) {
            printf("Matrícula: %d\n", alumnos[i].matricula);
            printf("Nombre: %s\n", alumnos[i].name);
            printf("Apellido Paterno: %s\n", alumnos[i].apellido);
            printf("Apellido Materno: %s\n", alumnos[i].apellidoM);
            printf("Número telefónico: %s\n", alumnos[i].tel);
            printf("Domicilio: %s\n", alumnos[i].domicilio);
            printf("Licenciatura: %s\n", alumnos[i].licenciatura);
            printf("------------------------\n");  // Separador entre contactos
        }
        if (conta == 0) {
            printf("No hay contactos para ordenar.\n");
            free(alumnos);
            return;
        }else{
            printf("Contactos leídos: %d\n", conta);
        }
        fclose(archivo);
        printf("¿Ordenar nuevamente? (y/n): ");
        scanf(" %c", &again);
    } while (again == 'y' || again == 'Y');
    free(alumnos);  //Se libera la memoria.
}

void eliminar_contacto_txt() {
    char linea[256];
    char matricula_buscada[20];
    int encontrado = 0;

    printf("Ingrese la Matricula del alumno que desea eliminar: ");
    scanf("%s", matricula_buscada);
    while (getchar() != '\n');  // limpiar buffer

    FILE *archivo = fopen("contactos.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!archivo || !temp) {
        printf("Error al abrir archivos.\n");
        if (archivo) fclose(archivo);
        if (temp) fclose(temp);
        return;
    }

    while (fgets(linea, sizeof(linea), archivo)) {
        // Copiar la matrícula desde la línea actual
        char matricula[20];
        sscanf(linea, "%[^,]", matricula);

        // Comparar con la matrícula a eliminar
        if (strcmp(matricula, matricula_buscada) == 0) {
            encontrado = 1;
            printf("Contacto con Matricula %s eliminado.\n", matricula_buscada);
            continue; // No escribir esta línea en el archivo temporal
        }
        fputs(linea, temp);
    }

    fclose(archivo);
    fclose(temp);

    remove("contactos.txt");
    rename("temp.txt", "contactos.txt");

    if (!encontrado) {
        printf("No se encontro ningun contacto con esa Matricula.\n");
    }

    printf("Presione Enter para continuar...");
    getchar();
    system("cls");
}
 
int main(){
    int option = 0, contactos = 0, conta = 0, capacidad = 10;
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
                    ingresar_datos(&alumnos[conta]);
                    conta++;
                }
 
                for (int i = 0; i < conta; i++) {
                    fprintf(archivo, "Matricula: %d\n", alumnos[i].matricula);
                    fprintf(archivo, "Nombre: %s\n", alumnos[i].name);
                    fprintf(archivo, "Apellido Paterno: %s\n", alumnos[i].apellido);
                    fprintf(archivo, "Apellido Materno: %s\n", alumnos[i].apellidoM);
                    fprintf(archivo, "Telefono: %s\n", alumnos[i].tel); // Asumiendo que tel es char[]
                    fprintf(archivo, "Domicilio: %s\n", alumnos[i].domicilio); // Asumiendo que tel es char[]
                    fprintf(archivo, "Licenciatura: %s\n", alumnos[i].licenciatura); // Asumiendo que tel es char[]
                    fprintf(archivo, "------------------------\n");
                }
 
                fclose(archivo);
 
                printf("%d contactos guardados exitosamente en 'contactos.txt'.\n", contactos);
                getchar();
                system("cls");
                break;
 
            case 2:
                mostrar_datos();
                break;
            case 3:
                busqueda_alumno();
                getchar();
                system("cls");
                break;
            case 4:
            eliminar_contacto_txt();
                break;
            case 5:
                break;
            case 6:
                ordenarDatos(alumnos);
                getchar();
                system("cls");
                break;
       }
    } while (option != 10);
    return 0;
}