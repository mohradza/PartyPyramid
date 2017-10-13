// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
#define PANEL1 6

// Choose the mode pin
// Analog Pin 1
#define MODEPIN 2
int mode = 0;
int last_mode;

// Numer of LED "modes or routines
#define NMODES 6

// How many NeoPixels are attached to the Arduino?
#define NUM_LEDS 36

#define BRIGHTNESS 75

// Keep all patterns the same, use one output pin.
Adafruit_NeoPixel panel1 = Adafruit_NeoPixel(NUM_LEDS, PANEL1, NEO_GRBW + NEO_KHZ800);


int delay_val = 500; // delay for half a second
void rainbowCirclesInOut();
void startup();
//uint32_t Wheel(byte WheelPos);
uint8_t red(uint32_t c);
uint8_t green(uint32_t c);
uint8_t blue(uint32_t c);
void rainbowCirclesInOut();

byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };


// MAIN SETUP CALL
void setup() {
  Serial.begin(9600);      // open the serial port at 9600 bps:    


  panel1.setBrightness(BRIGHTNESS);
  panel1.begin();
  panel1.show(); // Initialize all pixels to 'off'


}

void pureGreen(uint32_t c, uint8_t wait){
  last_mode = mode;
  for(uint16_t i=0; i<panel1.numPixels(); i++) {
    panel1.setPixelColor(i, c);  //Green

    mode = mode_check();
    if (mode != last_mode){
      break;
    }
    delay(wait);
    last_mode = mode;
  }
  panel1.show();
  
} // end pureGreen

void hotTrackAlert(uint32_t c1, uint32_t c2, uint8_t wait){
  last_mode = mode;
  for(uint16_t i=0; i<panel1.numPixels(); i++) {
    panel1.setPixelColor(i, c1);  //Red 
  }
  panel1.show();
  delay(wait);

  for(uint16_t i=0; i<panel1.numPixels(); i++) {
    panel1.setPixelColor(i, c2);  //White
  }
    
  panel1.show();
  delay(wait);
  mode = mode_check();
  if (mode != last_mode){
    return;
  }
  last_mode = mode;
  
} // end hotTrackAlert

void nicePurp(uint32_t c, uint8_t wait){
  last_mode = mode;
  for(uint16_t i=0; i<panel1.numPixels(); i++) {
    panel1.setPixelColor(i, c);  //Green

    mode = mode_check();
    if (mode != last_mode){
      break;
    }
    last_mode = mode;
  }
  panel1.show();
  delay(wait);
  
} // end nicePurp

// DOES NOT WORK
void setPanelColor(uint8_t wait){
  last_mode = mode;
 // This should map the desired
 // number of modes to the potentiometer reading.
 float mode_scaler = 36 / 1023;
   uint32_t wheelVal;
  int redVal, greenVal, blueVal;
 
 // This will be between 0 - 256
  int mode_val = analogRead(MODEPIN);  
  int i = mode_val*mode_scaler;
  wheelVal = Wheel(((i * 256 / panel1.numPixels())) & 255);

  redVal = red(wheelVal);
  greenVal = green(wheelVal);
  blueVal = blue(wheelVal);
  
  for(uint16_t i=0; i<panel1.numPixels(); i++) {
    panel1.setPixelColor(i, panel1.Color( redVal, greenVal, blueVal )) ;

    mode = mode_check();
    Serial.print(mode);
    if (mode != 0){
      return;
    }
    last_mode = mode;
  }
  panel1.show();
      delay(wait);
  
} // end nicePurp


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  last_mode = mode;
  for(uint16_t i=0; i<panel1.numPixels(); i++) {
    panel1.setPixelColor(i, c);  
    panel1.show();

    mode = mode_check();
    Serial.print(mode);
    if (mode != last_mode){
      return;
    }
    delay(wait);
    last_mode = mode;
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return panel1.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return panel1.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return panel1.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}


uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}

// Returns the mode value (int)
// so that a reoutine can be selected.
int mode_check(){
 // This should map the desired
 // number of modes to the potentiometer reading.
 int mode_scaler = 1023 / NMODES;
 
 // This will be between 0 - 1023
 int mode_val = analogRead(MODEPIN);     

 for(int i = 0; i < NMODES; i++){
    if ((mode_val > mode_scaler*i) && (mode_val <= mode_scaler*(i+1))){
        mode = i;
        Serial.print(mode);
        // NEED TO SERIAL PRINT THIS TO VERIFY
    }
 }
 return mode;
} // End mode_check()


void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops) {
  last_mode = mode;
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;
  for(int k = 0 ; k < rainbowLoops ; k ++){
    
    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel

      for(int i=0; i< panel1.numPixels(); i++) {

        wheelVal = Wheel(((i * 256 / panel1.numPixels()) + j) & 255);

        redVal = red(wheelVal);
        greenVal = green(wheelVal);
        blueVal = blue(wheelVal);

        panel1.setPixelColor( i, panel1.Color( redVal, greenVal, blueVal ) );
      }
      
        panel1.show();
        mode = mode_check();
        if (mode != last_mode){
          break;
        }
        delay(10);
        last_mode = mode;
    }

  
  }
}

// MAIN LOOP //

void loop() {
  //    // Check mode switch / set mode
    mode = mode_check(); 
//
//    
//// WORKING SCRIPTS
if(mode == 1){
    colorWipe(panel1.Color(255, 0, 0), 100); // Red
    colorWipe(panel1.Color(0, 255, 0), 100); // Green
    colorWipe(panel1.Color(0, 0, 255), 100); // Blue
    colorWipe(panel1.Color(0, 0, 0, 255), 100); // White
} else if (mode == 2){ 
   rainbowFade2White(1,3,0);
} else if (mode == 3){
  rainbowCirclesInOut(1,3);
} else if (mode == 4){
  hotTrackAlert(panel1.Color(255,0,0), panel1.Color(0,0,0,255),500);
} else if (mode == 5){
   nicePurp(panel1.Color(160,32,250),100); // Lavendar
} else {
  // Display all green    
  pureGreen(panel1.Color(0, 255, 0), 100); // Green
}

} // Close main loop


//// Mode 0: Startup


 //Mode 1: WORKING!! 
 //Circles eminating outwards RGB
void rainbowCirclesInOut(uint8_t wait, int rainbowloops) {
  last_mode = mode;
    //int wait = int(delayValue*speed_scalar);
    int j,o,l,c;
    int redVal, greenVal, blueVal;
    uint32_t wheelVal, wheelVal1, wheelVal2, wheelVal3;
    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel
      for(int i=0; i< panel1.numPixels(); i++) {
        // Circle #1: Inner
        wheelVal = Wheel(((i * 256 / panel1.numPixels()) + j) & 255);
        
        redVal = red(wheelVal);
        greenVal = green(wheelVal);
        blueVal = blue(wheelVal);
        
        panel1.setPixelColor(7, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(32, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(16, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(26, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(14, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(31, panel1.Color(redVal, greenVal, blueVal));

 
        // Circle 2: Largest
        l = i - 2;
        if (l <= 0){
           l = 255+l;
        }

        wheelVal1 = Wheel(((l * 256 / panel1.numPixels()) + j) & 255);
        
        redVal = red(wheelVal1);
        greenVal = green(wheelVal1);
        blueVal = blue(wheelVal1);
        
        panel1.setPixelColor(5, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(6, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(34, panel1.Color(redVal, greenVal, blueVal));    
        panel1.setPixelColor(33, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(35, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(18, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(17, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(19, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(27, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(25, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(24, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(15, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(13, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(12, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(28, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(29, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(30, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(4, panel1.Color(redVal, greenVal, blueVal));

        // Circle 3: Incomplete Outer
        
        o = l - 2;
        if (0 <= 0){
            o = 255+o;
        }

        wheelVal2 = Wheel(((o * 256 / panel1.numPixels()) + j) & 255);
        
        redVal = red(wheelVal2);
        greenVal = green(wheelVal2);
        blueVal = blue(wheelVal2);
        panel1.setPixelColor(8, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(9, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(11, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(22, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(21, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(20, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(3, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(1, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(2, panel1.Color(redVal, greenVal, blueVal));

//
        // Corners
        c = o - 2;  
        if (o <= 0){
           c = 255+c;
        }

        wheelVal3 = Wheel(((c * 256 / panel1.numPixels()) + j) & 255);
        
        redVal = red(wheelVal3);
        greenVal = green(wheelVal3);
        blueVal = blue(wheelVal3);
        
        panel1.setPixelColor(0, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(10, panel1.Color(redVal, greenVal, blueVal));
        panel1.setPixelColor(23, panel1.Color(redVal, greenVal, blueVal)); 
 
      }
      mode = mode_check();
      Serial.print(mode);
      if (mode != last_mode){
        break;      
      }
      panel1.show();
      delay(10);
    }
}



//// Mode 3: 
//// Circles eminating inwards RGB
//void rainbowCirclesOutIn() {
//  while(true){
//    //int wait = int(delayValue*speed_scalar);
//    uint16_t j,o,l,c;
//   
//    c = 0;
//    while (c<256) {
//        // Corners
//        panel1.setPixelColor(0, Wheel(c) & 255);
//        panel1.setPixelColor(10, Wheel(c) & 255);
//        panel1.setPixelColor(23, Wheel(c) & 255);  
//
//        // Incomplete Outer
//        if (c == 0){
//           o = 255;
//        } else {
//            o = c - 1;
//        }
//        panel1.setPixelColor(8, Wheel(o) & 255);
//        panel1.setPixelColor(9, Wheel(o) & 255);
//        panel1.setPixelColor(11, Wheel(o) & 255);
//        panel1.setPixelColor(22, Wheel(o) & 255);
//        panel1.setPixelColor(21, Wheel(o) & 255);
//        panel1.setPixelColor(20, Wheel(o) & 255);
//        panel1.setPixelColor(31, Wheel(o) & 255);
//        panel1.setPixelColor(2, Wheel(o) & 255);
//
//        // Circle 2: Largest
//        if (o == 0){
//           l = 255;
//        } else {
//           l = o - 1;
//        }
//        panel1.setPixelColor(5, Wheel(l) & 255);
//        panel1.setPixelColor(6, Wheel(l) & 255);
//        panel1.setPixelColor(35, Wheel(l) & 255);    
//        panel1.setPixelColor(33, Wheel(l) & 255);
//        panel1.setPixelColor(34, Wheel(l) & 255);
//        panel1.setPixelColor(18, Wheel(l) & 255);
//        panel1.setPixelColor(19, Wheel(l) & 255);
//        panel1.setPixelColor(27, Wheel(l) & 255);
//        panel1.setPixelColor(25, Wheel(l) & 255);
//        panel1.setPixelColor(24, Wheel(l) & 255);
//        panel1.setPixelColor(15, Wheel(l) & 255);
//        panel1.setPixelColor(13, Wheel(l) & 255);
//        panel1.setPixelColor(12, Wheel(l) & 255);
//        panel1.setPixelColor(28, Wheel(l) & 255);
//        panel1.setPixelColor(29, Wheel(l) & 255);
//        panel1.setPixelColor(31, Wheel(l) & 255);
//        panel1.setPixelColor(4, Wheel(l) & 255);
//
//        // Circle #1: Inner
//        if (l == 0){
//           j = 255;
//        } else {
//           j = l - 1;
//        }
//        panel1.setPixelColor(7, Wheel(j) & 255);
//        panel1.setPixelColor(32, Wheel(j) & 255);
//        panel1.setPixelColor(16, Wheel(j) & 255);
//        panel1.setPixelColor(26, Wheel(j) & 255);
//        panel1.setPixelColor(14, Wheel(j) & 255);
//        panel1.setPixelColor(31, Wheel(j) & 255);
//          
//      } // End While
//      c++;
//      if (c == 256){
//        c = 0;
//      }
//      panel1.show();
//      delay(delayValue);
//  }
//}


// Mode 3: 
// Colors cascade from RGB down

//void topDownCascade() {
//    while(true);
//    i = 0;
//        while(i<256){
//
//            // Top Row
//            panel1.setPixelColor(23, Wheel(i) & 255);
//            panel2.setPixelColor(23, Wheel(i) & 255);
//            panel3.setPixelColor(23, Wheel(i) & 255);
//
//            // SECOND ROW
//            if ( i = 0){
//                j = 255;
//            } else {
//                j = i-1;
//            }
//            panel1.setPixelColor(20, Wheel(j) & 255);
//            panel1.setPixelColor(21, Wheel(j) & 255);
//            panel1.setPixelColor(22, Wheel(j) & 255);
//            
//            panel2.setPixelColor(20, Wheel(j) & 255);
//            panel2.setPixelColor(21, Wheel(j) & 255);
//            panel2.setPixelColor(22, Wheel(j) & 255);
//            
//            panel3.setPixelColor(20, Wheel(j) & 255);
//            panel3.setPixelColor(21, Wheel(j) & 255);
//            panel3.setPixelColor(22, Wheel(j) & 255);
//
//
//            // THIRD ROW
//            if (j = 0){
//                c = 255;
//            } else {
//                c = j-1;
//            }
//            panel1.setPixelColor(15, Wheel(c) & 255);
//            panel1.setPixelColor(24, Wheel(c) & 255);
//            panel1.setPixelColor(25, Wheel(c) & 255);
//            panel1.setPixelColor(27, Wheel(c) & 255);
//            panel1.setPixelColor(19, Wheel(c) & 255);
//
//            panel2.setPixelColor(15, Wheel(c) & 255);
//            panel2.setPixelColor(24, Wheel(c) & 255);
//            panel2.setPixelColor(25, Wheel(c) & 255);
//            panel2.setPixelColor(27, Wheel(c) & 255);
//            panel2.setPixelColor(19, Wheel(c) & 255);
//
//            panel3.setPixelColor(15, Wheel(c) & 255);
//            panel3.setPixelColor(24, Wheel(c) & 255);
//            panel3.setPixelColor(25, Wheel(c) & 255);
//            panel3.setPixelColor(26, Wheel(c) & 255);
//            panel3.setPixelColor(19, Wheel(c) & 255);
//
//
//            // FOURTH ROW
//            if (c = 0){
//                f = 255;
//            } else {
//                f = c-1;
//            }
//            panel1.setPixelColor(12, Wheel(f) & 255);
//            panel1.setPixelColor(13, Wheel(f) & 255);
//            panel1.setPixelColor(14, Wheel(f) & 255);
//            panel1.setPixelColor(26, Wheel(f) & 255);
//            panel1.setPixelColor(16, Wheel(f) & 255);
//            panel1.setPixelColor(17, Wheel(f) & 255);
//            panel1.setPixelColor(18, Wheel(f) & 255);
//
//            panel2.setPixelColor(12, Wheel(f) & 255);
//            panel2.setPixelColor(13, Wheel(f) & 255);
//            panel2.setPixelColor(14, Wheel(f) & 255);
//            panel2.setPixelColor(26, Wheel(f) & 255);
//            panel2.setPixelColor(16, Wheel(f) & 255);
//            panel2.setPixelColor(17, Wheel(f) & 255);
//            panel2.setPixelColor(18, Wheel(f) & 255);
//
//            panel3.setPixelColor(12, Wheel(f) & 255);
//            panel3.setPixelColor(13, Wheel(f) & 255);
//            panel3.setPixelColor(14, Wheel(f) & 255);
//            panel3.setPixelColor(26, Wheel(f) & 255);
//            panel3.setPixelColor(16, Wheel(f) & 255);
//            panel3.setPixelColor(17, Wheel(f) & 255);
//            panel3.setPixelColor(18, Wheel(f) & 255);
//
//
//
//            // FIFTH ROW
//            if (f = 0){
//                r = 255;
//            } else {
//                r = f-1;
//            }
//            panel1.setPixelColor(3, Wheel(r) & 255);
//            panel1.setPixelColor(28, Wheel(r) & 255);
//            panel1.setPixelColor(29, Wheel(r) & 255);
//            panel1.setPixelColor(31, Wheel(r) & 255);
//            panel1.setPixelColor(7, Wheel(r) & 255);
//            panel1.setPixelColor(32, Wheel(r) & 255);
//            panel1.setPixelColor(33, Wheel(r) & 255);
//            panel1.setPixelColor(35, Wheel(r) & 255);
//            panel1.setPixelColor(11, Wheel(r) & 255);
//            
//            panel2.setPixelColor(3, Wheel(r) & 255);
//            panel2.setPixelColor(28, Wheel(r) & 255);
//            panel2.setPixelColor(29, Wheel(r) & 255);
//            panel2.setPixelColor(31, Wheel(r) & 255);
//            panel2.setPixelColor(7, Wheel(r) & 255);
//            panel2.setPixelColor(32, Wheel(r) & 255);
//            panel2.setPixelColor(33, Wheel(r) & 255);
//            panel2.setPixelColor(35, Wheel(r) & 255);
//            panel2.setPixelColor(11, Wheel(r) & 255);
//
//            panel3.setPixelColor(3, Wheel(r) & 255);
//            panel3.setPixelColor(28, Wheel(r) & 255);
//            panel3.setPixelColor(29, Wheel(r) & 255);
//            panel3.setPixelColor(31, Wheel(r) & 255);
//            panel3.setPixelColor(7, Wheel(r) & 255);
//            panel3.setPixelColor(32, Wheel(r) & 255);
//            panel3.setPixelColor(33, Wheel(r) & 255);
//            panel3.setPixelColor(35, Wheel(r) & 255);
//            panel3.setPixelColor(11, Wheel(r) & 255);
//
//
//
//            // LAST FUCKING ROW
//            if (r = 0){
//                d = 255;
//            } else {
//                d = f-1;
//            }
//            panel1.setPixelColor(0, Wheel(d) & 255);
//            panel1.setPixelColor(1, Wheel(d) & 255);
//            panel1.setPixelColor(2, Wheel(d) & 255);
//            panel1.setPixelColor(30, Wheel(d) & 255);
//            panel1.setPixelColor(4, Wheel(d) & 255);
//            panel1.setPixelColor(5, Wheel(d) & 255);
//            panel1.setPixelColor(6, Wheel(d) & 255);
//            panel1.setPixelColor(34, Wheel(d) & 255);
//            panel1.setPixelColor(8, Wheel(d) & 255);
//            panel1.setPixelColor(9, Wheel(d) & 255);
//            panel1.setPixelColor(10, Wheel(d) & 255);
//
//            panel1.setPixelColor(0, Wheel(d) & 255);
//            panel1.setPixelColor(1, Wheel(d) & 255);
//            panel1.setPixelColor(2, Wheel(d) & 255);
//            panel1.setPixelColor(30, Wheel(d) & 255);
//            panel1.setPixelColor(4, Wheel(d) & 255);
//            panel1.setPixelColor(5, Wheel(d) & 255);
//            panel1.setPixelColor(6, Wheel(d) & 255);
//            panel1.setPixelColor(34, Wheel(d) & 255);
//            panel1.setPixelColor(8, Wheel(d) & 255);
//            panel1.setPixelColor(9, Wheel(d) & 255);
//            panel1.setPixelColor(10, Wheel(d) & 255);
//
//            panel1.setPixelColor(0, Wheel(d) & 255);
//            panel1.setPixelColor(1, Wheel(d) & 255);
//            panel1.setPixelColor(2, Wheel(d) & 255);
//            panel1.setPixelColor(30, Wheel(d) & 255);
//            panel1.setPixelColor(4, Wheel(d) & 255);
//            panel1.setPixelColor(5, Wheel(d) & 255);
//            panel1.setPixelColor(6, Wheel(d) & 255);
//            panel1.setPixelColor(34, Wheel(d) & 255);
//            panel1.setPixelColor(8, Wheel(d) & 255);
//            panel1.setPixelColor(9, Wheel(d) & 255);
//            panel1.setPixelColor(10, Wheel(d) & 255);
//                        
//
//            i++;
//            if (i == 256){
//                i = 0;
//            }
//        }
//}




