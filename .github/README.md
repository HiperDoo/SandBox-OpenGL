<!--
=====> ENCABEZADO <=====
-->
<div align="center">
    <h1>
        <img src="https://avatars.githubusercontent.com/u/47906376?v=4" width="300">
        <br>SandBox OpenGL [Modern]
    </h1>
    <h4>Mi pequeña caja recreativa para experimentar con OpenGL</h4>

[![](https://dcbadge.vercel.app/api/server/VA5ENPXWPJ)](https://discord.gg/VA5ENPXWPJ)

[![](https://img.shields.io/github/repo-size/HiperDoo/SandBox-OpenGL?style=for-the-badge&logo=Github&color=green)](https://github.com/HiperDoo/SandBox-OpenGL/releases)
[![](https://img.shields.io/github/last-commit/HiperDoo/SandBox-OpenGL?style=for-the-badge&logo=Github&color=19A1CA)](https://github.com/HiperDoo/SandBox-OpenGL)
[![](https://img.shields.io/github/stars/HiperDoo/SandBox-OpenGL?style=for-the-badge&logo=Github&color=yellow)](https://github.com/HiperDoo/SandBox-OpenGL/stargazers)
    <br><br><p>Desarrollado con:</p>

![C++17](https://img.shields.io/badge/C++17-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Terminal](https://img.shields.io/badge/GCC%2013.1.1-%234D4D4D.svg?style=for-the-badge&logo=windows-terminal&logoColor=white)
![CMake](https://img.shields.io/badge/CMake%203.25.2-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)

![OpenGL](https://img.shields.io/badge/OpenGL%204.5-%23FFFFFF.svg?style=for-the-badge&logo=opengl)
![Arch](https://img.shields.io/badge/Arch%20Linux-1793D1?logo=arch-linux&logoColor=fff&style=for-the-badge)
</div>

<!--
=====> CUERPO <=====
-->

---
Links de descarga del programa

![Download](https://img.shields.io/badge/Descargar%20x64-0.0.1-%234ea94b.svg?style=for-the-badge)
<img src="https://github.com/HiperDoo/media/blob/main/icons/w10.png" width="28">
<img src="https://github.com/HiperDoo/media/blob/main/icons/linux.png" width="28">
<br>

## 🌟 Motivación
<p align="justify">
    &nbsp;&nbsp;&nbsp;&nbsp;Ser capaz de entender todo lo que necesita una computadora para dibujar un simple triángulo en la pantalla es algo asombroso, pero entender cómo crear un <code>Motor de Juegos</code> propio lo es todavía más, el cual también es un gran reto. Tener una meta o un proyecto deseado en mente es un excelente incentivo para seguir aprendiendo un área considerado difícil por muchos.
</p>
<br>

## 🧩 ¿Y esto pa ke?
<p align="justify">
    &nbsp;&nbsp;&nbsp;&nbsp;Este entorno es donde junto, experimento y mejoro diversas mecánicas de renderizado usando <strong>OpenGL</strong>, las cuales forman parte de un proyecto más grande. Las <strong>optimizaciones</strong> y <strong>reducción de abstracciones</strong> dentro del código son una prioridad que, además de obtener resultados favorables en la ejecución, resultan ser de gran ayuda a la hora de depurar (por muchas experiencias ._.).
    <br><br>Cabe resaltar que este proyecto está desarrollado usando las funciones <a href="https://www.khronos.org/opengl/wiki/Direct_State_Access#:~:text=Direct%20State%20Access%20(DSA)%20is,all%20parts%20of%20an%20application."><strong>DSA OpenGL</strong></a> (que resumidamente, permite la capacidad de modificar los objetos de <strong>OpenGL</strong> sin usar <code>binds</code>, reduciendo código y aumentando considerablemente el rendimiento del programa).
</p>

> 📌 Los métodos de programación que utilicé son mi **propio estándar** sobre cómo debería hacer un proyecto en **C/C++** (en constante mejora).\
📌 Esta clase de proyectos los considero como hobbies, por lo que tienen la **menor prioridad** sobre el tiempo de mi día, avanzo en esta clase de proyectos cuando tengo tiempo y ganas :p

> **Warning** - Este repositorio **NO** es un tutorial ni un estándar definitivo, pero cualquier tipo de duda es bien recibida.

<br>

## 📷 Capturas y Videos
* **TODO** - Agregar media en esta sección (todo se guardará en el directorio .github).

<br>

## 🏅 Características
#### 🔶 Código
* Limpieza y legibilidad del código.
* Minimizar los alojamientos de memoria reciclando buffers y llevando la lógica del código en un modelo predictivo.
* Compilación de librerías de forma estática (tanto para Linux como para Windows).
* Máxima simplificación de pasos requeridos para compilar el proyecto en dos simples comandos.
#### 🔶 Programa
* Cubemap (simulación de entorno tridimensional).
* Cargador de modelos 3D (formato de archivo personal). {Mucho por mejorar}
* **TODO** - Características sobresalientes en proceso de desarrollo...

<br>

## 📜 TODO
* Reordenar código y lógica de esta en zonas críticas (renderizado de objetos).
* Mover recursos visuales del Stack al Heap (falta total de dinamismo en la creación de objetos).
* **TODO** - Agregar siguiente lista de metas al terminar las la lista anterior…

<br>

## 🧰 Requisitos y Dependencias
> **Note** - La sección de `Aplicaciones/Paquetes`, `Librerías/Dependencias` y `Compilación Linux y Windows` sólo son importantes si se desea compilar el proyecto.

**🔩 Compatibilidad del Sistema**
* **OS** - Linux/Windows 10 (no ha sido probado en las diferentes distribuciones de Linux que existen, pero no hay razón por la que no sea posible ejecutar el programa).
* **CPU** - De `64 bits`.
* **GPU** - Con soporte para OpenGL `4.5` o superior.
* **RAM** - `180MB` (ni se para que especifico esto :p).
* **VRAM** - `130MB` (otro dato que no es de preocuparse).

**📦 Aplicaciones/Paquetes**
> Para Linux y Windows descargar...
* **[CMake](https://cmake.org/download/)** - `3.25.2` Administrador para la compilación.
* **[Git](https://git-scm.com/downloads)** - `3.39.1` Encargado de descargar este repositorio (se recomienda usar esto ya que descarga y acomodar las librerías de forma automática).
> Para Linux descargar...
* **[GCC](https://www.geeksforgeeks.org/how-to-install-gcc-compiler-on-linux/)** - `1.13.1` Compilador de código **C/C++**.
* **[Make](https://www.geeksforgeeks.org/how-to-install-make-on-ubuntu/)** - `4.4` Para compilar las librerías de forma estática (mayor portabilidad y velocidad en el ejecutable final).
> Para Windows descargar...
* **[MinGW](https://github.com/niXman/mingw-builds-binaries/releases)** - `12.2.0` Compilador de código **C/C++** y también **mingw32-make** (lo mismo que **make**).

**📚 Librerías/Dependencias**
* **[GLFW](https://github.com/glfw/glfw)** - `3.3.8` Creación de ventanas y comunicación con las funciones de **OpenGL**.
* **[GLEW](https://github.com/Perlmint/glew-cmake/)** - `2.1` Cargador y administrador de las extensiones de **OpenGL** que mejor se adapten al equipo a ejecutar.
* **[GLM](https://github.com/g-truc/glm)** - `0.9.9` Conjunto de funciones matemáticas basadas en las especificaciones del lenguaje de Shaders (**GLSL**).
* **[STB_IMAGE](https://github.com/nothings/stb/blob/master/stb_image.h)** - `2.27` Cargador de imágenes (único archivo .h).
* **[FMT](https://github.com/fmtlib/fmt)** - `9.1.0` Alternativa más eficiente que **std::cout** y **printf()**.

<details>
<summary>Árbol de dependencias:</summary>

> Para que las dependencias (librerías) sean visibles para CMake, es necesario tener un orden de estas dentro de la carpeta `/SandBox-OpenGL/dep` de la siguiente manera (solamente en caso de **NO** haber usando el comando `git clone …`, sino mediante descargas manuales):
> ```
> $ SandBox-OpenGL
> ├── .github
> ├── dep
> │   ├── fmt
> |   |   ├── include/*.h
> |   |   ├── src/*.cc
> |   |   └── CMakeLists.txt
> │   ├── glew
> |   |   ├── build
> |   |   ├── include/GL/*.h
> |   |   └── src/*.c
> |   ├── glfw
> |   |   ├── include/*.h
> |   |   └── src/*.c & *.h
> |   ├── glm
> |   |   ├── glm
> |   |   |  ├── directorios...
> |   |   |  ├── CMakeLists.txt
> |   |   |  └── /*.hpp
> |   |   └── CMakeLists.txt
> │   └── stb
> |   |   └── stb_image.h
> ├── include
> ├── Release
> ├── src
> ├── .gitignore
> ├── .gitmodules
> ├── CMakeLists.txt
> └── README.md
> ```
</details>

<br>

<!-- REG ADD HKCU\CONSOLE /f /v VirtualTerminalLevel /t REG_DWORD /d 1 -->
## 🔨 Compilación Linux y Windows
> **Note** - Sección para compilar el proyecto (si solo deseas correr el programa, vuelve arriba a la sección de descargas).

Para compilar correctamente este repositorio con sus respectivas librerías (sub-módulos), diríjase al directorio donde desee hacer la instalación dentro de una Terminal/CMD usando los comandos `cd /tu/directorio`. Una vez dentro del directorio, ejecute la siguiente serie de comandos:
```bash
# Descarga del repositorio con sus respectivos sub-módulos
git clone --recurse-submodules -j4 https://github.com/HiperDoo/SandBox-OpenGL.git

# Entrar a la carpeta raíz
cd SandBox-OpenGL

# Construir repositorio
./sh.bat setup

# Compilar código
./sh.bat build

# Ejecución del programa
./sh.bat run
```
Repita únicamente los últimos dos pasos al hacer cambios al código
<br>

## 💣 ¿Algún error?
> Aquí hay una lista de algunos de los posibles errores con soluciones que podrías llegar a enfrentarte (si tienes algún error o problema, recuerda que puedes ir a **[Issues](https://github.com/HiperDoo/SandBox-OpenGL/issues)** y describir la situación en la que te encuentras para poder ayudarte).
* Para usuarios de Windows, verificar que los directorios de MinGW y CMake estén en Variables de Entorno.
* Verificar que las versiones tanto de programas como librerías sean las mismas (no debería de haber problema con nuevas versiones, pero nunca se sabe :p).
* Para usuarios de Windows, si la terminal no muestra los colores en el texto (en cambio una combinación de caracteres extraños), ejecutar `REG ADD HKCU\CONSOLE /f /v VirtualTerminalLevel /t REG_DWORD /d 1` en una nueva terminal y volver ejecutar el programa.