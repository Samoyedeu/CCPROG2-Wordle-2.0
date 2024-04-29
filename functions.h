#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "functionsDeclarations.h"

#define MAX_TRIES 6

#define CLA 0
#define H2H 1
#define CLM 2
#define ZEN 3

#define PMAX 20

/* This function shortens the command for clearing the screen. */
void clrs(){
	system("cls");
}

/* This function prompts the user to enter a character to proceed. */
void promptGetch(){	
	printf("\nPress any key to continue..\n");
	getch();
}

/* This function is a combination of the first two functions; it clears the screen and prompts a character. */
void getchClrs(){
	promptGetch();
	clrs();
}

/* Core Functions */
/* This function is the core function of the program. It checks if the player's guess matches the word to guess. 

	Preconditions: cGuess and cWord must be both 5 characters long
	
	@param cWord[] is the players' secret word
	@param cGuess[] is the players' guess to the secret word
*/
void Wordle(char cGuess[], char cWord[]) {
	
	int i, j;													// counter variables
	char cWordCopy[6]; 											// array to store a copy of cWord
	char cClues[6] = {'x','x','x','x','x','\0'};				// strings that stores clues
	strcpy(cWordCopy, cWord);									// copies cWord to cWordCopy
	
	/* This loop checks for correct letters and correct positions */
	for(i = 0; i < 5; i++) {
	
		if(cWordCopy[i] == cGuess[i]) {							// checks if each letter of cGuess matches cWord copy
			cClues[i] = cGuess[i];								// assigns the correct letter guess to cClues
			cWordCopy[i] = '*';									// assigns * to cWordCopy to ignore correctly guessed letters for the next loop
		}			
	}
	
	/* This loop checks for correct letters in wrong positions */ 
	for(i = 0; i < 5; i++) {
		for(j = 0; j < 5; j++) {
							
			if(cWordCopy[j] == cGuess[i] && j != i && cClues[i] == 'x') { 			// checks every element of cWordCopy, j != i means indexes are not equal			
				cClues[i] = '+';													// assigns + to cClues
				cWordCopy[j] = '*';													// assigns * to cWordCopy for the letter to be ignored for index j
			} 
		}
	}		
	printf("%s\n", cClues);										// prints the cClues string (feedback)
}

/* This function picks a random word from the dictionary.
	
	Preconditions: cFile must be the dictionary file's name

	@param *cWord is the word that the player will guess
d	@param cFile[] is the dictionary file
*/
void pickWord(char *cWord, char cFile[]){
	FILE *fp;										// file pointer
	
	fp = fopen(cFile, "r");							// opens the dictionary file
	
	int nRandom;									// variable for word randomizing from dictionary
	int nWTotal = 0;								// counter variable for total number of words from dictFile
	int nMin = 1;									// minimum value for rand
	int i;											// counter variable
	
	nWTotal = getWordCount(cFile);					// stores word count from function call	
	nRandom = nMin + rand() % (nWTotal + 1 - nMin);		// stores random number		
	printf("%d\n", nRandom);// cheats
	for(i = 0; i < nRandom; i++) {					// scans the nRandom-th word from the dictionary file											
		fscanf(fp, "%5s", cWord);					
	}
	printf("%s\n", cWord);// cheats
	strcpy(cWord, Capitalize(cWord));				// copies capitalized cWord to cWord
	fclose(fp);										// closes the dictionary file
}

/* This function gets the secret word/word to guess of the player (from another player)
	
	Preconditions: cFile[] must be the dictionary file's name

	@param *cWord is the word that the player will guess
	@param cFile[] is the dictionary file
*/
void askSecret (char *cWord, char cFile[]){

	int len = 0;								// stores length of string
	int nDict;									// stores truth value for checking

	while(len != 5 || nDict == 1){				// loop
		printf("\nEnter guess: ");
		scanf("%s", cWord); 					// scans for cWord
		
		strcpy(cWord, Capitalize(cWord));		// copies the capitalized cWord to cWord
			
		len = strlen(cWord);					// stores length of cWord

		if(len < 5 || len > 5){					// if cWord is not 5 characters long
			promptNot5();						// function call to prompt 
		}

		else {
			nDict = checkDict(cWord, cFile);	// stores value of function call

			if(nDict == 1) {					// condition if word is not from dictionary
				printf("This word is not in the dictionary.\n");
			}
		}
	}
}

/* This function gets the guess of the player to the cWord

	Preconditions: cFile[] must be the dictionary file's name

	@param *cGuess is the entered guess of the player
	@param cFile[] is the dictionary file
*/
void askGuess(char *cGuess, char cFile[]){	

	int len = 0;								// stores length of string
	int nDict;									// stores truth value for checking

	while(len != 5 || nDict == 1){				// loop
		printf("\nEnter guess: ");
		scanf("%s", cGuess); 					// scans for cGuess
		
		strcpy(cGuess, Capitalize(cGuess));		// Copies the capitalized cGuess to cGuess
			
		len = strlen(cGuess);					// stores length of cGuess

		if(len < 5 || len > 5){					// if cGuess is not 5 characters long
			promptNot5();						// function call to prompt 
		}

		else {
			nDict = checkDict(cGuess, cFile);	// stores value of function call

			if(nDict == 1) {					// condition if word is not from dictionary
				printf("This word is not in the dictionary.\n");
			}
		}
	}
}

/* This function checks the guess of the player depending on the mode

	Preconditions:  cWord[] must be 5 characters long
					mode must be an integer
					cFile[] must be the dictionary file's name

	@param cWord[] is the word that the player will guess
	@param mode determines what type of checking the function will do
	@param cFile[] is the dictionary file
*/
void checkGuess(char cWord[], int mode, char cFile[]){
	
	time_t tstart, tstop;								// for time monitoring

	int i;												// counter variable
	int P1;												// stores win of Player 1
	int P2;												// stores win of Player 2

	int nTries = 0;										// stores number of tries
	int nCTries;										// stores copy of nTries
	
	int nExit = 1;										// variable to exit loop
	int guessed = -1;									// stores strcmp value
	
	int timeP0, timeP1, timeP2;							// stores time of players

	char cGuess[6];										// guess of the player
	
	switch(mode) 
	{
		case 1: // Head to head
										
			for(i = 1; i < 3; i++) {		
				if(i == 1) 									// if-else condition to print whose turn is it to give secret word
					printf("Player 2's Turn");
				else
					printf("Player 1's Turn");
				
				askSecret(cWord, cFile);					// function call to ask for the secret word
				getchClrs();								// function call
				
				printf("Player %d's GUESS SCREEN", i);		// prints the Guess Screen of the respective player
										
				do{
					tstart = time(NULL);					// starts tracking the time
					do{ 
						askGuess(cGuess, cFile);			// function call to ask player for a Guess
						nTries++;							// increments nTries
						
						guessed = strcmp(cWord, cGuess);	// stores strcmp
						if(!guessed) {						// checks if the cGuess matches cWord		
							if(i == 1) {			
								P1++;						// increments the variable for Player 1
								printf("\nPlayer 1");
							}
							else {
								P2++;						// increments the variable for Player 2
								printf("\nPlayer 2");
							} 
							
							printf(" guessed it correctly!\n");
							printf("%s\n", cWord);			// prints the cWord
							nCTries = nTries;			// stores the nTries in nCTries
							nTries = 6;						// to exit loop
						}
						
						else {								// else statement to check cGuess manually and provide feedback to the player 
							Wordle(cGuess, cWord);			// function call 
							
							if(nTries == 6) 				// checks if there are no more attempts
							{
								if(i == 1) 					
								{
									P1++;					// increments the variable for Player 1
									printf("\nPlayer 1");
								}
								
								else {
									P2++;					// increments the variable for Player 2
									printf("\nPlayer 2");
								} 
								
								printf(" ran out of guesses!\n");						 					
								printf("\nThe word was: %s\n", cWord);		// Prints the cWord
								nCTries = nTries;						// Stores the nTries in nCTries
							}
						}	
												
					} while (nTries != MAX_TRIES);			// loops until the player has made 6 guesses
					nExit = 1;								// to exit loop
					
				} while (nExit != 1);						// loops until nExit is not
				
				tstop = time(NULL);							// stops tracking the time
				timeP0 = tstop-tstart;						// stores player's time
				printf("\nPlayer %d finished with %d seconds\nGuesses: %d out of 6\n\n", i, timeP0, nCTries);
				getchClrs(); 								// function call					
				
				if(i == 1) 									// if else statement to store the time taken to the respective players
					timeP1 = tstop - tstart;				// stores Player 1's time
				else
					timeP2 = tstop - tstart;				// stores Player 2's time
				
				nExit = 0;									// resets to enter loop again
				nTries = 0;									// resets the number of tries
			}
			
			resultH2H(P1, P2, timeP1, timeP2);				// function call
			
			P1 = 0;											// Resets Player 1's correct guess status
			P2 = 0;											// Resets Player 2's correct guess status
			break;
	
		case 2:												// Classic (manual input)
		
			printf("GUESS SCREEN\n");
			
			do { // loop for time tracking	
				tstart = time(NULL);						// starts tracking the time							
				do {										// loop for number of guesses
					askGuess(cGuess, cFile);				// ask for guess						
					nTries++;								// increments nTries

					guessed = strcmp(cWord, cGuess);		// stores strcmp

					if(!guessed) {							// checks if the cWord is correctly guessed
						printf("%s\n", cWord);
						printf("\nYou won!\n");		
						nCTries = nTries;					// retains original number of attempts
						nTries = 6;							// ends the while loop
					}							
					
					else {									// else statement to check cGuess manually and provide feedback to the player
						Wordle(cGuess, cWord);				// function call
						
						if(nTries == 6) {					// condition if all attempts have been used up	 					
							printf("\nYou lost!\n");
							printf("The word was: %s\n", cWord);		
							nCTries = nTries;			// retains original number of attempts before it is reset
						}
					}								
					
				} while(nTries != MAX_TRIES);				// loops until the player has made 6 guesses
				
				tstop = time(NULL);							// stops tracking the time
				timeP0 = tstop-tstart;						// stores player's time
				printf("Your time is %d seconds\nYour Guesses: %d out of 6\n", timeP0, nCTries);			
				
				nExit = 1;									// exits the do while loop
				
			} while(nExit != 1);							// loops until nExit is not 1
						
			nTries = 0;										// resets the number of nTries	
			break;		
		
		case 3: 											// Zen
			
			printf("GUESS SCREEN\n");
			
			do { // loop for time tracking	
				tstart = time(NULL);						// starts tracking the time							
				do 
				{
					nTries++;								// increments nTries	
					askGuess(cGuess, cFile);				// ask for guess
															
					guessed = strcmp(cWord, cGuess);		// stores strcmp

					if(!guessed) {							// checks if the cWord is correctly guessed
						printf("%s\n", cWord);
						printf("\nYou won!\n");		
						nCTries = nTries;				// retains original number of attempts
						nTries = 6;							// ends the while loop 
					}							
					
					else {									// else statement to check cGuess manually and provide feedback to the player
						Wordle(cGuess, cWord);				// function call
					}	
												
				} while(guessed != 0);						// loops until the player has made 6 guesses
				
				tstop = time(NULL);							// stops tracking the time
				timeP0 = tstop-tstart;						// stores player's time
				printf("Your time is %d seconds\nYour Guesses: %d\n", timeP0, nCTries);
				
				nExit = 1;									// exits the do while loop
				
			} while(nExit != 1);							// Loops until nExit is not 1
						
			nTries = 0;										// resets the number of nTries	
			break;
	}
}

/* This function checks the guess of the player depending on the mode

	Preconditions:  cWord[] must be 5 characters long
					cFile[] must be the dictionary file's name
					*nNoPlayers must be an integer

	@param cWord[] is the word that the player will guess
	@param *nNoPlayers is the number of players existing on the player.txt file
	@param Player P[] is the struct that stores player's information
	@param cFile[] is the dictionary file
*/
void checkGuessCl(char cWord[], int *nNoPlayers, struct Player P[], char cFile[]){
	
	time_t tstart, tstop;				// for time monitoring						

	int timeP0;							// stores player's time

	int nTries = 0;						// stores number of tries
	int nCTries;						// stores copy of nTries

	int guessed = -1;					// stores strcmp
	
	int nExit;							// exit variable
	int nCWS = 0;						// stores current winstreak
	int win = -1;						// win status

	char name[4];						// stores player's name
	char cGuess[6];						// guess of the player

	askName(name);						// function call
	getchClrs();						// function call

	printf("GUESS SCREEN\n");
	
	do { // loop for time tracking	
		tstart = time(NULL);					// starts tracking the time							
		do {									// loop for number of guesses	
			askGuess(cGuess, cFile);			// ask for guess						
			nTries++;							// increments nTries
			guessed = strcmp(cWord, cGuess);	

			if(!guessed) {						// checks if the cWord is correctly guessed
				printf("%s\n", cWord);			
				printf("\nYou won!\n");		
				nCTries = nTries;			// retains original number of attempts
				nCWS++;							// increments winstreak
				nTries = 6;						// ends while loop
				win = 1;						// won
			}							
			
			else {								// else statement to check cGuess manually and provide feedback to the player
				Wordle(cGuess, cWord);			// function call
				
				if(nTries == 6) {						 					
					printf("\nYou lost!\n");
					printf("The word was: %s\n", cWord);		
					nCTries = nTries;		// retains original number of attempts before it is reset
					nCWS = 0;					// resets winstreak
					win = 0;					// did not win
				}
			}								
			
		} while(nTries != MAX_TRIES);			// Loops until the player has made 6 guesses
		
		tstop = time(NULL);						// Stops tracking the time

		timeP0 = tstop-tstart;					// stores player's time
		printf("Your time is %d seconds\nYour Guesses: %d out of 6\n", timeP0, nCTries);	

		/* function to save player's record*/
		saveRecord(win, name, timeP0, nCTries, nCWS, nNoPlayers, P);
		
		nExit = 1;								// exits the do while loop
		
	} while(nExit != 1);						// loops until nExit is not 1

	win = -1;									// resets win
	nCWS = 0;									// resets winstreak
	nTries = 0;									// resets the number of nTries	
}

/* This function gets the word count of the dictionary file 

	Preconditions: cFile[] must be the dictionary file's name

	@param cFile[] is the dictionary file

	@return nWTotal - the total word count of the dictionary file
*/
int getWordCount(char cFile[]) {
	
	FILE *fp;							// file pointer
	fp = fopen(cFile, "r");				// opens the dictionary file

	int nWTotal = 0;					// stores the total word count of the dictionary file
	char hold[6];						// stores a 5-character long string
	
	/* scans for the word until EOF (limited to 6000 entries) */							
	while((fscanf(fp, "%5s", hold)) != EOF) {																
		if(nWTotal < 6000) {
			nWTotal++;					// increments nWTotal
		}
	}
	
	return nWTotal;						// return value

	fclose(fp);							// closes the dictionary file
}

/* This checks if the word passed is found on the dictionary

	Preconditions:  cCheck[] must be 5 characters long
					cFile[] must be the dictionary file's name

	@param cCheck[] is a word passed to be checked against the dictionary file
	@param cFile[] is the dictionary file

	@return 0 - if the word is on the dictionary
	@return 1 - if the word is not on the dictionary
*/
int checkDict(char cCheck[], char cFile[]){
	
	FILE *fp;										// file pointer

	fp = fopen(cFile, "r");							// opens the dictionary file

	char hold[6];									// stores a 5-character long string
	
	/* scans the words from the dictionary file */							
	while((fgets(hold, sizeof(hold), fp))){
		strcpy(hold, Capitalize(hold));				// copies the capitalized hold to hold		
														
		if(!(strcmp(cCheck, hold))) {				// if the word is found on the dictionary
			return 0;
		}
	}
	
	return 1;										// if the word is not found on the dictionary
	
	fclose(fp);										// closes the dictionary file
}

/* This function prints out the result of the players' in Head-to-Head mode

	Preconditions: P1, P2, timeP1, timeP2 must all be positive integers

	@param P1 is Player 1's win status
	@param P2 is Player 2's win status
	@param timeP1 is Player 1's time
	@param time P2 is Player 2's time

*/
void resultH2H(int P1, int P2, int timeP1, int timeP2){
						
	if(P1 && P2)														// if both players correctly guessed the cWord
	{
		if(timeP1 < timeP2)												// when Player 1 has less time taken
			printf("Player 1 wins with %d seconds.\n", timeP1);
						
		else if(timeP2 < timeP1)										// when Player 2 has less time taken
			printf("Player 2 wins with %d seconds.\n", timeP2);
			
		else 															// when both players have the same time taken
			printf("It's a draw!\n");
	}
	
	else if(P1 > P2)													// when Player 1 guessed the cWord
		printf("Player 1 won by guessing in less time.\n");
	else if(P1 < P2)													// when Player 2 guess the cWord
		printf("Player 2 won by guessing in less time.\n");
	else																// when both players did not guess the word correctly
		printf("It's a draw!\n");										
																																																			
	printf("\n"); 									// prints new line
	printf("Player 1's Time: %d\n", timeP1);		// prints Player 1's time
	printf("Player 2's Time: %d\n", timeP2);		// prints Player 2's time
}

/* Menu */
/* This function prints the menu for the Help option */
void helpMenu(){
	printf("1. Classic\n");
	printf("2. Classic (Manual Input)\n");
	printf("3. Head to Head\n");
	printf("4. Zen\n");		
	printf("5. Stats\n");
	printf("6. Exit\n");		
}

/* This function prints the Main Menu options 

	@param menuArr is an array containing the main menu options
*/
void mainMenu(char menuArr[8][25]){

	int i, j = 1;									// counter variables, j starts at 1 because the menu choices starts at 1
	printf("Main Menu\n");							// prints the Title
	
	/* array storing the title (strings) of main menu options */	
	
	for(i = 0; i < 7; i++) {						// for loop to print the array
		if(j <= 7) {
			printf("%d. %s\n", j, menuArr[i]);
			j++;
		}
	}	
	printf("\nEnter choice: ");
}

/* This function prints the Sub-Menu for Stats */
void statsMenu(){
	printf("1. Player Statistics\n");
	printf("2. All-Time Score Statistics\n");
	printf("3. All-Time Rankings\n");
	printf("4. Exit\n");
}

/* This function prints the options for Statistics > All-Time Rankings > N-Turn Section*/
void allTimeTurnMenu(){
	int i;		// counter variable

	printf("Choose N-Turn Sort.\n");

	/* for loop for printing */
	for(i = 1; i < 7; i++) {
		printf("[%d] Turn %d\n", i, i);
	}		

	printf("[0] Exit\n");
}

/* This function prints the menu for Statistics > All-Time Rankings */
void allTimeRMenu(){
	printf("1. (All-time) Winstreak\n");
	printf("2. N-turn wins\n");
	printf("3. Best time\n");
	printf("4. Exit\n");
}

/* Help */
/* These functions utilize sleep() for a momentary pause between the printing of the lines */
/* This function prints an introductory note for the Help option */
void helpIntro() {
	printf("The program is called WordLe C Plus. As the name implies, the mechanics of the game \nis similar to Wordle but with a variety of gamemodes and additional features.\n\n");
	sleep(1);
	printf("The mechanics of the game are as follows:\n");
	sleep(2);
	printf("\nYou are tasked to guess a 5-letter word that is either\nrandomly selected by the program or inputted by the user/s.\n");
	sleep(2);
	printf("\nYou are then given a maximum of 6 guesses before the word is revealed.\n");
	sleep(2);
	printf("\nThe time it took for the player to guess is also recorded and displayed.\n\n");
	sleep(2); 
}

/* This function prints information about Classic Mode */
void helpClassic(){
	clrs();					// function call

	printf("Classic\n");									
	printf("Classic mode is your regular Wordle wherein the program randomly picks the word to guess from the dictionary file.\n");
	sleep(2);
	printf("Here you will be asked for a 3-letter name which will be vital in recording your statistics.\n\n");
	
	getchClrs();			// function call
}

/* This function prints information about Classic (Manual Input) Mode */
void helpClassicMI(){
	clrs();					// function call

	printf("Classic (Manual input)\n");						
	printf("Classic (Manual input) mode is a variation of the Classic mode wherein the program asks the user for the word to guess.\n");
	sleep(2);
	printf("The word inputted must be a valid word from the dictionary file.\n\n");
	
	getchClrs();			// function call															
}

/* This function prints information about Head-to-Head Mode */
void helpH2H(){
	clrs();				// function call

	printf("Head to Head\n");												
	printf("Head to Head mode mode is a variation of the Classic (manual input) mode.\n");
	sleep(2);
	printf("The word inputted must be a valid word from the dictionary file.\n\n");
	
	getchClrs();		// function call
}

/* This function prints information about Zen Mode */
void helpZen(){		
	clrs();				// function call

	printf("Zen\n");												// zen
	printf("Zen mode is a variation of the Classic mode.\n");
	sleep(2);
	printf("This time the user has unlimited number of guesses.\n");
	
	getchClrs();		// function call
}

/* This function prints information about Stats Mode */
void helpStats(){
	clrs();
	printf("Stats\n");												
	printf("Stats or Statistics will read the player.txt file wherein the players' data is saved\n");
	sleep(2);
	printf("\nPlayer Statistics include a player's:\n");
	printf("Winstreak (current and max)\n");
	printf("Frequency Distribution of the number of turns to win\n");
	printf("Best time\n");
	sleep(2);
	printf("\nAll-Time Score Statistics include:\n");
	printf("Frequency Distribution of the number of turns to win (Masterlist)");
	printf("Top 3 Longest Winstreaks\n");
	sleep(2);
	printf("\nAll-Time Rankings (Top 5) include:\n");
	printf("Winstreak\n");
	printf("N-turn wins (N is user selectable)\n");
	printf("Best time\n");
	
	getchClrs();
}

/* Display Prompts */
/* This function prompts the user that their input is invalid */
void promptInvalid(){
	printf("\nInvalid input.\n");							// prints warning
}

/* This function prompts the user that their string input is not 5 letters */	
void promptNot5(){
	printf("Please enter a 5-letter word.\n");				// prints warning	
}

/* This function prompts the user that their string input is not 3 letters */
void promptNot3(){
	printf("\nPlease enter a 3-letter code name.\n\n");		// Prints warning	
}


/* Inputs */
/* This function gets an integer input from the user and returns it 

	Precondition: nInput should be an integer
	
	@return nInput - returns the integer input
*/
int getInt() {
	int nInput;								// stores input of user
	
	scanf("%d", &nInput);					// scans for integer input and stores it to nInput
	
	return nInput;							// returns nInput
}
/* This function prompts the user for their options after completing a game mode.
	It also loops until the user enters a valid choice.

	@return nChoice - returns the integer input (nChoice) to go back to the Main 
						Menu or quit the game
*/
int promptOneZero() {
	int nChoice;								// stores user's choice
	
	do {
		printf("\nEnter 1 to go back to the Main Menu. Enter 0 to exit.\n");
		printf("Enter choice: ");
		
		nChoice = getInt();
	
		if(nChoice != 1 && nChoice != 0) {		// if input is not 1 or 0
			promptInvalid();					// function call
			promptGetch();						// function call
		}
	
	} while(nChoice != 1 && nChoice != 0);		// loops until user enters 1 or 0
	
	return nChoice;								// return value
}

/* This function prompt the user for their options.
	It also loops until the user enters a valid choice.

	@param nMin is the lower bound of the options
	@param nMax is the upper bound of the options

	@return nChoice - returns the integer input (nChoice) to go back to the Main 
						Menu or quit the game
*/
int promptInt(int nMin, int nMax) {
	int nChoice;								// stores user's choice
	
	do {
		printf("\nEnter choice: ");
		
			nChoice = getInt();
	
		if(nChoice < nMin && nChoice > nMax) {	// if input is not within range	
			promptInvalid();					// function call
			promptGetch();						// function call
		}
	
	} while(nChoice < nMin && nChoice > nMax);	// loops until input is valid
	
	return nChoice;								// return value
}


/* Helpers */
/* This function capitalizes the strings that are passed to it. This is to address
	the case sensitivity of the program. 

	Preconditions: *str must be 5-characters long
	
	@param *str is the string passed to function

	@return str returns the string
*/
char *Capitalize(char *str){
	int i;										// counter variable
	int len = strlen(str);						// variable to store string length of str									
	
	for(i = 0; i < len; i++)					// koop that starts from the first index
	{
		if(str[i] >= 'a' && str[i] <= 'z') 		// checks if the string has lower case letters
		{
			str[i] -= 32; // subtracts 32 and replaces the lower case characters with its respective upper case character
		}
	}
	return str;									// returns the string
}

/* Statistics Core */
/* This function asks for a 3-character code name for Classic Mode.
	It also loops until the user enters a valid string.

	@param *name is the player's name
*/
void askName(char *name){
	int len;									// stores length of string
	
	do{											// asks secret word
		printf("Enter 3-letter name: ");
		scanf("%s", name); 						// scans for name
		strcpy(name, Capitalize(name));			// copies the capitalized name to name
			
		len = strlen(name);						// stores len of the name 
	
		if(len != 3)							// if the name is not 3-characters long
			promptNot3();						// function call 
						
	} while(len != 3);							// loops until string entered is of length 5		
}

/* This function saves the players' data onto player.txt

	Preconditions:  win must be an integer
					name must be a 3-character long string
					timeP0, nCTries, nCWS, *nNoPlayers must be integers
	
	@param win is player's wins status
	@param name[] is the player's name
	@param timeP0 is the player's time
	@param nCTries is the player's number of attempts
	@param *nNoPlayers is the number of players existing on the player.txt file
	@param nCWS is the current winstreak
	@param Player P[] is the struct that stores player's information
*/
void saveRecord(int win, char name[], int timeP0, int nCTries, int nCWS, int *nNoPlayers, struct Player P[]){

	FILE *fp;									// file pointer

	int nI;										// stores index of player name
	
	fp = fopen("player.txt", "r");				// opens the players' data file
	
	if(fp == NULL) {							// if file does not exist
		fp = fopen("player.txt", "w");			// creates players' data file
	}

	nI = searchPlayer(name, P);					// stores return value of the function call
	
	if(nI == -1 && win == 1) {					// if player does not exist and wins
		if(*nNoPlayers == PMAX) {				// if number of players on the file reaches 20 
			resetData(P);						// function call
			*nNoPlayers = 0;					// resets the number of players
		}

		addNew(name, timeP0, nCTries, nCWS, nNoPlayers, P);		// function call	
	}
	
	else if(nI == -1 && !win) {					// if player does not exist and does not win
	// ignore
	}

	else if(nI != -1 && win == 1){				// if player exists and wins
		updateExisting(nI, timeP0, nCTries, P);					// function call
	}

	else if(nI != -1 && !win) {					// if player exists and does not not win
		nCTries = 0;							// resets the number of tries
		updateExisting(nI, timeP0, nCTries, P);					// function call
	}
	
	fclose(fp);									// closes the players' data file
}

/* This function adds data of a player to the player.txt.

	Preconditions:  name[] must be a 3-character code
					timeP0, nCTries, nCWS, *nNoPlayers must be integers

	@param name[] is the player's name
	@param timeP0 is the player's time
	@param nCTries is the player's number of attempts
	@param *nNoPlayers is the number of players existing on the player.txt file
	@param nCWS is the current winstreak
	@param Player P[] is the struct that stores player's information
*/
void addNew(char name[], int timeP0, int nCTries, int nCWS, int *nNoPlayers, struct Player P[]){

	int i;												// counter variable
	int nI = *nNoPlayers;								// stores uses the current number of players as index

	strcpy(P[nI].name, name);							// copies name to the struct's name member
	P[nI].timePC = timeP0;								// copies time to the struct's current time member
	P[nI].timePB = P[nI].timePC;						// copies struct's current time member to struct's best time member
	P[nI].nCWS = nCWS;									// copies current winstreak to struct's current winstreak
	P[nI].nHWS = P[nI].nCWS;							// copies struct's current winstreak to struct's highest winstreak

	for(i = 0; i < 6; i++)
		P[nI].turns[i] = 0;								// initializes struct's turns member to 0
	
	switch(nCTries) {// adds an initial count to respective turn 
		case 1: P[nI].turns[0] = 1; break;
		case 2: P[nI].turns[1] = 1; break;
		case 3: P[nI].turns[2] = 1; break;
		case 4: P[nI].turns[3] = 1; break;
		case 5: P[nI].turns[4] = 1; break;
		case 6: P[nI].turns[5] = 1; break;		
	}

	*nNoPlayers += 1;									// increments the number of current players
}

/* This function updates data of an existing player from player.txt.

	Preconditions:  nI, timeP0, nCTries, and nCWS must be integers

	@param nI is the index of the player to be updated
	@param timeP0 is the player's time
	@param nCTries is the player's number of attempts
	@param Player P[] is the struct that stores player's information
*/
void updateExisting(int nI, int timeP0, int nCTries, struct Player P[]){

	//P[nI].timePC = 0;						// initializes struct's current time member to 0

	if (nCTries > 0)						// if has more than 0 guesses (this is a win condition for the function that calls it)
	{
		P[nI].nCWS += 1;					// increments struct's current winstreak by one
		P[nI].timePC = timeP0;				// copies the time to the struct's current time member

		if(P[nI].timePC < P[nI].timePB)			// if the struct's current time is less than the struct's best time member
			P[nI].timePB = P[nI].timePC;		// replace the best time with the current time

		else if(P[nI].timePC < P[nI].timePB && P[nI].timePB == 0)
		P[nI].timePB = P[nI].timePC;

		if(P[nI].nCWS > P[nI].nHWS)				// if the struct's current winstreak is greater than the struct's highest winstreak
			P[nI].nHWS = P[nI].nCWS;			// replace the highest winstreak with the current winstreak
		
		switch(nCTries) {	// adds a count to respective turn 
			case 1: P[nI].turns[0] += 1; break;
			case 2: P[nI].turns[1] += 1; break;
			case 3: P[nI].turns[2] += 1; break;
			case 4: P[nI].turns[3] += 1; break;
			case 5: P[nI].turns[4] += 1; break;
			case 6: P[nI].turns[5] += 1; break;		
		}
	}
	
	else {									// if did not win
		P[nI].nCWS = 0;						// sets the struct's current winstreak to 0
		P[nI].timePC = timeP0;					// sets the struct's current time member to 0
	}
	

}

/* This function writes the data to player.txt.

	Preconditions:  *nNoPlayers must be an integer

	@param *nNoPlayers is the current number of players
	@param Player P[] is the struct that stores player's information
*/
void writeFile(int *nNoPlayers, struct Player P[]){
	int i;									// counter variable

	FILE *fp;								// file pointer
	
	fp = fopen("player.txt", "w");			// writes to the players' data file
	
	/* loop to write the array of structs to the player.txt */
	for(i = 0; i < *nNoPlayers; i++){		
		fprintf(fp, "%s %d %d %d %d %d %d %d %d %d %d\n", P[i].name, P[i].nHWS, P[i].nCWS, P[i].timePB, P[i].timePC, P[i].turns[0], P[i].turns[1], P[i].turns[2], P[i].turns[3], P[i].turns[4], P[i].turns[5]);
	}	
	
	fclose(fp);								// closes the players' data file
}

/* This function reads the data from player.txt.

	Preconditions:  *nNoPlayers must be an integer

	@param *nNoPlayers is the current number of players
	@param Player P[] is the struct that stores player's information
*/
void readFile(int *nNoPlayers, struct Player P[]){
	int i = 0; 								// index
	
	FILE *fp;								// file pointer
	
	fp = fopen("player.txt", "r");			// opens the players' data file
	
	/* loop to reads the array of structs from the player.txt */
	while(fscanf(fp, "%s %d %d %d %d %d %d %d %d %d %d", P[i].name, &P[i].nHWS, &P[i].nCWS, &P[i].timePB, &P[i].timePC, &P[i].turns[0], &P[i].turns[1], &P[i].turns[2], &P[i].turns[3], &P[i].turns[4], &P[i].turns[5]) != EOF)
	{
		*nNoPlayers += 1;					// increments the number of players
		i++;								// increments the index
	}

	fclose(fp);								// closes the players' data file
}


/* This function resets the data written on player.txt.

	@param Player P[] is the struct that stores player's information
*/
void resetData(struct Player P[]){
	int i, j;									// counter variables

	printf("player.txt has been reset.\n");		// prompt 

	/* loop that sets all struct's members' data to 0 */
	for(i = 0; i <= PMAX; i++) {				
		strcpy(P[i].name, "0");					// resets name
		P[i].nHWS = 0;							// resets highest winstreak
		P[i].nCWS = 0;							// resets current winstreak
		P[i].timePB = 0;						// resets best time
		P[i].timePC = 0;						// resets current time
		
		/* loop that resets turns */
		for(j = 0; j < 6; j++) {
			P[i].turns[j] = 0;				
		}
	}
}

/* This function searches for the player on the struct and 
returns its	respective index.

	@param name[] is the player's name
	@param Player P[] is the struct that stores player's information

	@return i returns index of the player
	@return -1 if the player is not found
*/
int searchPlayer(char name[], struct Player P[]){
	int i;					// counter variable/index
	
	/* loop that goes through all the players*/
	for(i = 0; i < PMAX; i++){
		if(!(strcmp(P[i].name, name)))	// checks if the name 
			return i;					// returns index
	}
	
	return -1;							// if player is not found
}


/* Statistics */
/* This function prints the Statistics > Player Statistics
	Player Statistics contain Win streak (current and max)
	Frequency distribution of the number of turns to win
	Best time

	@param Player P[] is the struct that stores player's information
 */
void playerStats(struct Player P[PMAX]){
	FILE *fp;								// file pointer
	fp = fopen("player.txt", "r");			// opens the players' data file
	/* index */
	int i;	
	int nI;

	char name[4];							// player's name

	clrs();									// function call

	printf("Player Statistics\n");
	
	/* checks for the name and the index */
	askName(name);
	nI = searchPlayer(name, P);
	
	/* if player is found, print data */
	if(nI != -1) {
		printf("\n%s\n", P[nI].name);
		printf("Current winstreak: %d\n", P[nI].nCWS);
		printf("Max winstreak: %d\n", P[nI].nHWS);

		printf("\nFrequency Distribution\n");

		for(i = 0; i < 6; i++)
			printf("Turn %d - %d\n", i + 1, P[nI].turns[i]);

		printf("\nBest time: %d\n\n", P[nI].timePB);
	}
	
	else{
		printf("\nPlayer does not exist.\n");
		printf("Returning to Stats Menu.\n");
	} 

	getchClrs();							// function call

	fclose(fp);								// closes the players' data file
}

/* This function prints the 

	Statistics > All-Time Score Statistics > Frequency distribution 
	of the number of turns to win

	@param Player P[] is the struct that stores player's information
	@param nNoPlayers is the number of current players
 */
void printFreqDist(struct Player P[], int nNoPlayers){
	int i, j;							// counter variables

	int turns[6];						// temp array to store turns

	/* initializes array to 0 */
	for(i = 0; i < 6; i++) 
		turns[i] = 0;
	
	printf("Master Frequency Distribution\n");
	
	/* adds value from struct */
	for(i = 0; i < nNoPlayers; i++) {
		for(j = 0; j < 6; j++) 
			turns[j] += P[i].turns[j];
	}

	/* prints */
	for(i = 0; i < 6; i++)
		printf("Turn %d - %d\n", i + 1, turns[i]);

	printf("\n");						// prints new line
}

/* This function prints the 

	Statistics > All-Time Score Statistics 
	
	It contains Frequency distribution of the number of turns to win and
	the Top 3 Longest winstreaks

	@param Player P[] is the struct that stores player's information
	@param nNoPlayers is the number of current players
 */
void allTimeSS(struct Player P[], int nNoPlayers){
	FILE *fp;							// file pointer
	fp = fopen("player.txt", "r");		// opens the players' data file

	/* function calls */
	clrs();								
	getTop3WS(P, nNoPlayers);
	printFreqDist(P, nNoPlayers);
	
	getchClrs();						// function call

	fclose(fp);							// closes the players' data file
}

/* This function prints the 

	Statistics > All-Time Rankings 
	
	It contains the All-Time:
		Winstreak
		N-turn wins (N is user selectable)
		Best time

	@param Player P[] is the struct that stores player's information
	@param nNoPlayers is the number of current players
 */
void allTimeR(struct Player P[], int nNoPlayers){
	FILE *fp;								// file pointer
	fp = fopen("player.txt", "r");			// opens the players' data file
	
	int nChoice = 0;						// stores menu choice
	int nMin = 1;							// lower bound of menu choice
	int nMax = 4;							// upper bound of menu choice

	clrs();									// function call

	do
	{	
		printf("All-Time Rankings\n");	
		allTimeRMenu();						// function call
		
		nChoice = promptInt(nMin, nMax);	// stores value of function call
		
		/* respective function calls */
		switch(nChoice) {
			case 1: allTimeWS(P, nNoPlayers); break;
			case 2: allTimeTurn(P, nNoPlayers); break;
			case 3: allTimeBT(P, nNoPlayers); break;
			case 4: nChoice = 4; 

			default: 
				getchClrs();				// clears screen
		}

	} while(nChoice != 4);					// until player wants to exit

	clrs();									// function call

	fclose(fp);								// closes the players' data file
}

/* This function prints the 

	Statistics > All-Time Rankings > All-time Winstreak

	@param Player P[] is the struct that stores player's information
	@param nNoPlayers is the number of current players
 */
void allTimeWS(struct Player P[], int nNoPlayers){	
	int i, j, minIndex;						// counter variables/index
	int nTemp;								// temp int
	char sTemp[4];							// temp string
	
	int nHWS[PMAX];							// array for highest winstreak
	char name[nNoPlayers][4];				// array for name

	clrs();									// function call

	printf("(All-time) Winstreaks\n");

	/* copies related struct data to local array */
	for(i = 0; i < nNoPlayers; i++) {
		nHWS[i] = P[i].nHWS;
		strcpy(name[i], P[i].name);
	}

	/* copies related struct data to local array */
    for (i = 0; i < nNoPlayers - 1; i++)
	{
		minIndex = i;
        for (j = i + 1; j < nNoPlayers; j++)
		{
            if (nHWS[j] > nHWS[minIndex]) 
			{				
				minIndex = j;
		    }
		}
		nTemp = nHWS[minIndex];
		nHWS[minIndex] = nHWS[i]; 
		nHWS[i] = nTemp;  

		strcpy(sTemp, name[minIndex]);
		strcpy(name[minIndex], name[i]); 
		strcpy(name[i], sTemp);  
	}

	/* prints */
	for(i = 0; i < 5; i++) {
		if(i < nNoPlayers) 
			printf("[Player %d] %s: %d\n", i+1, name[i], nHWS[i]);

		else printf("[Player %d] EMPTY\n", i+1);
	}
	printf("\n");

	getchClrs();						// function call
}

/* This function prints the 

	Statistics > All-Time Rankings > Best time

	@param Player P[] is the struct that stores player's information
	@param nNoPlayers is the number of current players
 */
void allTimeBT(struct Player P[], int nNoPlayers){

	int i, j, minIndex;						// counter variables/index
	int nTemp;								// temp int
	char sTemp[4];							// temp string
	
	int timePB[PMAX];						// array for best time
	char name[nNoPlayers][4];				// array for name

	clrs();									// function call

	printf("All-Time Best Time\n\n");

	/* copies related struct data to local array */
	for(i = 0; i < nNoPlayers; i++) {
		timePB[i] = P[i].timePB;
		strcpy(name[i], P[i].name);
	}

	/* sorting algorithm */
    for (i = 0; i < nNoPlayers - 1; i++)
	{
		minIndex = i;
        for (j = i + 1; j < nNoPlayers; j++)
		{
            if (timePB[j] < timePB[minIndex]) 
			{			
				minIndex = j;
		    }
		}
		nTemp = timePB[minIndex];
		timePB[minIndex] = timePB[i]; 
		timePB[i] = nTemp;  

		strcpy(sTemp, name[minIndex]);
		strcpy(name[minIndex], name[i]); 
		strcpy(name[i], sTemp);  
	}

	/* prints */
	for(i = 0; i < 6; i++) {
		if(i < nNoPlayers) 
			printf("[Player %d] %s: %d seconds\n", i+1, name[i], timePB[i]);
		else printf("[Player %d] EMPTY\n", i+1);
		
	}
	printf("\n");						// prints new line

	getchClrs();						// function call
}

/* This function prints the 

	Statistics > All-Time Rankings > N-turn wins > respective turn

	@param Player P[] is the struct that stores player's information
	@param nNoPlayers is the number of current players
 */
void allTimeTurn(struct Player P[], int nNoPlayers){

	int nChoice;						// menu choice
	int nI = -1;						// index
	
	int nMin = 0;						// lower bound of menu choice
	int nMax = 6;						// upper bound of menu choice

	clrs();
	do
	{
		allTimeTurnMenu();				// function call

		nChoice = promptInt(nMin, nMax);// stores choice

		switch(nChoice) 
		{	/* respective calls */
			case 0: nChoice = 0; break;
			case 1: nI = 0; nTurn(P, nNoPlayers, nI); break;
			case 2: nI = 1; nTurn(P, nNoPlayers, nI); break;
			case 3: nI = 2; nTurn(P, nNoPlayers, nI); break;
			case 4: nI = 3; nTurn(P, nNoPlayers, nI); break;
			case 5: nI = 4; nTurn(P, nNoPlayers, nI); break;
			case 6: nI = 5; nTurn(P, nNoPlayers, nI); break;

			default:
				promptInvalid();		// function call 
				getchClrs();			// function call				
		}

	} while(nChoice != 0);				// until player wants to exit

	clrs();								// function call
}

/* This function prints the 

	Statistics > All-Time Rankings > N-turn wins

	@param Player P[] is the struct that stores player's information
	@param nNoPlayers is the number of current players
	@param nI is the index
*/
void nTurn(struct Player P[], int nNoPlayers, int nI){

	int i, j, minIndex;						// counter variables/index
	int nTemp;								// temp int
	char sTemp[4];							// temp string
	
	int turns[5];							// array for turns
	char name[nNoPlayers][4];				// array for name

	/* copies related struct data to local array */	
	for(i = 0; i < nNoPlayers; i++) {
		turns[i] = P[i].turns[nI];
		strcpy(name[i], P[i].name);
	}

	/* sorting algorithm */
	for (i = 0; i < nNoPlayers - 1; i++)
	{
		minIndex = i;
        for (j = i + 1; j < nNoPlayers; j++)
		{
            if (turns[j] > turns[minIndex]) 
			{			
				minIndex = j;
		    }
		}
		nTemp = turns[minIndex];
		turns[minIndex] = turns[i]; 
		turns[i] = nTemp;  

		strcpy(sTemp, name[minIndex]);
		strcpy(name[minIndex], name[i]); 
		strcpy(name[i], sTemp);  
	}

	/* printing */
	printf("\n");
	for(i = 0; i < 6; i++){ // checking
		if(turns[i] > 0 && i < nNoPlayers) { 
			printf("[Player %d] %s: %d\n", i + 1, name[i], turns[i]);
		}
		else printf("[Player %d] EMPTY\n", i+1);
	}
		
	getchClrs();				// function call
}

/* This function prints the 

	Statistics > All-Time Score Statistics > Top 3 longest win streaks

	@param Player P[] is the struct that stores player's information
	@param nNoPlayers is the number of current players
 */
void getTop3WS(struct Player P[], int nNoPlayers){

	int i, j, minIndex;						// counter variables/index
	int nTemp;								// temp int
	char sTemp[4];							// temp string
	
	int nHWS[PMAX];							// array for highest winstreak
	char name[nNoPlayers][4];				// array for name

	printf("Top 3 Longest Winstreaks\n");

	/* copies related struct data to local array */
	for(i = 0; i < nNoPlayers; i++) {
		nHWS[i] = P[i].nHWS;
		strcpy(name[i], P[i].name);
	}

	/* sorting algorithm */
    for (i = 0; i < nNoPlayers - 1; i++)
	{
		minIndex = i;
        for (j = i + 1; j < nNoPlayers; j++)
		{
            if (nHWS[j] > nHWS[minIndex]) 
			{				
				minIndex = j;
		    }
		}
		nTemp = nHWS[minIndex];
		nHWS[minIndex] = nHWS[i]; 
		nHWS[i] = nTemp;  

		strcpy(sTemp, name[minIndex]);
		strcpy(name[minIndex], name[i]); 
		strcpy(name[i], sTemp);  
	}

	/* prints */
	for(i = 0; i < 3; i++) {
		if(i < nNoPlayers) 
			printf("[Player %d] %s: %d\n", i+1, name[i], nHWS[i]);

		else printf("[Player %d] EMPTY\n", i+1);
	}
	printf("\n");	
}


