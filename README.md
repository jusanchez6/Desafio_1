# Desafío 1 - Proyecto de Compresión y Decodificación

Este proyecto implementa una aplicación en C++ para la lectura y análisis de archivos encriptados, con soporte para compresión y resolución mediante pistas externas.  
La finalidad de este desafío es demostrar el manejo de punteros y memoria dinámica en C++.  

Más información sobre los algoritmos desarrollados se encuentra en la carpeta `Documentación` de este repositorio.

---

## Estructura del proyecto

```
DESAFIO\_1/
├── build/                # Carpeta de compilación (Debug/Release)
│   └── Desktop-Debug
├── include/              # Archivos de cabecera
│   ├── app.hpp
│   ├── compress.hpp
│   └── solver.hpp
├── src/                  # Código fuente
│   ├── app.cpp
│   ├── compress.cpp
│   ├── main.cpp
│   └── solver.cpp
├── .gitignore
├── Desafio\_1.pro         # Proyecto Qt (qmake)
└── Desafio\_1.pro.user    # Configuración local de Qt Creator

```

---

## Requisitos

- **QtCreator** (recomendado para compilar fácilmente).  
- Alternativamente: compilador **C++17** con `qmake` o `cmake`.  
- (Opcional) **Doxygen** para generar documentación.

---

## Compilación y ejecución

### Con QtCreator
1. Abrir `Desafio_1.pro`.
2. Compilar en modo Debug o Release.
3. El binario se generará en `build/Desktop-Debug/bin/`.

### En Linux

```bash
./Desafio_1
```

### En Windows

Ejecutar desde QtCreator o directamente el binario compilado.

---

## Uso

1. Ejecuta el programa.
2. Ingresa el número de archivos a analizar.
3. El programa buscará archivos con los nombres:

   * `encriptadoX.txt`
   * `pistaX.txt`

   donde `X` corresponde al índice.

Ejemplo:

```bash

Ingrese el numero de archivos para la prueba: 2
** encriptado1.txt **
Compresión: LZ78
Rotación: 3
k: 15
Mensaje:

Hola Mundo!
```

> Asegúrate de que los archivos `encriptadoX.txt` y `pistaX.txt` estén en la misma carpeta que el binario (`build/Desktop-Debug/bin`) o en el directorio desde el cual ejecutes el programa.

---

## Documentación

El código está documentado con **Doxygen**.
Para generar la documentación:

```bash
doxygen -g       # solo la primera vez
doxygen Doxyfile # construye la documentación
```

La salida se generará en `docs/html/index.html`.

---

## Autores

* **Julián Sánchez**
* **Jaider Bedoya**

Curso: Informática 2
Semestre: 2025-2


