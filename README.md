🎨 Motor de Trazado de Rayos 3D (Ray Tracer) — High Performance C++
Este proyecto implementa un motor de Trazado de Rayos 3D (Ray Tracing) desarrollado en C++ enfocado en programación orientada al rendimiento y eficiencia energética, como parte de la asignatura Arquitectura de Computadores (Curso 2025-26) en la Universidad Carlos III de Madrid (UC3M).
El renderizador simula físicamente el comportamiento de la luz rebotando en un entorno tridimensional con figuras geométricas (esferas y cilindros) y diversos materiales, generando como salida imágenes en formato `.ppm` (notación RGB $0-255$).
---
👥 Autores y Colaboradores
María Arias Rodríguez
Jorge Ignacio Castañeda Vallenilla
Ana Díaz Jiménez
Jaime Sánchez Sánchez
Grupo: 81 | ID de Grupo: M81-01
Universidad: Universidad Carlos III de Madrid (UC3M)
---
🏛️ Arquitectura y Diseño por Capas
El motor se estructuró mediante una arquitectura modular organizada en 8 capas de abstracción:
Matemáticas y Núcleo Aleatorio:
`Vector`: Operaciones algebraicas (suma, resta, producto escalar/vectorial, normalización).
`Mersenne Twister`: Generación de números pseudoaleatorios para simulación de Monte Carlo y Anti-aliasing.
Modelado Geométrico Fundamental:
`Rayo`: Representación paramétrica de la trayectoria de luz ($P(\lambda) = \text{Origen} + \text{Dirección} \cdot \lambda$).
`Intersección`: Cálculo y gestión de colisiones del rayo con objetos (punto de impacto, normal y distancia $t$).
Primitivas Geométricas:
`Esfera` y `Cilindro`: Implementación matemática de colisiones específicas para cada figura.
Propiedades de Materiales:
`Mate`: Superficies opacas con reflexión difusa aleatoria.
`Metal`: Superficies reflectantes tipo espejo con dispersión ajustada.
`Refractivo` (Cristal): Superficies transparentes con refracción física según la ley de Snell.
Cámara y Configuración:
`Configuración`: Resolución, calidad, semillas aleatorias y degradado de cielo.
`Geometría` y `TamañoVentana`: Posicionamiento de cámara, dirección, campo de visión y relación de aspecto.
Estructura de la Escena:
`Escena`: Colector de objetos 3D y evaluación del impacto más cercano.
`LectorArchivoEscena`: Parser de ficheros de escena en texto plano.
Motor de Renderizado:
`Ventana` y `TrazadorRayos`: Generación de múltiples rayos por píxel (Anti-aliasing), trazado de rebotados y promediado de color.
Estrategias de Almacenamiento de Memoria:
`AOS` (Array of Structures): Píxeles organizados de forma contigua en memoria `[RGB, RGB, RGB, ...]`.
`SOA` (Structure of Arrays): Datos de color separados en estructuras planas `[RRRR..., GGGG..., BBBB...]`.
---
🚀 Optimizaciones de Rendimiento
Durante el desarrollo se aplicaron técnicas avanzadas de optimización orientadas a microarquitectura:
Eliminación de punteros y fallos de caché: Se sustituyeron las referencias por punteros a objetos/materiales por estructuras contiguas reservadas en vectores plans (`struct` unificados de esferas y cilindros), reduciendo los LLC-misses (fallos de caché L3) y mejorando la localidad espacial.
Inlining y Métodos Fijos: Optimización del generador aleatorio Mersenne Twister para el cálculo frecuente de dispersión de rayos por píxel.
Análisis de Disposición de Memoria: Se comprobó que la variante AOS obtuvo una eficiencia energética un $5%-7%$ superior ($\sim 680-711 \text{ M inst/J}$) y una reducción en los tiempos de ejecución y fallos de caché frente a SOA.
---
📊 Evaluación y Métricas de Rendimiento
Evaluado en Linux mediante la herramienta `perf stat` sobre 4 escenarios de prueba de complejidad creciente:
Instrucciones por Ciclo (IPC): Promedio de $\sim 2.0-2.16$, indicando una excelente utilización del pipeline de ejecución.
Branch-misses: Tasa de fallos de predicción de salto muy reducida ($0.13% - 0.62%$).
Uso de CPU: $99% - 100%$ de aprovechamiento en renderizados complejos, demostrando que la aplicación está acotada por cálculo de CPU (CPU-bound) y no por I/O.
Caché L3 (LLC): Tasa de fallos sumamente baja ($\approx 0.09 \text{ LLC/M inst}$), confirmando que los datos de la escena caben eficientemente en las cachés del procesador.
---
🧪 Pruebas y Validación
El proyecto cuenta con una amplia suite de testing automatizada mediante scripts de Python:
Pruebas Unitarias (Google Test / C++): 92 tests que cubren el $100%$ de los componentes principales (`Vector`, `Rayo`, `Materiales`, `Escena`, `AOS`, `SOA`).
Pruebas Funcionales: Evaluación de renderizado visual en 11 casos de prueba (esferas básicas, cilindros, materiales combinados, refracciones complejas y manejo de errores de ficheros inexistentes).
---
⚙️ Compilación y Ejecución
Requisitos
Compilador C++17 o superior (`g++` o `clang++`)
Sistema operativo Linux (recomendado para la recolección de métricas con `perf`)
Herramientas sugeridas: `clang-tidy`, `cmake` / `make`
Uso del programa
```bash
# Compilar el proyecto
make

# Ejecutar el renderizador
./raytracer <fichero_configuracion.txt> <fichero_escena.txt> <fichero_salida.ppm>
```
---
📄 Licencia
Desarrollado para el entorno académico de la Universidad Carlos III de Madrid (UC3M).
Readme hecho por Gemini
