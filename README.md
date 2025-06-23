| <center> &#x1F344;&#x1F344;&#x1F344;&emsp;MUSHROOMER &emsp;&#x1F344;&#x1F344;&#x1F344; |
|---|
| &#x1F3DB;&#xFE0F;&emsp;**Universidad de Buenos Aires - Facultad de Ingeniería** |
| &#x1F4DA;&emsp;**86.65 - Sistemas Embebidos** |
| &#x1F464;&emsp;**Gabriel Sosa (87311)** |
| &#x1F5D3;&emsp;**Primer cuatrimestre de 2025** |

# Requisitos del sistema

&emsp;Se utilizan los siguientes marcadores para indicar el estado de los requisitos del sistema **Mushroomer** propuesto:

| Estado del requisito | Descripción |
|----------------------|-------------|
| &#x1F7E2; | Requisito cumplido |
| &#x1F7E1; | Requisito parcialmente cumplido o próximo a implementar |
| &#x1F7E0; | Requisito a cumplir sólo si es posible hacerlo dentro del tiempo estipulado |
| &#x1F534; | Requisito a excluir |

<p align="center"><em>Tabla 1: Nomenclatura adoptada para indicar el estado de los requisitos del sistema en desarrollo.</em></p>

| Grupo | ID | Descripción | Observaciones |
|---|---|---|---|
| Sensor de humedad | 1.1 | &#x1F7E2;&nbsp; El sistema debe medir la humedad relativa del aire cada 120 segundos automáticamente o cuando el usuario solicite una lectura por pantalla. | |
| Sensor de temperatura | 1.2  | &#x1F7E2;&nbsp; El sistema debe medir la temperatura cada 120 segundos automáticamente o cuando el usuario solicite una lectura por pantalla. | |
| Humidificación | 2.1 | &#x1F7E1;&nbsp; El sistema debe activar un humidificador mediante un relé. | Falta conseguir un módulo relé. |
| Ventilación | 3.1 | &#x1F7E2;&nbsp; El sistema debe encender un cooler y abrir una compuerta mediante un servomotor de forma sincronizada cada 5 minutos automáticamente o cuando el usuario solicite su activación. | |
| | 3.2 | &#x1F7E0;&nbsp; El conjunto cooler-compuerta debe estar montado correctamente en el recinto. | |
| Control de temperatura | 4.1 | &#x1F7E1;&nbsp; El sistema debe regular la temperatura dentro de los límites establecidos. | Falta implementar el módulo Peltier mediante el empleo del módulo L298N (un doble puente H para invertir la polaridad). |
| | 4.2 | &#x1F7E0;&nbsp; El módulo Peltier para el control de temperatura debe estar montado correctamente en el recinto. | |
| Interfaz gráfica | 5.1 | &#x1F7E2;&nbsp; El sistema debe mostrar en pantalla las variables ambientales actuales (humedad y temperatura) y el estado del sistema. | |
| | 5.2 | &#x1F7E2;&nbsp; El sistema debe mostrar un menú con acciones disponibles. | |
| Registro de eventos| 6.1 | &#x1F7E2;&nbsp; El sistema debe registrar los eventos en memoria SD si está disponible. | |
| Modo manual | 7.1 | &#x1F7E2;&nbsp;El usuario podrá activar/desactivar ventilación o iluminación desde el menú. |
| Seguridad | 8.1 | &#x1F534; El sistema debe prevenir fallos por sensores desconectados. |
| Conectividad | 9.1 | &#x1F7E1;&nbsp;El sistema debe tener la capacidad de informar las variables ambientales y ser controlado mediante interfaz Bluetooth a través de una aplicación móvil. | Se dispone del módulo de Bluetooth Low Energy HM-10. Falta realizar la implementación en la aplicación. |

<p align="center"><em>Tabla 2: Estado de los requisitos del sistema Mushroomer al 21 de junio de 2025.</em></p>
