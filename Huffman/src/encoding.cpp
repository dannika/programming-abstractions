/* File: encoding.cpp
 * --------------------------------
 * Name: Dannika Thompson
 * Class: CS106B
 * SL: Ana-Maria Istrate
 *
 * This file implements the Huffman compression algorithm, which compresses
 * text data to make a file occupy a smaller number of bytes by assessing
 * character frequency.
 *
 */

#include "encoding.h"
#include "filelib.h"
#include "pqueue.h"
#include "strlib.h"

/* Prototype declarations */
void buildEncodingMap(HuffmanNode *node, Map<int, string> &encodingMap, string &encoding);
void bitWriter(obitstream &output, string const &str);
bool decodeData(ibitstream &input, HuffmanNode* encodingTree, ostream &output, HuffmanNode* current);

/*
 * Reads the input file one character at a time, adding each character to the frequency map.
 * Returns the map of characters and corresponding frequencies.
 */
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;
    while (true) {
        int charInput = input.get();
        if (charInput == -1) {
            break;
        } else if (freqTable.containsKey(charInput)) {
            freqTable[charInput] += 1;
        } else {
            freqTable[charInput] = 1;
        }
    }
    freqTable[PSEUDO_EOF] = 1;
    return freqTable;
}

/*
 * Creates a Huffman encoding tree based on the frequencies of characters in the map.
 * Returns a pointer to the root of the tree.
 */
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode*> priorityQ;
    for (int character : freqTable) {
        HuffmanNode *huffNode = new HuffmanNode(character, freqTable[character], NULL, NULL);
        priorityQ.enqueue(huffNode, freqTable[character]);
    }
    while (priorityQ.size() > 1) {
        HuffmanNode *first = priorityQ.dequeue();
        HuffmanNode *second = priorityQ.dequeue();
        HuffmanNode *joined = new HuffmanNode(NOT_A_CHAR, first->count + second->count,
                                              first, second);
        priorityQ.enqueue(joined, joined->count);
    }

    return priorityQ.dequeue();
}

/*
 * Returns a Huffman encoding map of character/binary encoding string pairs based on the
 * structure of a Huffman tree
 */
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    string encoding = "";
    buildEncodingMap(encodingTree, encodingMap, encoding);
    return encodingMap;
}

/* Populates the encoding map */
void buildEncodingMap(HuffmanNode *node, Map<int, string> &encodingMap, string &encoding) {
    if (node->isLeaf()) {
        encodingMap[node->character] = encoding;
    } else {
        string zero = encoding + "0";
        buildEncodingMap(node->zero, encodingMap, zero);
        string one = encoding + "1";
        buildEncodingMap(node->one, encodingMap, one);
    }
}

/*
 * Encodes each character of the input file to binary using the encoding map.
 * Writes the encoded binary bits to the output stream.
 */
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    while (true) {
        int charInput = input.get();
        if (charInput == -1) {
            break;
        }
        string codedData = encodingMap[charInput];
        bitWriter(output, codedData);
    }
    string eofCode = encodingMap[PSEUDO_EOF];
    bitWriter(output, eofCode);
}

/* Writes encoded bits to the output stream */
void bitWriter(obitstream &output, string const &str) {
    int len = str.length();
    for (int i = 0; i < len; i++) {
        if (str[i] == '0') {
            output.writeBit(0);
        } else if (str[i] == '1') {
            output.writeBit(1);
        }
    }
}

/* Decodes a given input based on the structure of the encoding tree */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    bool decoding = true;
    while (decoding) {
        decoding = decodeData(input, encodingTree, output, encodingTree);
    }
}

/* Reads bits from given file and walks through the encoding tree to write original contents */
bool decodeData(ibitstream &input, HuffmanNode *encodingTree, ostream &output, HuffmanNode *current) {
    if (current->isLeaf()) {
        if (current->character == PSEUDO_EOF) {
            return false;
        } else {
            output.put(current->character);
            return true;
        }
    }
    int bitInput = input.readBit();
    if (bitInput == 0) {
        return decodeData(input, encodingTree, output, current->zero);
    } else if (bitInput == 1) {
        return decodeData(input, encodingTree, output, current->one);
    }
    return false;
}

/*
 * Creates an encoding map from the input and adds the frequency table to the output as
 * a header. Input is rewinded and encoded, and the encoded message is outputted.
 */
void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode *encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    rewindStream(input);
    output << freqTable;
    encodeData(input, encodingMap, output);
    freeTree(encodingTree);
}

/*
 * Obtains the frequency table from the input stream and generates an encoding tree.
 * Decodes the remainder of the input.
 */
void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable;
    input >> freqTable;
    HuffmanNode *encodingTree = buildEncodingTree(freqTable);
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
}

/* Deletes node after exploration */
void freeTree(HuffmanNode* node) {
    if (!node->isLeaf()) {
        freeTree(node->zero);
        freeTree(node->one);
    }
    delete node;
}
