#include <arduino.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"

AudioGeneratorWAV *wav;
AudioFileSourcePROGMEM *file;
AudioOutputI2S *out;


// In deze file staan alle voorzieningen om het ledstripje van 6 pixels leds aan te sturen

#include <FastLED.h>
#define DATA_PIN D7 // deze pin, GPIO13 (D7) geeft de data door naar de ledstrip
#define NUM_LEDS 6 // We hebben 6 leds op onze ledstrip
CRGB leds[NUM_LEDS];

// tabel om de helderheid van de leds meer op een menselijke schaal te brengen
uint8_t kcie[] = {
    0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,
    2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,
    4,   5,   5,   5,   5,   5,   6,   6,   6,   6,   6,   7,   7,   7,   7,   8,   8,   8,   8,
    9,   9,   9,   9,   10,  10,  10,  11,  11,  11,  11,  12,  12,  12,  13,  13,  13,  14,  14,
    15,  15,  15,  16,  16,  16,  17,  17,  18,  18,  19,  19,  19,  20,  20,  21,  21,  22,  22,
    23,  23,  24,  24,  25,  25,  26,  27,  27,  28,  28,  29,  29,  30,  31,  31,  32,  33,  33,
    34,  35,  35,  36,  37,  37,  38,  39,  39,  40,  41,  42,  42,  43,  44,  45,  45,  46,  47,
    48,  49,  50,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  59,  60,  61,  62,  63,  64,
    65,  66,  67,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  80,  81,  82,  83,  84,  85,
    87,  88,  89,  90,  92,  93,  94,  96,  97,  98,  100, 101, 102, 104, 105, 106, 108, 109, 111,
    112, 114, 115, 117, 118, 120, 121, 123, 124, 126, 127, 129, 131, 132, 134, 136, 137, 139, 141,
    142, 144, 146, 147, 149, 151, 153, 155, 156, 158, 160, 162, 164, 166, 168, 169, 171, 173, 175,
    177, 179, 181, 183, 185, 187, 189, 191, 194, 196, 198, 200, 202, 204, 206, 209, 211, 213, 215,
    218, 220, 222, 224, 227, 229, 231, 234, 236, 255, 255, 255, 255};

uint8_t getPWM (uint8_t index)
{ return kcie[index];
}


void SetupLedStrip(void)
{ // koppelen van de leds aan een uitgang van de ESP8266 
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

  // alle leds op zwart of uit
  for(int i=0; i<NUM_LEDS; i++){
        leds[i].setRGB(0, 0, 0);
  }
  FastLED.show();
  FastLED.show(); // 2x want de eerste call gaat vaak mis
}  

// definitie van een functie die we verderop gaan uitschrijven
void PasKleurenAan(void);

// variabelen die we gebruiken om te bepalen of het al tijd is om wat te doen
int milliSecondes;
int tiendeSecondes;
int tiendeSecondesVorige;
int tiendeTeller = 0;
  
void UpdateLedStrip(void)
{ milliSecondes = millis();
  tiendeSecondes = milliSecondes / 10;
  
  if(tiendeSecondesVorige != tiendeSecondes)
  { tiendeSecondesVorige = tiendeSecondes;
    tiendeTeller += 1;
    PasKleurenAan();    
  }
}

// we gebruiken een variable uit het hoofd programma
extern int AnimatieFase;
extern int AnimatieTimer;
uint8_t LichtSterkte;
uint8_t LichtSterkteHuidig = 0;

int LichtSterkteGecorrigeerd;
int VorigeAnimatieFase = -1;

void PasKleurenAan(void)
{ // afhankelijk van waar we in de animatie zitten, doen we het een of het ander
  switch(AnimatieFase)
  { case 0: // kastje is in rust
//      Serial.print("AnimatieFase="); Serial.println(AnimatieFase);
      leds[0].setRGB(0, 0, 0);
      leds[1].setRGB(0, 0, 0);
      leds[2].setRGB(0, 0, 0);
      leds[3].setRGB(0, 0, 0);
      leds[4].setRGB(0, 0, 0);
      leds[5].setRGB(0, 0, 0);
      FastLED.show(); // en stuur het naar de ledstrip
      break;
    case 1: // animatie begint vanwege de schakelaar
      break;
    case 2: // deksel gaat langzaam open, hierbij loopt de animatieTimer van 0-3000 op, die gebruiken we om de lichtsterkte te laten oplopen tijdens het openen
      LichtSterkte = (255 * AnimatieTimer) / 3000;
      if(LichtSterkteHuidig !=  LichtSterkte)
      { LichtSterkteHuidig =  LichtSterkte;
//        Serial.println(LichtSterkte);
        LichtSterkte = getPWM (LichtSterkte);
        // we geven de lichtsterkte alleen aan de blauwe led (RGB, R=rood, G=groen, B-blauw) voor blauw licht
        leds[0].setRGB(0, 0, LichtSterkte);
        leds[1].setRGB(0, 0, LichtSterkte);
        leds[2].setRGB(0, 0, LichtSterkte);
        leds[3].setRGB(0, 0, LichtSterkte);
        leds[4].setRGB(0, 0, LichtSterkte);
        leds[5].setRGB(0, 0, LichtSterkte);
        FastLED.show(); // en stuur het naar de ledstrip
      }  
      break;
    case 3: // het armpje gaan in 1 seconde naar buiten, hierbij loopt de animatieTimer van 4000-5000 op, die gebruiken we om de lichtsterkte te laten oplopen tijdens het openen
      // van 3000-4000 is nog even wachten
      if(AnimatieTimer<4000)
      { if(VorigeAnimatieFase != AnimatieFase) // alleen de eerste keer honoreren
        { // alle leds op geel, dat is rood en groen gemengd
          for(int i=0; i<NUM_LEDS; i++)leds[i].setRGB(255, 255, 0);
          FastLED.show(); // en stuur het naar de ledstrip
        }  
      }
      else // armpje gaat nu bewegen
      { LichtSterkte = (255 * (AnimatieTimer-4000)) / 1000;
        LichtSterkte = getPWM (LichtSterkte);
        if(LichtSterkteHuidig !=  LichtSterkte)
        { LichtSterkteHuidig =  LichtSterkte;
          // we geven de lichtsterkte alleen aan de rode led (RGB, R=rood, G=groen, B-blauw) voor blauw licht
          for(int i=0; i<NUM_LEDS; i++)leds[i].setRGB(LichtSterkte, 0, 0);      
          FastLED.show(); // en stuur het naar de ledstrip
        }  
      }
      break;
    case 4: // armpje naar binnen is gestart
      break;
    case 5: 
     // tot AnimatieTimer 5500 is de tijd dat het armpje intrekt
      for(int i=0; i<NUM_LEDS; i++)leds[i].setRGB(0, 255, 0);      
      FastLED.show(); // en stuur het naar de ledstrip
      break;
    case 6: 
      // nu is de deksel aan het zakken, duurt van 5500 tot 7000, maar de schakelaar staat niet goed
      // gaan we vanaf rood de leds dimmen
      LichtSterkte = (255 * (7000 - AnimatieTimer)) / 1500;
      LichtSterkte = getPWM (LichtSterkte);
      for(int i=0; i<NUM_LEDS; i++)leds[i].setRGB(LichtSterkte, 0, 0);      
      FastLED.show(); // en stuur het naar de ledstrip
      break;
    case 7:
      // nu is de deskel aan het zakken, duurt van 5500 tot 7000
      // gaan we de groene leds dimmen, want de missie is geslaagd
      if(AnimatieTimer<9000)
      { LichtSterkte = (uint8_t)(255 * (7000 - AnimatieTimer)) / 1500;
        LichtSterkte = getPWM (LichtSterkte);
        for(int i=0; i<NUM_LEDS; i++)leds[i].setRGB(0, LichtSterkte, 0);      
        FastLED.show(); // en stuur het naar de ledstrip
      }  
      break;
    case 8: // de lachsalvo is gestart
      if(VorigeAnimatieFase != AnimatieFase) // alleen de eerste keer honoreren
      { // doe maar paars licht, dat piept misschien nog wel een beetje door de kier van de deksel
        for(int i=0; i<NUM_LEDS; i++)leds[i].setRGB(255, 0, 255);      
        FastLED.show(); // en stuur het naar de ledstrip
      }
      break;
    default:
      break;  
  }

  VorigeAnimatieFase = AnimatieFase;

}
