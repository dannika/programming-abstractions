/* File: Boggle.cpp
 * --------------------------
 * Name: Dannika Thompson
 * SL: Ana-Maria Istrate
 *
 * This file contains all of the logic needed to execute a game of Boggle.
 * All of the methods of the Boggle class are implemented in this file
 * along with the recursive function used during the player's turn and the
 * recursive backtracking function used by the computer.
 */

#include "Boggle.h"
#include "bogglegui.h"
#include "grid.h"
#include "strlib.h"
#include "shuffle.h"

/* Constant variables */
const int SIZE = 4;
const int DELAY = 100;

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

/* Constructs the Boggle board */
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    dict = dictionary;
    humanScore = 0;
    compScore = 0;
    found = {};
    board.resize(SIZE, SIZE);
    text = boardText;

    if (text == "") {
        getRandomBoard();
    } else {
        getCustomBoard();
    }
}

/* Shuffles the cubes to generate a random board */
void Boggle::getRandomBoard() {
    shuffle(CUBES, SIZE * SIZE);
    for (int i = 0; i < (SIZE * SIZE); i++) {
        string current = shuffle(CUBES[i]);
        board[i / SIZE][i % SIZE] = current[0];
        text += current[0];
    }
}

/* Generates a board according to the player's custom string input */
void Boggle::getCustomBoard() {
    int textLength = text.length();
    for (int i = 0; i < textLength; i++) {
        board[i / SIZE][i % SIZE] = toUpperCase(charToString(text[i]));
    }
}

/* Returns character in Boggle board at given row and column */
char Boggle::getLetter(int row, int col) {
    return stringToChar(board[row][col]);
}

/* Checks validity of user's input word */
bool Boggle::checkWord(string word) {
    word = toUpperCase(word);
    if (!dict.contains(toLowerCase(word)) || word.length() < 4 || found.contains(word)) {
        return false;
    }
    return true;
}

/* Checks the validity of the player's guess */
bool Boggle::humanWordSearch(string word) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            word = toUpperCase(word);
            Grid<bool> visited(SIZE, SIZE, false);
            if (humanSearchHelper(row, col, word, visited)) {
                found.add(word);
                humanScore += word.length() - 3;
                return true;
            }
        }
    }
    return false;
}

/* Searches the board to determine if the player's guess can be formed from the given letters */
bool Boggle::humanSearchHelper(int row, int col, string word, Grid<bool> &visited) {
    BoggleGUI::setHighlighted(row, col, true);
    if (!board.inBounds(row, col)) {
        return false;
    } else if (visited.inBounds(row, col) && word.length() == 1 && board[row][col] == word) {
        return true;
    } else {
        if (board[row][col] == charToString(word[0])) {
            visited[row][col] = true;
            BoggleGUI::setAnimationDelay(DELAY);
            for (int r = -1; r <= 1; r++) {
                for (int c = -1; c <= 1; c++) {
                    if (visited.inBounds(row + r, col + c) && !visited[row + r][col + c]
                            && !(r == 0 && c == 0)) {
                        if (humanSearchHelper(row + r, col + c, word.substr(1), visited)) {
                            return true;
                        }
                    }
                }
            }
            visited[row][col] = false;
        }
        BoggleGUI::setHighlighted(row, col, false);
        return false;
    }
}

/* Returns the player's current score */
int Boggle::getScoreHuman() {
    return humanScore;
}

/* Returns the set of words the player has found on the board */
Set<string> Boggle::getFoundWords() {
    return found;
}

/* Returns the number of words the player has found */
int Boggle::numFound() {
    return found.size();
}

/* Returns the set of all words that can be made on the board that the player did not already find */
Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            Grid<bool> visited(SIZE, SIZE, false);
            compSearchHelper(row, col, "", result, visited);
        }
    }
    numCompFound = result.size();
    return result;
}

/*
 * Searches the board to find all words that can be formed and
 * have not already been found by the player
 */
void Boggle::compSearchHelper(int row, int col, string word,
                              Set<string> &result, Grid<bool> &visited) {
    if (!board.inBounds(row, col) || !dict.containsPrefix(word)) {
        return;
    } else if (dict.contains(word) && word.length() >= 4 && !result.contains(word)
            && !found.contains(word)) {
        compScore += word.length() - 3;
        result.add(word);
    } else {
        word = word + board[row][col];
        visited[row][col] = true;
        for (int r = -1; r <= 1; r++) {
            for (int c = -1; c <= 1; c++) {
                if (visited.inBounds(row + r, col + c) && !visited[row + r][col + c]) {
                    compSearchHelper(row + r, col + c, word, result, visited);
                }
            }
        }
        visited[row][col] = false;
    }
}

/* Returns the CPU's current score */
int Boggle::getScoreComputer() {
    return compScore;
}

/* Returns number of words the CPU has found */
int Boggle::numCPUWords() {
    return numCompFound;
}

/* Returns the string of letters for the player's custom board */
string Boggle::getCustom() {
    return text;
}

/* Prints Boggle object to the console */
ostream& operator<<(ostream& out, Boggle& boggle) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            out << boggle.board[row][col];
        }
        out << endl;
    }
    return out;
}
