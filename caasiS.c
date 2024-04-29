/*********************************************************************************************************
This is to certify that this project is my own work, based on my personal efforts in studying and applying the concepts
learned. I have constructed the functions and their respective algorithms and corresponding code by myself. The
program was run, tested, and debugged by my own efforts. I further certify that I have not copied in part or whole or
otherwise plagiarized the work of other students and/or persons.
Samantha Nicole L. Caasi, DLSU ID# 12134473
*********************************************************************************************************/

/*
Description: 	The program is called WordLe C Plus. As the name implies, the mechanics of the game is similar to 
				Wordle but with a variety of gamemodes and additional features.

Programmed by: 	S16A
Last modified: 	June 21, 20222
[Acknowledgements: https://www.geeksforgeeks.org/getch-function-in-c-with-examples/#:~:text=getch()%20is%20a%20nonstandard,single%20character%20from%20the%20keyboard.
					https://mathbits.com/MathBits/CompSci/LibraryFunc/rand.htm]

======================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "functions.h"

int main(int argc, char *argv[])
{
	/* array storing the Player's data */
	struct Player P[PMAX];

	/* array storing the title (strings) of the Main Menu choices*/
	char menuArr[8][25] = {"Classic", "Classic (Manual Input)", "Head to Head", "Zen", "Stats", "Help", "Exit", "\0"};	

	int nChoice = 0;				// variable for argc	
	int nMMChoice = 0; 				// variable to store the Main Menu Choice
	int nSChoice = 0;				// statistics menu choice
	int nHChoice = 0;				// help menu choice
	int nGoBack = 1;				// variable for the Main Menu loop

	int nMin = -1;					// lower bound of menu choice
	int nMax = -1;					// upper bound of menu choice
	
	int nNoPlayers = 0;				// variable that stores the number of players

	char cWord[6]; 					// array that stores the secret word/word to be guessed
	char cGMode[1];					// game mode
	char cFile[10] = "dict.txt";	// default dictionary file which can be replaced using command line args

	srand(time(NULL));				// random

	readFile(&nNoPlayers, P);		// function call

	switch(argc) 
	{
		case 1: break; // no arguments present
		case 2: // game mode argument present
				strcpy(cGMode, argv[1]);				// copies arg to cGMode
				
				if(!(strcmp(cGMode, "1"))) {//Classic
					nMMChoice = 1;
					nChoice = 1;
				}

				else if(!(strcmp(cGMode, "2"))) {//Classic (manual input)
					nMMChoice = 2;
					nChoice = 1;
				}

				else if(!(strcmp(cGMode, "3"))) {//Head-to-head
					nMMChoice = 3;
					nChoice = 1;
				}

				else if(!(strcmp(cGMode, "4"))) {//Zen
					nMMChoice = 4;
					nChoice = 1;
				}

				else printf("Invalid game mode argument.\n");
				break;

		case 3: // game mode and dictfile present
				strcpy(cGMode, argv[1]);					// copies arg to cGMode
				strcpy(cFile, argv[2]);						// copies arg to cFile

				if(!(strcmp(cGMode, "1"))) {//Classic
					nMMChoice = 1;
					nChoice = 1;
				}

				else if(!(strcmp(cGMode, "2"))) {//Classic (manual input)
					nMMChoice = 2;
					nChoice = 1;
				}

				else if(!(strcmp(cGMode, "3"))) {//Head-to-head
					nMMChoice = 3;
					nChoice = 1;
				}

				else if(!(strcmp(cGMode, "4"))) {//Zen
					nMMChoice = 4;
					nChoice = 1;
				}

				else printf("Invalid game mode argument.\n");
				break;
			
		default: 
			printf("Invalid arguments.\n");
			nMMChoice = 7;									// exits the program
			break;

	}
	
	printf("Welcome to Wordle C Plus!\n");
	sleep(1);
	
	do
	{
		clrs();
		mainMenu(menuArr);															// function call to display Main Menu options
		
		if(!nChoice)
			nMMChoice = getInt();													// function call to get Main Menu Choice and stores it to nMMChoice
								
		switch(nMMChoice) 
		{
			case 1: clrs();															// function call
					printf("%s\n", menuArr[0]);										// Classic							
					getchClrs();													// function call
																
					pickWord(cWord, cFile);											// function call to replace cWord
					checkGuessCl(cWord, &nNoPlayers, P, cFile);						// function call to dedicated Classic guess screen					
					
					writeFile(&nNoPlayers, P);										// function call
					
					nGoBack = promptOneZero();										// assigns user's choice of 1 or 0 to nGoBack
					break;	
																																			
			case 2: clrs();															// function call
					printf("%s\n", menuArr[1]);										// Classic (Manual input)
					askSecret(cWord, cFile);										// ask for secret word
					
					printf("\nThe word to guess: %s\n", cWord);						// shows the secret word before guess screen
					getchClrs();													// function call
																																
					checkGuess(cWord, CLM, cFile);									// calls guess screen
							
					nGoBack = promptOneZero();										// Assigns user's choice of 1 or 0 to nGoBack
					break;
			
			case 3: clrs();															// function call
					printf("%s\n\n", menuArr[2]);									// Head-to-Head			
					checkGuess(cWord, H2H, cFile);	
								
					nGoBack = promptOneZero();										// Assigns user's choice of 1 or 0 to nGoBack
					break;									
										
			case 4: clrs();															// function call
					printf("%s\n\n", menuArr[3]);									// Zen

					pickWord(cWord, cFile);											// function call
					checkGuess(cWord, ZEN, cFile);									// function call
					
					nGoBack = promptOneZero();										// Assigns user's choice of 1 or 0 to nGoBack
					break;									
					
			case 5: clrs();															// function call				
					
					do
					{	
						printf("%s\n", menuArr[4]);									// Stats
						statsMenu();												// function call
						
						/* setting the range values for input validation */
						nMin = 1;
						nMax = 4;


						nSChoice = promptInt(nMin, nMax);							// stores int				 
						
							switch(nSChoice) {
								case 1: playerStats(P); 
										break;
								case 2: allTimeSS(P, nNoPlayers); 
										break;
								case 3: allTimeR(P, nNoPlayers); 
										break;
								case 4: nSChoice = 4; nGoBack = 1; break;

								default: 
									promptInvalid();								// function call 
									getchClrs();									// function call
								
							}

					} while(nSChoice != 4);											// while user doesn't exit
					
					break;									
					
			case 6: clrs();															// function call
					printf("%s\n\n", menuArr[5]);									// help 

					/* setting the range values for input validation */
					nMin = 1;
					nMax = 6;

					helpIntro();													// function call		

					do
					{	
						helpMenu();													// displays menu
						printf("\nEnter respective number to view the mode's description\n");
						nHChoice = promptInt(nMin, nMax);							// stores int

						/* prints respective calls */
						switch(nHChoice) {
							case 1: helpClassic(); break;
							case 2: helpClassicMI(); break;
							case 3: helpH2H(); break;
							case 4: helpZen(); break;
							case 5: helpStats(); break;
							case 6: nHChoice = 6; nGoBack = 1; break;
							
							default:
								promptInvalid();							// function call 
								getchClrs();								// function call
						}
					} while(nHChoice != 6);
						
					break;									
						
			case 7: nGoBack = 0; 											// Exits the game by exiting the loop
					break;					
			
			default: 
				nGoBack = 1;										// returns to Main Menu
				promptInvalid();									// function call 
				getchClrs();										// function call
				break;
		}

	/* resets values */
	nChoice = 0;											
	nMin = -1;
	nMax = -1;	

	} while(nGoBack != 0);											// loops until nGoBack is not 0
	
	return 0;	
}
	
	
