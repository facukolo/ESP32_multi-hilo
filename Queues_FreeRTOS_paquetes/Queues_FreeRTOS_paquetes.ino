/*
  * Facuno N Koloswki, octubre 2023 *
  Las colas (o queues) se utilizan para el envío ordenado de datos desde una tarea hacia otra. De modo
  que una tarea A coloca hasta n datos (del mismo tipo, por ejemplo: n enteros, valor estático en tiempo de ejecución)
  a cola (queue) de la que la tarea B extrae datos para su uso. Los datos se agrupan como una pila FIFO donde la tarea B los va sacando 
  y procesando, es decir que se trata de que cada tarea opera asíncronicamente con la otra.
  Por ejemplo, tarea A hace de muestreador, tarea B hace FFT de las muestras.
  
  Vamos a hacer un código que genere de manera aleatoria un vector de 128 muestras y ponga ese vector en cola, y los siguientes. 
  La tarea secundaria los mostrará en pantalla.
  Dejo las tareas con los LEDs sólo para adornar el trabajo: los blancos indican que hay comunicación (LEDz envió y LEDy recepción)
  entre las tareas, el verde indica que se creó el queue. El rojo LEDse indica que no se envió el paquete porque la cola está llena 
  y LEDre que no se recibió porque la cola está vacía.
  En este caso, se usará una bandera que inhibirá los "Serial.print /ln" para ahorrar tiempo en el micro
*/
//********************************
//****Inclusiones****
#include <arduinoFFT.h>
#include <stdint.h>
//***Deefiniciones***
#define LEDz GPIO_NUM_23
#define LEDy GPIO_NUM_22
#define LEDg GPIO_NUM_21
#define LEDse GPIO_NUM_19
#define LEDre GPIO_NUM_18
#define delayz 50
#define memoria 2048 // bits de RAM para cada tarea
#define QUEUE_LENGTH 20 // tamaño de la cola
#define SAMPLES 128          // Número de muestras
#define SAMPLING_FREQUENCY 10000.0 // Frecuencia de muestreo en Hz
#define FFT_N 128             // Tamaño de la FFT (debe ser igual a SAMPLES)	
bool debug=false;             //true si se quieren ver los "serial.print /ln" de las instrucciones que están condicionadas por esa bandera
//********************************
//***Declaraciones de las tarea***
TaskHandle_t a;   //identificador de tarea
TaskHandle_t b;       //identificador de tarea
QueueHandle_t queue;
void crearTareas(void);
void crearQueues(void);
void TaskA(void* pvParameters);//tarea
void TaskB(void* pvParameters);    //tares
//********************************
//**Estructura de datos para FFT**
typedef struct {
    double data[128];
} doubleArray128;
//******Definición del setup******
void setup() {
  Serial.begin(9600);
  rutina();
  //*******************************
  //**Creaciones de las tareas y queues**
  crearTareas();      
  crearQueues();
}
//********************************
//******Definición del main*******
void loop() {
}
//********************************
//******Definición de Tasks*******
void TaskA(void* pvParameters){
  while(1){
    doubleArray128 myData;              //instancia de la estructura que contiene un flotante de 128 elementos
    for(size_t j=0;j<5;j++){            //crea 8 vectores y sale del for
      int i=0;                          //declara e inicia la variable para rellenear el vector
      while(i<SAMPLES){
        myData.data[i]=random(-100,100);     //relleno del vector
        i++;
      }

      if (!xQueueSend(queue, &myData, pdMS_TO_TICKS(2*delayz))) {
          gpio_set_level(LEDse,1);//indica que la cola está llena
          vTaskDelay(pdMS_TO_TICKS(delayz));
          gpio_set_level(LEDse,0);
      }
      else {
        gpio_set_level(LEDz,1);           //enciende LEDz
        vTaskDelay(pdMS_TO_TICKS(delayz));
        gpio_set_level(LEDz,0);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(delayz));//delay aconsejado para no atascar la cola NO QUITAR
  }

}
void TaskB(void* pvParameters){
  bool mostrar=false;
  while(1){
    doubleArray128 receivedData;
    if(!xQueueReceive(queue, &receivedData, pdMS_TO_TICKS(0.1*delayz))){//tiempo de espera a la recepción menor
        gpio_set_level(LEDre,1);//indica que la cola esta vacía
        vTaskDelay(pdMS_TO_TICKS(delayz));
        gpio_set_level(LEDre,0);
      }
    else{
      vTaskDelay(pdMS_TO_TICKS(delayz));
      gpio_set_level(LEDy,1);
      if(debug){Serial.println("Vector recibido: ");}
      int i=0;
      if(debug && mostrar){
        while(i<SAMPLES){
          Serial.print(receivedData.data[i]);Serial.print(",");
          i++;
        }
        Serial.println("");
      }
      vTaskDelay(pdMS_TO_TICKS(delayz));
      gpio_set_level(LEDy,0);//nota: esto es análogo al digitalWrite, no se si es más o menos veloz
      vTaskDelay(pdMS_TO_TICKS(delayz));
    }
  }
}


//**Funciones del Setup**
void crearTareas(void){
  xTaskCreatePinnedToCore(TaskA, /*función de la tarea*/
            "a",                 /*nombre de la tareas*/
            memoria,             /*cantidad de memoria para la tarea */
            NULL,                /*parámetros de la tarea*/
            0,                   /*prioridad de la tarea*/
            &a,                  /*identidicador de la tarea*/ 
            0);                  /*identificador de núcleo*/

  xTaskCreatePinnedToCore(TaskB, /*función de la tarea*/
          "b",                   /*nombre de la tareas*/
          memoria,               /*cantidad de memoria para la tarea */
          NULL,                  /*parámetros de la tarea*/
          1,                     /*prioridad de la tarea*/
          &b,                    /*identidicador de la tarea*/ 
          1);                    /*identificador de núcleo*/
}

void crearQueues(void){
  queue = xQueueCreate(QUEUE_LENGTH, sizeof(doubleArray128));//se crea una cola de QUEUE_LENGTH elementos de 
    if (queue == NULL) {
    Serial.println("Error al crear la cola. Permanecerá en bucle.");
    while(1){/***/}
    }
    else{
      Serial.println("Queue creado.");
      gpio_set_level(LEDg,1);
    }
}
void rutina(void){
  pinMode(LEDz,OUTPUT);  //configura el pin LEDz como salida
  digitalWrite(LEDz,LOW);//inicializa el pin en LOW
  pinMode(LEDy,OUTPUT);  //configura el pin LEDy como salida
  digitalWrite(LEDy,LOW);//inicializa el pin en LOW
  pinMode(LEDg,OUTPUT);  //configura el pin LEDg como salida
  digitalWrite(LEDg,LOW);//inicializa el pin en LOW
  pinMode(LEDse,OUTPUT);  //configura el pin LEDg como salida
  digitalWrite(LEDse,LOW);//inicializa el pin en LOW
  pinMode(LEDre,OUTPUT);  //configura el pin LEDg como salida
  digitalWrite(LEDre,LOW);//inicializa el pin en LOW
  //Rutina de encendido
    gpio_set_level(LEDz,1);//led de envio a queue
    vTaskDelay(pdMS_TO_TICKS(4*delayz));
    gpio_set_level(LEDz,0);
    gpio_set_level(LEDy,1);//led de queue recibido
    vTaskDelay(pdMS_TO_TICKS(4*delayz));
    gpio_set_level(LEDy,0);
    gpio_set_level(LEDg,1);//led de queue en funcionamiento
    vTaskDelay(pdMS_TO_TICKS(4*delayz));
    gpio_set_level(LEDg,0);
    gpio_set_level(LEDse,1);//led de error en envío
    vTaskDelay(pdMS_TO_TICKS(4*delayz));
    gpio_set_level(LEDse,0);
    gpio_set_level(LEDre,1);//led de error en recepción
    vTaskDelay(pdMS_TO_TICKS(4*delayz));
    gpio_set_level(LEDre,0);
}

