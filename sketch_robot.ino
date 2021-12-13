
#include <Thread.h>
#include <StaticThreadController.h>
#include <ThreadController.h>

int pin_boton = 11;
int pin_motor_1 = 12;
int pin_motor_2 = 13;
int pin_led_1 = 2;
int pin_led_2 = 3;
int pin_led_3 = 4;
int pin_led_4 = 5;
int pin_led_5 = 6;
int pin_led_6 = 7;



int Inputpins[] = {pin_boton};
int Outpins[] = {pin_motor_1, pin_motor_2,pin_led_1, pin_led_2, pin_led_3, 
                 pin_led_4, pin_led_5, pin_led_6};

int state_boton = 0;
/*
  state_boton = 0 -> parar
  state_boton = 1 -> avanzar
*/
const int timeThreshold = 150;
long startTime = 0;
int value_avanzar = 255;
int value_parar = 0;

Thread button_thread;
Thread motors_thread;
Thread leds_thread;


void setup() {
  for (int index = 0; index < 1; index++)
    pinMode(Inputpins[index], INPUT); 
    
  for (int index = 0; index < 8; index++)
    pinMode(Outpins[index], OUTPUT); 
  
  // Creamos button_thread con el tiempo correspondiente
  button_thread.enabled = true;
  button_thread.setInterval(100);
  button_thread.onRun(blink_pulsador);
  
  motors_thread.enabled = true;
  motors_thread.setInterval(100);
  motors_thread.onRun(move_motors);
  
  leds_thread.enabled = true;
  leds_thread.setInterval(100);
  leds_thread.onRun(led_conection);
  
  Serial.begin(9600);
}

void blink_pulsador(){
  int buttonState = digitalRead(pin_boton);
  if (buttonState == HIGH) {
    if (millis() - startTime > timeThreshold)
  	{
  		if(state_boton == 0){
        state_boton = 1;
      }
      else {
        state_boton = 0;
      }
    }
		startTime = millis();
		Serial.println(state_boton);
	}
}

void move_motors() {

  if(state_boton == 0){
    analogWrite(pin_motor_1,value_parar);
    analogWrite(pin_motor_2,value_parar);

  }
  else {
    analogWrite(pin_motor_1,155);
    analogWrite(pin_motor_2,value_avanzar);

  }
  
  
}

void led_conection(){
  if(state_boton == 0){
    for (int index = 2; index < 8; index++)
    digitalWrite(Outpins[index], LOW); 

  }
  else {
    for (int index = 2; index < 8; index++)
    digitalWrite(Outpins[index], HIGH);

  }
}

void loop() {
  if(button_thread.shouldRun())
    button_thread.run();
  if(motors_thread.shouldRun())
    motors_thread.run();
  if(leds_thread.shouldRun())
    leds_thread.run();
}

