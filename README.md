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

[![](https://img.shields.io/github/release/HiperDoo/SandBox-OpenGL?style=for-the-badge&logo=Github&color=blue)](https://github.com/HiperDoo/SandBox-OpenGL/releases)
[![](https://img.shields.io/github/last-commit/HiperDoo/SandBox-OpenGL?style=for-the-badge&logo=Github&color=19A1CA)](https://github.com/HiperDoo/SandBox-OpenGL)
[![](https://img.shields.io/github/stars/HiperDoo/SandBox-OpenGL?style=for-the-badge&logo=Github&color=yellow)](https://github.com/HiperDoo/SandBox-OpenGL/stargazers)
    <br><br><p>Desarrollado con:</p>

![C++17](https://img.shields.io/badge/C++17-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Terminal](https://img.shields.io/badge/GCC%2012.2.1-%234D4D4D.svg?style=for-the-badge&logo=windows-terminal&logoColor=white)
![CMake](https://img.shields.io/badge/CMake%203.25.2-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)

![OpenGL](https://img.shields.io/badge/OpenGL%203.3-%23FFFFFF.svg?style=for-the-badge&logo=opengl)
![Arch](https://img.shields.io/badge/Arch%20Linux-1793D1?logo=arch-linux&logoColor=fff&style=for-the-badge)
<!--![Windows](https://img.shields.io/badge/Windows%2010-0078D6?style=for-the-badge&logo=windows&logoColor=white)-->
</div>

<!--
=====> CUERPO <=====
-->

---
<br><br>
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
📌 Esta clase de proyectos los considero como hobbies, por lo que tienen la **menor prioridad** sobre el tiempo de mi día, avanzo en esta clase de proyectos cuando tengo tiempo y ganas :p\
📌 Este repositorio **NO** es un tutorial ni un estándar definitivo, pero cualquier tipo de duda es bien recibida.

<br>

## 🧰 Requisitos y Dependencias
**Compatibilidad del sistema**
* "TODO: Cosas de hardware"

**Aplicaciones o paquetes**
* **[CMake](https://cmake.org/)** - 3.0 o superior.
* **[GCC](https://www.geeksforgeeks.org/how-to-install-gcc-compiler-on-linux/)** - Opté en usar la versión actual (1.12.1), no he probado versiones anteriores.

<!--TODO: reparar links-->
**Librerías o dependencias**
* **[GLFW](https://www.glfw.org/)** - 3.3.8 o superior.
* **[GLEW](https://glew.sourceforge.net/)** - 2.1 o superior.
* **[GLM](https://github.com/g-truc/glm)** - 0.9.9 o superior.

<br>

## 🔨 Compilación [Linux]
Abra una nueva terminal, vaya a la carpeta raíz de este proyecto (/SandBox) y ejecute los siguientes comandos:
```bash
# Construir proyecto (ejecutar una única vez):
./exe setup

# Compilación del proyecto:
./exe build

# Ejecución del programa:
./exe run
```