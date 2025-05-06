#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void limpieza(){
    int c;
    while((c = getchar()) != '\n' && c != EOF); // Limpia el búfer
    printf("Presione Enter para regresar al menu...");
    getchar(); // Ahora sí espera Enter real
}

void limpiar_buffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

typedef struct directorio{ 
    int matricula;
    char name[50];// 50 para evitar desbordamiento
    char apellido[50];
    char apellidoM[50];
    int tel;
    char domicilio[80];
    char licenciatura[50];
}directorio;

void imprimir_alumno(const struct directorio *temp){//struct directorio: Tipo de dato (estructura) que contiene los campos del alumno.
    //*temp: Puntero a una estructura directorio. Se usa para acceder a los datos del alumno sin copiar toda la estructura (eficiente en memoria).
    printf("Matricula: %d\n", temp->matricula);//temp->matricula: Accede al campo matricula de la estructura directorio
    printf("Nombre: %s\n", temp->name);
    printf("Apellido Paterno: %s\n", temp->apellido);
    printf("Apellido Materno: %s\n", temp->apellidoM);
    printf("Telefono: %d\n", temp->tel);
    printf("Domicilio: %s\n", temp->domicilio);
    printf("Licenciatura: %s\n", temp->licenciatura);
}

int menu(){
    int option = 0;
    do{
    printf(
        "\n---------- A G E N D A - D I G I T A L ----------\n"
        "Por favor, seleccione una opcion(1-5):\n"
        "1.- Agregar contacto\n"
        "2.- Ver datos\n"
        "3.- Buscar alumno\n"
        "4.- Modificar o eliminar alumno\n"
        "5.- Ordenar contacto\n:"
    );
    scanf("%d", &option);
    }while(option<1 || option>5);
    return option;
}

void validar_numero(const char *mensaje, int *destino, int longitud_minima){
    char buffer[11];
    int valido;

    do{
        valido = 1;
        printf("%s", mensaje);
        scanf("%11s", buffer);  // Leer como cadena
        limpiar_buffer();
        // Validar que todos los caracteres sean digitos
        for(int i = 0; buffer[i] != '\0'; i++){
                if (!isdigit((unsigned char)buffer[i])){
                printf("Datos invalidos. Solo se aceptan numeros.\n");
                valido = 0;
                break;
            }
        }

        if (strcmp(mensaje, "Telefono: ") == 0){
            // Validar longitud mínima 10
            //size_t vs int:
            //strlen() retorna size_t (sin signo)
            //longitud_minima es int (con signo)
            //Compararlos directamente puede generar warnings
            if(valido && longitud_minima > 0 && strlen(buffer) < (size_t)longitud_minima){
                printf("Se requieren al menos %d dígitos.\n", longitud_minima);
                valido = 0;
            }
        }

        // Convertir a entero si todo está bien
        if (valido) {
            *destino = atoi(buffer);
        }
    } while (!valido);

}

void verificar_datos(const char *mensaje, char *destino, size_t tamano) {
    char buffer[100];//buffer[100]: Buffer temporal para almacenar la entrada del usuario
    int valido;
    
    do {
        valido = 1;
        printf("%s", mensaje);
        fgets(buffer, sizeof(buffer), stdin);//fgets() lee la entrada de forma segura 
        // Elimina el \n solo si existe
        buffer[strcspn(buffer, "\n")] = '\0'; 

        size_t len = strlen(buffer);//Esta línea calcula la longitud (cantidad de caracteres) del texto que está almacenado en buffer y guarda ese número en la variable len.
        
        if(len > 0 && buffer[len-1] == '\n'){
            buffer[len-1] = '\0';
        }else if (len == sizeof(buffer) - 1){ // Manejar líneas muy largas, detecta cuando el usuario escribio más texto del que cabe en el buffer
            limpiar_buffer();
        }
        
        // Validar que no esté vacío
        if(strlen(buffer) == 0){
            printf("Por favor, escriba algo.\n");
            valido = 0;
            continue;
        }
        
        for(size_t i = 0; buffer[i] != '\0'; i++){        
            if(!isalpha((unsigned char)buffer[i]) && buffer[i] != ' '){
                printf("Solo se permiten letras y espacios.\n");
                valido = 0;
                break;
            }
        }
        
        if(valido){
            strncpy(destino, buffer, tamano-1);//Copia una cadena de forma segura 
            destino[tamano-1] = '\0';    
        }
    }while(!valido);
}

void ingresar_datos(struct directorio *alumnos) {
    validar_numero("Matricula: ", &alumnos->matricula, 1);
    verificar_datos("Nombre Completo: ", alumnos->name, sizeof(alumnos->name));//verificar_datos() valida que solo contengan letras y espacios
    verificar_datos("Apellido paterno: ", alumnos->apellido, sizeof(alumnos->apellido));//sizeof() para garantizar que no se exceda el tamaño del campo
    verificar_datos("Apellido materno: ", alumnos->apellidoM, sizeof(alumnos->apellidoM));
    validar_numero("Telefono: ", &alumnos->tel, 1);
    printf("Domicilio: ");
    fgets(alumnos->domicilio, sizeof(alumnos->domicilio), stdin);
    alumnos->domicilio[strcspn(alumnos->domicilio, "\n")] = '\0';  // Elimina el \n
    verificar_datos("Licenciatura: ", alumnos->licenciatura, sizeof(alumnos->licenciatura));
    printf("\n");
}

void busqueda_datos(const char *campo_buscado) {
    struct directorio temp;
    char busqueda[50];
    int encontrados = 0;

    FILE *archivo = fopen("contactos.dat", "rb");
    if (!archivo) {
        perror("Error al abrir el archivo");
        return;
    }

    printf("Ingresa el %s que deseas buscar: ", campo_buscado);
    fgets(busqueda, sizeof(busqueda), stdin);
    busqueda[strcspn(busqueda, "\n")] = '\0'; // quitar salto de línea

    while(fread(&temp, sizeof(struct directorio), 1, archivo)){  
        int coincidencia = 0;
        if(strcmp(campo_buscado, "Nombre") == 0 && strcmp(temp.name, busqueda) == 0) coincidencia = 1;
        
        else if(strcmp(campo_buscado, "Apellido Paterno") == 0 && strcmp(temp.apellido, busqueda) == 0) coincidencia = 1;
        else if(strcmp(campo_buscado, "Apellido Materno") == 0 && strcmp(temp.apellidoM, busqueda) == 0) coincidencia = 1;
        else if(strcmp(campo_buscado, "Telefono") == 0) {
            char telefono_str[11];
            snprintf(telefono_str, sizeof(telefono_str), "%d", temp.tel);
            if(strcmp(telefono_str, busqueda) == 0) coincidencia = 1;
        }
        else if(strcmp(campo_buscado, "Matricula") == 0 && temp.matricula == atoi(busqueda)) coincidencia = 1;
        
        if(coincidencia){
            imprimir_alumno(&temp);
            printf("------------------------\n");
            encontrados++;
        }
    }

    fclose(archivo);

    if(encontrados == 0){
        printf("\nNo se encontraron contactos con ese %s.\n", campo_buscado);
    }else{
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
        "4.- Matricula\n"
    );
    scanf("%d", &opcion);
    while (getchar() != '\n'); // Limpiar buffer


    switch (opcion){
        case 1: busqueda_datos("Nombre"); break;
        case 2: busqueda_datos("Apellido Paterno"); break;
        case 3: busqueda_datos("Apellido Materno"); break;
        case 4: busqueda_datos("Matricula"); break;
    }

    printf("Desea hacer otra busqueda? (S/N): ");
    scanf(" %c", &condicion);  // El espacio antes de %c limpia el espacion en blanco
    limpiar_buffer();

    if(tolower(condicion) == 's'){//Convierte a minusculas y compara
        system("cls");
        busqueda_alumno();  
    }
}

void mostrar_datos(){
    FILE *archivo = fopen("contactos.dat", "rb");
    if(!archivo){
        printf("No hay contactos registrados aun.");
        return;
    }

    struct directorio temp;
    int contacto_actual = 0;

    printf("=== LISTA DE CONTACTOS ===\n");

    while(fread(&temp, sizeof(struct directorio), 1, archivo)){
        imprimir_alumno(&temp);
        printf("------------------------\n");
        contacto_actual++;
    }

    fclose(archivo);
    printf("Total: %d contactos\n", contacto_actual);  
}

void valor_a_editar(struct directorio *temp){
    int opcion = 0;
    char condicion;

    do{
        printf("\nIngrese el numero del dato que desea cambiar:\n"
               "1.- Nombre\n"
               "2.- Apellido Paterno\n"
               "3.- Apellido Materno\n"
               "4.- Telefono\n"
               "5.- Domicilio\n"
               "6.- Licenciatura\n"
                "7.- Matricula\n");
               
        scanf("%d", &opcion);
        while (getchar() != '\n'); // Limpiar buffer

        switch(opcion) {
            case 1:
                verificar_datos("Nuevo nombre Completo: ", temp->name, sizeof(temp->name));
                break;

            case 2:
                verificar_datos("Nuevo Apellido Paterno: ", temp->apellido, sizeof(temp->apellido));
                break;

            case 3:
                verificar_datos("Nuevo Apellido Materno: ", temp->apellidoM, sizeof(temp->apellidoM));                
                break;

            case 4:
                validar_numero("Nuevo Telefono: ", &temp->tel, 1);
                break;

            case 5:
                printf("Nuevo Domicilio: ");
                fgets(temp->domicilio, sizeof(temp->domicilio), stdin);
                temp->domicilio[strcspn(temp->domicilio, "\n")] = '\0';
                break;

            case 6:
                verificar_datos("Nueva Licenciatura: ", temp->licenciatura, sizeof(temp->licenciatura));
                break;

            case 7:
                validar_numero("MNueva Matricula: ", &temp->matricula, 1);
                break;

            default:
                printf("Opcion invalida.\n");
        }

        printf("Desea editar otro campo? (S/N): ");
        scanf(" %c", &condicion);
        while(getchar() != '\n');

    } while(tolower(condicion) == 's');
}

void editar_datos(){
    int matricula = 0;
    int encontrado = 0;
    struct directorio temp;

    FILE *archivo = fopen("contactos.dat", "rb+");
    if (!archivo){
        printf("Error al abrir el archivo.\n");
        return;
    }

    printf("Ingrese la matricula del alumno que desea editar: ");
    scanf("%d", &matricula);
    while(getchar() != '\n'); // limpiar buffer

    while(fread(&temp, sizeof(struct directorio), 1, archivo)){
        if(temp.matricula == matricula){
            printf("\nAlumno encontrado:\n");
            imprimir_alumno(&temp);
            valor_a_editar(&temp);
        
            fseek(archivo, (long)(-sizeof(struct directorio)), SEEK_CUR);
            fwrite(&temp, sizeof(struct directorio), 1, archivo);//Sobrescribe el registro con los nuevos datos
            encontrado = 1;
            printf("\nContacto actualizado correctamente.\n");
            break;
            }
    }

    if (!encontrado){
        printf("No se encontró ningún alumno con ID %d.\n", matricula);
    }

    fclose(archivo);
}

void eliminar_datos(){
    int matricula_buscada;
    struct directorio temp;

    printf("Ingrese la Matricula del alumno que desea eliminar: ");
    scanf("%d", &matricula_buscada);
    while (getchar() != '\n');

    FILE *archivo = fopen("contactos.dat", "rb");
    if (!archivo){
        printf("\nError: No se puede acceder al archivo de contactos\n");
        return;
    }

    FILE *temp_archivo = fopen("temp.dat", "wb");
    if (!temp_archivo){
        printf("\nError: No se pudo crear archivo temporal\n");
        fclose(archivo);
        return;
    }

    int contactos_restantes = 0;
    while(fread(&temp, sizeof(struct directorio), 1, archivo) == 1) {
        if(temp.matricula == matricula_buscada){
            printf("\nContacto encontrado para eliminacion:\n");
            printf("Matricula: %d\n", temp.matricula);
            printf("Nombre: %s\n", temp.name);
        }else{
            if(fwrite(&temp, sizeof(struct directorio), 1, temp_archivo) != 1){
                printf("\nError durante la escritura\n");
                fclose(archivo);
                fclose(temp_archivo);
                remove("temp.dat");
                return;
            }
            contactos_restantes++;
        }
    }
    
    fclose(archivo);
    fclose(temp_archivo);

    if (remove("contactos.dat") != 0){
        perror("\nError eliminando archivo original");
        remove("temp.dat");
        return;
    }
    if (rename("temp.dat", "contactos.dat") != 0){
        perror("\nError renombrando archivo temporal");
        remove("temp.dat");
        return;
    }
    printf("\nOperacion exitosa! Contactos restantes: %d\n", contactos_restantes);
    
}

void ordenarDatos(struct directorio *alumnos){
    char again;
    int orderOption = 0, conta = 0;

    FILE *archivo = fopen("contactos.dat", "rb");
    if (archivo == NULL){
        printf("Error al abrir el archivo binario.\n");
        return;
    }

    // Cargar todos los contactos desde el archivo binario
    while (fread(&alumnos[conta], sizeof(struct directorio), 1, archivo) == 1) {
        conta++;
    }
    fclose(archivo);

    if (conta == 0){
        printf("No hay contactos para ordenar.\n");
        return;
    }

    do {
        printf("\nPor que campo desea ordenar?\n1. Nombre\n2. Telefono\n: ");
        scanf(" %d", &orderOption);

        switch (orderOption) {
            case 1:  // Ordenar por nombre
                for (int i = 0; i < conta - 1; i++) {
                    for (int j = i + 1; j < conta; j++) {
                        if (strcmp(alumnos[i].name, alumnos[j].name) > 0) {
                            struct directorio temp = alumnos[i];
                            alumnos[i] = alumnos[j];
                            alumnos[j] = temp;
                        }
                    }
                }
                break;
            case 2:  // Ordenar por teléfono
                for (int i = 0; i < conta - 1; i++) {
                    for (int j = i + 1; j < conta; j++) {
                        if (alumnos[i].tel > alumnos[j].tel) {
                            struct directorio temp = alumnos[i];
                            alumnos[i] = alumnos[j];
                            alumnos[j] = temp;
                        }
                    }
                }
                break;
            default:
                printf("Opcion invalida.\n");
                continue;
        }

        // Escribir los contactos ordenados en el archivo binario
        archivo = fopen("contactos.dat", "wb");
        if (archivo == NULL){
            printf("Error al escribir el archivo binario.\n");
            return;
        }

        fwrite(alumnos, sizeof(struct directorio), conta, archivo);
        fclose(archivo);

        // Mostrar los contactos ordenados por consola
        for (int i = 0; i < conta; i++){
            imprimir_alumno(&alumnos[i]);
            printf("------------------------\n");
        }

        printf("Contactos ordenados: %d\n", conta);
        printf("Ordenar nuevamente? (y/n): ");
        scanf(" %c", &again);
    } while (again == 'y' || again == 'Y');
}
 
int main(){
    int option = 0, contactos = 0, conta = 0, capacidad = 10, opcion2=0;
    struct directorio *alumnos = malloc(sizeof(struct directorio) * capacidad); //memoria dinamica
    //struct directorio alumnos[10]; para memoria estatica

    do{
        option = menu();
        switch (option){
            case 1:
                FILE *archivo = fopen("contactos.dat", "ab"); // Modo append (agregar al final)
                if (archivo == NULL){
                    printf("Error al abrir el archivo.\n");
                    break;
                }

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
                    fwrite(&alumnos[conta], sizeof(struct directorio), 1, archivo);  // Guardar en archivo
                    conta++;
                }
                fclose(archivo);

                printf("%d contactos guardados exitosamente en 'contactos.dat'.\n", contactos);
                limpieza();
                system("cls");
                break;

            case 2:
                mostrar_datos();
                limpieza();
                system("cls");

                break;
            case 3:
                busqueda_alumno();
                limpieza();
                system("cls");
                break;

            case 4:
                printf("Ingrese la opcion de la accion que desea realizar\n"
                "1-Editar contacto\n"
                "2-Borrar contacto\n");
                scanf(" %d", &opcion2);
                if(opcion2 == 1){
                    editar_datos();
                }else{
                    eliminar_datos();
                }
                limpieza();
                system("cls");
                break;

            case 5:
                ordenarDatos(alumnos);
                limpieza();
                system("cls");
                break;
       }

    } while (option != 6);
    free(alumnos);
    return 0;
}