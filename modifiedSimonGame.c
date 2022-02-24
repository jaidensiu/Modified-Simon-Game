/*
 * Author:           Jaiden Siu
 * Student Number:   71961403
 * Lab Section:      L2D
 * Date:             4/13/2021 7:01:55 PM       
 * Purpose:          To implement a modified version of the Simon game in C using the DAQ simulator.
 */

#define _CRT_SECURE_NO_WARNINGS

/* Header Files */
#include <stdio.h>
#include <stdlib.h>
#include <DAQlib.h>
#include <time.h>
#include <Windows.h>

/* I/O Channels */
#define BUTTON0 0
#define BUTTON1 1
#define BUTTON2 2
#define BUTTON3 3
#define NUM_BUTTON 4

/* Symbolic Constants */
#define ON 1
#define OFF 0
#define GREEN 0
#define RED 1
#define TRUE 1
#define FALSE 0
#define LENGTH 5

/* Time Constants */
#define ONE_SECOND 1000
#define TWO_SECONDS 2000

/* Simulator Configuration */
#define setupNum 6

/* Function Prototypes */
void runSimon(void);
void generateSequence(int length, int data[]);
int randInt(int lower, int upper);
void guess(int guess[NUM_BUTTON], int currentState);
int readButton(void);
int check(int guess[NUM_BUTTON], int answer[LENGTH], int currentState);
void display(int answer[LENGTH], int currentState);
void flashColor(int color);

int main(void)
{
	if (setupDAQ(setupNum) == TRUE)
	{
		printf("Welcome to the Simon Game!\n");
		Sleep(TWO_SECONDS);
		printf("Green flashes indicates you guessed correctly, red flashes indicates you guessed incorrectly.\n");
		Sleep(TWO_SECONDS);
		printf("Good luck and have fun!\n");
		Sleep(TWO_SECONDS);
		printf("New game starts in\n");
		printf("3\n");
		Sleep(ONE_SECOND);
		printf("2\n");
		Sleep(ONE_SECOND);
		printf("1\n");
		Sleep(ONE_SECOND);

		runSimon();
	}
	else
	{
		printf("ERROR: Cannot initialize system\n");
	}

	system("PAUSE");
	return 0;
}

/* 
 * Name: runSimon
 * Purpose: Run Simon Game
 * Parameter(s): void
 * Return Value: void 
 */
void runSimon(void)
{
	int guessNum[] = { 0, 0, 0, 0, 0 };
	int answer[LENGTH];
	int currentState = 1;

	generateSequence(LENGTH, answer);

	while (continueSuperLoop())
	{
		do
		{
			Sleep(TWO_SECONDS);
			display(answer, currentState);
			guess(guessNum, currentState);

			if (check(guessNum, answer, currentState) == FALSE)
			{
				flashColor(RED);
				printf("You lose!\n");
				printf("New game starts in\n");
				printf("3\n");
				Sleep(ONE_SECOND);
				printf("2\n");
				Sleep(ONE_SECOND);
				printf("1\n");
				Sleep(ONE_SECOND);
				runSimon();
			}
			else
			{
				flashColor(GREEN);
			}

			currentState++;
		} while (check(guessNum, answer, currentState) && currentState <= LENGTH);

		if (currentState == 6)
		{
			flashColor(GREEN);
			printf("You win!\n");
			printf("New game starts in\n");
			printf("3\n");
			Sleep(ONE_SECOND);
			printf("2\n");
			Sleep(ONE_SECOND);
			printf("1\n");
			Sleep(ONE_SECOND);
			runSimon();
		}
	}
}

/* 
 * Name: generateSequence
 * Purpose: Generate the random number sequence using the randInt function and store it into an array
 * Parameter(s): The length of the sequence and the array that the sequence is being stored into
 * Return Value: void 
 */
void generateSequence(int length, int data[])
{
	int index;

	srand((unsigned)time(NULL));

	for (index = 0; index < length; index++)
	{
		data[index] = randInt(0, 3);
	}
}

/* 
 * Name: randInt
 * Purpose: Generate a random number within a range
 * Parameter(s): The range of random numbers
 * Return Value: The random number 
 */
int randInt(int lower, int upper)
{
	int divider = upper - lower + 1;

	return rand() % divider;
}

/* 
 * Name: guess
 * Purpose: Store the player's guess values in an array
 * Parameter(s): The array in which the guess values are being stored and the current state of the game
 * Return Value: void 
 */
void guess(int guessNum[NUM_BUTTON], int currentState)
{
	int index;

	for (index = 0; index < currentState; index++)
	{
		guessNum[index] = readButton();
	}
}

/* 
 * Name: readButton
 * Purpose: Record and output the player's pressed button. Wait until the player has pressed and released the button, then output the value
 * Parameter(s): void
 * Return Value: The pressed button value 
 */
int readButton(void)
{
	int buttonState0 = OFF;
	int buttonState1 = OFF;
	int buttonState2 = OFF;
	int buttonState3 = OFF;

	do
	{
		buttonState0 = digitalRead(0);
		buttonState1 = digitalRead(1);
		buttonState2 = digitalRead(2);
		buttonState3 = digitalRead(3);

		while (buttonState0 == ON && buttonState1 == OFF && buttonState2 == OFF && buttonState3 == OFF)
		{
			while (digitalRead(0) == OFF)
			{
				return 0;
			}
		}

		while (buttonState0 == OFF && buttonState1 == ON && buttonState2 == OFF && buttonState3 == OFF)
		{
			while (digitalRead(1) == OFF)
			{
				return 1;
			}
		}

		while (buttonState0 == OFF && buttonState1 == OFF && buttonState2 == ON && buttonState3 == OFF)
		{
			while (digitalRead(2) == OFF)
			{
				return 2;
			}
		}

		while (buttonState0 == OFF && buttonState1 == OFF && buttonState2 == OFF && buttonState3 == ON)
		{
			while (digitalRead(3) == OFF)
			{
				return 3;
			}
		}

	} while ((buttonState0 + buttonState1 + buttonState2 + buttonState3) == 0 || (buttonState0 + buttonState1 + buttonState2 + buttonState3) == 1);

	return 4; /* Return anything but 0, 1, 2, 3 */
}

/* 
 * Name: check
 * Purpose: Check if the guessed values are the same as the answers
 * Parameter(s): The guessed number array, the answer array, and the state that the player is on
 * Return Value: TRUE or FALSE, depending if the guessed numbers are identical with the answers up to the current state 
 */
int check(int guessNum[NUM_BUTTON], int answer[LENGTH], int currentState)
{
	int index;

	for (index = 0; index < currentState; index++)
	{
		if (guessNum[index] != answer[index])
		{
			return FALSE;
		}
	}

	return TRUE;
}

/* 
 * Name: display
 * Purpose: Display the answer values up to the current state
 * Parameter(s): The answer array and the state of that the player is on
 * Return Value: the pressed button value 
 */
void display(int answer[LENGTH], int currentState)
{
	int index;

	for (index = 0; index < currentState; index++)
	{
		Sleep(TWO_SECONDS / 5);
		digitalWrite(answer[index], ON);
		Sleep(TWO_SECONDS / 5);
		digitalWrite(answer[index], OFF);
	}
}

/* 
 * Name: flashColor
 * Purpose: Flashes the red or green LED three times
 * Parameter(s): red or green
 * Return Value: void 
 */
void flashColor(int color)
{
	int index;

	for (index = 0; index < 3; index++)
	{
		digitalWrite(color, ON);
		Sleep(ONE_SECOND / 6);
		digitalWrite(color, OFF);
		Sleep(ONE_SECOND / 6);
	}
}
