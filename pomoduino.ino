#include <LED.h>
#include <FiniteStateMachine.h>
#include <Button.h>

/*
  Pomoduino tiene 2 estados:
  1. Inactivo - No se muestra nada en el display
  2. Activo - Empieza la cuenta regresiva y actualiza el display cada segundo
*/
const byte NUMBER_OF_STATES = 2;

//initialize states
State Off = State(pomoduinoOff);
State On = State(pomoduinoOn);


FSM pomoduinoStateMachine = FSM(Off);     //initialize state machine, start in state: On
Button button = Button(8, BUTTON_PULLUP); //initialize the button (wire between pin 12 and ground)
LED led = LED(9);                 //initialize the LED
byte buttonPresses = 0;            //counter variable, hols number of button presses

void setup() { 
  pomoduinoStartDisplay();
}

//poor example, but then again; it's just an example
void loop() {
  if (button.uniquePress()){
    //increment buttonPresses and constrain it to [ 0, 1 ]
    buttonPresses = ++buttonPresses % NUMBER_OF_STATES; 

    Serial.println(buttonPresses);
    pomoduinoUpdateDisplay();
    
    switch (buttonPresses){
      case 0: pomoduinoStateMachine.transitionTo(On); break;
      case 1: pomoduinoStateMachine.transitionTo(Off); break;
    }
  }
  pomoduinoStateMachine.update();
}

// Aqui estara lo chilo :p
void pomoduinoOn() {
  led.on();


}

void pomoduinoOff() {
  led.off(); 


}

void pomoduinoStartDisplay() {
  Serial.begin(9600);
  Serial.println("Hello World!");
}

void pomoduinoUpdateDisplay() {
  // No hace nada por el momento
}
//end utility functions
