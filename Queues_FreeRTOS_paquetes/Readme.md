En este programa se crea un vector de SAMPLES valores aleatorios entre +/-100 y se pasa ese vector a otra tarea. Lo novedoso respecto al primer script de Queues es que
en este se envían vectores de valores y no un valor al queue y estos se reciben como tales. Hay una bandera en el programa que permitirá ver los valores que se reciben: "mostrar".
# El objetivo de este programa es ser util para hacer FFT de muestras de algun sistema de adquisición de datos.
### las siguientes líneas serán utiles para ello:
<pre>
#include <arduinoFFT.h>
#define SAMPLING_FREQUENCY 10000.0 // Frecuencia de muestreo en Hz
#define FFT_N 128             // Tamaño de la FFT (debe ser igual a SAMPLES)
arduinoFFT FFT = arduinoFFT();
// Realiza la FFT en el vector de muestras
      FFT.Windowing(FFT_WIN_TYP_RECTANGLE,FFT_FORWARD);//ventaneo rectangular e indica que la FFT se hace del dominio temporal al frecuencial
      FFT.Compute(vectorMuestras, nullptr, FFT_N, FFT_FORWARD);
      FFT.ComplexToMagnitude(vectorMuestras,nullptr, FFT_N);
//arduinoFFT FFT = arduinoFFT();
      // Ahora se puede hacer algo con los resultados de la FFT en el vector de muestras
      // Por ejemplo, imprimir los valores de la magnitud de la FFT
      for (int i = 0; i < FFT_N / 2; i++) {
        Serial.print(vectorMuestras[i]);
      }
      Serial.println("");
</pre>
En el video se aprecia que la cola alcanza a llenarse, sin embargo el delay que se da al encendido del led LEDse es suficiente para descongestionar la cola.
Por otro lado, el LEDre nunca llega a encenderse. Eso significa que la extracción de datos desde la cola es rápida pero no tanto como para lograr vaciar la cola.
