/*
 * buzzer.cpp
 *
 *  Created on: 03.09.2016
 *      Author: Raptoaaah
 */
#include "buzzer.h"

HAL_GPIO buzzerIO(GPIO_033);

Buzzer buzzer;
int tempo = 175;

char notes[] = "EDEDEbDCaceabegbCeEDEDEbDCaceabeCbaEDEDEbDCaceabegbCeEDEDEbDCaceabeCbabCDEgFEDfEDCeDCbeEeEDEDEDEDEDEDEbDCaceabegbCeEDEDEbDCaceabeCba";

//              E D E D E b D C a c e a b e g b C e E D|E D E b D C a c e a b e C b a E D E D E b D C|a c e a b e g b C e E D E D E b D C a c e a|b e C b a b C D E g F E D f E D C e D C|b e E e E D E D E D E D E D E D E b D C a c e a b e g b C e E D E D E b D C a c e a b e C b a
int beats[] = { 1,1,1,1,1,1,1,1,3,1,1,1,3,1,1,1,3,1,1,1,1,1,1,1,1,1,3,1,1,1,3,1,1,1,3,1,1,1,1,1,1,1,1,3,1,1,1,3,1,1,1,3,1,1,1,1,1,1,1,1,1,3,1,1,1,3,1,1,1,3,1,1,1,2,1,1,1,2,1,1,1,2,1,1,1,3,1,5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,1,1,3,1,1,1,3,1,1,1,1,1,1,1,1,1,3,1,1,1,3,1,1,1,6};

void Buzzer::init(){
	buzzerIO.init(true,1,0);
}
void Buzzer::run(){
	while(true){
		for (int i = 0; i < sizeof(notes); i++) {
		    if (notes[i] == ' ') {
		      suspendCallerUntil(NOW()+(beats[i] * tempo)*MICROSECONDS); // rest
		    } else {
		      playNote(notes[i], beats[i] * tempo);
		    }
		    // pause between notes
		    suspendCallerUntil(NOW()+(tempo / 2)*MICROSECONDS);
		    if (i == sizeof(notes)-1){
		      suspendCallerUntil(NOW()+(1000)*MICROSECONDS);
		    }
		  }
	}
}
void Buzzer::playNote(char note, int duration){
	char names[] = {'c','d','e','f','g','a','b','C', 'D', 'E', 'F', 'G', 'A', 'B', 'U'};
	  int tones[] = {3822,3424,3033,2864,2551,2272,2024, 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956};

	// play the tone corresponding to the note name
	  for (int i = 0; i < sizeof(tones); i++) {
	    if (names[i] == note) {
	      playTone(tones[i], duration);
	    }
	  }
}
void Buzzer::playTone(int tone, int duration){
	for (long i = 0; i < duration * 1000L; i += tone * 2) {
		buzzerIO.setPins(1);
		suspendCallerUntil(NOW()+tone*MICROSECONDS);
		buzzerIO.setPins(0);
		suspendCallerUntil(NOW()+tone*MICROSECONDS);
	}
}
