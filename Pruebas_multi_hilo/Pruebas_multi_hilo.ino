#define N 128
TaskHandle_t prim_task;
TaskHandle_t sec_task;

bool b = false;
int stack[N];

void setup() {
   Serial.begin(115200); 
  xTaskCreatePinnedToCore(writeStack, // Function to call
                          "prim_task", // Name for this task, mainly for debug
                          100000, // Stack size
                          NULL, // pvParameters to pass to the function
                          2, // Priority 
                          &prim_task, // Task handler to use
                          1 //nucleo en que se ejecuta
                          );
  xTaskCreatePinnedToCore(
                          Read, // Function to call
                          "sec_task", // Name for this task, mainly for debug
                          10000, // Stack size
                          NULL, // pvParameters to pass to the function
                          1, // Priority 
                          &sec_task, // Task handler to use
                          0 //nucleo en que se ejecuta
                          );
}
void writeStack(void* pvParameters){
  while(true){
    if(b==false){
      delay(2000);
      Serial.println("**********************************");
      Serial.println("Prueba de procesamiento multi-hilo en la carga");
      Serial.println("Núcleo: "+ String(xPortGetCoreID()));
      int i=0;
      while(i<N){
        stack[i]=random(-100,100);
        i++;
      }
      Serial.println("Pila llena");b=true;
      Serial.println("**********************************");
    }
    vTaskDelay(10);
  }
}
void Read(void* pvParameters){
  while(true){
    if(b==true){

      Serial.println("Prueba de procesamiento multi-hilo en la descarga");
      Serial.println("Núcleo: "+ String(xPortGetCoreID()));
      int i=0;
      for(i=0;i<N;i++){
        Serial.print(stack[i]);
        Serial.print(" ");
      }
      Serial.println(" ");
      b=false;
    }
    vTaskDelay(10);
  }
}

void loop() { }



