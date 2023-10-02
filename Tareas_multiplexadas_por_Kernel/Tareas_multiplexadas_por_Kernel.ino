/*
  * Facuno N Koloswki, octubre 2023 *
  Se trata de una prueba de código donde se pretende realizar 3 tareas, a demás del loop principal,
  que se ejecutarán en simultáneo, el Kernel del ESP32 se encarga de gestionar la forma de conseguir hacer todas
  esas tareas multiplexando el hilo de proceso entre ellos. En otras palabras, es un "multi-core" gestionado por el Kernel (FreeRTOS).
  Es interesante saber que en este ejemplo el Kernel decide qué tareas se hacen en cada núcleo, así si detecta que el N0 está fulleado de tareas, asignará la nueva 
  tarea al otro núcleo. De este modo y por ejemplificar, si queremos hacer tres tareas, sería importante mandar la más pesada a un núcleo especifico y las otras dos dejar en automático.
  ref.: https://www.youtube.com/watch?v=Vus7HE3wc6A&list=PL-Hb9zZP9qC48GcXj_BsDipCPAzwcps6e&index=4&ab_channel=EasyLearning
*/
#define LEDw 23 
#define LEDr 22
#define LEDg 21
#define delayw 400
#define delayr 200
#define delayg 100
#define delayloop 500
#define memoria 2048 // bits de RAM para cada tarea
//********************************
//****Definición de las tareas****
TaskHandle_t blanco;//identificador de tarea
TaskHandle_t rojo;  //identificador de tarea
TaskHandle_t verde; //identificador de tarea
void crearTareas(void);
void TaskW(void* pvParameters);//tarea
void TaskR(void* pvParameters);//tarea
void TaskG(void* pvParameters);//tarea
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
    Serial.println("Núcleo: "+ String(xPortGetCoreID()));
    digitalWrite(LEDw,HIGH);//setea el pin LEDw
    delay(delayw);          //espera "delayw" segundos
    digitalWrite(LEDw,LOW); //resetea el  pin LEDw
    delay(delayw);          //espera "delayw" milisegundos
  }
}
void TaskR(void* pvParameters){

  while(1){
    Serial.println("Núcleo: "+ String(xPortGetCoreID()));
    digitalWrite(LEDr,HIGH);//setea el pin LEDr
    delay(delayr);          //espera "delayr" milisegundos
    digitalWrite(LEDr,LOW); //resetea el pin LEDr
    delay(delayr);          //espera "delayr" milisegundos
  }
}
void TaskG(void* pvParameters){

  while(1){
    Serial.println("Núcleo: "+ String(xPortGetCoreID()));
    digitalWrite(LEDg,HIGH);//setea el pin LEDg
    delay(delayg);          //espera "delayg" segundos
    digitalWrite(LEDg,LOW); //resetea el pin LEDg
    delay(delayg);          //espera "delayg" milisegundos
  }
}

void crearTareas(void){

  xTaskCreate(TaskW,        /*función de la tarea*/
              "ledblanco",  /*nombre de la tareas*/
              memoria,      /*cantidad de memoria para la tarea*/
              NULL,         /*parámetros de la tarea*/
              1,            /*prioridad de la tarea*/
              &blanco);     /*identidicador de la tarea*/

  xTaskCreate(TaskR,      /*función de la tarea*/
              "ledrojo",  /*nombre de la tareas*/
              memoria,    /*cantidad de memoria para la tarea*/
              NULL,       /*parámetros de la tarea*/
              1,          /*prioridad de la tarea*/
              &rojo);     /*identidicador de la tarea*/

  xTaskCreate(TaskG,      /*función de la tarea*/
              "ledverde", /*nombre de la tareas*/
              memoria,    /*cantidad de memoria para la tarea */
              NULL,       /*parámetros de la tarea*/
              1,          /*prioridad de la tarea*/
              &verde);    /*identidicador de la tarea*/ 

}
