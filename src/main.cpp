#include <Arduino.h>
#include "FastLED.h"
#include "config.h"
#include "timeObj.h"


int TurnSignalState = LOW;
int StopSignalState = LOW;
int RunSignalState = LOW;

int brightens2[4] = {80,40,20,0};

CRGB TurnSignalStrip[TS_LED_NUMBER];
CRGB StopSignalStrip[SS_LED_NUMBER];
CRGB RunningStrip[RS_LED_NUMBER];

int delayTurnLedAnim = 15; 
bool onStartupRunning = true;
bool fStop = true;

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

timeObj STOPTimer(10.0*1000.0,false);
bool triggerOnce = true;

void timeLoop (unsigned long int startMillis, unsigned long int interval) { // delay substitute function
  while (millis() - startMillis < interval) {}
}
// FUNCTIONS
void StartUpAnimation(){
  for(int i=0;i<=SS_LED_NUMBER;i++){
    for(int y=0;y<SS_LED_NUMBER-i;y++){
      TurnSignalStrip[y] = CHSV( 150, 150, brightens2[0]);
      TurnSignalStrip[y-1] = CHSV( 0, 0, 0);

      StopSignalStrip[y] = CHSV( 150, 150, brightens2[0]);
      StopSignalStrip[y-1] = CHSV( 0, 0, 0);

      RunningStrip[y] = CHSV( 150, 150, brightens2[0]);
      RunningStrip[y-1] = CHSV( 0, 0, 0);

      FastLED[TURN_INDEX].showLeds();
      FastLED[STOP_INDEX].showLeds();
      FastLED[RUN_INDEX].showLeds();
      timeLoop(millis(),SPEED_CONTROL-40);
    }
    timeLoop(millis(),SPEED_CONTROL+20);
  }

  timeLoop(millis(),1500);
  FastLED[TURN_INDEX].showColor(CRGB::Black);
  FastLED[STOP_INDEX].showColor(CRGB::Black);
  FastLED[RUN_INDEX].showColor(CRGB::Black);
  
}
void TurnSignalAnimationON(){
  // follow animation
  for(int dot=0;dot<TS_LED_NUMBER;dot++){
      if(dot<=TS_LED_NUMBER-2){
        TurnSignalStrip[dot] = CHSV( TURN_HUE, TURN_SAT, brightens2[0]);
        TurnSignalStrip[dot+1] = CHSV( TURN_HUE, TURN_SAT, brightens2[1]);
        TurnSignalStrip[dot+2] = CHSV( TURN_HUE, TURN_SAT, brightens2[2]);
      }else{
        TurnSignalStrip[dot] = CHSV( TURN_HUE, TURN_SAT, brightens2[0]);
      }
      FastLED[TURN_INDEX].showLeds();
      currentMillis = previousMillis = millis();
      while(previousMillis + delayTurnLedAnim >= currentMillis){
        FastLED[TURN_INDEX].showLeds();
        currentMillis = millis();
      }
    }
    for(int dot=0;dot<TS_LED_NUMBER;dot++){
      if(dot<=TS_LED_NUMBER-2){
        TurnSignalStrip[dot] = CHSV( TURN_HUE, TURN_SAT, brightens2[3]);
        TurnSignalStrip[dot+1] = CHSV( TURN_HUE, TURN_SAT, brightens2[2]);
        TurnSignalStrip[dot+2] = CHSV( TURN_HUE, TURN_SAT, brightens2[1]);
      }else{
        TurnSignalStrip[dot] = CHSV( TURN_HUE, TURN_SAT, brightens2[3]);
      }
      FastLED[TURN_INDEX].showLeds();
      currentMillis = previousMillis = millis();
      while(previousMillis + delayTurnLedAnim >= currentMillis){
        FastLED[TURN_INDEX].showLeds();
        currentMillis = millis();
      }
    }
}
void TurnSignalAnimationOFF(){
  fill_solid(TurnSignalStrip,TS_LED_NUMBER,CRGB::Black);
  FastLED[TURN_INDEX].showLeds();
}
void StopSignalAnimationON(){
  if(fStop){
      triggerOnce = true;
      for(int i=0;i<15;i++){
        if(i%2 !=0){
          fill_solid(StopSignalStrip,SS_LED_NUMBER,CHSV(STOP_HUE,STOP_SAT,brightens2[0]));
          FastLED[STOP_INDEX].showLeds();
        }else{
          fill_solid(StopSignalStrip,SS_LED_NUMBER,CRGB::Black);
          FastLED[STOP_INDEX].showLeds();
        }
        if(i==14) fStop = false;
        timeLoop(millis(),45);
      }
    }else{
      fill_solid(StopSignalStrip,SS_LED_NUMBER,CHSV(STOP_HUE,STOP_SAT,brightens2[0]));
      FastLED[STOP_INDEX].showLeds();
    }
}
void StopSignalAnimationOFF(){
  fill_solid(StopSignalStrip,SS_LED_NUMBER,CRGB::Black);
    FastLED[STOP_INDEX].showLeds();
    if(triggerOnce){
      STOPTimer.start();
      triggerOnce = false;
    }
}
void RunSignalAnimationON(){
  if(onStartupRunning){
      for(int i=0;i<10;i++){
        fill_solid(RunningStrip,RS_LED_NUMBER,CHSV(RUN_HUE,RUN_SAT,i*25));
        currentMillis = previousMillis = millis();
        while(previousMillis + 100 >= currentMillis){
          FastLED[RUN_INDEX].showLeds();
          currentMillis = millis();
        }
      }
      onStartupRunning = false;
    }else{
      fill_solid(RunningStrip,RS_LED_NUMBER,CHSV(RUN_HUE,RUN_SAT,255));
      FastLED[RUN_INDEX].showLeds();
    }
}
void RunSignalAnimationOFF(){
  onStartupRunning = true;
    fill_solid(RunningStrip,RS_LED_NUMBER,CRGB::Black);
    FastLED[RUN_INDEX].showLeds(); 
}
// FUNCTIONS

void setup() {
  pinMode(TURNS_INPUT_PIN,INPUT);
  pinMode(STOPS_INPUT_PIN,INPUT);
  pinMode(RUNS_INPUT_PIN,INPUT);

  pinMode(TURNS_OUTPUT_PIN,OUTPUT);
  pinMode(STOPS_OUTPUT_PIN,OUTPUT);
  pinMode(RUNS_OUTPUT_PIN,OUTPUT);

  FastLED.addLeds<LED_TYPE, TURNS_OUTPUT_PIN, COLOR_ORDER>(TurnSignalStrip, TS_LED_NUMBER).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, STOPS_OUTPUT_PIN, COLOR_ORDER>(StopSignalStrip, SS_LED_NUMBER).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, RUNS_OUTPUT_PIN, COLOR_ORDER>(RunningStrip, RS_LED_NUMBER).setCorrection(TypicalLEDStrip);

  fill_solid(TurnSignalStrip,TS_LED_NUMBER,CRGB::Black);
  fill_solid(StopSignalStrip,SS_LED_NUMBER,CRGB::Black);
  fill_solid(RunningStrip,RS_LED_NUMBER,CRGB::Black);

  StartUpAnimation();
}

void loop() {
  TurnSignalState = digitalRead(TURNS_INPUT_PIN);
  StopSignalState = digitalRead(STOPS_INPUT_PIN);
  RunSignalState = digitalRead(RUNS_INPUT_PIN);

  if(TurnSignalState == HIGH){
    TurnSignalAnimationON();
  }
  else if(TurnSignalState == LOW){
    TurnSignalAnimationOFF();
  }
  
  if(StopSignalState == HIGH){
    StopSignalAnimationON();
    // if X time passed after pedal is DEpressed or hold - make animation active again, else static light
  }
  else if(StopSignalState == LOW){
    StopSignalAnimationOFF();
  }
  
  if(RunSignalState == HIGH){
    RunSignalAnimationON();    
  }
  else if(RunSignalState == LOW){
    RunSignalAnimationOFF();  
  }
  
  if(STOPTimer.ding()){
    fStop = true;
    triggerOnce = true;
  }
}
