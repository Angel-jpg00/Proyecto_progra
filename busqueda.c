void busqueda_datos(const char *valor_buscado){
    char busqueda[50], linea[100], almacenamiento[500];
    int alumnos_encontrados = 0 ;
    int imprimir_contacto = 0;
    int indice_buffer = 0;  // Índice circular para el buffer
    int lineas_restantes = 0; // Contador de líneas posteriores a imprimir

    printf("Ingresa el %s que desea buscar: ", valor_buscado);
    fgets(busqueda, sizeof(busqueda), stdin);
    busqueda[strcspn(busqueda, "\n")] = 0;//Elimina salto de lineas

    FILE *archivo = fopen("contactos.txt", "r");
    if (archivo == NULL){
        printf("No se encontro el archivo o error al abrirlo");
        return;
    }

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        strcpy(almacenamiento[indice_buffer], linea);
        indice_buffer = (indice_buffer + 1) % 300;

        if (strstr(linea, valor_buscado) != NULL && strstr(linea, busqueda) != NULL){
            imprimir_contacto = 1;
            //lineas_restantes = 4; // Ajusta según campos de tu estructura
            alumnos_encontrados++;
        }

        if (imprimir_contacto) {//imprime los datos del contacto cuando se encuentra una coincidencia.
            printf("%s", linea );
            if (strstr(linea, "------------") != NULL) {
                imprimir_contacto = 0;
            }
        }
    }