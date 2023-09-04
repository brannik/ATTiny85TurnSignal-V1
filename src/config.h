#ifndef _CONFIG_H
#define _CONFIG_H

#define TURNS_INPUT_PIN     A3
#define STOPS_INPUT_PIN     A4
#define RUNS_INPUT_PIN      A5

#define TURNS_OUTPUT_PIN     5
#define STOPS_OUTPUT_PIN     6
#define RUNS_OUTPUT_PIN      7

// led config
#define TS_LED_NUMBER   16
#define SS_LED_NUMBER   21
#define RS_LED_NUMBER   21
#define COLOR_ORDER     GRB
#define LED_TYPE        WS2812B

#define TURN_INDEX      0
#define RUN_INDEX       2
#define STOP_INDEX      1

#define SPEED_CONTROL 40 // in milliseconds

// hue/saturation setup
#define TURN_HUE    32
#define TURN_SAT    255

#define STOP_HUE    5 // to fix color
#define STOP_SAT    255 // to fix color

#define RUN_HUE     5 // to fix color
#define RUN_SAT     255 // to fix color

#endif