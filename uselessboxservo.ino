#include "uselessled.h"

// we gebruiken twee servo motortjes, een voor de deksel en een voor het armpje
// deze zijn aangesloten op massa (G) en 5 volt (VIN) voor de energie
// de stuurdraadjes van de servo's zijn aangesloten op D3 (deksel) en D1 (armpje) van de printplaat
Servo ServoDeksel, ServoArmpje; 
#define DEKSEL_DICHT 75  // Deksel dicht is 0 graden van de servo
#define DEKSEL_OPEN  5 // Deksel open is 180 graden van de servo
#define DEKSEL_KIER  40 // Deksel open is 180 graden van de servo

#define ARMPJE_IN    180  // Armpje naar binnen is 0 graden van de servo
#define ARMPJE_UIT   5 // Armpje naar buiten is 90 graden van de servo
#define ARMPJE_BIJNA 30 // Armpje niet helemaal buiten is 60 graden van de servo



// de versterker word aangestuurd met 3 signalen en 5V voeding (+5V en GND)
// van links naar rechts, LRC, BCLK, DIN, GAIN, SD, GND en VIN
// LCR gaat naar D4 (grijs)
// BCLK gaat naar D8 (wit)
// DIN gaat naar RX (zwart)

// de muziek en geluidseffecten die we kunnen gebruiken
#include "music.h"
#include "jingle2.h"
#include "drumroll.h"
#include "piepdeur.h"
#include "lach.h"
#include "lach2.h"
#include "blijferaf.h"
#include "poing4x.h"


void setup() {
  // Alle code in dit setup deel wordt alleen bij opstarten 1x uitgevoerd
  WiFi.mode(WIFI_OFF); // geen wifi functies, spaart stroom ook
  
  // hier de servo's aansluitingen opgeven en ze in de rust stand zetten, armpje in en deksel dicht
  ServoDeksel.attach(0, 500, 2500); // aansluiting GPIO0 of ook wel D3 genoemd
  ServoDeksel.write(DEKSEL_DICHT);  
  ServoArmpje.attach(5, 500, 2600); // aansluiting GPIO5 of ook wel D1 genoemd
  ServoArmpje.write(ARMPJE_IN);  

  // de schakelaar is aangesloten op massa (G) en ingang D2 (GPIO4)
  // de ingang word ook voorzien van een positieve spanning om de stand van de schakelaar te kunnen detecteren
  pinMode(4, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println("We zijn begonnen!");

  // audio benodigdheden voor de geluidjes
  audioLogger = &Serial;
  out = new AudioOutputI2S();
  wav = new AudioGeneratorWAV();

  // dit is nodig om het stripje met de 6 leds te laten werken
  SetupLedStrip();
   
}


int AnimatieFase = 0;
int AnimatieStart = 0;
int AnimatieTimer = 0;
int Angle;
int ArmSlag;
int LoterijGetal;
int stuiter = 0;

void loop() {
  // dit is het hoofdprogramma, hier gebeurt het allemaal
  // het is een lus die continue en razendsnel doorlopen wordt
  
  UpdateAnimatieTimer();
  
 
  // afhankelijk van de AnimatieFase waar we in verkeren, schakelen we naar de bijbehorende 'case'.
  switch(AnimatieFase)
  { case 0:
      if(!wav->isRunning()) // er speelt geen muziekje
      { if(digitalRead(4) == LOW)
        { // de schakelaar is gesloten 
          AnimatieFase = 1;
        }
      }  
      break;

    case 1: // eerste stap na sluiten schakelaar, start de muziek 
      ResetAnimatieTimer(); // tijdstip van start moment
      // willekeurig getal van 0 t/m 99
      LoterijGetal = random(100);
      if (LoterijGetal > 75) file = new AudioFileSourcePROGMEM( jingle, sizeof(jingle) );
      else if (LoterijGetal > 50) file = new AudioFileSourcePROGMEM( jingle2, sizeof(jingle2) );
      else if (LoterijGetal > 25) file = new AudioFileSourcePROGMEM( drumroll, sizeof(drumroll) );
      else file = new AudioFileSourcePROGMEM( piepdeur, sizeof(piepdeur) );
      
      wav->begin(file, out);
      AnimatieFase = 2;
      break;

    case 2: // nu de deksel openen
      if(AnimatieTimer < 3000)
      { Angle = map(AnimatieTimer, 0, 3000, DEKSEL_DICHT, DEKSEL_OPEN);
        Serial.printf("Deksel Angle = %d\n", Angle);
        ServoDeksel.write(Angle);
      }
      else // tijd van openen is verstreken, straks verder met het armpje
      { // willekeurig getal van 0 t/m 99
        LoterijGetal = random(100);
        if (LoterijGetal < 70) // in 70% het geval
        { ArmSlag = ARMPJE_UIT; // volle slag
        }
        else 
        { ArmSlag = ARMPJE_BIJNA; // soms dus een korte slag die het niet haalt
        }
        AnimatieFase = 3; 
      }
      break;

    case 3: // nu het armpje naar buiten
      if(AnimatieTimer > 4000 && AnimatieTimer < 5000 )
      { Angle = map(AnimatieTimer, 4000, 5000, ARMPJE_IN, ArmSlag);
        Serial.printf("Arm Angle = %d\n", Angle);
        ServoArmpje.write(Angle);
      }
      if(AnimatieTimer>5000)
      { AnimatieFase = 4;
      }
      break;

    case 4:
      // armpje in
      ServoArmpje.write(ARMPJE_IN);
      AnimatieFase = 5;
      break;  

    case 5:
      // deksel dicht als muziek klaar is
      if(!wav->isRunning()) // er speelt geen muziekje meer
      { if(AnimatieTimer>5500)
        { //ServoDeksel.write(DEKSEL_DICHT);
          LoterijGetal = random(100);
          if(LoterijGetal<50)
          { file = new AudioFileSourcePROGMEM( piepdeur, sizeof(piepdeur) );
            stuiter = 0;
          }
          else 
          { file = new AudioFileSourcePROGMEM( poing4x, sizeof(poing4x) ); 
            stuiter = 10;
          }
          wav->begin(file, out);
          AnimatieFase = 6;
        }  
      }
      break;  

    case 6:
      // deksel langzaam dicht of stuiterend dicht
      if(AnimatieTimer < 8500)
      { if(stuiter==0)
        { Angle = map(AnimatieTimer, 5500, 8500, DEKSEL_OPEN, DEKSEL_DICHT);
          Serial.printf("Deksel Angle = %d\n", Angle);
          ServoDeksel.write(Angle);
        }
        else
        { if(stuiter==10)
          { ServoDeksel.write(DEKSEL_DICHT);
            stuiter--;
          }
          else if(stuiter==9) // eerste keer kier
          { if(AnimatieTimer > 5650)
            { ServoDeksel.write(DEKSEL_KIER);
              stuiter--;
            }
          }
          else if(stuiter==8) // naar beneden
          { if(AnimatieTimer > 5800)
            { ServoDeksel.write(DEKSEL_DICHT);
              stuiter--;
            }
          }
          else if(stuiter==7) // tweede keer kier
          { if(AnimatieTimer > 5950)
            { ServoDeksel.write(DEKSEL_KIER);
              stuiter--;
            }
          }
          else if(stuiter==6) // naar beneden
          { if(AnimatieTimer > 6100)
            { ServoDeksel.write(DEKSEL_DICHT);
              stuiter--;
            }
          }
          else if(stuiter==5) // tweede keer kier
          { if(AnimatieTimer > 6250)
            { ServoDeksel.write(DEKSEL_KIER);
              stuiter--;
            }
          }
          else if(stuiter==4) // naar beneden
          { if(AnimatieTimer > 6500)
            { ServoDeksel.write(DEKSEL_DICHT);
              stuiter--;
            }
          }


          
        }    
      }
      else // tijd van sluiten is verstreken
      { AnimatieFase = 7;
      }
      break;  


    case 7: 
      if(digitalRead(4) == LOW) // als de schakelaar niet terug is gezet, even wachten en dan opnieuw proberen  
      { if(AnimatieTimer > 10000) 
        { ResetAnimatieTimer(); // tijdstip van start moment resetten
          file = new AudioFileSourcePROGMEM( blijferaf, sizeof(blijferaf) );
          wav->begin(file, out);
          AnimatieFase = 2;
        }  
      }
      else AnimatieFase = 8;
      break;

    case 8: // kist is dicht, schakelaar is in rust, nu even wachten en dan de lach 
      if(AnimatieTimer > 12000)
      { // willekeurig getal van 0 t/m 99
         LoterijGetal = random(100);
        if (LoterijGetal > 50) file = new AudioFileSourcePROGMEM( lach, sizeof(lach) );
        else file = new AudioFileSourcePROGMEM( lach2, sizeof(lach2) );
        wav->begin(file, out);
        AnimatieFase = 9;       
      }
      else
      { // of je zet de schakelaar meteen weer aan, dan geen lach, maar opnieuw starten
        if(digitalRead(4) == LOW)
        { AnimatieFase = 1;
        }
      }
      break;

    case 9:
      if(!wav->isRunning()) // uitgelachen
      { AnimatieFase = 0;
      }
      else
      { if(digitalRead(4) == LOW) // tijdens het lachen de schakelaar al omzetten ?
        { wav->stop();
        }
      }
      break;
            
    default:
      AnimatieFase = 0;
      break;    
  }
  
  // steeds of er muziek of geluidseffect spelen, en dit een zetje geven tot het volledig afgespeeld is
  if (wav->isRunning()) 
  { if (!wav->loop()) 
    { Serial.printf("WAV stop\n");
      wav->stop();
    }
  } 

  UpdateLedStrip(); // dit regelt het omschakelen van de led kleuren 


  // dit is het einde van de lus en springen we vanzelf weer naar boven, naar het begin van loop()
}
