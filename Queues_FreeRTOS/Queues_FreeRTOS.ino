/*
  * Facuno N Koloswki, octubre 2023 *
  Las colas (o queues) se utilizan para el envío ordenado de datos desde una tarea hacia otra. De modo
  que una tarea A coloca hasta n datos (del mismo tipo, por ejemplo: n enteros, valor estático en tiempo de ejecución)
  a cola (queue) de la que la tarea B extrae datos para su uso. Los datos se agrupan como una pila FIFO donde la tarea B los va sacando 
  y procesando, es decir que se trata de que cada tarea opera asíncronicamente con la otra.
  Por ejemplo, tarea A hace de muestreador, tarea B hace FFT de las muestras.
  Este codigo va a usar una cola de enteros, a medida que se mande un dato a TaskB se enciende el LEDz y cada vez que
  TaskB lee ese dato enciende LEDy. Por otro lado, LEDg indica que el programa está funcionando.
*/
//********************************
//****Inclusiones****

//***Deefiniciones***
#define LEDz GPIO_NUM_23
#define LEDy GPIO_NUM_22
#define LEDg GPIO_NUM_21
#define delayz 500
#define memoria 2048 // bits de RAM para cada tarea
#define QUEUE_LENGTH 20 // tamaño de la cola


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
//******Definición del setup******
void setup() {
  Serial.begin(9600);
  pinMode(LEDz,OUTPUT);  //configura el pin LEDz como salida
  digitalWrite(LEDz,LOW);//inicializa el pin en LOW
  pinMode(LEDy,OUTPUT);  //configura el pin LEDy como salida
  digitalWrite(LEDy,LOW);//inicializa el pin en LOW
  pinMode(LEDg,OUTPUT);  //configura el pin LEDg como salida
  digitalWrite(LEDg,LOW);//inicializa el pin en LOW
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
    for(size_t i=0;i<8;i++){
      vTaskDelay(pdMS_TO_TICKS(delayz));
      gpio_set_level(LEDz,1);

      if (!xQueueSend(queue, &i, pdMS_TO_TICKS(100))) {
        Serial.println("Error al enviar dato.");
      }
      else {
        Serial.println("Se envió el valor: " + String(i) + " a queue");
      }

      vTaskDelay(pdMS_TO_TICKS(delayz));
      gpio_set_level(LEDz,0);
    }
    vTaskDelay(pdMS_TO_TICKS(delayz));//delay aconsejado para no reventar la cola
  }

}
void TaskB(void* pvParameters){
  int valor_recibido=0;
  while(1){
    if(!xQueueReceive(queue,&valor_recibido,pdMS_TO_TICKS(100))){
      Serial.println("No se recibió ningún valor.");
      }
    else{
      vTaskDelay(pdMS_TO_TICKS(delayz));
      gpio_set_level(LEDy,1);
      Serial.println("Se recibió el valor: " + String(valor_recibido) + " desde queue");
      vTaskDelay(pdMS_TO_TICKS(delayz));
      gpio_set_level(LEDy,0);
      }
      vTaskDelay(pdMS_TO_TICKS(delayz));
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
  queue = xQueueCreate(QUEUE_LENGTH, sizeof(uint32_t));//se crea una cola de QUEUE_LENGTH elementos de 
    if (queue == NULL) {
    Serial.println("Error al crear la cola. Permanecerá en bucle.");
    while(1){/***/}
    }
    else{
      Serial.println("Queue creado.");
      gpio_set_level(LEDg,1);
    }
}
