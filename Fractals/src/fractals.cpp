/*
 * File: fractals.cpp
 * --------------------------
 * Name: Dannika Thompson
 * Section leader: Ana-Maria Istrate
 *
 * This file draws several recursive graphical patterns (fractals)to the console: a Sierpinski
 * triangle, a recursive tree, and a Mandelbrot Set. Each figure is customizable according to
 * the user inputs and interactions with the GUI.
 */


#include "fractals.h"
#include "gwindow.h"
#include "gobjects.h"
#include <cmath>

using namespace std;

const int LEAF_COLOR = 0x2e8b57;   /* Color of all leaves of recursive tree (level 1) */
const int BRANCH_COLOR = 0x8b7765; /* Color of all branches of recursive tree (level >=2) */
const int BRANCH_DISP = 45; /* Displacement angle of first branch in orders > 1 */
const int ANGLE_BETWEEN_BRANCHES = 15; /* Angle between each branch */
const int TRUNK_THETA = 90; /* Angle between the trunk (order = 1) and the x-axis */

/**
 * Draws a Sierpinski triangle of the specified size and order, placing its
 * top-left corner at position (x, y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Sierpinski triangle.
 * @param x - The x coordinate of the top-left corner of the triangle.
 * @param y - The y coordinate of the top-left corner of the triangle.
 * @param size - The length of one side of the triangle.
 * @param order - The order of the fractal.
 */
void drawSierpinskiTriangle(GWindow& gw, double x, double y, double size, int order) {
    double h = (sqrt(3.0) / 2.0) * size;
    double halfSize = size / 2.0;

    if (order < 0) {
        throw("Error: Order must be positive integer.");
    } else if (order == 1) {
        gw.drawLine(x, y, x + size, y);
        gw.drawLine(x, y, x + halfSize, y + h);
        gw.drawLine(x + halfSize, y + h, x + size, y);
    } else if (order > 1) {
        drawSierpinskiTriangle(gw, x, y, halfSize, order - 1);
        drawSierpinskiTriangle(gw, x + halfSize, y, halfSize, order - 1);
        drawSierpinskiTriangle(gw, x + (halfSize / 2.0), y + (h / 2.0), halfSize, order - 1);
    }
}

/*
 * Checks the order of each branch to determine its color. If the branches are on the
 * outermost level, they will be green; otherwise, they will be brown.
 */
void checkLeaves(GWindow& gw, int order) {
    if (order == 2) {
        gw.setColor(LEAF_COLOR);
    } else {
        gw.setColor(BRANCH_COLOR);
    }
}

/*
 * Recursively draws the branches of the tree. For any branch extending from the 1st-order
 * "trunk," all of the limbs of that branch are drawn until it reaches the level of the
 * leaves (the inputed order). This process is repreated for the remaining branches extending
 * from the 1st-order "trunk."
 */
void drawBranches(GWindow& gw, double x, double y, double size, int order, double degrees) {
    GPoint base = gw.drawPolarLine(x, y, size, degrees);
    if (order > 1) {
        for (int branch = 0; branch < 7; branch++) {
            checkLeaves(gw, order);
            drawBranches(gw, base.getX(), base.getY(), size / 2.0, order - 1,
                         (degrees - BRANCH_DISP) + (ANGLE_BETWEEN_BRANCHES * branch));
        }
    }
}

/**
 * Draws a recursive tree fractal image of the specified size and order,
 * placing the bounding box's top-left corner at position (x,y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the recursive tree.
 * @param x - The x coordinate of the top-left corner of the bounding box.
 * @param y - The y coordinate of the top-left corner of the bounding box.
 * @param size - The length of one side of the bounding box.
 * @param order - The order of the fractal.
 */
void drawTree(GWindow& gw, double x, double y, double size, int order) {
    double baseX = x + (size / 2.0);
    double baseY = y + size;

    gw.setColor(BRANCH_COLOR);

    if (x < 0 || y < 0 || size < 0 || order < 0) {
        throw("Error: inputs must be positive.");
    } else if (order > 0) {
        drawBranches(gw, baseX, baseY, size / 2.0, order, TRUNK_THETA);
    }
}

/**
 * Draws a Mandelbrot Set in the graphical window give, with maxIterations
 * (size in GUI) and in a given color (zero for palette)
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Mandelbrot set.
 * @param minX - left-most column of grid
 * @param incX - increment value of columns of grid
 * @param minY - top-most row of grid
 * @param incY - increment value of rows of grid
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @param color - The color of the fractal; zero if palette is to be used
 */
void mandelbrotSet(GWindow& gw, double minX, double incX,
                   double minY, double incY, int maxIterations, int color) {

    // Creates palette of colors
    // To use palette:
    // pixels[r][c] = palette[numIterations % palette.size()];
    Vector<int> palette = setPalette();

    int width = gw.getCanvasWidth();
    int height = gw.getCanvasHeight();
    GBufferedImage image(width,height,0xffffff);
    gw.add(&image);
    Grid<int> pixels = image.toGrid(); // Convert image to grid

    for (int row = 0; row < pixels.numRows(); row++) {
        for (int col = 0; col < pixels.numCols(); col++) {
            Complex c = Complex(minX + col * incX, minY + row * incY);

            int num = mandelbrotSetIterations(c, maxIterations);

            if (num == maxIterations && color != 0) {
                pixels[row][col] = color;
            } else if (color == 0) {
                pixels[row][col] = palette[num % palette.size()];
            }
        }
    }

    image.fromGrid(pixels); // Converts and puts the grid back into the image
}

/**
 * Runs the Mandelbrot Set recursive formula on complex number c a maximum
 * of maxIterations times.
 *
 * Calculates the number of iterations needed to determine if c is unbounded.
 *
 * @param c - Complex number to use for recursive formula.
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @return number of iterations needed to determine if c is unbounded
 */
int mandelbrotSetIterations(Complex c, int maxIterations) {
    Complex z = Complex(0, 0);
    return maxIterations - mandelbrotSetIterations(z, c, maxIterations);
}

/**
 * An iteration of the Mandelbrot Set recursive formula with given values z and c, to
 * run for a maximum of maxIterations.
 *
 * Updates z until |z| > 4 or until there are no remaining iterations.
 *
 * @param z - Complex number for a given number of iterations
 * @param c - Complex number to use for recursive formula.
 * @param remainingIterations - The remaining number of iterations to run recursive step
 * @return number of iterations needed to determine if c is unbounded
 */
int mandelbrotSetIterations(Complex z, Complex c, int remainingIterations) {
    if (z.abs() > 4 || remainingIterations == 0) {
        return remainingIterations;
    } else {
        Complex temp = z * z + c;
        return mandelbrotSetIterations(temp, c, remainingIterations - 1);
    }
}

// Helper function to set the palette
Vector<int> setPalette() {
    Vector<int> colors;

    // Feel free to replace with any palette.
    // You can find palettes at:
    // http://www.colourlovers.com/palettes

    // Example palettes:
    // http://www.colourlovers.com/palette/4480793/in_the_middle
    // string colorSt = "#A0B965,#908F84,#BF3C43,#9D8E70,#C9BE91,#A0B965,#908F84,#BF3C43";

    // http://www.colourlovers.com/palette/4480786/Classy_Glass
    // string colorSt = "#9AB0E9,#C47624,#25269A,#B72202,#00002E,#9AB0E9,#C47624,#25269A";

    // The following is the "Hope" palette:
    // http://www.colourlovers.com/palette/524048/Hope
    string colorSt =  "#04182B,#5A8C8C,#F2D99D,#738585,#AB1111,#04182B,#5A8C8C,#F2D99D";
    Vector<string>colorsStrVec = stringSplit(colorSt,",");
    for (string color : colorsStrVec) {
        colors.add(convertColorToRGB(trim(color)));
    }
    return colors;
}
