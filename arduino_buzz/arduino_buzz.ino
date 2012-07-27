#include <avr/pgmspace.h>
#include <TVout.h>
#include <fontALL.h>
#include "logo.h"

#define P1_BUTTON 2
#define P2_BUTTON 3

TVout TV;

prog_char question_0[] PROGMEM = "How much does a single DeltaHacker issue cost?"; // 6.40 - 5.50
prog_char question_1[] PROGMEM = "What is the longest subscription one can make to DeltaHacker?"; // 12 months - 24 months
prog_char question_2[] PROGMEM = "What is the nickname of DeltaHacker's chief editor?"; // SpiralEvolution - ElipticEvolution
prog_char question_3[] PROGMEM = "IDA Pro is a..."; // decompiler - compiler
prog_char question_4[] PROGMEM = "giannoug uses..."; // Windows - Mac OS X
prog_char question_5[] PROGMEM = "DeltaHacker 001, MiTM by subZraw. Facebook.com was redirected to..."; // Anchorage Alaska Travel Guide - Travel Alaska
prog_char question_6[] PROGMEM = "DDoS stands for..."; // Distributed Denial of Service - Digital Disk operating System
prog_char question_7[] PROGMEM = "Linux Torvalds made which OS?"; // Unix - Linux
prog_char question_8[] PROGMEM = "Who invented the C programming language?"; // Dennis Richie - Bill Gates
prog_char question_9[] PROGMEM = "'S' in SET stands for..."; // Social - Semantic

prog_char answer_0_a[] PROGMEM = "6.40 euros";
prog_char answer_0_b[] PROGMEM = "5.50 euros";
prog_char answer_1_a[] PROGMEM = "24 months";
prog_char answer_1_b[] PROGMEM = "12 months";
prog_char answer_2_a[] PROGMEM = "SpiralEvolution";
prog_char answer_2_b[] PROGMEM = "ElipticEvolution";
prog_char answer_3_a[] PROGMEM = "compiler";
prog_char answer_3_b[] PROGMEM = "decompiler";
prog_char answer_4_a[] PROGMEM = "Windows";
prog_char answer_4_b[] PROGMEM = "Mac OS X";
prog_char answer_5_a[] PROGMEM = "Travel Alaska";
prog_char answer_5_b[] PROGMEM = "Anchorage Alaska Travel Guide";
prog_char answer_6_a[] PROGMEM = "Digital Disk operating System";
prog_char answer_6_b[] PROGMEM = "Distributed Denial of Service";
prog_char answer_7_a[] PROGMEM = "Unix";
prog_char answer_7_b[] PROGMEM = "Linux";
prog_char answer_8_a[] PROGMEM = "Dennis Richie";
prog_char answer_8_b[] PROGMEM = "Bill Gates";
prog_char answer_9_a[] PROGMEM = "Social";
prog_char answer_9_b[] PROGMEM = "Semantic";

PROGMEM const char *questions[] = {
	question_0,
	question_1,
	question_2,
	question_3,
	question_4,
	question_5,
	question_6,
	question_7,
	question_8,
	question_9
};

PROGMEM const char *answers[][10] = {
	{answer_0_a, answer_0_b},
	{answer_1_a, answer_1_b},
	{answer_2_a, answer_2_b},
	{answer_3_a, answer_3_b},
	{answer_4_a, answer_4_b},
	{answer_5_a, answer_5_b},
	{answer_6_a, answer_6_b},
	{answer_7_a, answer_7_b},
	{answer_8_a, answer_8_b},
	{answer_9_a, answer_9_b},
};

// 0 if the correct answer is A
// 1 if it's B
boolean correct_answers[] = {
	0, 1, 0, 1, 0, 1, 1, 1, 0, 0
};


char buffer[70];

byte playerOneScore;
byte playerTwoScore;

void setup() {
	if (TV.begin(PAL)) // The default resolution is 128x96 (and we obviously use that).
		error();

	pinMode(P1_BUTTON, INPUT); // Enable internal pullups!
	pinMode(P2_BUTTON, INPUT);

	digitalWrite(P1_BUTTON, HIGH);
	digitalWrite(P2_BUTTON, HIGH);
}

void loop() {
	introScreen();
	mainScreen();
	endScreen();
}

void introScreen() {
	playerOneScore = 0;
	playerTwoScore = 0;

	TV.clear_screen();
	TV.bitmap(6, 0, logo);

	TV.select_font(font8x8);
	TV.print(8, 40, "Press any key!");

	TV.select_font(font4x6);
	TV.print(80, 90, "by giannoug");

	waitForInput();

	TV.delay(2000);
}

void mainScreen() {
	TV.clear_screen();

	TV.select_font(font8x8);
	TV.print(12, 0, "Get ready...!");

	for (int i = 5; i >= 1; i--) {
		TV.print(60, 8, i);
		TV.delay(1000);
	}

	for (int i = 0; i < 10; i++) {
		TV.clear_screen();
		TV.print(24, 40, "Question ");
		TV.print(i+1);
		TV.delay(2000);

		TV.clear_screen();

		strcpy_P(buffer, (char*)pgm_read_word(&(questions[i])));
		TV.println(0, 0, buffer);

		strcpy_P(buffer, (char*)pgm_read_word(&(answers[i][0])));
		TV.print("A: ");
		TV.println(buffer);
		strcpy_P(buffer, (char*)pgm_read_word(&(answers[i][1])));
		TV.print("B: ");
		TV.println(buffer);

		int player = waitForFirstInput();

		TV.print(0, 88, "Player ");
		TV.print(player);
		TV.println("...");

		TV.delay(1000);

		TV.print("Your answer: ");
		int choice = waitForFirstInput();

		if (choice == 1) {
			TV.print("A");
		}
		else if (choice == 2) {
			TV.print("B");
		}

		TV.delay(1000);

		TV.clear_screen();

		boolean answer = correct_answers[i];

		if (answer == --choice) {
			TV.print(32, 44, "CORRECT!");
			if (player == 1) {
				playerOneScore++;
			}
			else if (player == 2) {
				playerTwoScore++;
			}
		}
		else {
			TV.print(24, 44, "WRONG.");
		}

		TV.delay(1000);
	}
}

void endScreen() {
	TV.clear_screen();
	TV.println(24, 0, "Game over!");

	TV.println("Scores: ");
	TV.print("Player 1: ");
	TV.println(playerOneScore, DEC);
	TV.print("Player 2: ");
	TV.println(playerTwoScore, DEC);

	if (playerOneScore > playerTwoScore) {
		TV.println("Player 1 won!");
	}
	else if (playerOneScore < playerTwoScore) {
		TV.println("Player 2 won!");
	}
	else {
		TV.println("Its a tie!");
	}

	waitForInput();

	TV.delay(2000);
}

/*
 * Some helper functions.
 */

int readButton(int button) {
	return !digitalRead(button);
}

void waitForInput() {
	for (;;) {
		if (readButton(P1_BUTTON)) {
			return;
		}
	}
}

int waitForFirstInput() {
	for (;;) {
		if (readButton(P1_BUTTON)) {
			return 1;
		}
		if (readButton(P2_BUTTON)) {
			return 2;
		}
	}
}

void error() {
	for(;;) {
		digitalWrite(13, HIGH);
		delay(500);
		digitalWrite(13, LOW);
		delay(500);
	}
}