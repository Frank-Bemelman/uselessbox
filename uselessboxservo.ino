
#include <arduino.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#include "uselessled.h"

// we gebruiken twee servo motortjes, een voor de deksel en een voor het armpje
// deze zijn aangesloten op massa (G) en 5 volt (VIN) voor de energie
// de stuurdraadjes van de servo's zijn aangesloten op D3 (deksel) en D1 (armpje) van de printplaat
Servo ServoDeksel, ServoArmpje; 



// de versterker word aangestuurd met 3 signalen en 5V voeding (+5V en GND)
// van links naar rechts, LRC, BCLK, DIN, GAIN, SD, GND en VIN
// LCR gaat naar D4
// BCLK gaat naar D8
// DIN gaat naar RX

#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"

// de muziek en geluidseffecten die we gebruiken
#include "music.h"
#include "lach.h"
#include "blijferaf.h"

AudioGeneratorWAV *wav;
AudioFileSourcePROGMEM *file;
AudioOutputI2S *out;



void setup() {
  // Alle code in dit deel wordt alleen bij opstarten 1x uitgevoerd
  WiFi.mode(WIFI_OFF); // geen wifi functies, spaart stroom ook
  
  // hier de servo's aansluitingen opgeven en ze in de nul stand zetten
  ServoDeksel.attach(0, 544, 2500); // GPIO0 of ook wel D3 genoemd
  ServoDeksel.write(0);  
  ServoArmpje.attach(5, 544, 2500); // GPIO5 of ook wel D1 genoemd
  ServoArmpje.write(0);  

  // de schakelaar is aangesloten op massa (G) en ingang D2 (GPIO4)
  // de ingang word ook voorzien van een positieve spanning om de stand van de schakelaar te kunnen detecteren
  pinMode(4, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println("We zijn begonnen!");

  // audio benodigdheden
  audioLogger = &Serial;
  file = new AudioFileSourcePROGMEM( jingle, sizeof(jingle) );
  out = new AudioOutputI2S();
  wav = new AudioGeneratorWAV();

  #ifdef LEDS
  ledSetup();
  #endif
   
}


int animatieFase = 0;
int animatieStart = 0;
int animatieTimer = 0;
int angle;
int armSlag;

void loop() {
  // dit is het hoofdprogramma, hier gebeurt het allemaal
  // het is een lus die continue en razendsnel doorlopen wordt
  
  animatieTimer = millis()-animatieStart;
  #ifdef LEDS
  updateLeds();
  #endif

  switch(animatieFase)
  { case 0:
      if(!wav->isRunning()) // er speelt geen muziekje
      { if(digitalRead(4) == LOW)
        { // de schakelaar is gesloten 
          animatieFase = 1;
        }
      }  
      break;

    case 1: // eerste stap na sluiten schakelaar, start de muziek 
      animatieStart = millis(); // tijdstip van start moment
      file = new AudioFileSourcePROGMEM( jingle, sizeof(jingle) );
      wav->begin(file, out);
      animatieFase = 2;
      break;

    case 2: // nu de deksel openen
      Serial.printf("animatieTimer = %d\n", animatieTimer);
      if(animatieTimer < 3000)
      { angle = (animatieTimer * 180)/ 3000;
        Serial.printf("angle = %d\n", angle);
        ServoDeksel.write(angle);
      }
      else // tijd van openen is verstreken, straks verder met het armpje
      { animatieFase = 3;
        // willekeurig getal van 0 t/m 99
        if(random(100)<70) // in 70% het geval
        { armSlag = 90; // volle slag
        }
        else 
        { armSlag = 60; // soms dus een korte slag die het niet haalt
        }
      }
      break;

    case 3: // nu het armpje naar buiten
      Serial.printf("animatieTimer = %d\n", animatieTimer);
      if(animatieTimer > 4000 && animatieTimer < 5000 )
      { angle = ((animatieTimer-4000) * armSlag)/ 1000;
        Serial.printf("angle = %d\n", angle);
        ServoArmpje.write(angle);
      }
      if(animatieTimer>5000)
      { animatieFase = 4;
      }
      break;

    case 4:
      // armpje in
      ServoArmpje.write(0);
      animatieFase = 5;
      break;  

    case 5:
      // deksel dicht als muziek klaar is
      if(!wav->isRunning()) // er speelt geen muziekje meer
      { if(animatieTimer>5500)
        { ServoDeksel.write(0);
          animatieFase = 6;
        }  
      }
      break;  

    case 6: 
      if(digitalRead(4) == LOW) // als de schakelaar niet is omgezet, even wachten en dan opnieuw proberen  
      { if(animatieTimer > 7000) 
        { animatieStart = millis(); // tijdstip van start moment resetten
          file = new AudioFileSourcePROGMEM( blijferaf, sizeof(blijferaf) );
          wav->begin(file, out);
          animatieFase = 2;
        }  
      }
      else animatieFase = 7;
      break;

    case 7: // kist is dicht, schakelaar is in rust, nu even wachten en dan de lach 
      if(animatieTimer > 9000)
      { file = new AudioFileSourcePROGMEM( lach, sizeof(lach) );
        wav->begin(file, out);
        animatieFase = 8;       
      }
      else
      { // of je zet de schakelaar meteen weer aan, dan geen lach, maar opnieuw starten
        if(digitalRead(4) == LOW)
        { animatieFase = 1;
        }
      }
      break;

    case 8:
      if(!wav->isRunning()) // uitgelachen
      { animatieFase = 0;
      }
      else
      { if(digitalRead(4) == LOW) // tijdens het lachen de schakelaar al omzetten ?
        { wav->stop();
        }
      }
      break;
            
    default:
      animatieFase = 0;
      break;    
  }
  
  // steeds of er muziek of geluidseffect spelen, en dit een zetje geven tot het volledig afgespeeld is
  if (wav->isRunning()) 
  { if (!wav->loop()) 
    { Serial.printf("WAV stop\n");
      wav->stop();
    }
  } 

  // dit is het einde van de lus en springen we vanzelf weer naar boven, naar het begin van loop()
}
