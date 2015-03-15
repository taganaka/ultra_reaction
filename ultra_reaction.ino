//
//  Ultra reaction: Who is the faster? 
//  2 players game
//  
//  Created by Francesco Laurita on 3/14/15.
//  Copyright (c) 2015 Francesco Laurita <francesco.laurita@gmail.com>. 
//  All rights reserved.
//

#include <Bounce2.h>


#define PLAYER_1_LED  2
#define PLAYER_2_LED  4
#define RED_LED       3
#define SWITCH_1_PIN  0
#define SWITCH_2_PIN  5

#define LIGHT_PROB 75
#define WINNING_AT 5

volatile unsigned long game_started_at;

// Machine-state
Bounce debouncer_1 = Bounce();
Bounce debouncer_2 = Bounce();
bool in_game = false;
int p1_score = 0;
int p2_score = 0;

void setup() {
  
  pinMode(RED_LED,    OUTPUT);
  pinMode(PLAYER_1_LED, OUTPUT);
  pinMode(PLAYER_2_LED, OUTPUT);
  
  pinMode(SWITCH_1_PIN, INPUT);
  pinMode(SWITCH_2_PIN, INPUT);
  
  debouncer_1.attach(SWITCH_1_PIN);
  debouncer_1.interval(5);
  debouncer_2.attach(SWITCH_2_PIN);
  debouncer_2.interval(5);
  
  Serial.begin(9600);
  randomSeed(analogRead(0));
  
  digitalWrite(RED_LED, HIGH);
  digitalWrite(PLAYER_1_LED, HIGH);
  digitalWrite(PLAYER_2_LED, HIGH);
 
  in_game = false;
  delay(1000);
  reset();
  delay(1000);
}

void toggle_digital(int pin) {
  digitalWrite(pin, !digitalRead(pin));
}

void blink(int pin, int t, int d){//182 - ah ah
   for (int i = 0; i < t; i++){
     toggle_digital(pin);
     delay(d);
     toggle_digital(pin);
     delay(d);
  }
}

void reset(){
  in_game = false;
  digitalWrite(RED_LED, LOW);
  digitalWrite(PLAYER_1_LED, LOW);
  digitalWrite(PLAYER_2_LED, LOW);
}

void loop() {
  unsigned long b_pressed_at;
  unsigned long reaction_time;
  int winner = -1;
 
  
  if (in_game) {
    debouncer_1.update();
    debouncer_2.update();
    
    if ( debouncer_1.fell() ) {
      Serial.println("Player 1 clicked first!");
      winner = PLAYER_1_LED;
      p1_score++;
    } else if ( debouncer_2.fell() ) {
      Serial.println("Player 2 clicked first!");
      winner = PLAYER_2_LED;
      p2_score++;
    }
    
    if (winner >= 0) {
      b_pressed_at = millis();
      reaction_time = b_pressed_at - game_started_at;
      Serial.print("Reaction time: "); Serial.print(reaction_time);
      Serial.print(" P1 Score: ");    Serial.print(p1_score);
      Serial.print(" P2 Score: ");    Serial.print(p2_score);
      Serial.println();
      digitalWrite(winner, HIGH);
      delay(2000);
      reset();
      if (p1_score == WINNING_AT || p2_score == WINNING_AT) {
        int winner_led = p1_score > p2_score ?  PLAYER_1_LED : PLAYER_2_LED;
        blink(winner_led, 10, 30);
        p1_score = 0;
        p2_score = 0;
      }
     
      delay(2000);
    }
  
  } else {
    int r = random(0, 100);
    if ( r >= LIGHT_PROB ) {
      digitalWrite(RED_LED, HIGH);
      in_game = true;
      game_started_at = millis();
    } else 
      delay(800);
  }

}
