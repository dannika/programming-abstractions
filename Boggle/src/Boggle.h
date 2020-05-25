/* File: Boggle.h
 * --------------------------
 * Name: Dannika Thompson
 * SL: Ana-Maria Istrate
 *
 * This file contains all of the methods and private variables of
 * the Boggle class.
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"

using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    int getScoreHuman();
    Set<string> getFoundWords();
    int numFound();
    Set<string> computerWordSearch();
    int getScoreComputer();
    int numCPUWords();
    string getCustom();
    friend ostream& operator<<(ostream& out, Boggle& boggle);

private:
    Grid<string> board;
    string text;
    void getRandomBoard();
    void getCustomBoard();
    Set<string> found;
    Lexicon dict;
    bool humanSearchHelper(int row, int col, string word, Grid<bool> &visited);
    void compSearchHelper(int row, int col, string word, Set<string> &result, Grid<bool> &visited);
    int humanScore;
    int compScore;
    int numCompFound;
};

#endif // _boggle_h
