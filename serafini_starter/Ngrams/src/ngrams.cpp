/*
 * Name: Dannika Thompson
 * Class: CS106B
 * Section Leader: Ana-Maria Istrate
 * File: ngrams.cpp
 *
 * This program constructs an n-gram, a structure that randomly generates new text that
 * sounds as if it came from the original author of the input file. Given an input file and
 * a value for N, with which a number of words will be clustered, a random string of text will
 * be created according to that N value and the random window of the original text from which
 * words will be chosen.
 *
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "map.h"
#include "random.h"
#include "simpio.h"
#include "vector.h"

using namespace std;

/* Declaration of prototypes. */
void printIntro();
void runNGrams();
int getNValue();
int getNumWords();
void readFile(Map<Vector<string>, Vector<string>> &nGrams, ifstream &fileIn, int nValue);
void wrapPrefixes(Vector<string> first, Map<Vector<string>,
                  Vector<string>> &nGrams, Vector<string> &window);
void printText(Map<Vector<string>, Vector<string>> &nGrams, int numRand);

int main() {
    printIntro();
    runNGrams();

    cout << "Exiting." << endl;
    return 0;
}

/* Prints the welcome message and the instructions for the NGram program. */
void printIntro() {
    cout << "Welcome to CS 106B Random Writer ('N-grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl << endl;
}

/*
 * Obtains a file and an N value from the user, ensuring that both are valid inputs. The
 * user is then prompted for a random number of words to be generated (or 0 to end the
 * program). If the sentinel is not entered, the file will be read, and text will be
 * generated.
 */
void runNGrams() {
    Map<Vector<string>,Vector<string>> nGrams;
    string fileName = promptUserForFile("Input file name? ");
    ifstream fileIn;
    openFile(fileIn, fileName);

    int nValue = getNValue();
    cout << endl;

    while (true) {
        int numRand = getNumWords();
        if (numRand == 0) {
            break;
        } else {
            readFile(nGrams, fileIn, nValue);
            printText(nGrams, numRand);
        }
    }
    fileIn.close();
}

/* Gets/returns an N value from the user, ensuring that it is valid per the instructions. */
int getNValue() {
    int nValue = getInteger("Value of N? ");
    while (nValue < 2) {
        cout << "N must be 2 or greater." << endl;
        nValue = getInteger("Value of N? ");
    }
    return nValue;
}

/* Gets/returns a random number from the user, ensuring that it is valid per the instructions. */
int getNumWords() {
    int numRand = getInteger("# of random words to generate (0 to quit)? ");
    while (numRand != 0 && numRand < 4) {
        cout << "Must be at least 4 words." << endl << endl;
        numRand = getInteger("# of random words to generate (0 to quit)? ");
    }
    return numRand;
}

/*
 * Reads each word of the file one word at a time, eventually adding each word to the map
 * according to the N value given by the user.
 */
void readFile(Map<Vector<string>, Vector<string>> &nGrams, ifstream &fileIn, int nValue) {
    Vector<string> window;
    Vector<string> first;
    string lineIn;

    while(window.size() < nValue - 1) {
        fileIn >> lineIn;
        window.add(lineIn);
    }
    first = window;
    while (true) {
        fileIn >> lineIn;
        if (fileIn.eof()) {
            break;
        }
        nGrams[window].add(lineIn);
        int size = window.size() - 1;
        for (int i = 0; i < size; i++) {
            window[i] = window[i + 1];
            window[size] = lineIn;
        }
    }
    wrapPrefixes(first, nGrams, window);
}

/* Wraps the map such that the last prefixes connect to the first words in the file. */
void wrapPrefixes(Vector<string> first, Map<Vector<string>,
                  Vector<string>> &nGrams, Vector<string> &window) {
    int fSize = first.size();
    int wSize = window.size() - 1;

    for (int i = 0; i < fSize; i++) {
        nGrams[window].add(first[i]);

        for (int i = 0; i < wSize; i++) {
            window[i] = window[i + 1];
            window[wSize] = first[i];
        }
    }
}

/* Chooses a random prefix from the map and prints the randomly generated text to the console. */
void printText(Map<Vector<string>, Vector<string>> &nGrams, int numRand) {
    int randomKeyInt = randomInteger(0, nGrams.size() - 1);
    Vector<string> window = nGrams.keys()[randomKeyInt];
    cout << "... ";

    for (int n = 0; n < numRand; n++) {
        cout << window.get(0) << " ";

        int rand = randomInteger(0, nGrams[window].size() - 1);
        string randWord = nGrams[window][rand];
        int size = window.size() - 1;

        for (int w = 0; w < size; w++) {
            window[w] = window[w + 1];
            window[size] = randWord;
        }
    }
    cout << "..." << endl << endl;
}
