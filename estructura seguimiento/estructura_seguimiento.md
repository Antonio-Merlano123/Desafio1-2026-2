# seguimiento del proyecto sweet crush

Este archivo registra el avance del proyecto. La idea es trabajar por etapas pequenas,
revisar cada cambio y dejar una explicacion sencilla de lo que se agrego.

No se hacen commits en este momento. El commit se hara despues de revisar las etapas.

## estado real de los commits

La rama `main` local y `origin/main` estan sincronizadas en este momento. El problema no
es una diferencia entre las ramas, sino que las actualizaciones del documento no coinciden
uno a uno con los commits que existen en el historial.

### actualizaciones que ya tienen commit

- 01 y 02: cubiertas por los commits iniciales del proyecto.
- 03: organizar los modulos del proyecto.
- 04: calcular la memoria minima para las fichas.
- 05: limpiar y validar la memoria.

### actualizaciones hechas pero sin commit propio

- 06: leer una ficha dentro de un byte.
- 07: leer una ficha partida.
- 08: escribir una ficha dentro de un byte.
- 09: escribir una ficha partida.

Estas cuatro actualizaciones aparecen como realizadas en el documento, pero no deben
considerarse subidas hasta crear sus commits correspondientes.

### siguiente orden de trabajo

El siguiente commit debe cerrar la actualizacion 06. Despues se subiran 07, 08 y 09 en
orden. Solo cuando esas etapas tengan su commit se continuara con la actualizacion 10.

No se mezclaran comentarios, correcciones de estilo o cambios de documentacion con esos
commits funcionales.

## actualizacion 01 - crear la base del proyecto

**estado:** realizada

### que se hizo

- se creo `Desafio1.pro`.
- se configuro el proyecto como una aplicacion sencilla de consola en c++.
- se dejo activado c++17.
- se definieron los archivos que formaran los modulos del programa.
- se dejo el proyecto como aplicacion de consola.
- por ahora el proyecto solo compila el inicio y la interfaz basica.

### archivos relacionados

- `Desafio1.pro`
- `README.md`

### decision tomada

Se usara una aplicacion de consola en c++ porque el desafio no exige una ventana grafica.
Esto permite concentrar el trabajo en la memoria dinamica, los bits y las reglas del juego.

---

## actualizacion 02 - crear el inicio del programa

**estado:** realizada

### que se hizo

- se creo `main.cpp`.
- se creo un inicio normal de c++ sin `QCoreApplication`.
- el programa llama a `mostrarInterfaz()` para comenzar la ejecucion.
- se dejo un mensaje inicial en consola usando `cout`.
- se dejaron fuera del archivo `.pro` los modulos que todavia no se han trabajado.

### archivos relacionados

- `main.cpp`
- `interfaz.h`
- `interfaz.cpp`

### decision tomada

`main.cpp` solo inicia el programa. La lectura de opciones y la presentacion del tablero
quedan en el modulo de interfaz para no mezclar responsabilidades.

### aclaracion posterior

el proyecto no usa qt. se dejo la configuracion de consola con `CONFIG -= qt`, como en
el ejemplo de clase. `main.cpp` tampoco usa `QCoreApplication`.

---

## estado actual antes de continuar

La base del programa ya inicia y muestra un mensaje sencillo. Los modulos de bits, tablero
y juego estan guardados en la carpeta, pero todavia no se conectan al proyecto porque se
trabajaran en las siguientes actualizaciones.

La representacion interna del tablero debe seguir siendo un bloque dinamico de bytes,
con tres bits por ficha. La forma de mostrarlo en consola puede parecer una matriz, pero
esa matriz no sera el almacenamiento real del tablero.

### forma de escribir el codigo

los comentarios importantes se escriben en minuscula y sin tildes. se usan para explicar
decisiones o recorridos que pueden ser dificiles de seguir. los nombres de variables y
funciones tambien se mantienen en minuscula, algunos pegados y otros con guion bajo,
segun lo que se entienda mejor.

---

# plan de las 20 actualizaciones

## 01. crear la base del proyecto

Crear y configurar `Desafio1.pro` para una aplicacion de consola en c++17 sin qt.

**estado:** realizada.

## 02. crear el inicio del programa

Crear `main.cpp` y dejar el punto de entrada separado de la interfaz.

**estado:** realizada.

## 03. organizar los archivos de los modulos

Revisar que cada modulo tenga su archivo `.h` y `.cpp`, sin agregar logica complicada.

**estado:** realizada.

### que se hizo

- se conectaron `bits.cpp`, `tablero.cpp` y `juego.cpp` al archivo `.pro`.
- se conectaron tambien sus archivos de encabezado.
- cada modulo conserva una responsabilidad distinta.
- no se agregaron librerias nuevas ni logica en esta etapa.

### decision tomada

se mantienen cuatro modulos sencillos: `bits` para los tres bits, `tablero` para la
memoria, `juego` para las reglas e `interfaz` para la consola. asi cada parte se puede
revisar por separado.

## 04. calcular los bytes necesarios

Crear la funcion que calcule la cantidad minima de bytes para tres bits por ficha.

**estado:** realizada.

### que se hizo

- se reviso la funcion `bytes_necesarios` del modulo `bits`.
- se dejo la cuenta de tres bits por cada posicion.
- se agrego el redondeo para completar el ultimo byte.
- se agrego una validacion sencilla para devolver cero cuando no hay posiciones.

### decision tomada

la cuenta se hace con enteros y no se guarda una ficha completa por separado. asi la
memoria se calcula antes de pedir el bloque dinamico.

## 05. limpiar el bloque de memoria

Agregar una funcion sencilla para inicializar los bytes del tablero en cero.

**estado:** realizada.

### que se hizo

- se reviso `limpiar_bytes` dentro del modulo `bits`.
- se dejo el recorrido que pone cada byte en cero.
- se agrego una validacion para no recorrer un puntero vacio.
- tambien se evita recorrer cuando la cantidad de bytes no es positiva.

### decision tomada

la limpieza se hace byte por byte porque el tablero se guarda como un bloque continuo.
no se usa una matriz auxiliar ni una funcion externa para borrar la memoria.

## 06. leer una ficha dentro de un byte

Leer los tres bits de una ficha cuando no cruzan la frontera de un byte.

**estado:** realizada.

### que se hizo

- se reviso la forma de localizar la ficha por su indice.
- se calcula el byte inicial y el desplazamiento dentro del byte.
- se usa la mascara `00000111` para obtener solo los tres bits de la ficha.
- la lectura se hace sin tocar el resto del bloque de memoria.

### decision tomada

la operacion usa tres bits por ficha y el bit menos significativo como base. asi cada ficha
ocupa una zona ordenada dentro del bloque y no se mezclan valores de otras fichas.

### validacion simple

se revisa que el valor leido coincida con el valor escrito cuando la ficha queda dentro del
mismo byte. esto confirma que la mascara y el desplazamiento no rompen los bits vecinos.

## 07. leer una ficha partida

Completar la lectura cuando una ficha ocupa partes de dos bytes.

**estado:** realizada.

### que se hizo

- se detecta cuando la ficha empieza cerca del final del byte.
- se junta la parte baja del primer byte con la parte alta del segundo byte.
- la union se recorta con una mascara final para quedarse con solo los tres bits validos.

### decision tomada

la lectura partida solo une los fragmentos necesarios. no se cambia la estructura del bloque
ni se reserva memoria extra para la ficha. la operacion se mantiene simple y directa.

## 08. escribir una ficha dentro de un byte

Modificar tres bits sin afectar las fichas vecinas.

**estado:** realizada.

### que se hizo

- se limpia la zona de la ficha con una mascara.
- se escribe el valor nuevo en la posicion exacta.
- se conserva el resto de bits del byte sin cambiar.

### decision tomada

la escritura debe borrar solo los tres bits que pertenecen a esa ficha y no tocar la
restante parte del almacenamiento. asi cada valor queda aislado sin dañar al vecino.

## 09. escribir una ficha partida

Completar la escritura cuando los bits de una ficha estan en dos bytes.

**estado:** realizada.

### que se hizo

- se calcula la fraccion que entra en el primer byte y la fraccion que cae en el segundo.
- se limpia cada tramo con mascaras distintas.
- se escribe el valor en ambos bytes manteniendo los bits ajenos intactos.

### decision tomada

la escritura partida sigue la misma regla que la lectura partida: cada ficha se escribe en
sus dos fragmentos sin romper el bloque general. si la ficha no cabe en un solo byte, se
reparte exactamente en los bits correctos.

## 10. crear y destruir el tablero

Reservar y liberar la memoria dinamica del tablero segun las dimensiones iniciales.

**estado:** pendiente.

## 11. acceder por fila y columna

Convertir una fila y una columna en un indice logico del tablero.

**estado:** pendiente.

## 12. llenar el tablero inicial

Generar las fichas iniciales usando una funcion manual que produzca valores de 0 a 5.

**estado:** pendiente.

## 13. mostrar bits y fichas

Mostrar primero los bytes en binario y luego el tablero con sus fichas, indices de filas
y columnas.

**estado:** pendiente.

## 14. eliminar una ficha

Permitir que el usuario indique una fila y una columna para eliminar una ficha.

**estado:** pendiente.

## 15. buscar combinaciones

Recorrer el tablero horizontal y verticalmente para encontrar tres o mas fichas iguales.

**estado:** pendiente.

## 16. eliminar combinaciones simultaneas

Marcar primero todas las fichas y eliminarlas juntas para resolver cruces y combinaciones
que aparecen en el mismo recorrido.

**estado:** pendiente.

## 17. aplicar la gravedad

Hacer que las fichas caigan desde arriba despues de una eliminacion automatica.

**estado:** pendiente.

## 18. rellenar y resolver cascadas

Rellenar todos los espacios despues de eliminar y volver a buscar combinaciones hasta que
el tablero quede estable.

**estado:** pendiente.

## 19. modificar filas, columnas y memoria

Agregar y eliminar una fila o columna por operacion. Crear bloques nuevos cuando sea
necesario y aplicar la regla del 65 por ciento al reducir la memoria.

**estado:** pendiente.

## 20. completar estadisticas y documentacion

Mostrar dimensiones, movimientos, puntos, fichas eliminadas, combinaciones, cascadas y
la memoria usada. Revisar el README y preparar las pruebas del video.

**estado:** pendiente.

---

## reglas que se deben conservar

- no usar `struct`, `class`, `template`, STL ni estructuras dinamicas de bibliotecas.
- no usar codigo ANSI C.
- no usar `printf`, `scanf`, `stdio`, `rand` ni `QRandomGenerator`.
- usar c++ sencillo y nombres de variables claros.
- mantener los comentarios cortos y naturales.
- guardar el tablero en memoria dinamica y empaquetar cada ficha en tres bits.
- mostrar los bits y el tablero de fichas despues de cada jugada.
- hacer las operaciones del menu de una en una.
- eliminar todas las combinaciones actuales antes de rellenar.
- contar las caidas verticales como cascadas.
- dejar los bits sobrantes agrupados a la izquierda de la trama.
- no hacer commits hasta revisar cada avance.
