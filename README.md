# The Guardian Journey | Algoritmos y Estructuras de Datos U2

The Guardian Journey, es una  simulación interactiva de un mundo de guardianes y aldeas. Permite a los usuarios seleccionar guardianes, asignarlos a aldeas, viajar entre aldeas, enfrentar oponentes y realizar conexiones entre aldeas utilizando la alquimia. A continuación, se detallan los conceptos principales y las características del programa.

## Enlace De video
https://drive.google.com/file/d/1sODQ3jcMkcdt-3rQcpHy3xAW-4bCGWtU/view?usp=sharing

## Conceptos principales
### Guardianes
 Son personajes con un nivel de poder. Pueden ser seleccionados, asignados a aldeas y enfrentar oponentes.
### Aldeas
 Son ubicaciones en el mundo del programa. Cada aldea tiene un nombre, un maestro principal y una lista de guardianes asignados a ella.
Oponentes: Son guardianes enemigos que pueden encontrarse en una aldea y desafiar a los guardianes seleccionados por el usuario.
### Alquimia
Es un proceso especial que permite conectar dos aldeas existentes. Al realizar una conexión, se reduce el nivel de poder de los guardianes seleccionados y se establece una conexión bidireccional entre las aldeas.
Estrategia de desarrollo

## Caracteristicas del programa
El programa se ha desarrollado siguiendo una estructura modular, donde cada concepto principal se representa mediante clases y funciones relacionadas. Se utilizan estructuras de datos como vectores y punteros para almacenar y manipular los objetos Guardianes y Aldeas.

A continuación, se mencionan algunos detalles importantes de ciertas funciones del programa:

`createGuardian()`: Esta función permite al usuario crear un nuevo guardián, solicitando su nombre y nivel de poder. El guardián se crea como un objeto de la clase Guardian y se almacena en el vector guardians para su posterior uso.

`selectApprentice()`: Esta función permite al usuario seleccionar un aprendiz existente o crear uno nuevo. Si se selecciona un aprendiz existente, se muestra una lista de aprendices disponibles y se actualiza su MainMaster y PowerLevel según las reglas del programa.

`travelBetweenVillages()`: Esta función permite al usuario viajar entre aldeas. Se muestra una lista de aldeas disponibles y se le pide al usuario que elija una. Si la aldea seleccionada es "Tesla", se verifica si los guardianes seleccionados tienen suficiente poder para acceder a ella. Después del viaje, se actualiza el nivel de poder de los guardianes seleccionados y se realizan enfrentamientos con los oponentes en la aldea.

`alquimistas()`: Esta función permite al usuario realizar una conexión de aldeas utilizando la alquimia. Se muestra una lista de aldeas disponibles (excluyendo "Tesla") y se le pide al usuario que elija una aldea para conectar con la aldea actual. Se establece una conexión bidireccional entre las aldeas seleccionadas y se reduce el nivel de poder de los guardianes seleccionados.

`encounterOpponents()`: Esta función se utiliza para simular un encuentro entre el guardián seleccionado y una lista de oponentes. Toma como argumentos un puntero al guardián seleccionado y un vector de punteros a los oponentes. Durante el encuentro, se comparan los niveles de poder de los participantes y se determina el resultado del enfrentamiento. Esta función es crucial para el desarrollo de la trama y las interacciones entre los guardianes.

`cargarVillas()`: Esta función se utiliza para cargar las aldeas desde un archivo de datos. Toma como argumento el nombre del archivo y devuelve un vector de punteros a las aldeas cargadas. La función lee el archivo, extrae la información de cada aldea (nombre, maestro principal, aldeas conectadas, etc.) y crea objetos de la clase Village correspondientes. Luego, los objetos de aldea se almacenan en un vector y se devuelven al programa principal para su uso.
`showCombatHistory()`: muestra el historial de combates del jugador, incluyendo las victorias y derrotas, y contra quién se produjeron. Proporciona información relevante sobre las batallas anteriores del jugador.

Estas son solo algunas de las funciones destacadas del programa. Se recomienda revisar el código fuente completo para obtener una comprensión detallada de todas las funcionalidades implementadas.

## 📃 Instrucciones Generaless


Este programa simula un mundo de guardianes y aldeas, donde los jugadores pueden interactuar con diferentes personajes, viajar entre aldeas, enfrentarse a oponentes y más. A continuación, se detallan las instrucciones para utilizar el programa correctamente.

### Requisitos del Sistema
- Sistema operativo compatible (Windows)
- Compilador C++ compatible
- Archivos guardianes.csv y villas.csv

### Configuración e Instalación
1. Clona o descarga este repositorio en tu máquina local.
2. Asegúrate de tener un compilador C++ instalado y configurado correctamente en tu sistema.
3. Abre el archivo del programa principal `ProyectoU2.cpp` en tu entorno de desarrollo o editor de código preferido.

### Compilación y Ejecución
1. Compila el código fuente del programa utilizando tu compilador C++. Esto puede variar dependiendo del sistema operativo y del compilador utilizado.
   - Ejemplo de compilación en línea de comandos con g++ 
     ```shell
     g++ ProyectoU2.cpp -o ProyectoU2
     ```
2. Una vez compilado correctamente, ejecuta el programa resultante.
   - Ejemplo de ejecución en línea de comandos 
     ```shell
     ./ProyectoU2
     ```

### Instrucciones de Juego
1. Crear un nuevo aprendiz:
   - Selecciona la opción "Crear un nuevo aprendiz" en el menú principal.
   - Sigue las instrucciones para proporcionar el nombre y el poder inicial del aprendiz.
   - Una vez creado, el aprendiz se agregará al mundo de Guardianes y Aldeas.

2. Viajar entre aldeas:
   - Utiliza la opción "Viajar entre aldeas" en el menú principal.
   - Se mostrará una lista de aldeas disponibles para viajar.
   - Selecciona la aldea a la que deseas viajar y sigue las indicaciones del programa.
   - Durante el viaje, podrías encontrar oponentes en la aldea de destino. Prepárate para enfrentarlos.

3. Enfrentamientos con oponentes:
   - Los oponentes pueden apareceran durante tus viajes o en las aldeas que visites, se va a sugerir aquel de menor nivel para que te enfrentes.

4. Conexión de aldeas:
   - Utiliza la función "Alquimistas" para conectar aldeas.
   - Se te presentará una lista de aldeas disponibles para conectar.
   - Selecciona las aldeas que deseas conectar y sigue las indicaciones del programa.
   - Ten en cuenta que conectar aldeas requiere un descuento en el poder de tus guardianes seleccionados.

## Objetivo del Juego
El objetivo principal del juego es derrotar a Stormheart en la ciudad Tesla.
