#include <Display.h>

Display screen = Display(11,10,9,6,A1,3,4,5,A0,7,8);

/* Reloj */
#define maxMins 25     // Max de minutos
int pinBoton=2;        // Pin del boton (Aun no funciona)
int mins = maxMins;    
int segs = 0;
int milis = -59;       // Auxiliar. Estabiliza el calculo de los segundos.
                       // Lo iniciamos en -59 para que el contador segs inicie en 0

boolean estado=true;

void setup()
{
  
}

void loop()
{
  reloj();
}

void reloj()
{
    
  if(segs==0 && mins==0 || estado==false)
  {
    screen.displayNumber(0);
    return;
  }
  
  //     59 + Auxiliar - tiempo actual sistema
  segs = 59 + milis    - millis()/1000 ;
  if(segs < 0)
  {
    milis = millis()/1000;
    segs = 59;
    mins--;
  }
  
  screen.displayNumber(100*mins + segs);
}
