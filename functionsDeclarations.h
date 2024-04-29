#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_TRIES 6
#define PMAX 20

struct Player {
	char name[4];						// player's 3-letter code name
	int nHWS;							// highest winstreak
	int nCWS;							// current winstreak
	int timePB;							// personal best time
	int timePC;							// current time
    int turns[6];						// for frequency distribution
};

void promptGetch();
void clrs();
void getchClrs();

/* Core Functions */
void Wordle(char cGuess[], char cWord[]);
void pickWord(char *cWord, char cFile[]);
void askSecret (char *cWord, char cFile[]);
void askGuess (char *cGuess, char cFile[]);
void checkGuess(char cWord[], int mode, char cFile[]);
void checkGuessCl(char cWord[], int *nNoPlayers, struct Player P[], char cFile[]);
int getWordCount(char cFile[]);
int checkDict(char cCheck[], char cFile[]);
void resultH2H(int P1, int P2, int timeP1, int timeP2);

/* Menu */
void help();
void mainMenu();
void statsMenu();
void allTimeTurnMenu();
void allTimeRMenu();

/* Help */
void helpIntro();
void helpClassic();
void helpClassicMI();
void helpH2H();
void helpZen(); 
void helpStats();

/* Display prompts */
void promptInvalid();
void promptNot5();

/* Inputs */
int getInt();
int promptOneZero();
int promptInt(int nMin, int nMax);


/* Helpers */
char *Capitalize(char *str);

/* Statistics Core */
void askName(char *name);
void saveRecord(int win, char name[], int timeP0, int nCTries, int nCWS, int *nNoPlayers, struct Player P[]);
void addNew(char name[], int timeP0, int nCTries, int nCWS, int *nNoPlayers, struct Player P[]);
void updateExisting(int nI, int timeP0, int nCTries, struct Player P[]);
void writeFile(int *nNoPlayers, struct Player P[]);
void readFile(int *nNoPlayers, struct Player P[]);
void resetData(struct Player P[]);
int searchPlayer(char name[], struct Player P[]);

/* Statistics */
void playerStats(struct Player P[PMAX]);
void printFreqDist(struct Player P[], int nNoPlayers);
void allTimeSS(struct Player P[], int nNoPlayers);
void allTimeR(struct Player P[], int nNoPlayers);
void allTimeWS(struct Player P[], int nNoPlayers);
void allTimeBT(struct Player P[], int nNoPlayers);
void allTimeTurn(struct Player P[], int nNoPlayers);
void nTurn(struct Player P[], int nNoPlayers, int nI);
void getTop3WS(struct Player P[], int nNoPlayers);



