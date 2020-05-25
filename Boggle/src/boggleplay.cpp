/* File: boggleplay.cpp
 * --------------------------
 * Name: Dannika Thompson
 * SL: Ana-Maria Istrate
 *
 * This file runs a single game of Boggle. It contains all of the user interactions
 * with the player, including initializing the board, executing the player's and the
 * computer's turns, and determining a winner. THe file handles all calls to the Boggle
 * class and interacts with the player on both the console and the GUI.
 */

#include "lexicon.h"
#include "simpio.h"
#include "console.h"
#include "Boggle.h"
#include "bogglegui.h"

/* Constant variables */
const int SIZE = 4;
const int NUM_CUBES = 16;

/* Prototype declarations */
Boggle getNewBoggle(Lexicon &dictionary);
string fillCustomBoard();
void humanTurn(Boggle &boggle, string word);
void displayHumanStats(Boggle &boggle, string word, string situation);
void compTurn(Boggle &boggle);
void displayCompStats(Boggle &boggle, Set<string> &foundByComp);
void declareWinner(Boggle &boggle);

/* Runs a single game of Boggle */
void playOneGame(Lexicon& dictionary) {
    BoggleGUI::initialize(SIZE, SIZE);
    Boggle boggle = getNewBoggle(dictionary);
    BoggleGUI::labelAllCubes(boggle.getCustom());

    displayHumanStats(boggle, "", "turn");
    string word = getLine("Type a word (or Enter to stop): ");
    humanTurn(boggle, word);
    compTurn(boggle);
    declareWinner(boggle);
}

/* Generates a new Boggle board for the one game */
Boggle getNewBoggle(Lexicon &dictionary) {
    bool randBoard = getYesOrNo("Do you want to generate a random board? ");
    if (randBoard){
        Boggle newBoggle(dictionary, "");
        return newBoggle;
    } else {
        string customLetters = fillCustomBoard();
        Boggle newBoggle(dictionary, customLetters);
        return newBoggle;
    }
}

/* Generates a custom Boggle board based on the player's input string */
string fillCustomBoard() {
    string userInput;
    bool invalid = true;
    while (invalid) {
        userInput = toUpperCase(getLine("Type the 16 letters to appear on the board: "));
        if (userInput.size() == NUM_CUBES) {
            int num = userInput.length();
            for (int i = 0; i < num; i++) {
                if (!(userInput[i] <= 'Z' && userInput[i] >= 'A')) {
                    invalid = true;
                    break;
                }
                invalid = false;
            }
        }
        if (invalid) {
            cout << "That is not a valid 16-letter board string. Try again. " << endl;
        }
    }
    return userInput;
}

/* Executes the player's turn */
void humanTurn(Boggle &boggle, string word) {
    while (word != "") {
        BoggleGUI::clearHighlighting();
        word = toUpperCase(word);
        if (boggle.checkWord(word)) {
            if (boggle.humanWordSearch(word)) {
                clearConsole();
                BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
                displayHumanStats(boggle, word, "found");
            } else {
                clearConsole();
                displayHumanStats(boggle, word, "notFound");
            }
        } else {
            displayHumanStats(boggle, word, "invalid");
        }
        word = getLine("Type a word (or Enter to stop): ");
    }
}

/* Prints status messages to the player; prints player word and score after correct guesses */
void displayHumanStats(Boggle &boggle, string word, string situation) {
    clearConsole();
    if (situation == "turn") {
        cout << "It's your turn!" << endl;
        BoggleGUI::setStatusMessage("It's your turn!");
    } else if (situation == "found") {
        cout << "You found a new word! \"" << word << "\"" << endl;
        BoggleGUI::setStatusMessage("You found a new word! \"" + word + "\"");
    } else if (situation == "notFound") {
        cout << "That word can't be formed on this board." << endl;
        BoggleGUI::setStatusMessage("That word can't be found on this board.");
    } else {
        cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
        BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
    }
    cout << boggle;
    cout << endl;
    cout << "Your words (" << boggle.numFound() << "): " << boggle.getFoundWords() << endl;
    cout << "Your score: " << boggle.getScoreHuman() << endl;
    BoggleGUI::setScore(boggle.getScoreHuman(), BoggleGUI::HUMAN);
}

/* Executes the computer's turn, finding all solutions unfound by the player */
void compTurn(Boggle &boggle) {
    Set<string> foundByComp = boggle.computerWordSearch();
    displayCompStats(boggle, foundByComp);
}

/* Prints computer's words and score to the console */
void displayCompStats(Boggle &boggle, Set<string> &foundByComp) {
    cout << endl << "It's my turn!" << endl;
    cout << "My words: (" << boggle.numCPUWords() << "): " << foundByComp << endl;
    cout << "My score: " << boggle.getScoreComputer() << endl;
    for (string word : foundByComp) {
        BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
    }
    BoggleGUI::setScore(boggle.getScoreComputer(), BoggleGUI::COMPUTER);
}

/* Compares player and computer scores to determine winner */
void declareWinner(Boggle &boggle) {
    int finalCompScore = boggle.getScoreComputer();
    int finalHumanScore = boggle.getScoreHuman();

    if (finalCompScore > finalHumanScore) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    } else {
        cout << "WOW, you defeated me! Congratulations!" << endl;
        BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations!");
    }
}
