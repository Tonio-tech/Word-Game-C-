/*
File Name: proj1.cpp
Assignent Name: CMSC 202 Project 1 - Guess Word
Author's Name: Toni Olafunmiloye
Date: Feburary 11, 2021
Section: 53
Email: oolafun1@umbc.edu
Description: Project 1 is a guessing word program that implements the use of functions.
*/


#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstring>
using namespace std;


// Function Prototypes

//opens the file and counts how many words are in it
int openFile();  

// Display Puzzle – Shows the word being guessed – if the letter hasn’t been guessed then it shows an underscore _
void displayPuzzle(int numOfWords, string listOfWords[]);

// Make List - make a list of the words from the imported file
void makeList(string listOfWords[], int numOfWords);

//Chooses the word that will be guessed 
string chooseWord(int numOfWords, string listOfWords[]);

// Check Letter - basically checks if the letter is in the word and returns the letters remaining
int checkLetter1(char guessedLetter, char lettersList[], string guessWord, int lettersRemain);

//Check Letter 2 - gets the number of wrong guesses left
int checkLetter2(char guessedLetter, string guessWord, int numWrongGuesses);

// When the user either guesses all the letters or uses up all the wrong guesses, the game ends
void endGame(string guessWord, int lettersRemain, int numWrongGuesses, string listOfWords[], int numOfWords);


// CONSTANTS
const char GUESSED_LETTERS_LIST[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
const int ALPHABET = 26;
const int SIX = 6;


int main () { 
    int numOfWords = 0; 
    srand(time(NULL));

    // starts by opening the file, counting the number of words
    cout << endl;
    cout << "Welcome to UMBC Word Guess!" << endl;
    numOfWords = openFile();

    // then we make a list of words according to the number of words found in the file
    string listOfWords[numOfWords] = {};
    makeList(listOfWords, numOfWords);

    // then we pick a word from the list and start the game
    displayPuzzle(numOfWords, listOfWords);

    return 0;
}


/*
Name: Open File
Pre-Condition: None
Post-Condition: Using a loop it counts how many words are in the file and returns the number, then closes the file
*/
int openFile() {
    fstream wordFile;
    string word;
    int numOfWords = 0; 

    wordFile.open("words.txt"); 

    if(wordFile.is_open()) {
        cout << "Your file was imported." << endl;
        while(wordFile >> word) {
            numOfWords += 1;
            
        }
        cout << numOfWords << " words imported." << endl;
        
    } else {
        cout << "Unable to open file" << endl; 
    }  

    wordFile.close();
    
    return numOfWords;

}



/*
Name: Make List
Pre-Condition: Takes in an empty list called listofwords and the number of words in the file that was prevoiusly recorded
Post-Condition: Using a loop it adds all the words from the wordfile to the list, updating it, and closes the file.
*/
void makeList(string listOfWords[], int numOfWords) { 
    fstream wordFile;

    wordFile.open("words.txt");

    for(int i = 0; i < numOfWords; i++) {
        wordFile >> listOfWords[i];
    }
    wordFile.close();
}



/*
Name: Choose Word
Pre-Condition: Takes in he number of words from the file and the new updated list of words
Post-Condition: Using rand(), the function picks a random word from the list and returns it as the string variable guessWord
*/
string chooseWord(int numOfWords, string listOfWords[]) {
    int randNum = rand() % numOfWords + 0;
    string guessWord = listOfWords[randNum];
    return guessWord;

}
    


/*
Name: Display Puzzle
Pre-Condition: Takes in the number of words and the list of words. Also calls chooseWord to pick a word. 
Post-Condition: Updates the letters remaining and the number of wrong guesses after each guessed letter, and outputs it
*/
void displayPuzzle(int numOfWords, string listOfWords[]) {
    // Chooses a word from the chooseWord function
    string guessWord = chooseWord(numOfWords, listOfWords);  

    int wordLength = guessWord.length();
    int lettersRemain = guessWord.length();
    int numWrongGuesses = SIX;

    char guessedLetter;
    char lettersList[ALPHABET] = {};

    bool foundLetter = false;
    bool inList = false;
    bool lowerCase = false;
    

    cout << "I am thinking of a word that has " << wordLength << " letters." << endl;


    // while the number of wrong guesses is not 0, keep asking for a letter and checking
    do {  
        for(int i = 0; i < wordLength; i++) {
            foundLetter = false;
            for(int j = 0; j < ALPHABET; j++) {
                if(guessWord[i] == lettersList[j]) {
                    cout << guessWord[i];
                    foundLetter = true;
                    j = ALPHABET + 1;
                }
            }
            if(foundLetter == false) {
                cout << "_ ";
            }
        } 
        cout << endl;
        cout << endl;
        cout << "What letter would you like to guess? " << endl;
        cin >> guessedLetter;
        cin.ignore(256, '\n');

        // need to make sure the inputted letter is a letter, if not reprompt the user
        while(lowerCase == false) { 
            for(int i = 0; i < ALPHABET; i++) {
                if(guessedLetter == GUESSED_LETTERS_LIST[i]) {
                    lowerCase = true;
                    i = ALPHABET + 1;
                }
            }
            if(lowerCase == false) {
                cout << "Please enter a lowercase letter: " << endl;
                cin >> guessedLetter;
            }
        }
        lowerCase = false;

        // If user has already guessed the letter, decrease number of wrong guesses
        for( int i = 0; i < ALPHABET; i++) { 
            if(guessedLetter == lettersList[i]) {
                inList = true;
                numWrongGuesses -= 1;
                cout << "You have already guessed this letter" << endl;
                cout << "You have " << numWrongGuesses << " bad guesses left." << endl;
                i = ALPHABET + 1;
            }
        }

        if(inList == false) {
            lettersRemain = checkLetter1(guessedLetter, lettersList, guessWord, lettersRemain);
            numWrongGuesses = checkLetter2(guessedLetter, guessWord, numWrongGuesses);
        }

        inList = false;

        cout << lettersRemain << " letters remain." << endl;

    } while(numWrongGuesses != 0 && lettersRemain > 0);

    endGame(guessWord, lettersRemain, numWrongGuesses, listOfWords, numOfWords); // once the user wins or loses the game

    cout << endl;
}



/*
Name: Check Letter 1
Pre-Condition: Takes in the guessed letter, an empty list cllaed the letter list (to store all the guessed letters), and 
                word the user is trying to guess, and the letters that remain in the word. This
                function check the guessed letter to see if it in in the word

Post-Condition: If the guessed letter is in the word, the function returns an updates number for the letters remaining and 
                puts the letter into the letters list. 
*/
int checkLetter1(char guessedLetter, char lettersList[], string guessWord, int lettersRemain) {
    int wordLength = guessWord.length();

    for(int j = 0; j < wordLength; j++) {
        if(guessedLetter == guessWord[j]) {
            lettersRemain -= 1;
            for(int k = 0; k < ALPHABET; k++) {
                if (lettersList[k] == 0) {
                    lettersList[k] = guessedLetter;
                    k = ALPHABET + 1;
                }
            }
        } 
    }
    return lettersRemain;
}



/*
Name: Check Letter 2
Pre-Condition: Takes in the guessed letter, the word the user is trying to guess and the number of wrong guesses.
                Declares an int varibale names index used to find the index of the letter in the word.
Post-Condition: If the letter is not in any index of the word, then the number of wrong guesses decreases. The function
                then returns the updated number of wrong guesses the user has left.
*/
int checkLetter2(char guessedLetter, string guessWord, int numWrongGuesses) {  
    int wordLength = guessWord.length();
    int wordIndex = 0;

    for(int i = 0; i < wordLength; i++) {
        if(guessedLetter == guessWord[i]) {
            wordIndex = i;
            i = wordLength + 1;
        }
    }

    if(guessedLetter != guessWord[wordIndex]) {
        cout << "There are no " << guessedLetter << " in the puzzle." << endl;
        numWrongGuesses -= 1;
        cout << "You have " << numWrongGuesses << " bad guesses left." << endl;
        return numWrongGuesses;
        
    }

    return numWrongGuesses;
}   



/*
Name: End Game
Pre-Condition: Takes in all the information, the guessed word, letters remaining, number of wrong guesses, 
                list of words, and number of words. Declares a string varibale called play again
Post-Condition: Using the letters remaining, the function determines whther the player has won or lost. 
                If the user answers yes to play again, then it calls the display puzzle function and restarts the game.
                If the user says no, the program ends
*/
void endGame(string guessWord, int lettersRemain, int numWrongGuesses, string listOfWords[], int numOfWords) {
    string playAgain;

    cout << endl;
    cout << "The word was " << guessWord << "!" << endl;

    if(lettersRemain == 0) {
        cout << "You won!" << endl;
    } else {
        cout << "You lost!" << endl;
    }

    cout << endl;

    cout << "GAME OVER. Would you like to play again? (yes or no)" << endl;
    cin >> playAgain;

    while(playAgain != "yes" && playAgain != "no") {
        cout << "Would you like to play again? (yes or no)" << endl;
        cin >> playAgain;
    }

    if(playAgain == "yes") {
        cout << endl;
        cout << "Alright! Here we go!" << endl;
        displayPuzzle(numOfWords, listOfWords);
    } else {
        cout << "Ok! Thank you for playing! Goodbye!" << endl;
    }
}



