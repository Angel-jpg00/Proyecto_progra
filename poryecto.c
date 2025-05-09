#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void limpieza(){
    int c;
    //getchar(): Lee un caracter del bufer de entrada estandar (stdin).
    //!= '\n': Compara si el caracter leido no es un salto de linea (Enter).
    //!= EOF: Compara si el caracter leido no es el indicador de fin de archivo (End Of File)
    while((c = getchar()) != '\n' && c != EOF); // Limpia el bufer
    //El bucle continua leyendo y descartando caracteres del bufer hasta que encuentre un \n (Enter) o EOF.
    printf("Presione Enter para regresar al menu...");
    getchar();//Ahora sí espera Enter real
}

void limpiar_buffer(){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

typedef struct directorio{ 
    int matricula;
    char name[50];//50 para evitar desbordamiento
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
    int opcion = 0;
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
    scanf("%d", &opcion);
    }while(opcion < 1 || opcion > 5);
    return opcion;
}

void validar_numero(const char *mensaje, int *destino, int longitud_minima){
    //int *destino: Puntero a un entero donde se guardara el valor validado.
    //int longitud_minima: Longitud minima requerida para el numero (ej: 10 para teléfonos).
    char buffer[11];//buffer[11]: Arreglo de caracteres para almacenar temporalmente la entrada del usuario (tamaño 11 para numeros de hasta 10 digitos 
    int valido;

    do{
        valido = 1;
        printf("%s", mensaje);
        scanf("%11s", buffer);//Lee como cadena
        limpiar_buffer();

        //Validar que todos los caracteres sean digitos
        for(int i = 0; buffer[i] != '\0'; i++){//for: Recorre cada caracter en buffer hasta encontrar \0 (fin de la cadena).
                if (!isdigit((unsigned char)buffer[i])){//isdigit: Verifica si el carácter es un digito (0-9).
                printf("Datos invalidos. Solo se aceptan numeros.\n");
                valido = 0;
                break;
            }
        }

        if (strcmp(mensaje, "Telefono: ") == 0){//strcmp: Compara el mensaje con "Telefono: " para aplicar validacion solo a telefonos
            // Validar longitud minima 10
            if(valido && longitud_minima > 0 && strlen(buffer) < (size_t)longitud_minima){
                //valido: Solo verifica longitud si los caracteres son digitos.
                //strlen(buffer) < longitud_minima: Comprueba que la entrada tenga suficientes digitos.
                printf("Se requieren al menos %d dígitos.\n", longitud_minima);
                valido = 0;
            }
        }
        if(valido){
            *destino = atoi(buffer);//atoi: Convierte la cadena numerica en buffer a un entero (int).
            //Guarda el resultado en la dirección apuntada por destino (modificando la variable original)
        }
    }while(!valido);

}

void verificar_datos(const char *mensaje, char *destino, size_t tamano) {
    char buffer[90];//Buffer temporal para almacenar la entrada del usuario
    int valido;
    
    do {
        valido = 1;
        printf("%s", mensaje);
        fgets(buffer, sizeof(buffer), stdin);//fgets() lee la entrada de forma segura 
        buffer[strcspn(buffer, "\n")] = '\0';//Elimina el \n solo si existe
        //buffer: Arreglo de caracteres (array) donde se almacenara lo que el usuario escriba

        // Validar que no este vacio
        if(strlen(buffer) == 0){
            printf("Por favor, escriba algo.\n");
            valido = 0;
            continue;
        }
        /*
        Validar caracteres
        Codigo verifica que una cadena de texto solo contenga letras y espacios.
        size_t:Indica explicitamente que i se usa para manejar tamaños/posiciones (no para calculos matematicos).
        */
        for(size_t i = 0; buffer[i] != '\0'; i++){   
            //buffer[i] != '\0', El bucle continua mientras no lleguemos al final de la cadena
            /*
            que hace? -> !isalpha((unsigned char)buffer[i]):
            buffer[i]: Obtiene el caracter en posicion i (como char)
            (unsigned char): Convierte el caracter a su equivalente sin signo (0-255)
            */     
            if(!isalpha((unsigned char)buffer[i]) && buffer[i] != ' '){
                //isalpha():Funcion que verifica si un caracter es alfabetico (A-Z, a-z)
                //Devuelve verdadero si es una letra   
                printf("Solo se permiten letras y espacios.\n");
                valido = 0;
                break;
            }
        }
        
        if(valido){
            strncpy(destino, buffer, tamano-1);//Copia una cadena de forma segura
            /*
            como se usa? -> 
            destino: Donde queremos copiar el texto
            buffer: De donde copiamos el texto (lo que escribio el usuario)
            tamano-1: Cantidad maxima de caracteres a copiar (menos 1 para dejar espacio al \0)
            */
            destino[tamano-1] = '\0';
            /*
            destino[tamano-1] = '\0'
            Asegura que la cadena termine correctamente con caracter nulo
            Esto es importante porque strncpy no siempre añade el \0 automaticamente
            */
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
    alumnos->domicilio[strcspn(alumnos->domicilio, "\n")] = '\0';//Elimina el \n
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
    busqueda[strcspn(busqueda, "\n")] = '\0';//quitar salto de linea
     /*
    Que hace el while?:
    Lee cada registro del archivo uno por uno, compara segun el campo especificado (campo_buscado)
    
    fread() es como sacar una foto del album
    El bucle while sigue sacando fotos mientras haya mas en el album
    Cuando no quedan fotos, el bucle se detiene
    */
    while(fread(&temp, sizeof(struct directorio), 1, archivo)){
        /*
        Como funciona fread()?: 
        &temp: Direccion de memoria donde se almacenara el registro leido
        sizeof(struct directorio): Tamaño en bytes de cada registro
        1: Cantidad de registros a leer en cada operación
        archivo: Puntero al archivo binario abierto previamente
        */ 
        int coincidencia = 0;
        if(strcmp(campo_buscado, "Nombre") == 0 && strcmp(temp.name, busqueda) == 0) coincidencia = 1;
        //strcmp(campo_buscado, "Nombre") == 0:
        //Compara si el campo a buscar es exactamente "Nombre".
        //strstr(temp.name, busqueda):
        //Busca la cadena 'busqueda' dentro del nombre del alumno (temp.name)
        //Devuelve:
        //Puntero a la posición donde se encontró (verdadero)
        else if(strcmp(campo_buscado, "Apellido Paterno") == 0 && strcmp(temp.apellido, busqueda) == 0) coincidencia = 1;
        else if(strcmp(campo_buscado, "Apellido Materno") == 0 && strcmp(temp.apellidoM, busqueda) == 0) coincidencia = 1;
        else if(strcmp(campo_buscado, "Telefono") == 0){
            //Creamnos un buffer temporal (telefono_busqueda) para almacenar el telefono como cadena
            //snprintf() convierte el entero temp.tel a string de forma segura:
            char telefono_buscado[11];
            //strcmp() devuelve 0 cuando son exactamente iguales
            snprintf(telefono_buscado, sizeof(telefono_buscado), "%d", temp.tel);
            if(strcmp(telefono_buscado, busqueda) == 0) coincidencia = 1;
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
    while (getchar() != '\n');//Limpiar buffer


    switch (opcion){
        case 1: busqueda_datos("Nombre"); break;//Llama a la funcion con el valor a buscar
        case 2: busqueda_datos("Apellido Paterno"); break;
        case 3: busqueda_datos("Apellido Materno"); break;
        case 4: busqueda_datos("Matricula"); break;
    }

    printf("Desea hacer otra busqueda? (S/N): ");
    scanf(" %c", &condicion);//El espacio antes de %c limpia el espacion en blanco
    limpiar_buffer();

    if(tolower(condicion) == 's'){//tolower convierte a minuscula la variable y compara
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

    struct directorio temp;//hacemos una estructura temporal con temp
    int contacto_actual = 0;

    printf("=== LISTA DE CONTACTOS ===\n");

    while(fread(&temp, sizeof(struct directorio), 1, archivo)){
    //con la estructura temp guarda los datos del archivo bninario y los imprime en la forma de la estructura.
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
        while (getchar() != '\n');//Limpiar buffer

        switch(opcion) {
            case 1:
                verificar_datos("Nuevo nombre Completo: ", temp->name, sizeof(temp->name));//Guarda el nuevo nombre en temp con su tamaño
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
        //while: condicion para repetir el codigo si el usuario desea editar nuevamente
    } while(tolower(condicion) == 's');//tolower: convierte una mayuscula en minuscula.
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
    while(getchar() != '\n');//limpiar buffer

    while(fread(&temp, sizeof(struct directorio), 1, archivo)){
        if(temp.matricula == matricula){//Aqui compara si la matricula dada por el usuario es la misma en donde se encuentra temp.matricula.
            printf("\nAlumno encontrado:\n");
            imprimir_alumno(&temp);//imprime los datos del alumno
            valor_a_editar(&temp);//Aqui llamamos a la funcion donde se editan los datos datos
            /*Volver al inicio de ese registro
            //fseek() mueve el puntero del archivo:
            //-sizeof(struct directorio): Retrocede el tamaño de un registro
            //SEEK_CUR: Desde la posicion actual
            //Esta instruccion rebobina el puntero del archivo exactamente un registro hacia atras 
            para posicionarse al inicio del registro que acabamos de leer y queremos modificar.
            */
            fseek(archivo, (long)(-sizeof(struct directorio)), SEEK_CUR);
            //fseek(): Función estandar de C para mover el puntero de posicion en un archivo
            /*
            archivo: El puntero FILE* del archivo abierto
            (long)(-sizeof(struct directorio)): Distancia a mover (en bytes)
            SEEK_CUR: Punto de referencia para el movimiento

            sizeof(struct directorio):
            Calcula el tamaño en bytes de la estructura del registro
            Ejemplo: Si la estructura ocupa 100 bytes, esto devuelve 100
            */
            fwrite(&temp, sizeof(struct directorio), 1, archivo);//Sobrescribe el registro con los nuevos datos
            encontrado = 1;
            printf("\nContacto actualizado correctamente.\n");
            break;
            }
    }
    if(!encontrado){
        printf("No se encontró ningún alumno con ID %d.\n", matricula);
    }
    fclose(archivo);
}

void eliminar_datos(){
    int matricula_buscada,contactos_restantes = 0;;
    struct directorio temp;//un directorio temporal

    printf("Ingrese la Matricula del alumno que desea eliminar: ");
    scanf("%d", &matricula_buscada);
    while(getchar() != '\n');

    FILE *archivo = fopen("contactos.dat", "rb");
    if(!archivo){
        printf("\nError: No se puede acceder al archivo de contactos\n");
        return;
    }

    FILE *temp_archivo = fopen("temp.dat", "wb");
    if (!temp_archivo){//Verifica si el archivo temporal se creo correctamente.
        printf("\nError: No se pudo crear archivo temporal\n");
        fclose(archivo);
        return;
    }

    while(fread(&temp, sizeof(struct directorio), 1, archivo) == 1){//lee los registros del archivo uno por uno
        if(temp.matricula == matricula_buscada){//Compara la matricula del registro actual con la matricula buscada
            printf("\nContacto encontrado para eliminacion:\n");
            printf("Matricula: %d\n", temp.matricula);
            printf("Nombre: %s\n", temp.name);
        }else{
            //Si el alumno no es el buscado, entonces hace lo siguiente.
            //escribe el registro actual en el archivo temporal:
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
        perror("\nError eliminando archivo original");//Da el error exacto
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
    char opcion2;
    int opcion = 0, conta = 0;

    FILE *archivo = fopen("contactos.dat", "rb");
    if (archivo == NULL){
        printf("Error al abrir el archivo binario.\n");
        return;
    }

    //Carga todos los contactos desde el archivo binario
    //Lee registros del archivo uno por uno y los almacena en el array alumnos
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
        scanf(" %d", &opcion);

        switch (opcion) {
            case 1:  // Ordenar por nombre
                for (int i = 0; i < conta - 1; i++){
                    for (int j = i + 1; j < conta; j++){
                        //Compara nombres con strcmp()
                        if (strcmp(alumnos[i].name, alumnos[j].name) > 0){
                            //Intercambia registros si estan en orden incorrecto
                            struct directorio temp = alumnos[i];
                            alumnos[i] = alumnos[j];
                            alumnos[j] = temp;
                        }
                    }
                }
                break;
            case 2:  // Ordenar por telefono
                for (int i = 0; i < conta - 1; i++){
                    for (int j = i + 1; j < conta; j++){
                        //Comparamos directamente los valores numericos de telefono
                        if (alumnos[i].tel > alumnos[j].tel){
                            //Intercambiamos registros si estan en orden
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

        //Escribe los contactos ordenados en el archivo binario
        archivo = fopen("contactos.dat", "wb");
        if (archivo == NULL){
            printf("Error al escribir el archivo binario.\n");
            return;
        }

        //Escribe todos los registros ordenados de vuelta al archivo
        fwrite(alumnos, sizeof(struct directorio), conta, archivo);
        fclose(archivo);

        //Mostrar los contactos ordenados por consola
        for (int i = 0; i < conta; i++){
            imprimir_alumno(&alumnos[i]);
            printf("------------------------\n");
        }

        printf("Contactos ordenados: %d\n", conta);
        printf("Ordenar nuevamente? (y/n): ");
        scanf(" %c", &opcion2);
    } while (opcion2 == 'y' || opcion2 == 'Y');
}
 
int main(){
    int opcion = 0, contactos = 0, conta = 0, capacidad = 10, opcion2=0;
    struct directorio *alumnos = malloc(sizeof(struct directorio) * capacidad);//memoria dinamica
    //struct directorio alumnos[10]; para memoria estatica

    do{
        opcion = menu();
        switch (opcion){
            case 1:
                FILE *archivo = fopen("contactos.dat", "ab");//Modo append (agregar al final)
                if (archivo == NULL){
                    printf("Error al abrir el archivo.\n");
                    break;
                }
                printf("Cuantos contactos deseas agregar?:");
                scanf("%d", &contactos);
                limpiar_buffer();
                //bucle for para hacer llamadaa de la funcion ingresar datos dependiendo de los contactos a ingresar.
                for (int i = 0; i < contactos; i++){
                    if (conta >= capacidad){//condicion para aumentar la capacidad de alumnos si esta se ve rebasada
                        capacidad *= 2;
                        //realloc para redimensionar memoria.
                        //Temp necesario 
                        struct directorio *temp = realloc(alumnos, sizeof(struct directorio) * capacidad);
                        if (!temp){
                            printf("Error: No se pudo expandir la memoria.\n");
                            break;
                        }
                        //actualiza el puntero principal, asegura que apunte a la nueva memoria redimensionada
                        alumnos = temp;
                    }
                    ingresar_datos(&alumnos[conta]);//guarda los datos que ingresa el usuario
                    //Escribe el bloque de datos en el archivo binario. 
                    fwrite(&alumnos[conta], sizeof(struct directorio), 1, archivo);//Guardar en archivo
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
    } while (opcion != 6);
    free(alumnos);
    return 0;
}