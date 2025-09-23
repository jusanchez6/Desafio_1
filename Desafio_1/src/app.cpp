#include <app.hpp>

namespace
{
    size_t get_file_size(const char *path)
    {
        // Abrir fichero en modo binario para lectura
        FILE *f = fopen(path, "rb");

        // Si no se pudo abrir, devolver error
        if (!f)
        {
            fprintf(stderr, "Error: no se pudo abrir el archivo %s\n", path);
            return NO_SIZE;
        }

        // Mover el cursor al final para averiguar tamaño
        if (fseek(f, 0, SEEK_END) != 0)
        {
            fprintf(stderr, "Error: fallo al mover el puntero al final del archivo\n");
            fclose(f);
            return NO_SIZE;
        }

        // ftell devuelve la posición (tamaño) como long
        long sz = ftell(f);

        // Comprobar que ftell devolvió un valor válido
        if (sz < 0)
        {
            fprintf(stderr, "Error: fallo al obtener el tamaño del archivo\n");
            fclose(f);
            return NO_SIZE;
        }

        // Volver al inicio del fichero para la lectura
        if (fseek(f, 0, SEEK_SET) != 0)
        {
            fprintf(stderr, "Error: fallo al reposicionar el puntero al inicio\n");
            fclose(f);
            return NO_SIZE;
        }
        
        fclose(f);

        return static_cast<size_t>(sz);         // Este cast es para OBLIGAR al compilador a que no haga nada raro con la variable y la convierta en size_t
    }

};

my_error_t app_main()
{

    int n;
    cout << "Ingrese el numero de archivos para la prueba: ";
    cin >> n;

    if (n < 1)
    {
        cout << "Argumento invalido." << endl;
        return ERROR;
    }

    // Genera los nombres de los archivos a analizar

    for (int i = 1; i <= n; i++)
    {
        char enc_name[64];
        char pista_name[64];

        // Arma cada archivo con el indice del for
        snprintf(enc_name, sizeof(enc_name), "Encriptado%d.txt", i);
        snprintf(pista_name, sizeof(pista_name), "pista%d.txt", i);

        // ====== Lectura del archivo encriptado ============
        uint8_t *enc = nullptr;
        size_t enc_len = 0;

        if (!read_file(enc_name, &enc, &enc_len))
        {
            cout << "No se pudo leer: " << enc_name << endl;
            continue;
        }

        // ========= Lectura de la pista ==============

        char* frag = get_frag(pista_name);

        if (!frag)
        {
            cout << "No se pudo leer: " << pista_name << endl;
            delete[] enc; // liberar lo que ya reservamos
            continue;
        }

        cout << "** " << enc_name << " **\n";
        
        // buscar el n y k del archivo i

        char *out_msg = nullptr;
        char *out_method = nullptr;
        uint8_t out_n = 0;
        uint8_t out_k = 0;

        bool result = finder(enc, enc_len, frag, &out_msg, &out_method, &out_n, &out_k);

        if (result) {
            cout << "Compresión: " << out_method << endl;
            cout << "Rotación: " << (int)out_n << "\nk: " << (int)out_k << endl;
            cout << "Mensaje: \n\n" << out_msg << endl;

            // Liberar la memoria reservada
            delete[] out_msg;
            delete[] out_method;

        } else {
            cout << "No se encontró coincidencia con la pista.\n";
        }

        // Liberar memoria reservada
        delete[] enc;
        delete[] frag;
        
    }

    return OK;
}

my_error_t read_file(const char *path, uint8_t **out_buf, size_t *out_len)
{

    size_t sz = get_file_size(path);

    if(!sz) {
        return ERROR;
    }

    FILE *f = fopen(path, "r");
    if (!f) {
        cout << "Error: No se pudo abrir el archivo: " << path << "\n";
        return ERROR;
    }

    uint8_t *buf = new uint8_t[sz];
    size_t r = fread(buf, 1, sz, f);
    fclose(f);

    if (r != sz) {
        cout << "Error: No se leyó el archivo completo.\n";
        delete[] buf;
        return ERROR;
    }

    *out_buf = buf;
    *out_len = sz;
    return OK;

}

char *get_frag(const char *path)
{
    uint8_t* buf = nullptr;
    size_t len = 0;

    if (read_file(path, &buf, &len) != OK) {
        return nullptr;
    }

    // Hacemos una copia terminada en '\0'
    char *frag = new char[len + 1];
    memcpy(frag, buf, len);
    frag[len] = '\0';

    delete[] buf; // liberar el buffer
    return frag;

}