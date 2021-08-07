#define IN_DHO    PB3
#define IN_FLASH  PB4
#define OUT_DHO   PB2
#define OUT_FLASH PB0
#define PWM_OUT   PB1

byte PWM_DHO   = 17;     //17       // чем больше значение тем меньше напряжение на выходе
byte PWM_FLASH = 5;       //5

uint32_t timer = 0;              // таймер отслеживания отключения поворота

bool on_flag = false;            // флаг работы поворотника
bool DHO_ON = false;
bool FLASH_ON = false;
uint16_t delay_on_DHO = 1500;    // время задержки включения ДХО после поворота



void setup() {
  
  pinMode(IN_DHO, INPUT_PULLUP);
  digitalWrite(IN_DHO, HIGH);

  pinMode(IN_FLASH, INPUT_PULLUP);
  digitalWrite(IN_FLASH, HIGH);

  pinMode(OUT_DHO, OUTPUT);
  digitalWrite(OUT_DHO, LOW);
  
  pinMode(OUT_FLASH, OUTPUT);
  digitalWrite(OUT_DHO, LOW);

  pinMode(PWM_OUT, OUTPUT);
    
}

void loop() {

  if(digitalRead(IN_DHO) == LOW && digitalRead(IN_FLASH) == HIGH && DHO_ON == false){      // ВКЛ только ДХО
      DHO_ON = true;      
      FLASH_ON = false; 

      digitalWrite(OUT_DHO, HIGH);                                      // включения моста
      digitalWrite(OUT_FLASH, LOW);    
      analogWrite(PWM_OUT, PWM_DHO);                                    // напряжение для ДХО
  }

  if(digitalRead(IN_DHO) == LOW && digitalRead(IN_FLASH) == LOW && DHO_ON == true && FLASH_ON == false){       // ВКЛ  ДХО и включаем ПОВОРОТНИК или АВАРИЙКУ
      timer = 0;
      DHO_ON = false;
      FLASH_ON = true;
          
      digitalWrite(OUT_DHO, LOW);                                       // включения моста
      digitalWrite(OUT_FLASH, HIGH);  
      analogWrite(PWM_OUT, PWM_FLASH);                                  // напряжение для ПОВОРОТА

      for(;;){                                                          // вход в бесконечный цикл отслеживания работы поворота при работе ДХО
       
          if (digitalRead(IN_FLASH) == LOW && on_flag == false)  {
              digitalWrite(OUT_FLASH, HIGH);                            //вкл поворота
              on_flag = true;
          }
          if (digitalRead(IN_FLASH) == HIGH && on_flag == true) {  
              digitalWrite(OUT_FLASH, LOW);                             //выкл поворота
              on_flag = false;
              timer = millis();
          }

          if (millis()-timer >= delay_on_DHO && digitalRead(IN_FLASH) == HIGH && on_flag == false ){
          FLASH_ON = false;
            break;
          }
       }

       
  }

  if(digitalRead(IN_DHO) == HIGH && digitalRead(IN_FLASH) == LOW && DHO_ON == false && FLASH_ON == false){    // ВКЛ только ПОВОРОТ или АВАРИЙКА
            
      DHO_ON = false;
      FLASH_ON = true;

      digitalWrite(OUT_DHO, LOW);         // включения моста
      digitalWrite(OUT_FLASH, HIGH);     // выключение произойдет отключением питания схемы
      analogWrite(PWM_OUT, PWM_FLASH);    // напряжение для ПОВОРОТА
  }

}
