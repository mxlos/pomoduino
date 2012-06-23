#include <Event.h>
#include <Timer.h>
#include <LED.h>
#include <FiniteStateMachine.h>
#include <Button.h>

/*
  Pomoduino tiene 2 estados:
  1. Inactivo - No se muestra nada en el display
  2. Activo - Empieza la cuenta regresiva y actualiza el display cada segundo
*/
const byte NUMBER_OF_STATES = 2;

// La clase State tiene dos prototipos
// State(void updateFunction)  y
// State(void enterFunction, void UpdateFuntion, void exitFunction)
State Off = State(pomoduinoOff);
State On = State(pomoduinoStart, pomoduinoOn, pomoduinoStop);

FSM pomoduinoStateMachine = FSM(Off);

Button button = Button(8, BUTTON_PULLUP);

LED led = LED(9);

// counter variable, hols number of button presses
byte buttonPresses = 0;            

// 25 minutos x 60 segundos = 1500
int tiempoInicial = 5; 

// El estado del timer
int segundosTranscurridos = 0;

Timer timer;

void setup() { 
  Serial.begin(9600);
  Serial.println("Inicializando Pomoduino!");
  timer.every(1000, pomoduinoUpdateDisplay);
}

//poor example, but then again; it's just an example
void loop() {
  if (button.uniquePress()){
    //increment buttonPresses and constrain it to [ 0, 1 ]
    buttonPresses = ++buttonPresses % NUMBER_OF_STATES; 

    //Serial.println(buttonPresses);

    switch (buttonPresses){
      case 0: pomoduinoStateMachine.transitionTo(On); break;
      case 1: pomoduinoStateMachine.transitionTo(Off); break;
    }
  }
  pomoduinoStateMachine.update();
  timer.update();
}

/*-------------------------------------------------------------------------------
   Status = ON
-------------------------------------------------------------------------------*/
void pomoduinoStart() {
  segundosTranscurridos = tiempoInicial;
}

// Este codigo se ejecuta cada vez que se actualiza la state machine 
// (practicamente todo el tiempo!)
void pomoduinoOn() {
  led.on();
  
 if (segundosTranscurridos == 0) {
   pomoduinoStateMachine.immediateTransitionTo(Off);
 }
}

void pomoduinoStop() {
  // no hace nada por el momento ;)
  mostrarTimerFormateado(segundosTranscurridos);
  Serial.println("Stop!");
}

/*-------------------------------------------------------------------------------
   Status = Off
-------------------------------------------------------------------------------*/
void pomoduinoOff() {
  led.off();

  segundosTranscurridos = 0;
}


/*-------------------------------------------------------------------------------
   Actualizacion del timer
-------------------------------------------------------------------------------*/
void pomoduinoUpdateDisplay() {
  if (pomoduinoStateMachine.isInState(On)) {
    mostrarTimerFormateado(segundosTranscurridos);
    segundosTranscurridos -= 1;
  }
}

void mostrarTimerFormateado(int seconds) {
  byte minutes = seconds / 60;
  byte remaining = seconds % 60;
  Serial.print(minutes);Serial.print(':'); 
  if (remaining < 10) {
    Serial.print('0');
  }
  Serial.println(remaining); 
}
