

| <center> &#x1F344;&#x1F344;&#x1F344;&emsp;MUSHROOMER &emsp;&#x1F344;&#x1F344;&#x1F344; |
|---|
| &#x1F3DB;&#xFE0F;&emsp;**Universidad de Buenos Aires - Facultad de Ingeniería** |
| &#x1F4DA;&emsp;**86.65 - Sistemas Embebidos** |
| &#x1F464;&emsp;**Gabriel Sosa (87311)** |
| &#x1F5D3;&emsp;**Primer cuatrimestre de 2025** |



---

# 1. Selección del proyecto a implementar

## 1.1 Objetivo del proyecto y resultados esperados

&emsp;El objetivo de este proyecto es diseñar e implementar un sistema embebido que controle automáticamente las condiciones ambientales de una cámara de fructificación de hongos. El sistema, denominado **Mushroomer**, busca mantener la humedad, temperatura, ventilación e iluminación dentro de rangos definidos por el usuario, de forma autónoma y eficiente.

&emsp;Además, se pretende registrar eventos relevantes (por ejemplo, activación de actuadores) y permitir una interacción amigable con el usuario mediante una pantalla gráfica.


## 1.2 Proyectos similares

&emsp;Se consideraron los siguientes proyectos como alternativas viables para este trabajo final:

* **Invernadero automatizado**: sistema que permite controlar variables ambientales (temperatura, humedad, iluminación) dentro de un invernadero para favorecer el crecimiento de plantas. Suele integrar sensores y actuadores como ventiladores, lámparas y bombas de riego, operados de forma automática o semiautomática.

* **Estación meteorológica**: dispositivo diseñado para medir variables climáticas del entorno (temperatura, humedad, presión, lluvia, luminosidad, etcétera) y almacenar los datos en una memoria local o enviarlos a través de una red para su posterior análisis.

* **Sistema Mushroomer para cultivo de hongos (proyecto propuesto)**: sistema embebido orientado al control de una cámara de fructificación de hongos, que automatiza el manejo de la humedad, temperatura, ventilación e iluminación, e incluye una interfaz gráfica con registro digital de eventos. Diseñado de forma modular, está pensado para futuras expansiones.

&emsp;Para compararlos, se definieron los siguientes criterios y ponderaciones:

| Criterio                              | Peso | Descripción                                                                             |
| ------------------------------------- | ---- | ----------------------------------------------------------------------------------------------- |
| **Disponibilidad de hardware**        | 10   | Se prioriza que la mayoría de los componentes estén ya disponibles o sean fáciles de conseguir. |
| **Utilidad del proyecto**             | 5    | Se valora que tenga aplicación real y concreta.           |
| **Tiempo estimado de implementación** | 8    | Es necesario que pueda desarrollarse en las etapas del curso.                                   |
| **Costo**                             | 6    | Se busca que sea accesible.                             |
| **Interés personal**                  | 7    | El interés en la temática favorece la motivación y el compromiso con el desarrollo.  |
<p align="center"><em>Tabla 1.2.1: Criterios de evaluación y ponderación</em></p>

&emsp;A continuación, se presenta la tabla de evaluación de los proyectos:

| Criterio                       | Peso | Invernadero automatizado | Estación meteorológica | Mushroomer |
| ------------------------------ | ---- | ------------------------ | ---------------------- | ---------- |
| **Disponibilidad de hardware** | 10   | 7 → 70                   | 6 → 60                 | 9 → 90     |
| **Utilidad del proyecto**      | 5    | 8 → 40                   | 7 → 35                 | 8 → 40     |
| **Tiempo de implementación**   | 8    | 5 → 40                   | 6 → 48                 | 6 → 48     |
| **Costo**                      | 6    | 5 → 30                   | 7 → 42                 | 8 → 48     |
| **Interés personal**           | 7    | 6 → 42                   | 5 → 35                 | 9 → 63     |
| **Puntaje total**              | —    | **222**                  | **220**                | **289**    |
<p align="center"><em>Tabla 1.2.2: Evaluación de proyectos</em></p>

## 1.3 Selección del proyecto

&emsp;Considerando los criterios evaluados en la Tabla 1.3.1, el proyecto **Mushroomer** se presenta como la opción más conveniente, con un puntaje ponderado claramente superior a las otras alternativas. Este resultado se debe a su excelente disponibilidad de hardware, bajo costo relativo, y alto interés personal en temáticas relacionadas con el cultivo controlado y el monitoreo ambiental.

### 1.3.1 Diagrama de bloques

&emsp;En la Figura 1.3.1 se muestra el diagrama en bloques del sistema con los principales módulos del proyecto.
<div align="center"> <img src="img/Diagrama de bloques.png" alt="Diagrama de bloques del sistema Mushroomer"/> </div> <p align="center"><em>Figura 1.3.1: Diagrama en bloques del sistema Mushroomer</em></p>


# 2. Elicitación de requisitos y casos de uso

## 2.1 Análisis de producto competidor

&emsp;En el mercado argentino, existe un único producto comercial comparable al sistema propuesto: la **Incubadora automática INC101** desarrollada por [Infomicelium](https://infomicelium.com.ar/productos/pack-oferta-incubadora-automatica-inc101/). Esta incubadora está orientada al cultivo de hongos comestibles y ofrece control automático de temperatura y humedad, además de iluminación y ventilación temporizada.

&emsp;Sin embargo, el sistema **Mushroomer** se diferencia por ofrecer una arquitectura modular basada en microcontrolador, interfaz gráfica mediante pantalla TFT táctil y aplicación móvil (vía Bluetooth), posibilidad de registro digital de eventos en tarjeta SD y capacidad de expansión de funcionalidades.

<p align="center"><em>Tabla 2.1.1: Comparación entre INC101 (Infomicelium) y Mushroomer</em></p>

| Característica                         | INC101 (Infomicelium)         | Mushroomer                     |
|----------------------------------------|-------------------------------|--------------------------------|
| Control de humedad                     | ✔ (temporizador + sensor)    | ✔ (digital, controlado por umbral) |
| Control de temperatura                 | ✔ (controlador incluido)     | ✔ (con sensor analógico)       |
| Iluminación programable                | ✔ (temporal)                 | ✔ (menú interactivo)           |
| Ventilación                            | ✔ (ventilador incluido)      | ✔ (compuerta con servomotor + cooler)   |
| Interfaz gráfica                       | ✖                             | ✔ (pantalla TFT con menú)      |
| Registro de eventos                    | ✖                             | ✔ (memoria SD opcional)        |
| Modularidad / expansión                | ✖                             | ✔ (diseño escalable)           |
| Precio aproximado                      | > ARS 280.000                 | No determinado |

## 2.2 Requisitos del sistema

&emsp;A continuación se enumeran los requisitos funcionales y no funcionales del sistema **Mushroomer**, agrupados por subsistemas. Todos ellos fueron redactados siguiendo los criterios **SMART** (específicos, medibles, alcanzables, relevantes y acotados en el tiempo).

<p align="center"><em>Tabla 2.2.1: Requisitos del sistema Mushroomer</em></p>

| Grupo              | ID   | Descripción                                                                 |
|--------------------|------|-----------------------------------------------------------------------------|
| Sensor de humedad  | 1.1  | El sistema debe medir la humedad relativa del aire cada 5 segundos.         |
| Sensor de temperatura | 1.2  | El sistema debe medir la temperatura cada 5 segundos.                      |
| Iluminación        | 2.1  | El sistema debe permitir encender/apagar la luz desde un menú en pantalla. |
| Humidificación     | 3.1  | El sistema debe activar un humidificador mediante un relé.                  |
| Ventilación        | 4.1  | El sistema debe abrir/cerrar una compuerta mediante un servomotor.         |
| Interfaz gráfica   | 5.1  | El sistema debe mostrar en pantalla las variables actuales (humedad y temperatura). |
|                    | 5.2  | El sistema debe mostrar un menú con acciones disponibles.                  |
| Registro de eventos| 6.1  | El sistema debe registrar los eventos en memoria SD si está disponible.    |
| Modo manual        | 7.1  | El usuario podrá activar/desactivar ventilación o iluminación desde el menú. |
| Seguridad          | 8.1  | El sistema debe prevenir fallos por sensores desconectados. |

## 2.3 Casos de uso

&emsp;A continuación se describen tres casos de uso típicos, representando las interacciones entre el usuario y el sistema.

### Caso de uso 1: El usuario enciende la luz del cultivo

| Elemento         | Descripción                                                                 |
|------------------|------------------------------------------------------------------------------|
| **Disparador**   | El usuario desea iluminar la cámara de cultivo.                             |
| **Precondiciones** | El sistema está encendido. La pantalla está operativa.                    |
| **Flujo principal** | El usuario navega el menú hasta la opción "Encender luz" y la selecciona. La luz se enciende, y se actualiza el estado en pantalla. |
| **Flujos alternativos** | a. El sistema detecta una falla en el GPIO correspondiente y notifica el error. |

<p align="center"><em>Tabla 2.3.1: Caso de uso 1</em></p>

### Caso de uso 2: El sistema activa la humidificación automáticamente

| Elemento         | Descripción                                                                 |
|------------------|------------------------------------------------------------------------------|
| **Disparador**   | El sistema detecta humedad inferior al umbral configurado.                  |
| **Precondiciones** | Sensor de humedad funcional. Humidificador conectado vía relé.            |
| **Flujo principal** | El sistema activa el relé por un tiempo predefinido. El evento se registra en la SD. |
| **Flujos alternativos** | a. La SD no está disponible: el evento se registra sólo en RAM. b. El sensor no responde: el sistema notifica en pantalla. |

<p align="center"><em>Tabla 2.3.2: Caso de uso 2</em></p>

### Caso de uso 3: El usuario consulta las variables actuales

| Elemento         | Descripción                                                                 |
|------------------|------------------------------------------------------------------------------|
| **Disparador**   | El usuario desea ver en pantalla la temperatura y la humedad actual.        |
| **Precondiciones** | Sistema encendido y sensores conectados.                                  |
| **Flujo principal** | El sistema muestra los valores actualizados en pantalla cada 5 segundos. |
| **Flujos alternativos** | a. Algún sensor falla: se muestra un mensaje de error. |

<p align="center"><em>Tabla 2.3.3: Caso de uso 3</em></p>