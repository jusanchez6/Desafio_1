#include <app.hpp>

my_error_t app_main() {

    int n;
    cout << "Ingrese el numero de archivos para la prueba: ";
    cin >> n;

    if (n < 1) {
        cout << "Argumento invalido." << endl;
        return ERROR;
    } else {
        cout << "exito";
    }


    // Genera los nombres de los archivos a analizar

    for (int i = 1; i <= n; i++) {
        char enc_name[64];
        char pista_name[64];

        // Arma cada archivo con el indice del for
        snprintf(enc_name, sizeof(enc_name), "Encriptado%d.txt", i);
        snprintf(pista_name, sizeof(pista_name), "pista%d.txt", i);

        cout << "\nCaso " << i << ": " << enc_name << " con " << pista_name << endl;

        uint8_t* enc = nullptr;
        size_t enc_len = 0;

        if (!read_file(enc_name, &enc, &enc_len)) {
            cout << "No se pudo leer: " << enc_name << endl;
            continue;
        }

        // buscar el n y k del archivo i

        char* out_msg = nullptr;
        char* out_method = nullptr;
        uint8_t out_n = 0;
        uint8_t out_k = 0;

        
    }


    // PRUEBAS DE FUNCIONAMIENTO DE DESCOMPRESIÓN
    const uint8_t data[] = "0A0B1A2A3B";
    printf("\n\nTexto comprimido: %s\n", data);
    char* result = lz78_decompress(data, strlen((char*)data));
    if (result) {
        printf("Descomprimido: %s\n", result);
    }

    const uint8_t data2[] = "4A3B2C1D2A";
    printf("Texto comprimido: %s\n", data2);
    char* result2 = rle_decompress(data2, strlen((char*)data2));
    if (result2) {
        printf("Descomprimido: %s\n", result); // "AAAAABBBCC"
        delete[] result2;
    }



    return OK;
}



my_error_t read_file (const char* path, uint8_t** out_buf, size_t* out_len) {

    // Abrir fichero en modo binario para lectura
    FILE* f = fopen(path, "rb");

    // Si no se pudo abrir, devolver error
    if (!f) {
        fprintf(stderr, "Error: no se pudo abrir el archivo %s\n", path);
        return ERROR;
    }

    // Mover el cursor al final para averiguar tamaño
    if (fseek(f, 0, SEEK_END) != 0) {
        fprintf(stderr, "Error: fallo al mover el puntero al final del archivo\n");
        fclose(f);
        return ERROR;
    }

    // ftell devuelve la posición (tamaño) como long
    long sz = ftell(f);

    // Comprobar que ftell devolvió un valor válido
    if (sz < 0) {
        fprintf(stderr, "Error: fallo al obtener el tamaño del archivo\n");
        fclose(f);
        return ERROR;
    }

    // Volver al inicio del fichero para la lectura
    if (fseek(f, 0, SEEK_SET) != 0) {
        fprintf(stderr, "Error: fallo al reposicionar el puntero al inicio\n");
        fclose(f);
        return ERROR;
    }

    // Reservar un buffer dinámico del tamaño
    uint8_t *buf = new uint8_t[sz];
    // Leer todo el fichero en el buffer (r bytes leídos)
    size_t r = fread(buf, 1, (size_t)sz, f);
    // Cerrar fichero ya que la lectura terminó
    fclose(f);

    // Si no se leyeron todos los bytes esperados, limpiar y devolver error
    if (r != (size_t)sz) {
        fprintf(stderr, "Error: solo se leyeron %zu de %ld bytes\n", r, sz);
        delete[] buf;
        return ERROR;
    }

    // Asignar resultados a los punteros de salida
    *out_buf = buf;
    *out_len = (size_t)sz;

    return OK;

}
