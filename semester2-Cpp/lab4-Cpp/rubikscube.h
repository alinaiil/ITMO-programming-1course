#include <iostream>
#include <vector>
#include <GL/freeglut.h>
#include <windows.h>
#pragma once

/**
 * 0 - yellow (up)
 * 1 - orange (left)
 * 2 - blue (front)
 * 3 - red (right)
 * 4 - green (back)
 * 5 - white (down)
 */


class RubiksCube {
private:
    int faces[6][9]{};

    GLfloat colour[6][3][3][3]{};

    void facesToColour();

    std::vector<GLfloat> makeColour(int face);

    static bool checkTwoInv(int a, int b);

    static bool checkThreeInv(int a, int b, int c);

    bool checkInvariants();

    bool isSolved();

    void rotateFace(bool clockwise, int face);

    void PifPuf(bool right, int front, int top);

    int RightToWCross();

    int LeftToWCross();

    void WhiteCross();

    int CheckFirstStage();

    void EndFirstStage();

    void CornerSecondStage(int left, int right);

    void SecondStage();

    void ThirdStage();

    void EdgeThirdStage(int left, int right, bool toTheLeft);

    void PlaceEdge(int a, int b, int color);

    void FourthStage();

    void StickToCross(int front);

    void VToCross(int front);

    void FifthStage();

    void SixthStage();

    int CheckSixthStage();

    void SixthRotation(int front);

    void SeventhStage();

    int findCorner();
public:
    RubiksCube();

    void draw();

    void Solve();

    friend std::istream &operator>>(std::istream &in, RubiksCube &rubiksCube);

    friend std::ostream &operator<<(std::ostream &out, RubiksCube &rubiksCube);

    void rotateUp(bool clockwise);

    void rotateFront(bool clockwise, int front);

    void rotateRight(bool clockwise, int front);

    void rotateLeft(bool clockwise, int front);

    void rotateDown(bool clockwise);

    void rotateBack(bool clockwise, int front);

    void Shuffle();

    void openGL(int argc, char **argv);
};