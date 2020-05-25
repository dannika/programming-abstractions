/*
 * File: grammarsolver.cpp
 * --------------------------
 * Name: Dannika Thompson
 * Section leader: Ana-Maria Istrate
 *
 * This file accepts a file representing a language grammar, a symbol to randomly generate, and a
 * integer of the number of times to generate it. The program returns a vector of strings whose
 * contents are printed in the main file as randomly generaged examples of the symbol from the
 * given grammar.
 */

#include "grammarsolver.h"
#include "strlib.h"
#include "map.h"
#include "vector.h"
#include "random.h"

using namespace std;

/*
 * Reads the input file containing a grammar in Backus-Naur Form and parses its contents into
 * a map.
 */
void readFile(istream& input, Map<string, Vector<string>>& grammar) {
    string line;
    while (getline(input, line)) {
        Vector<string> nonTermRulePairs = stringSplit(line, "::=");
        string nonTerm = nonTermRulePairs[0];
        string rules = nonTermRulePairs[1];
        Vector<string> tokens = stringSplit(rules, "|");

        if (grammar.containsKey(nonTerm)) {
            throw ("Error: Grammar contains more than one line for non-terminal.");
        } else {
            grammar.put(nonTerm, tokens);
        }
    }
}

/* Randomly generates elements of the grammar. */
string randomGeneration(string symbol, Map<string, Vector<string>>& grammar, string& output) {
    if (!grammar.containsKey(symbol)) {
        output += " " + symbol;
    } else {
        Vector<string> rule = grammar[symbol];
        int rulesPerSymbol = rule.size();
        string randRule = rule.get(randomInteger(0, rulesPerSymbol - 1));
        Vector<string> nonTerms = stringSplit(randRule, " ");
        for (int i = 0; i < nonTerms.size(); i++) {
            string newSymbol = nonTerms[i];
            randomGeneration(newSymbol, grammar, output);
        }
    }
    return output;
}

/**
 * Generates grammar for a given symbol a certain number of times given
 * a BNF input file.
 *
 * This will be called by grammarmain.cpp.
 *
 * @param input - Input stream of BNF file.
 * @param symbol - Symbol to generate
 * @param times - Number of times grammar is generated
 * @return Vector of strings of size times with random generations of symbol
 */
Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Map<string, Vector<string>> grammar;
    readFile(input, grammar);
    Vector<string> listOfOutputs;

    if (times < 1) {
        return {};
    } else {
        for (int i = 0; i < times; i++) {
            string output;
            output = randomGeneration(symbol, grammar, output);
            listOfOutputs.add(output);
        }
    }

    return listOfOutputs;
}
