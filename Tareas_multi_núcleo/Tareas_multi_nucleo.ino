/*
  * Facuno N Koloswki, octubre 2023 *
  Se trata de una prueba de código donde se pretende realizar 3 tareas, a demás del loop principal,
  que se ejecutarán en simultáneo, el Kernel del ESP32 se encarga de gestionar la forma de conseguir hacer todas
  esas tareas de cada núcleo multiplexando el hilo de proceso entre ellos. En otras palabras, es un "multi-core" gestionado por el Kernel (FreeRTOS).
  En este ejemplo, el núcleo 0 estára controlando el blinkeo de los LEDs blanco y rojo minetras que el núcleo 1 gestionará el blinkeo de los LEDs rojo y azul.
  ref.: https://www.youtube.com/watch?v=J3itE-oXuco&list=PL-Hb9zZP9qC48GcXj_BsDipCPAzwcps6e&index=3&ab_channel=EasyLearning
*/
#define LEDw 23 
#define LEDr 22
#define LEDg 21
#define LEDb 19
#define LEDz 18
#define delayw 2000
#define delayr 1000
#define delayg 500
#define delayb 250
#define delayz 125
#define delayloop 500
#define memoria 2048 // bits de RAM para cada tarea
//********************************
//****Definición de las tareas****
TaskHandle_t blanco;//identificador de tarea
TaskHandle_t rojo;  //identificador de tarea
TaskHandle_t verde; //identificador de tarea
TaskHandle_t azul;  //identificador de tarea
TaskHandle_t z;     //identificador de tarea
void crearTareas(void);
void TaskW(void* pvParameters);//tarea
void TaskR(void* pvParameters);//tarea
void TaskG(void* pvParameters);//tarea
void TaskB(void* pvParameters);//tarea
void TaskZ(void* pvParameters);//tarea
//********************************
//******Definición del setup******
void setup() {
  Serial.begin(9600);
  pinMode(LEDw,OUTPUT);  //configura el pin LEDw como salida
  digitalWrite(LEDw,LOW);//inicializa el pin en LOW
  pinMode(LEDr,OUTPUT);  //configura el pin LEDr como salida
  digitalWrite(LEDw,LOW);//inicializa el pin en LOW
  pinMode(LEDg,OUTPUT);  //configura el pin LEDg como salida
  digitalWrite(LEDw,LOW);//inicializa el pin en LOW
  pinMode(LEDb,OUTPUT);  //configura el pin LEDb como salida
  digitalWrite(LEDb,LOW);//inicializa el pin en LOW
  pinMode(LEDz,OUTPUT);  //configura el pin LEDb como salida
  digitalWrite(LEDz,LOW);//inicializa el pin en LOW

  //*******************************
  //**Configuración de las tareas**
  crearTareas();        
}
//********************************
//******Definición del main*******
void loop() {
  delay(delayloop);
  Serial.println("Desde el loop");
}
//********************************
//******Definición de Tasks*******
void TaskW(void* pvParameters){

  while(1){
    Serial.println("Blanco en núcleo: "+ String(xPortGetCoreID()));//imprime en el monitor serie el ID del núcleo que realiza la tarea
    digitalWrite(LEDw,HIGH);                                       //setea el pin LEDw
    delay(delayw);                                                 //espera "delayw" segundos
    digitalWrite(LEDw,LOW);                                        //resetea el  pin LEDw
    delay(delayw);                                                 //espera "delayw" milisegundos
  }
}
void TaskR(void* pvParameters){

  while(1){
    Serial.println("Rojo en núcleo: "+ String(xPortGetCoreID()));//imprime en el monitor serie el ID del núcleo que realiza la tarea
    digitalWrite(LEDr,HIGH);                                     //setea el pin LEDr
    delay(delayr);                                               //espera "delayr" milisegundos
    digitalWrite(LEDr,LOW);                                      //resetea el pin LEDr
    delay(delayr);                                               //espera "delayr" milisegundos
  }
}
void TaskG(void* pvParameters){

  while(1){
    Serial.println("Verde en núcleo: "+ String(xPortGetCoreID()));//imprime en el monitor serie el ID del núcleo que realiza la tarea
    digitalWrite(LEDg,HIGH);                                      //setea el pin LEDg
    delay(delayg);                                                //espera "delayg" segundos
    digitalWrite(LEDg,LOW);                                       //resetea el pin LEDg
    delay(delayg);                                                //espera "delayg" milisegundos
  }
}
void TaskB(void* pvParameters){

  while(1){
    Serial.println("Azul en núcleo: "+ String(xPortGetCoreID()));//imprime en el monitor serie el ID del núcleo que realiza la tarea
    digitalWrite(LEDb,HIGH);                                     //setea el pin LEDg
    delay(delayb);                                               //espera "delayg" segundos
    digitalWrite(LEDb,LOW);                                      //resetea el pin LEDg
    delay(delayb);                                               //espera "delayg" milisegundos
  }
}

void TaskZ(void* pvParameters){

  while(1){
    Serial.println("Z en núcleo: "+ String(xPortGetCoreID()));//imprime en el monitor serie el ID del núcleo que realiza la tarea
    digitalWrite(LEDz,HIGH);                                  //setea el pin LEDz
    delay(delayz);                                            //espera "delayz" segundos
    digitalWrite(LEDz,LOW);                                   //resetea el pin LEDz
    delay(delayz);                                            //espera "delayz" segundos
  }
}

void crearTareas(void){
  // la API de TaskCrate funciona así, siempre se espera un formato identico para crear una tarea
  // sea de asignación dinámia o estática (con xTaskCreate, como en el tuto anterior, o xTaskCreatePinnedToCore, asignando o no un núcleo).
  //Asignaciones estáticas
  xTaskCreatePinnedToCore(TaskW, /*función de la tarea*/
              "ledblanco",       /*nombre de la tareas*/
              memoria,           /*cantidad de memoria para la tarea*/
              NULL,              /*parámetros de la tarea*/
              1,                 /*prioridad de la tarea*/
              &blanco,           /*identidicador de la tarea*/
              0);                /*identificador de núcleo*/

  xTaskCreatePinnedToCore(TaskR, /*función de la tarea*/
              "ledrojo",         /*nombre de la tareas*/
              memoria,           /*cantidad de memoria para la tarea*/
              NULL,              /*parámetros de la tarea*/
              1,                 /*prioridad de la tarea*/
              &rojo,             /*identidicador de la tarea*/
              0);                /*identificador de núcleo*/     

  xTaskCreatePinnedToCore(TaskG, /*función de la tarea*/
              "ledverde",        /*nombre de la tareas*/
              memoria,           /*cantidad de memoria para la tarea */
              NULL,              /*parámetros de la tarea*/
              1,                 /*prioridad de la tarea*/
              &verde,            /*identidicador de la tarea*/ 
              1);                /*identificador de núcleo*/

  xTaskCreatePinnedToCore(TaskB, /*función de la tarea*/
              "ledazul",         /*nombre de la tareas*/
              memoria,           /*cantidad de memoria para la tarea */
              NULL,              /*parámetros de la tarea*/
              1,                 /*prioridad de la tarea*/
              &azul,             /*identidicador de la tarea*/
              1);                /*identificador de núcleo*/
  
  // Asignación dinámica de núcleo
  xTaskCreatePinnedToCore(TaskZ, /*función de la tarea*/
              "ledz",            /*nombre de la tareas*/
              memoria,           /*cantidad de memoria para la tarea */
              NULL,              /*parámetros de la tarea*/
              1,                 /*prioridad de la tarea*/
              &z,                /*identidicador de la tarea*/
              tskNO_AFFINITY);   /*identificador de núcleo*/
}
