CarGL
=====

(https://github.com/rubenmv/CarGL)
Práctica para Gráficos por Computador. Ingeniería Multimedia (Universidad de Alicante)

CarGL es una ciudad virtual por la que usted puede desplazarse utilizando alguno de los vehículos que encontrará o mediante el uso de las cámaras disponibles. Se proporcionan paneles donde puede modificar ciertas características de la escena como puede ser la iluminación, sombreado, mostrar u ocultar objetos, entre otros.

Screenshots
-----------
![CarGL screenshot 1](/doc/cargl-1.PNG)
![CarGL screenshot 2](/doc/cargl-2.PNG)
![CarGL screenshot 3](/doc/cargl-3.PNG)

Plataformas
-----------
Este programa ha sido desarrollado en **Windows**, pero usa librerías disponibles en otros sistemas como Linux, por lo que compilarlo para estos no debería ser un problema.

Características
---------------
* Vehículos seleccionables
* Vehículos personalizables (se puede cambiar el color de la carrocería)
* Distintos tipos de cámara modificables
* Varias luces modificables
* Permite modificar las propiedades de la escena, como activar reflejos, luz ambiente, texturas...

Más información en los manuales de usuario y desarrollador dentro de la carpeta "doc".

Importante
----------
En este proyecto/práctica se utilizó código OpenGL antiguo en su mayor parte, hoy en día se recomiendan otras prácticas como las que se pueden encontrar en https://open.gl/. En cualquier caso si eres un iniciado este código puede ser útil para entener los básico de OpenGL y ciertos elementos del desarrollo en 3D.

TODO
----
* Implementación de algún tipo de modelo de sombras.

Compilación y uso
-----------------
Se incluye un fichero de proyecto para abrir con Code::Blocks. Simplemente revisar las rutas a las distintas librerías en las propiedades del proyecto e incluir las necesarias en los directorios donde se crean los binarios. Para cualquier problema enviarlo por aquí mismo.

Licencia
--------
Las librerías externas usadas se comparten bajo sus propias licencias, información en sus respectivos sitios:
* Glut para la interfaz (http://www.opengl.org/resources/libraries/glut/)
* tiny_obj_loader para la carga de los modelos 3D (https://github.com/syoyo/tinyobjloader)
* SOIL para la carga de imagenes para las texturas (http://www.lonesock.net/soil.html)

Los assets los comparto bajo licencia Creative Commons Attribute Share-Alike 4.0 International (http://creativecommons.org/licenses/by-sa/4.0/)

El resto de código está licenciado bajo la GNU GPLv3
(http://www.gnu.org/licenses/gpl-3.0.txt)
