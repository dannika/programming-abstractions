/*
 * Name: Dannika Thompson
 * Class: CS106B
 * Section Leader: Ana-Maria Istrate
 * File: life.cpp
 *
 * Executes the Game of Life program, which simulates the birth and death of cells as the progress
 * through generations. Each advancement in generation is bound by a set of rules:
 *  - A cell with 1 or fewer neighbors dies.
 *  - Locations with 2 neighbors remain stable.
 *  - Locations with 3 neighbors will create life.
 *  - A cell with 4 or more neighbors dies.
 *
 * The user enters a file, and interactively selects whether they want to animate, tick, or quit
 * the simulation.
 *
 * EXTENSION:
 * -Random world generation. If the user enters "random" into the file prompt, a random world
 *  is generated. The dimensions of this world can range from 1x1 to 50x50, and each cell is
 *  randomly assigned to be live or dead (20% probability of being live).
 *  (Method: getRandomGrid(grid), lines 333-358)
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"
using namespace std;

/* Declaration of prototypes. */
void printIntro();
void readFile(string fileName, Grid<bool> &grid);
void displayGrid(Grid<bool> &grid);
void updateTempGrid(Grid<bool> grid, Grid<bool> &temp, int row, int col, int count);
void countLiveNeighbors(Grid<bool> &grid, Grid<bool> &temp, string wrap);
void getNextGeneration(Grid<bool> &grid, string wrap);
void animateGenerations(Grid<bool> &grid, int numFrames, string wrap);
void getRandomGrid(Grid<bool> &grid);

/* Method: main() */
/**
 * Executes the program. The rules of the Game of Life are printed to the console. The
 * user is prompted for a text file and for their choice of wrapped or unwrapped simulation
 * (inputs are checked for validity, and the user will continue to be prompted until a valid
 * input is received). The inputted text file is parsed, and a grid is constructed representing
 * the file's information.
 *
 * While the program is running, the user is repetitively prompted for their choice of (a)nimation,
 * (t)icking, or (q)uitting (inputs, although not case-sensitive, are checked for validity, and
 * the user is prompted until a valid input is received).
 *
 * If the user selects "animate", they are prompted for an integer of frames, and that number of
 * life cycle progresses on the console, with a delay between each frame. If they select "tick,"
 * only the next lift cycle is displayed. If the user selects "quit," the program exits the while
 * loop and the game ends.
 *
 * @return 0, which ends the main method
 */
int main() {
    printIntro();
    Grid<bool> grid;

    string fileName = getLine("Grid input file name? ");
    while (!fileExists(fileName) && fileName != "random") {
        cout << "Unable to open file. Try again. " << endl;
        fileName = getLine("Grid input file name? ");
    }

    string wrap = toLowerCase(getLine("Should the simulation wrap around the grid (y/n)? "));
    while (wrap != "y" && wrap != "n") {
        cout << "Invalid choice. Try again. " << endl;
        wrap = toLowerCase(getLine("Should the simulation wrap around the grid (y/n)? "));
    }

    if (fileName == "random") {
        getRandomGrid(grid);
    } else {
        readFile(fileName, grid);
    }

    displayGrid(grid);

    while (true) {
        string choice = toLowerCase(getLine("a)nimate, t)ick, q)uit? "));
        while (choice != "a" && choice != "t" && choice != "q") {
            cout << "Invalid choice. Try again. " << endl;
            choice = toLowerCase(getLine("a)nimate, t)ick, q)uit? "));
        }

        if (choice == "a") {
            int numFrames = getInteger("How many frames? ");
            animateGenerations(grid, numFrames, wrap);
        } else if (choice == "t") {
            getNextGeneration(grid, wrap);
            displayGrid(grid);
        } else if (choice == "q") {
            break;
        }
    }

    cout << "Have a nice Life!" << endl;
    return 0;
}

/* Method: printIntro() */
/**
 * Prints the introductory sequence and the rules of the Game of Life to the console.
 * The rules of each life cycle are as follows:
 *  - A cell with 1 or fewer neighbors dies
 *  - Locations with 2 neighbors remain stable
 *  - Locations with 3 neighbors will create life
 *  - A cell with 4 or more neighbors dies
 */
void printIntro() {
    cout << "Welcome to the CS 106B Game of Life," << endl;
    cout << "a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells (X) live and die by the following rules:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." << endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl;
    cout << endl;
}

/* Method: readFile(fileName, grid) */
/**
 * Reads the text file, extrapolates its data, and builds a corresponding grid. The first
 * two lines of the file contain the number of rows and columns, respectively, of the grid,
 * and when converted to integers, these numbers are set to the dimensions of the grid.
 *
 * Each of the remaining rows of the file are iterated through (up to the number of total rows
 * as illustrated by the first number in the file). For each "live" ("X") or dead ("-") cell in
 * each column of each row, the grid's corresponding coordinate is assigned that value, ultimately
 * constructing a grid representation of the array depicted in the file.
 *
 * The text file is subsequently closed.
 *
 * @param fileName, the text file containing the original colony shape
 * @param grid, the grid that is built according to the specifications of the text file
 */
void readFile(string fileName, Grid<bool> &grid) {
    ifstream fileIn;
    openFile(fileIn, fileName);

    string line;
    getline(fileIn, line);
    int numRows = stringToInteger(line);
    getline(fileIn, line);
    int numCols = stringToInteger(line);

    grid.resize(numRows, numCols);

    for (int row = 0; row < numRows; row++) {
        getline(fileIn, line);
        for (int col = 0; col < (int) line.length(); col++) {
            if (line[col] == '-') {
                grid[row][col] = false;
            } else if (line[col] == 'X') {
                grid[row][col] = true;
            }
        }
    }
    fileIn.close();
}

/* Method: displayGrid(grid) */
/**
 * Prints the current grid to the console. For each column in each row, the "live" or "dead"
 * ("X" or "-") cell is printed, ultimately illustrating the current stage of the bacteria's life
 * cycle for the user.
 *
 * @param grid, the grid depicting the current stage in the bacteria's life cycle
 */
void displayGrid(Grid<bool> &grid) {
    for (int row = 0; row < grid.numRows(); row++) {
        for (int col = 0; col < grid.numCols(); col++) {
            if (grid[row][col]) {
                cout << 'X';
            } else {
                cout << '-';
            }
        }
        cout << endl;
    }
}

/* Method: updateTempGrid(grid, temp, row, col, count) */
/**
 * Updates the temporary grid (established to store a copy of the original grid) during the
 * advancement to the next generation. In other methods, the number of neighbors that a cell
 * in the grid has in counted (given the context of wrapping or non-wrapping). That value count,
 * which is accepted as a parameter, is then used as a reference to the original rules of the game,
 * as conditional statements determine what each cell will look like in the next generation.
 *
 * If count is less that 2 or greater than 3, that cell dies/becomes empty in the next generation
 * (stored in the temporary grid "temp"). If count equals 2, that cell is stable and maintains
 * its status from the previous generation. If count equals 3, that cell in the temporary grid
 * creates or sustains life.
 *
 * @param grid, the grid depicting the bacteria's current state
 * @param temp, the copy of the grid, which is updated to diplay the next generation
 * @param row, the appropriate grid row
 * @param col, the appropriate grid column
 * @param count, the number of neighbors a cell has in the current generation
 */
void updateTempGrid(Grid<bool> grid, Grid<bool> &temp, int row, int col, int count) {
    if (count <= 1 || count > 3) {
        temp[row][col] = false;
    }
    if (count == 2) {
        temp[row][col] = grid[row][col];
    }
    if (count == 3) {
        temp[row][col] = true;
    }
}

/* Method: countLiveNeighbors(grid, temp, wrap) */
/**
 * Counts the number of neighbors that each cell in the grid has.
 * The method iterates through each column of each row, effectively iterating through each cell
 * of the grid. Each cell has north, south, west, and east spheres surrounding it, and each
 * cardinal direction can be combined to retrieve the cells to the diagonal (or "corner") of
 * the current cell.
 *
 * The method that will count neighbors and created the grid for the next generation depending on
 * whether the user noted that they wanted the simulation to wrap around the grid (wrap == "y")
 * or adhere to the boundaires of the grid (wrap = "n"). If the user resquests a wrapped
 * simulation then the code in that if block will execute. Otherwise, if the user opted not
 * to incorporate wrapping, a different block will execute accordingly.
 *
 * Incorporating Wrapping:
 * A counter is establish to record the number of neighbors each cell has. If a neighboring cell
 * contains the boolean value "true" (== live cell), then the value of count is increased.
 *
 * OR
 *
 * Obeying Boundaries:
 * A counter is establish to record the number of neighbors each cell has. If a neighboring cell is
 * (1) within the boundaries of the grid and (2) contains the boolean value "true" (== live cell), and
 * (3) is not the current cell, then the value of count is increased.
 *
 * Once all live neighbors have been accounted for, the count value, along with the row and column
 * of the current cell, is passed as an argument to updateTempGrid(), which updates the temporary
 * grid according to the rules of the game.
 *
 * @param grid, the grid depicting the bacteria's current state
 * @param temp, the copy of the grid, which is updated to display the next generation
 * @param wrap, whether the user requests the simulation to wrap around the grid ("y" or "n")
 */
void countLiveNeighbors(Grid<bool> &grid, Grid<bool> &temp, string wrap) {
    int height = grid.numRows();
    int width = grid.numCols();

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++){
            int north = row - 1;
            int south = row + 1;
            int west = col - 1;
            int east = col + 1;

            int count = 0;

            for (int r = north; r <= south; r++) {
                for (int c = west; c <= east; c++) {
                    if (wrap == "y") {
                        if (!(r == row && c == col)) {
                            if (grid[(r + height) % height][(c + width) % width]) {
                                count++;
                            }
                        }
                    } else if (wrap == "n") {
                        if (grid.inBounds(r, c) && grid[r][c] && !(r == row && c == col)) {
                            count++;
                        }
                    }
                }
            }
            updateTempGrid(grid, temp, row, col, count);
        }
    }
}

/* Method: nextGeneration(grid, wrap) */
/**
 * Advances the current grid to the next generation. A temporary grid, temp, is established as
 * a copy of the original grid such that future cells can be changed without affecting the
 * original.
 *
 * After all cell changes have been made (once the next generation is established in the
 * temporary grid), the current grid is set to the value of the temporary grid such that the cycle
 * continues properly.
 *
 * @param grid, the grid depicting the bacteria's current state
 * @param wrap, whether the user requests the simulation to wrap around the grid ("y" or "n")
 */
void getNextGeneration(Grid<bool> &grid, string wrap) {
    Grid<bool> temp(grid.numRows(), grid.numCols());
    countLiveNeighbors(grid, temp, wrap);
    grid = temp;
}

/* Method: animateGenerations(grid, numFrames, wrap) */
/**
 * Animates the progression of the original grid through numFrames life cycles. The integer
 * numFrames, which is accepted as a parameter, is gathered from the user in the main() method.
 * This is the number of life cycles that is quickly animated on the console - that is, each
 * each generation is flashed on the screen accourding to the delay of pause time (in
 * milliseconds). The console is cleared between each frame to allow the next one to appear.
 *
 * @param grid, the grid depicting the bacteria's current state
 * @param numFrames, the number of life cycles to animate
 * @param wrap, whether the user requests the simulation to wrap around the grid ("y" or "n")
 */
void animateGenerations(Grid<bool> &grid, int numFrames, string wrap) {
    for (int frame = 0; frame < numFrames; frame++) {
        clearConsole();
        getNextGeneration(grid, wrap);
        displayGrid(grid);
        pause(50);
    }
}

/* Method: getRandomGrid(grid) */
/**
 * Creates a random grid. If the user inputs "random" into the file prompt,
 * a random grid of potential dimension 1x1 to 50x50 is generated. Each
 * cell in the grid is then randomly assigned to be a live or dead cell.
 *
 * @param grid, the grid depicting the bacteria's current state
 */
void getRandomGrid(Grid<bool> &grid) {
    int x = randomInteger(1, 50);
    int y = randomInteger(1, 50);

    grid.resize(x, y);

    int rand = 0;
    for (int row = 0; row < grid.numRows(); row++) {
        for (int col = 0; col < grid.numCols(); col++) {
            rand = randomInteger(0, 4);
            if (rand == 1) {
                grid[row][col] = true;
            } else {
                grid[row][col] = false;
            }
        }
    }
}
