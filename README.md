# ESP32_multi-hilo
Este código se ejecuta en Arduino IDE 2, con un DOIT ESP32 DEKIT V1.
### En este repositorio se guardarán los códigos FUNCIONALES en lo que se van haciendo varias pruebas del procesamiento multi hilo del ESP32 a partir de una serie de videos de guía (videotutoriales) de Youtube.
### ref.: 
```https://www.youtube.com/watch?v=Vus7HE3wc6A&list=PL-Hb9zZP9qC48GcXj_BsDipCPAzwcps6e&index=4&ab_channel=EasyLearning```
En el video se aprecia que la cola alcanza a llenarse, sin embargo el delay que se da al encendido del led LEDse es suficiente para descongestionar la cola.
Por otro lado, el LEDre nunca llega a encenderse. Eso significa que la extracción de datos desde la cola es rápida pero no tanto como para lograr vaciar la cola.
