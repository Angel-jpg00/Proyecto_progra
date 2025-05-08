#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void limpieza(){
    int c;
    //getchar(): Lee un carácter del búfer de entrada estándar (stdin).
    //!= '\n': Compara si el carácter leído no es un salto de línea (Enter).
    //!= EOF: Compara si el carácter leído no es el indicador de fin de archivo (End Of File)
    while ((c = getchar()) != '\n' && c != EOF); // Limpia el búfer
    //El bucle continúa leyendo y descartando caracteres del búfer hasta que encuentre un \n (Enter) o EOF.
    printf("Presione Enter para regresar al menu...");
    getchar(); // Ahora sí espera Enter real
}

void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

typedef struct directorio { 
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
    //int *destino: Puntero a un entero donde se guardará el valor validado.
    //int longitud_minima: Longitud mínima requerida para el número (ej: 10 para teléfonos).
    char buffer[11];//buffer[11]: Arreglo de caracteres para almacenar temporalmente la entrada del usuario (tamaño 11 para números de hasta 10 dígitos 

    int valido;

    do {
        valido = 1;
        printf("%s", mensaje);
        scanf("%10s", buffer);  // Leer como cadena
        limpiar_buffer();

        // Validar que todos los caracteres sean digitos
        for (int i = 0; buffer[i] != '\0'; i++){//for: Recorre cada carácter en buffer hasta encontrar \0 (fin de la cadena).
            if (!isdigit((unsigned char)buffer[i])) {//isdigit: Verifica si el carácter es un dígito (0-9).
                printf("Datos invalidos. Solo se aceptan numeros.\n");
                valido = 0;
                break;
            }
        }

        if (strcmp(mensaje, "Telefono: ") == 0){//strcmp: Compara el mensaje con "Telefono: " para aplicar validación solo a teléfonos
            // Validar longitud mínima 10
            if (valido && longitud_minima > 0 && strlen(buffer) < longitud_minima){
                //valido: Solo verifica longitud si los caracteres son dígitos.
                //strlen(buffer) < longitud_minima: Comprueba que la entrada tenga suficientes dígitos.
                printf("Se requieren al menos %d dígitos.\n", longitud_minima);
                valido = 0;
            }
        }

        // Convertir a entero si todo está bien
        if (valido) {
            *destino = atoi(buffer);//atoi: Convierte la cadena numérica en buffer a un entero (int).
            //Guarda el resultado en la dirección apuntada por destino (modificando la variable original)
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
        //buffer: Arreglo de caracteres (array) donde se almacenará lo que el usuario escriba
        //sizeof(buffer):Especifica el tamaño máximo a leer
        //sizeof(buffer):calcula automáticamente el tamaño del array
        //stdin: es un puntero a FILE* que representa la entrada del teclado
        //Tú escribes algo (ej: "pájaro" + Enter).
        //fgets() escucha a través de stdin (el teclado).
        
        // Eliminar salto de línea si existe
        size_t len = strlen(buffer);//Esta línea calcula la longitud (cantidad de caracteres) del texto que está almacenado en buffer y guarda ese número en la variable len.
        //significa "tamaño de tipo" (size type), Es un tipo de variable especial para guardar tamaños y longitudes
        //strlen() cuenta los caracteres "visibles" + \n si existen
        //buffer: Por ejemplo, si contiene "Hola", buffer sería: 'H' 'o' 'l' 'a' '\n' '\0'
        

        //Este if que hace?: Cuando el usuario escribe algo y presiona Enter, se agrega un \n (salto de línea) al final
        //Este código busca y elimina ese \n reemplazándolo con \0 (fin de cadena)
        if (len > 0 && buffer[len-1] == '\n') {
            //buffer[len-1] es buffer[4] (la 5ta posición, empezando desde 0)
            //Si es \n, lo cambia por \0
            buffer[len-1] = '\0';
        } else if (len == sizeof(buffer) - 1) { // Manejar líneas muy largas, detecta cuando el usuario escribio más texto del que cabe en el buffer
            limpiar_buffer();
        }
        
        // Validar que no esté vacío
        if (strlen(buffer) == 0) {
            printf("Por favor, escriba algo.\n");
            valido = 0;
            continue;
        }
        
        // Validar caracteres
        //código verifica que una cadena de texto solo contenga letras y espacios.
        for (size_t i = 0; buffer[i] != '\0'; i++) {
            //buffer[i] != '\0', El bucle continúa mientras no lleguemos al final de la cadena
            /*
            que hace? -> !isalpha((unsigned char)buffer[i]):
            buffer[i]: Obtiene el carácter en posición i (como char)
            (unsigned char): Convierte el carácter a su equivalente sin signo (0-255)
            Ejemplo: Si buffer[i] es 'á' (código -31 en sistemas con char con signo):
            La conversión lo cambia a 225 (equivalente sin signo)
            isalpha(): Recibe este valor sin ambigüedades de signo y verifica:
            Si está en el rango de letras (A-Z, a-z en ASCII básico)
            O si es una letra en tu configuración regional (como vocales acentuadas)
            ! al inicio: Niega el resultado ("si NO es una letra")
            */
         
            if (!isalpha((unsigned char)buffer[i]) && buffer[i] != ' ') {
                //isalpha():Función que verifica si un carácter es alfabético (A-Z, a-z)
                //Devuelve verdadero si es una letra     
                printf("Solo se permiten letras y espacios.\n");
                valido = 0;
                break;
            }
        }
        
        if (valido) {
            strncpy(destino, buffer, tamano-1);//Copia una cadena de forma segura
            /*
            como se usa? -> 
            destino: Donde queremos copiar el texto
            buffer: De donde copiamos el texto (lo que escribió el usuario)
            tamano-1: Cantidad máxima de caracteres a copiar (menos 1 para dejar espacio al \0)
            */
            destino[tamano-1] = '\0';
            /*
            destino[tamano-1] = '\0'
            Asegura que la cadena termine correctamente con carácter nulo
            Esto es importante porque strncpy no siempre añade el \0 automáticamente
            */
        }
    } while (!valido);
}

void ingresar_datos(struct directorio *alumnos) {

    validar_numero("Matricula: ", &alumnos->matricula, 1);
    limpiar_buffer();

    verificar_datos("Nombre Completo: ", alumnos->name, sizeof(alumnos->name));//verificar_datos() valida que solo contengan letras y espacios
    verificar_datos("Apellido paterno: ", alumnos->apellido, sizeof(alumnos->apellido));//sizeof() para garantizar que no se exceda el tamaño del campo
    verificar_datos("Apellido materno: ", alumnos->apellidoM, sizeof(alumnos->apellidoM));
    validar_numero("Telefono: ", &alumnos->tel, 1);
    printf("Domicilio: ");
    fgets(alumnos->domicilio, sizeof(alumnos->domicilio), stdin);
    //alumnos->domicilio[strcspn(alumnos->domicilio, "\n")] = '\0';  // Elimina el \n
    limpiar_buffer();
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

    /*
    Que hace el while?:
    Lee cada registro del archivo uno por uno, Compara según el campo especificado (campo_buscado)
    
    fread() es como sacar una foto del álbum
    El bucle while sigue sacando fotos mientras haya más en el álbum
    Cuando no quedan fotos, el bucle se detiene
    */

    while (fread(&temp, sizeof(struct directorio), 1, archivo)){
        /*
        Como funciona fread()?: 
        &temp: Dirección de memoria donde se almacenará el registro leído
        sizeof(struct directorio): Tamaño en bytes de cada registro
        1: Cantidad de registros a leer en cada operación
        archivo: Puntero al archivo binario abierto previamente
        */
        int coincidencia = 0;

        if (strcmp(campo_buscado, "Nombre") == 0 && strcmp(temp.name, busqueda) == 0) coincidencia = 1;
        //strcmp(campo_buscado, "Nombre") == 0:
        //Compara si el campo a buscar es exactamente "Nombre".
        //strstr(temp.name, busqueda):
        //Busca la cadena 'busqueda' dentro del nombre del alumno (temp.name)
        //Devuelve:
        //Puntero a la posición donde se encontró (verdadero)
        
        else if (strcmp(campo_buscado, "Apellido Paterno") == 0 && strcmp(temp.apellido, busqueda) == 0) coincidencia = 1;
        else if (strcmp(campo_buscado, "Apellido Materno") == 0 && strcmp(temp.apellidoM, busqueda) == 0) coincidencia = 1;
        else if (strcmp(campo_buscado, "Telefono") == 0) {
            //Creamnoa un buffer temporal (telefono_str) para almacenar el teléfono como cadena
            //snprintf() convierte el entero temp.tel a string de forma segura:
            char telefono_str[11];
            //strcmp() devuelve 0 cuando son exactamente iguales
            snprintf(telefono_str, sizeof(telefono_str), "%d", temp.tel);
            if (strcmp(telefono_str, busqueda) == 0) coincidencia = 1;
        }
        else if (strcmp(campo_buscado, "Matricula") == 0 && temp.matricula == atoi(busqueda)) coincidencia = 1;
        
        if (coincidencia) {
            imprimir_alumno(&temp);
            printf("------------------------\n");
            encontrados++;
        }
    }

    fclose(archivo);

    if (encontrados == 0) {
        printf("\nNo se encontraron contactos con ese %s.\n", campo_buscado);
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
        "4.- Matricula\n"
    );
    scanf("%d", &opcion);
    while (getchar() != '\n'); // Limpiar buffer


    switch (opcion){
        case 1: busqueda_datos("Nombre"); break;//Llama a la funcion con el valor a buscar
        case 2: busqueda_datos("Apellido Paterno"); break;
        case 3: busqueda_datos("Apellido Materno"); break;
        case 4: busqueda_datos("Matricula"); break;
    }

    printf("Desea hacer otra busqueda? (S/N): ");
    scanf(" %c", &condicion);  // El espacio antes de %c limpia el espacion en blanco
    limpiar_buffer();

    if (tolower(condicion) == 's'){//Convierte a minusculas y compara
        system("cls");
        busqueda_alumno();  
    }
}

void mostrar_datos(){
    FILE *archivo = fopen("contactos.dat", "rb");
    if (!archivo) {
        printf("No hay contactos registrados aun.");
        return;
    }

    struct directorio temp;//hacemos una estructura temporal con temp
    int contacto_actual = 0;

    printf("=== LISTA DE CONTACTOS ===\n");

    while (fread(&temp, sizeof(struct directorio), 1, archivo)){//con la estructura temp guarda los datos del archivo bninario y los imprime en la forma de la estructura.
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

    do {
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

        switch (opcion) {
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
        while (getchar() != '\n');

    } while (tolower(condicion) == 's');
}

void editar_datos() {
    int matricula = 0;
    int encontrado = 0;
    struct directorio temp;

    FILE *archivo = fopen("contactos.dat", "rb+");
    if (!archivo) {
        printf("Error al abrir el archivo.\n");
        return;
    }

    printf("Ingrese la matricula del alumno que desea editar: ");
    scanf("%d", &matricula);
    while (getchar() != '\n'); // limpiar buffer

    while (fread(&temp, sizeof(struct directorio), 1, archivo)){
        if (temp.matricula == matricula){//Aqui compara si la matricula dada por el usuario es la misma en donde se encuentra temp.matricula.
            printf("\nAlumno encontrado:\n");
            imprimir_alumno(&temp);//imprime los datos del alumno
            valor_a_editar(&temp);//Aqui llamamos a la funcion donde se editan los datos datos
                
            //Volver al inicio de ese registro
            //fseek() mueve el puntero del archivo:
            //-sizeof(struct directorio): Retrocede el tamaño de un registro
            //SEEK_CUR: Desde la posición actual
             //Esta instrucción rebobina el puntero del archivo exactamente un registro hacia atrás para posicionarse al inicio del registro que acabamos de leer y queremos modificar.
            fseek(archivo, (long)(-sizeof(struct directorio)), SEEK_CUR);
            //fseek(): Función estándar de C para mover el puntero de posición en un archivo
            /*
            archivo: El puntero FILE* del archivo abierto
            (long)(-sizeof(struct directorio)): Distancia a mover (en bytes)
            SEEK_CUR: Punto de referencia para el movimiento

            sizeof(struct directorio):
            Calcula el tamaño en bytes de la estructura del registro
            Ejemplo: Si la estructura ocupa 100 bytes, esto devuelve 100
             
            Por que necesitamos esto?: 
            Cuando leemos con fread(), ocurre esto:
            Antes de fread():
            Puntero está al inicio del registro (posición X)
            Después de fread():
            Puntero avanza automáticamente al siguiente registro (posición X + tamaño_registro)
            Para modificar el registro leído, debemos volver a donde empezaba
            
            Ejemplo practico: 
            Analogía con una cinta magnética
            Imagina el archivo como una cinta de casete:
            Leer un registro (fread) es como grabar lo que escuchas
            La cabeza lectora avanza automáticamente
            fseek es como rebobinar exactamente una canción para regrabarla

            Ejemplo numérico
            Supongamos:
            struct directorio ocupa 128 bytes
            Estamos en el 3er registro del archivo
            Posición inicial: 256 (inicio del 3er registro: 2*128)
            Tras fread(): Puntero en 384 (256 + 128)
            Tras fseek(): Vuelve a 256 (384 - 128)
            */

            
            fwrite(&temp, sizeof(struct directorio), 1, archivo);//Sobrescribe el registro con los nuevos datos
            encontrado = 1;
            printf("\nContacto actualizado correctamente.\n");
            break;
            }
    }

    if (!encontrado) {
        printf("No se encontró ningún alumno con ID %d.\n", matricula);
    }

    fclose(archivo);
}

void eliminar_datos() {

    int matricula_buscada;
    int encontrado = 0;//para contacto encontrado
    struct directorio temp;//un directorio temporal

    printf("Ingrese la Matricula del alumno que desea eliminar: ");
    scanf("%d", &matricula_buscada);
    while (getchar() != '\n');

    FILE *archivo = fopen("contactos.dat", "rb");
    if (!archivo) {
        printf("\nError: No se puede acceder al archivo de contactos\n");
        return;
    }

    FILE *temp_archivo = fopen("temp.dat", "wb");
    if (!temp_archivo){//Verifica si el archivo temporal se creó correctamente. 
        printf("\nError: No se pudo crear archivo temporal\n");
        fclose(archivo);
        return;
    }

    int contactos_restantes = 0;
    while (fread(&temp, sizeof(struct directorio), 1, archivo) == 1) {//lee los registros del archivo uno por uno
        if (temp.matricula == matricula_buscada){//Compara la matrícula del registro actual con la matrícula buscada
            encontrado = 1;
            printf("\nContacto encontrado para eliminacion:\n");
            printf("Matricula: %d\n", temp.matricula);
            printf("Nombre: %s\n", temp.name);
        }else{
            //Si el alumno no es el buscado, entonces hace lo siguiente.
            //escribe el registro actual en el archivo temporal:
            if (fwrite(&temp, sizeof(struct directorio), 1, temp_archivo) != 1){
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

void ordenarDatos(struct directorio *alumnos) {
    char again;
    int orderOption = 0, conta = 0;

    FILE *archivo = fopen("contactos.dat", "rb");
    if (archivo == NULL) {
        printf("Error al abrir el archivo binario.\n");
        return;
    }

    //Carga todos los contactos desde el archivo binario
    //Lee registros del archivo uno por uno y los almacena en el array alumnos
    while (fread(&alumnos[conta], sizeof(struct directorio), 1, archivo) == 1){
        conta++;
    }
    fclose(archivo);

    if (conta == 0) {
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
                        //Compara nombres con strcmp()
                        if (strcmp(alumnos[i].name, alumnos[j].name) > 0){
                            //Intercambia registros si están en orden incorrecto
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
                        //Comparamos directamente los valores numéricos de teléfono
                        if (alumnos[i].tel > alumnos[j].tel){
                            //Intercambiamos registros si están en orden
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

        //Escribir los contactos ordenados en el archivo binario
        archivo = fopen("contactos.dat", "wb");
        if (archivo == NULL) {
            printf("Error al escribir el archivo binario.\n");
            return;
        }
        //Escribe todos los registros ordenados de vuelta al archivo
        fwrite(alumnos, sizeof(struct directorio), conta, archivo);
        fclose(archivo);

        //Muestra los contactos ordenados por consola
        for (int i = 0; i < conta; i++) {
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

                FILE *archivo = fopen("contactos.dat", "rb+"); // Modo append (agregar al final)
                if (archivo == NULL) {
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
                    conta++;
                }

                for (int i = 0; i < conta; i++) {
                    fwrite(&alumnos[i], sizeof(struct directorio), 1, archivo);
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
    return 0;
}