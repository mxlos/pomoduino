#include <Event.h>
#include <Timer.h>
#include <LED.h>
#include <FiniteStateMachine.h>
#include <Button.h>
#include "pitches.h"

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

#define MELODY_SIZE 39

int melody[] = {
  NOTE_E6, NOTE_E6, NOTE_0, NOTE_E6, NOTE_0, NOTE_C6, NOTE_E6, NOTE_0, NOTE_G6, NOTE_0,
  NOTE_0, NOTE_0, NOTE_G5, NOTE_0, NOTE_0, NOTE_0, NOTE_C6, NOTE_0, NOTE_0, NOTE_G5,
  NOTE_0, NOTE_0, NOTE_E5, NOTE_0, NOTE_0, NOTE_A5, NOTE_0, NOTE_B5, NOTE_0, NOTE_AS5,
  NOTE_A5, NOTE_0, NOTE_G5, NOTE_E6, NOTE_G6, NOTE_A6, NOTE_0, NOTE_F6, NOTE_G6
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
  7, 7, 5, 5, 5
};

  
// counter variable, hols number of button presses
byte buttonPresses = 0;            

// 25 minutos x 60 segundos = 1500
int tiempoInicial = 1; 

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
  doSound();
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

void doSound() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < MELODY_SIZE; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(4, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(4);
  }
}
