/*
 * Name: Dannika Thompson
 * Class: CS106B
 * Section Leader: Ana-Maria Istrate
 * File: wordladder.cpp
 *
 * Given two different English words of the same length, this program generates a
 * "word ladder" between the two words - that is, it generates a list of words that
 * connect the two inputs, changing one letter at a time with the constraint that
 * each step of the sequence of letters must still for a valid word. This particular
 * algorithm ensures that the path taken is the shortest possible ladder.
 *
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "lexicon.h"
#include "strlib.h"
#include "vector.h"
#include "queue.h"

using namespace std;

/* Declaration of prototypes. */
void printIntro();
void runWordLadder();
bool checkInputs(string word1, string word2, Lexicon lex);
int buildLadder(string word1, string word2, Lexicon lex);
void printLadder(string word1, string word2, Stack<string> current);

int main() {
    printIntro();
    runWordLadder();

    cout << "Have a nice day." << endl;
    return 0;
}

/* Prints a welcome message along with the rules of Word Ladder.*/
void printIntro() {
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the" << endl;
    cout << "first into the second by changing one letter at a time." << endl << endl;
}

/*
 * Builds a lexicon from a dictionary input. Prompts user for two words to form the beginning
 * and end of the ladder. If the inputs are empty at any point during runtime, the program ends.
 * Otherwise, checkInputs() is called to determine if the two words are valid inputs according to
 * the rules of Word Ladder. If the two words pass that check, they are passed as arguments in
 * buildLadder(), which will construct the word ladder between them.
 *
 */
void runWordLadder() {
    string fileName = promptUserForFile("Dictionary file name? ");
    cout << endl;
    Lexicon lex(fileName);

    while (true) {
        string word1 = toLowerCase(getLine("Word #1 (or Enter to quit): "));
        if (word1.empty()) {
            break;
        }
        string word2 = toLowerCase(getLine("Word #2 (or Enter to quit): "));
        if (word2.empty()) {
            break;
        }

        if (checkInputs(word1, word2, lex)) {
            buildLadder(word1, word2, lex);
            cout << endl;
        }
    }
}

/*
 * Taking the two user inputs and the lexicon as parameters, this method checks the
 * validity of the user's two words. If they are two different words of the same length
 * and can both be found in the lexicon, the inputs are valid, and the method returns
 * true. Otherwise, if any of these conditions fail, it returns false.
 *
 */
bool checkInputs(string word1, string word2, Lexicon lex) {
    if (word1.length() != word2.length()) {
        cout << "The two words must be the same length." << endl << endl;
        return false;
    } else if (word1 == word2) {
        cout << "The two words must be different." << endl << endl;
        return false;
    } else if (!lex.contains(word1) || !lex.contains(word2)) {
        cout << "The two words must be found in the dictionary." << endl << endl;
        return false;
    }
    return true;
}

/*
 * Generates the word ladder between the two inputted words. The two words and the lexicon are
 * passed as parameters. If there exists a ladder between them, it is printed; otherwise, a
 * message is displayed indicating that no ladder was found.
 *
 */
int buildLadder(string word1, string word2, Lexicon lex) {
    Queue<Stack<string>> ladder;
    Stack<string> firstWord;
    Lexicon usedWords;

    firstWord.push(word1);
    usedWords.add(word1);
    ladder.enqueue(firstWord);

    while (!ladder.isEmpty()) {
        Stack<string> current = ladder.dequeue();
        if (current.peek() == word2) {
            printLadder(word1, word2, current);
            return 1;
        } else {
            int len = word1.length();
            for (int l = 0; l < len; l++) {
                for (char letter = 'a'; letter <= 'z'; letter++) {
                    string newWord = current.peek();
                    newWord[l] = letter;

                    if (lex.contains(newWord) && !usedWords.contains(newWord)) {
                        Stack<string> next = current;
                        next.push(newWord);
                        usedWords.add(newWord);
                        ladder.enqueue(next);
                    }
                }
            }
        }
    }
    if (ladder.isEmpty()) {
        cout << "No word ladder found from " << word2 << " back to " << word1 << "." << endl;
    }
    return 0;
}

/* Prints the word ladder to the console. */
void printLadder(string word1, string word2, Stack<string> current) {
    cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
    while (!current.isEmpty()) {
        string word = current.pop();
        cout << word << " ";
    }
    cout << endl;
}
