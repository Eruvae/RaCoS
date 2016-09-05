/*
 * buzzer.cpp
 *
 *  Created on: 03.09.2016
 *      Author: Raptoaaah
 */
#include "buzzer.h"

HAL_GPIO buzzerIO(GPIO_033);

Buzzer buzzer;

const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

void Buzzer::init(){
	buzzerIO.init(true,1,0);
}
void Buzzer::run(){
	while(false){
		playTone(a, 500);
		playTone(a, 500);
		playTone(a, 500);
		playTone(f, 350);
		playTone(cH, 150);
		playTone(a, 500);
		playTone(f, 350);
		playTone(cH, 150);
		playTone(a, 650);

		suspendCallerUntil(500*MILLISECONDS);

		playTone(eH, 500);
		playTone(eH, 500);
		playTone(eH, 500);
		playTone(fH, 350);
		playTone(cH, 150);
		playTone(gS, 500);
		playTone(f, 350);
		playTone(cH, 150);
		playTone(a, 650);

		suspendCallerUntil(500*MILLISECONDS);

		playTone(f, 250);
		playTone(gS, 500);
		playTone(f, 350);
		playTone(a, 125);
		playTone(cH, 500);
		playTone(a, 375);
		playTone(cH, 125);
		playTone(eH, 650);

		suspendCallerUntil(500*MILLISECONDS);

		playTone(eH, 500);
		playTone(eH, 500);
		playTone(eH, 500);
		playTone(fH, 350);
		playTone(cH, 150);
		playTone(gS, 500);
		playTone(f, 350);
		playTone(cH, 150);
		playTone(a, 650);

		suspendCallerUntil(500*MILLISECONDS);

		playTone(f, 250);
		playTone(gS, 500);
		playTone(f, 375);
		playTone(cH, 125);
		playTone(a, 500);
		playTone(f, 375);
		playTone(cH, 125);
		playTone(a, 650);

		suspendCallerUntil(650*MILLISECONDS);
	}
}
void Buzzer::playTone(int tone, int duration){
	long toneTime = 2000000/tone;
	for (long i = 0; i < duration * 1000L; i += tone * 2) {
		buzzerIO.setPins(1);
		suspendCallerUntil(NOW()+toneTime*MICROSECONDS);
		buzzerIO.setPins(0);
		suspendCallerUntil(NOW()+toneTime*MICROSECONDS);
	}
}
