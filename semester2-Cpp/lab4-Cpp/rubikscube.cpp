#include "rubikscube.h"
#include <iostream>
#include <ctime>
#include <random>

RubiksCube cube;
GLfloat rotate_y = 0;
GLfloat rotate_x = 0;

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glRotatef(rotate_x, 1.0, 0.0, 0.0);
    glRotatef(rotate_y, 0.0, 1.0, 0.0);

    glScalef(0.75, 0.75, 0.75);
    cube.draw();

    glFlush();
    glutSwapBuffers();
}

void specialKeys(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) {
        rotate_y += 5;
        glutPostRedisplay();
    }
    else if (key == GLUT_KEY_LEFT) {
        rotate_y -= 5;
        glutPostRedisplay();
    }
    else if (key == GLUT_KEY_UP) {
        rotate_x += 5;
        glutPostRedisplay();
    }
    else if (key == GLUT_KEY_DOWN) {
        rotate_x -= 5;
        glutPostRedisplay();
    }
    else if (key == GLUT_KEY_HOME) {
        cube.Shuffle();
    }
    else if (key == GLUT_KEY_END) {
        cube.Solve();
    }
}

void manualRotates(unsigned char key, int x, int y) {
    if (key == 'r') {
        cube.rotateRight(true, 2);
    } else if (key == 'R') {
        cube.rotateRight(false, 2);
    } else if (key == 'l') {
        cube.rotateLeft(true, 2);
    } else if (key == 'L') {
        cube.rotateLeft(false, 2);
    } else if (key == 'u') {
        cube.rotateUp(true);
    } else if (key == 'U') {
        cube.rotateUp(false);
    } else if (key == 'd') {
        cube.rotateDown(true);
    } else if (key == 'D') {
        cube.rotateDown(false);
    } else if (key == 'f') {
        cube.rotateFront(true, 2);
    } else if (key == 'F') {
        cube.rotateFront(false, 2);
    } else if (key == 'b') {
        cube.rotateBack(true, 2);
    } else if (key == 'B') {
        cube.rotateBack(false, 2);
    }
}

void RubiksCube::openGL(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 900);
    glutCreateWindow("Rubik's Cube");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(manualRotates);
    glutMainLoop();
}


RubiksCube::RubiksCube() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 9; j++) {
            this->faces[i][j] = i;
        }
    }
    facesToColour();
}

bool RubiksCube::checkTwoInv(int a, int b) {
    if (a == b) {
        return true;
    }
    if ((a == 0 && b == 5) || (a == 1 && b == 3) || (a == 2 && b == 4) ||
        (a == 3 && b == 1) || (a == 4 && b == 2) || (a == 5 && b == 0)) {
        return true;
    }
    return false;
}

bool RubiksCube::checkThreeInv(int a, int b, int c) {
    if (a == b || a == c || b == c) {
        return true;
    }
    if (a == 0 || a == 5) {
        if (b == 5 || c == 5 || b == 0 || c == 0) {
            return true;
        }
        if ((b == 4 && c == 2) || (c == 4 && b == 2) || (b == 3 && c == 1) || (c == 1 && b == 3)) {
            return true;
        }
    }
    if (a == 1 || a == 3) {
        if (b == 1 || c == 1 || b == 3 || c == 3) {
            return true;
        }
        if ((b == 4 && c == 2) || (c == 4 && b == 2) || (b == 0 && c == 5) || (c == 5 && b == 0)) {
            return true;
        }
    }
    if (a == 2 || a == 4) {
        if (b == 2 || c == 2 || b == 4 || c == 4) {
            return true;
        }
        if ((b == 0 && c == 5) || (c == 5 && b == 0) || (b == 3 && c == 1) || (c == 1 && b == 3)) {
            return true;
        }
    }
    return false;
}

bool RubiksCube::checkInvariants() {
    if (checkTwoInv(this->faces[0][1], this->faces[4][1]) || checkTwoInv(this->faces[0][3], this->faces[1][1]) ||
        checkTwoInv(this->faces[0][5], this->faces[3][1]) || checkTwoInv(this->faces[0][7], this->faces[2][1]) ||
        checkTwoInv(this->faces[5][1], this->faces[2][7]) || checkTwoInv(this->faces[5][3], this->faces[1][7]) ||
        checkTwoInv(this->faces[5][5], this->faces[3][7]) || checkTwoInv(this->faces[5][7], this->faces[4][7])) {
        return true;
    }
    if (checkThreeInv(this->faces[0][0], this->faces[4][2], this->faces[1][0]) ||
        checkThreeInv(this->faces[0][2], this->faces[4][0], this->faces[3][2]) ||
        checkThreeInv(this->faces[0][6], this->faces[2][0], this->faces[1][2]) ||
        checkThreeInv(this->faces[0][8], this->faces[2][2], this->faces[3][0]) ||
        checkThreeInv(this->faces[5][0], this->faces[2][6], this->faces[1][8]) ||
        checkThreeInv(this->faces[5][2], this->faces[2][8], this->faces[3][6]) ||
        checkThreeInv(this->faces[5][6], this->faces[4][8], this->faces[1][6]) ||
        checkThreeInv(this->faces[5][8], this->faces[4][6], this->faces[3][8])) {
        return true;
    }
    return false;
}

bool RubiksCube::isSolved() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 9; j++) {
            if (this->faces[i][j] != i) {
                return false;
            }
        }
    }
    return true;
}

void RubiksCube::rotateFace(bool clockwise, int face) {
    int tmpFace[9];
    for (int i = 0; i < 3; i++) {
        tmpFace[i * 3] = clockwise ? this->faces[face][6 + i] : this->faces[face][2 - i];
        tmpFace[i * 3 + 1] = clockwise ? this->faces[face][3 + i] : this->faces[face][5 - i];
        tmpFace[i * 3 + 2] = clockwise ? this->faces[face][i] : this->faces[face][8 - i];
    }
    for (int i = 0; i < 9; i++) {
        this->faces[face][i] = tmpFace[i];
    }
    int tmpBuf[4][3];
    for (int i = 0; i < 3; i++) {
        if (face == 0) {
            tmpBuf[0][i] = this->faces[1][i];
            tmpBuf[1][i] = this->faces[2][i];
            tmpBuf[2][i] = this->faces[3][i];
            tmpBuf[3][i] = this->faces[4][i];
        }
        if (face == 1) {
            tmpBuf[0][i] = this->faces[4][8 - i * 3];
            tmpBuf[1][i] = this->faces[0][i * 3];
            tmpBuf[2][i] = this->faces[2][i * 3];
            tmpBuf[3][i] = this->faces[5][6 - i * 3];
        }
        if (face == 2) {
            tmpBuf[0][i] = this->faces[1][8 - i * 3];
            tmpBuf[1][i] = this->faces[0][6 + i];
            tmpBuf[2][i] = this->faces[3][6 - i * 3];
            tmpBuf[3][i] = this->faces[5][i];
        }
        if (face == 3) {
            tmpBuf[0][i] = this->faces[2][2 + i * 3];
            tmpBuf[1][i] = this->faces[5][2 + i * 3];
            tmpBuf[2][i] = this->faces[4][6 - i * 3];
            tmpBuf[3][i] = this->faces[0][2 + i * 3];
        }
        if (face == 4) {
            tmpBuf[0][i] = this->faces[1][i * 3];
            tmpBuf[1][i] = this->faces[0][i];
            tmpBuf[2][i] = this->faces[3][2 + i * 3];
            tmpBuf[3][i] = this->faces[5][8 - i];
        }
        if (face == 5) {
            tmpBuf[0][i] = this->faces[1][6 + i];
            tmpBuf[1][i] = this->faces[2][6 + i];
            tmpBuf[2][i] = this->faces[3][6 + i];
            tmpBuf[3][i] = this->faces[4][6 + i];
        }
    }
    for (int i = 0; i < 3; i++) {
        if (face == 0) {
            this->faces[1][i] = clockwise ? tmpBuf[1][i] : tmpBuf[3][i];
            this->faces[2][i] = clockwise ? tmpBuf[2][i] : tmpBuf[0][i];
            this->faces[3][i] = clockwise ? tmpBuf[3][i] : tmpBuf[1][i];
            this->faces[4][i] = clockwise ? tmpBuf[0][i] : tmpBuf[2][i];
        }
        if (face == 1) {
            this->faces[4][2 + i * 3] = clockwise ? tmpBuf[3][i] : tmpBuf[1][2 - i];
            this->faces[0][i * 3] = clockwise ? tmpBuf[0][i] : tmpBuf[2][i];
            this->faces[2][i * 3] = clockwise ? tmpBuf[1][i] : tmpBuf[3][2 - i];
            this->faces[5][i * 3] = clockwise ? tmpBuf[2][i] : tmpBuf[0][i];
        }
        if (face == 2) {
            this->faces[1][2 + i * 3] = clockwise ? tmpBuf[3][i] : tmpBuf[1][2 - i];
            this->faces[0][6 + i] = clockwise ? tmpBuf[0][i] : tmpBuf[2][2 - i];
            this->faces[3][i * 3] = clockwise ? tmpBuf[1][i] : tmpBuf[3][2 - i];
            this->faces[5][i] = clockwise ? tmpBuf[2][i] : tmpBuf[0][2 - i];
        }
        if (face == 3) {
            this->faces[2][2 + i * 3] = clockwise ? tmpBuf[1][i] : tmpBuf[3][i];
            this->faces[5][2 + i * 3] = clockwise ? tmpBuf[2][i] : tmpBuf[0][i];
            this->faces[4][6 - i * 3] = clockwise ? tmpBuf[3][i] : tmpBuf[1][i];
            this->faces[0][2 + i * 3] = clockwise ? tmpBuf[0][i] : tmpBuf[2][i];
        }
        if (face == 4) {
            this->faces[1][i * 3] = clockwise ? tmpBuf[1][2 - i] : tmpBuf[3][2 - i];
            this->faces[0][i] = clockwise ? tmpBuf[2][i] : tmpBuf[0][2 - i];
            this->faces[3][2 + i * 3] = clockwise ? tmpBuf[3][i] : tmpBuf[1][i];
            this->faces[5][6 + i] = clockwise ? tmpBuf[0][i] : tmpBuf[2][2 - i];
        }
        if (face == 5) {
            this->faces[1][6 + i] = clockwise ? tmpBuf[3][i] : tmpBuf[1][i];
            this->faces[2][6 + i] = clockwise ? tmpBuf[0][i] : tmpBuf[2][i];
            this->faces[3][6 + i] = clockwise ? tmpBuf[1][i] : tmpBuf[3][i];
            this->faces[4][6 + i] = clockwise ? tmpBuf[2][i] : tmpBuf[0][i];
        }
    }
    facesToColour();
    Sleep(100);
    display();
}

void RubiksCube::rotateUp(bool clockwise) {
    rotateFace(clockwise, 0);
    if (clockwise) std::cout << "U ";
    if (!clockwise) std::cout << "U' ";
}

void RubiksCube::rotateFront(bool clockwise, int front) {
    rotateFace(clockwise, front);
    if (front == 2) {
        if (clockwise) std::cout << "F ";
        if (!clockwise) std::cout << "F' ";
    }
    if (front == 1) {
        if (clockwise) std::cout << "L ";
        if (!clockwise) std::cout << "L' ";
    }
    if (front == 3) {
        if (clockwise) std::cout << "R ";
        if (!clockwise) std::cout << "R' ";
    }
    if (front == 4) {
        if (clockwise) std::cout << "B ";
        if (!clockwise) std::cout << "B' ";
    }
}

void RubiksCube::rotateRight(bool clockwise, int front) {
    if (front == 4) {
        rotateFace(clockwise, 1);
        if (clockwise) std::cout << "L ";
        if (!clockwise) std::cout << "L' ";
    } else {
        rotateFace(clockwise, front + 1);
        if (front == 2) {
            if (clockwise) std::cout << "R ";
            if (!clockwise) std::cout << "R' ";
        }
        if (front == 1) {
            if (clockwise) std::cout << "F ";
            if (!clockwise) std::cout << "F' ";
        }
        if (front == 3) {
            if (clockwise) std::cout << "B ";
            if (!clockwise) std::cout << "B' ";
        }
    }
}

void RubiksCube::rotateLeft(bool clockwise, int front) {
    if (front == 1) {
        rotateFace(clockwise, 4);
        if (clockwise) std::cout << "B ";
        if (!clockwise) std::cout << "B' ";
    } else {
        rotateFace(clockwise, front - 1);
        if (front == 2) {
            if (clockwise) std::cout << "L ";
            if (!clockwise) std::cout << "L' ";
        }
        if (front == 4) {
            if (clockwise) std::cout << "R ";
            if (!clockwise) std::cout << "R' ";
        }
        if (front == 3) {
            if (clockwise) std::cout << "F ";
            if (!clockwise) std::cout << "F' ";
        }
    }
}

void RubiksCube::rotateDown(bool clockwise) {
    rotateFace(clockwise, 5);
    if (clockwise) std::cout << "D ";
    if (!clockwise) std::cout << "D' ";
}

void RubiksCube::rotateBack(bool clockwise, int front) {
    if (front == 3) {
        rotateFace(clockwise, 1);
        if (clockwise) std::cout << "L ";
        if (!clockwise) std::cout << "L' ";
    } else if (front == 4) {
        rotateFace(clockwise, 2);
        if (clockwise) std::cout << "F ";
        if (!clockwise) std::cout << "F' ";
    } else {
        rotateFace(clockwise, front + 2);
        if (front == 2) {
            if (clockwise) std::cout << "B ";
            if (!clockwise) std::cout << "B' ";
        }
        if (front == 1) {
            if (clockwise) std::cout << "R ";
            if (!clockwise) std::cout << "R' ";
        }
    }
}

std::istream &operator>>(std::istream &in, RubiksCube &rubiksCube) {
    int tmp[9];
    int counter = 0;
    std::string str;
    in >> str;
    int q[6] = {};
    bool centers[6] = {};
    for (int i = 0; i < 54; i++) {
        char current = str[i % 3];
        if (current >= '0' && current <= '5') {
            q[current - '0']++;
            tmp[counter % 9] = current - '0';
            if (!((counter + 1) % 9)) {
                centers[tmp[4]] = true;
                for (int j = 0; j < 9; j++) {
                    rubiksCube.faces[tmp[4]][j] = tmp[j];
                }
            }
        } else {
            throw std::logic_error("Wrong input");
        }
        counter++;
        if (!(counter % 3)) {
            in >> str;
            if (str.empty()) {
                in >> str;
            }
        }
        if (counter < 51 && in.eof()) {
            throw std::logic_error("Wrong input");
        }
    }
    for (int i = 0; i < 9; i++) {
        rubiksCube.faces[tmp[4]][i] = tmp[i];
    }
    if (rubiksCube.checkInvariants()) {
        throw std::logic_error("Wrong input. Check invariants");
    }
    for (int i = 0; i < 6; i++) {
        if (q[i] != 9) {
            throw std::logic_error("Wrong input");
        }
        if (!centers[i]) {
            throw std::logic_error("Wrong input");
        }
    }
    return in;
}

std::ostream &operator<<(std::ostream &out, RubiksCube &rubiksCube) {
    for (int i = 0; i < 3; i++) {
        out << "     " << rubiksCube.faces[0][i * 3] << rubiksCube.faces[0][1 + i * 3] << rubiksCube.faces[0][2 + i * 3]
            << '\n';
    }
    for (int i = 0; i < 3; i++) {
        out << rubiksCube.faces[1][i * 3] << rubiksCube.faces[1][1 + i * 3] << rubiksCube.faces[1][2 + i * 3] << "  "
            << rubiksCube.faces[2][i * 3] << rubiksCube.faces[2][1 + i * 3] << rubiksCube.faces[2][2 + i * 3] << "  "
            << rubiksCube.faces[3][i * 3] << rubiksCube.faces[3][1 + i * 3] << rubiksCube.faces[3][2 + i * 3] << "  "
            << rubiksCube.faces[4][i * 3] << rubiksCube.faces[4][1 + i * 3] << rubiksCube.faces[4][2 + i * 3] << '\n';
    }
    for (int i = 0; i < 3; i++) {
        out << "     " << rubiksCube.faces[5][i * 3] << rubiksCube.faces[5][1 + i * 3] << rubiksCube.faces[5][2 + i * 3]
            << '\n';
    }
    return out;
}

void RubiksCube::PifPuf(bool right, int front, int top) {
    if (top == 0) {
        if (right) {
            rotateRight(true, front);
            rotateUp(true);
            rotateRight(false, front);
            rotateUp(false);
        } else {
            rotateLeft(false, front);
            rotateUp(false);
            rotateLeft(true, front);
            rotateUp(true);
        }
    }
    if (top == 1) {
        if (right) {
            rotateDown(true);
            rotateLeft(true, 2);
            rotateDown(false);
            rotateLeft(false, 2);
        }
    }
    if (top == 5) {
        if (right) {
            rotateLeft(true, front);
            rotateDown(true);
            rotateLeft(false, front);
            rotateDown(false);
        } else {
            rotateRight(false, front);
            rotateDown(false);
            rotateRight(true, front);
            rotateDown(true);
        }
    }
}

int RubiksCube::RightToWCross() {
    int c = 0;
    for (int i = 1; i < 5; i++) {
        if (faces[i][3] == 5) {
            int j;
            if (i == 1) j = 7;
            if (i == 2) j = 3;
            if (i == 3) j = 1;
            if (i == 4) j = 5;
            while (faces[5][j] == 5) {
                rotateDown(true);
            }
            rotateLeft(true, i);
            c += 1;
        }
    }
    return c;
}

int RubiksCube::LeftToWCross() {
    int c = 0;
    for (int i = 1; i < 5; i++) {
        if (faces[i][5] == 5) {
            int j;
            if (i == 1) j = 1;
            if (i == 2) j = 5;
            if (i == 3) j = 7;
            if (i == 4) j = 3;
            while (faces[5][j] == 5) {
                rotateDown(true);
            }
            rotateRight(false, i);
            c += 1;
        }
    }
    return c;
}

void RubiksCube::WhiteCross() {
    for (int c = 0; c < 24; c++) {
        int counter = 0;
        for (int i = 1; i < 8; i += 2) {
            counter += (faces[5][i] == 5) ? 1 : 0;
        }
        counter += RightToWCross();
        counter += LeftToWCross();
        for (int i = 1; i < 5; i++) {
            if (faces[i][7] == 5) {
                rotateFront(true, i);
                counter += RightToWCross();
            }
            if (faces[i][1] == 5) {
                int j;
                if (i == 1) j = 3;
                if (i == 2) j = 1;
                if (i == 3) j = 5;
                if (i == 4) j = 7;
                while (faces[5][j] == 5) {
                    rotateDown(true);
                }
                rotateFront(false, i);
                counter += RightToWCross();
            }
        }
        for (int i = 1; i < 8; i += 2) {
            if (faces[0][i] == 5) {
                int k, j;
                if (i == 1) {
                    k = 4;
                    j = 7;
                }
                if (i == 3) {
                    k = 1;
                    j = 3;
                }
                if (i == 5) {
                    k = 3;
                    j = 5;
                }
                if (i == 7) {
                    k = 2;
                    j = 1;
                }
                while (faces[5][j] == 5) {
                    rotateDown(true);
                }
                rotateFront(true, k);
                counter += RightToWCross();
            }
        }
        if (counter == 4) {
            return ;
        }
    }
    throw std::logic_error("WhiteCross error: not a 4");
}

int RubiksCube::CheckFirstStage() {
    int counter = 0;
    for (int i = 1; i < 5; i++) {
        if (faces[i][7] == i) {
            counter++;
        }
    }
    return counter;
}

void RubiksCube::EndFirstStage() {
    int errorCounter = 0;
    while (CheckFirstStage() < 2) {
        rotateDown(true);
    }
    if (CheckFirstStage() == 2) {
        if (faces[1][7] == 1) {
            if (faces[2][7] == 2) {
                PifPuf(true, 1, 5);
                rotateLeft(true, 1);
            } else if (faces[3][7] == 3) {
                PifPuf(true, 1, 5);
                rotateLeft(true, 1);
                EndFirstStage();
            } else if (faces[4][7] == 4) {
                PifPuf(true, 4, 5);
                rotateLeft(true, 4);
            }
        } else if (faces[2][7] == 2) {
            if (faces[3][7] == 3) {
                PifPuf(true, 2, 5);
                rotateLeft(true, 2);
            } else if (faces[4][7] == 4) {
                PifPuf(true, 2, 5);
                rotateLeft(true, 2);
                EndFirstStage();
            }
        } else {
            PifPuf(true, 3, 5);
            rotateLeft(true, 3);
        }
    }
}

void RubiksCube::CornerSecondStage(int left, int right) {
    int caseCorner;
    int j;
    if (left == 4) j = 0;
    if (left == 1) j = 6;
    if (left == 2) j = 8;
    if (left == 3) j = 2;
    int a, b;
    if (faces[left][2] == 5) {
        a = faces[0][j];
        b = faces[right][0];
        caseCorner = 5;
    } else if (faces[right][0] == 5) {
        a = faces[0][j];
        b = faces[left][2];
        caseCorner = 1;
    } else {
        a = faces[left][2];
        b = faces[right][0];
        caseCorner = 3;
    }
    if (a == 1 || b == 1) {
        if (b == 4) {
            std::swap(a, b);
        }
        if (a == 2) {
            std::swap(a, b);
        }
    }
    if (a == 3 || b == 3) {
        if (b == 2) {
            std::swap(a, b);
        }
        if (a == 4) {
            std::swap(a, b);
        }
    }
    int errorCounter = 0;
    while (!(left == a && right == b) && errorCounter < 5) {
        rotateUp(true);
        left = (left != 1) ? left - 1 : 4;
        right = (right != 1) ? right - 1 : 4;
        errorCounter++;
    }
    if (errorCounter == 15) {
        throw std::logic_error("Invariant error ERROR COUNTER");
    }
    for (int i = 0; i < caseCorner; i++) {
        PifPuf(true, left, 0);
    }
}

void RubiksCube::SecondStage() {
    for (int c = 0; c < 5; c++) {
        if ((faces[1][8] == 5 || faces[2][6] == 5 || faces[5][0] == 5) && !(faces[1][8] == 1 && faces[2][6] == 2)) {
            PifPuf(true, 1, 0);
            CornerSecondStage(1, 2);
        }
        if ((faces[2][8] == 5 || faces[3][6] == 5 || faces[5][2] == 5) && !(faces[2][8] == 2 && faces[3][6] == 3)) {
            PifPuf(true, 2, 0);
            CornerSecondStage(2, 3);
        }
        if ((faces[3][8] == 5 || faces[4][6] == 5 || faces[5][8] == 5) && !(faces[3][8] == 3 && faces[4][6] == 4)) {
            PifPuf(true, 3, 0);
            CornerSecondStage(3, 4);
        }
        if ((faces[4][8] == 5 || faces[1][6] == 5 || faces[5][6] == 5) && !(faces[4][8] == 4 && faces[1][6] == 1)) {
            PifPuf(true, 4, 0);
            CornerSecondStage(4, 1);
        }
        if (faces[1][2] == 5 || faces[2][0] == 5 || faces[0][6] == 5) {
            CornerSecondStage(1, 2);
        }
        if (faces[2][2] == 5 || faces[3][0] == 5 || faces[0][8] == 5) {
            CornerSecondStage(2, 3);
        }
        if (faces[3][2] == 5 || faces[4][0] == 5 || faces[0][2] == 5) {
            CornerSecondStage(3, 4);
        }
        if (faces[4][2] == 5 || faces[1][0] == 5 || faces[0][0] == 5) {
            CornerSecondStage(4, 1);
        }
        int counter = 0;
        for (int i = 0; i < 9; i += 2) {
            if (faces[5][i] == 5) {
                counter++;
            }
        }
        if (counter == 5) {
            return;
        }
    }
    throw std::logic_error("Invariant error END SECOND STAGE");
}

void RubiksCube::EdgeThirdStage(int left, int right, bool toTheLeft) {
    if (toTheLeft) {
        rotateUp(false);
        PifPuf(false, right, 0);
        PifPuf(true, left, 0);
    } else {
        rotateUp(true);
        PifPuf(true, left, 0);
        PifPuf(false, right, 0);
    }
}

void RubiksCube::PlaceEdge(int a, int b, int color) {
    int counter = 0;
    while (a != color && counter < 5) {
        rotateUp(true);
        color = (color != 1) ? color - 1 : 4;
        counter++;
    }
    if (counter == 5) {
        std::swap(a, b);
    }
    while (a != color) {
        rotateUp(true);
        color = (color != 1) ? color - 1 : 4;
    }
    color = (color != 1) ? color - 1 : 4;
    if (b == color) {
        EdgeThirdStage(b, a, true);
    } else {
        EdgeThirdStage(a, b, false);
    }
}

void RubiksCube::ThirdStage() {
    if (faces[1][5] != 0 && faces[2][3] != 0 && (faces[1][5] != 1 || faces[2][3] != 2)) {
        EdgeThirdStage(1, 2, true);
    }
    if (faces[2][5] != 0 && faces[3][3] != 0 && (faces[2][5] != 2 || faces[3][3] != 3)) {
        EdgeThirdStage(2, 3, true);
    }
    if (faces[3][5] != 0 && faces[4][3] != 0 && (faces[3][5] != 3 || faces[4][3] != 4)) {
        EdgeThirdStage(3, 4, true);
    }
    if (faces[4][5] != 0 && faces[1][3] != 0 && (faces[4][5] != 4 || faces[1][3] != 1)) {
        EdgeThirdStage(4, 1, true);
    }
    if (faces[0][1] != 0 && faces[4][1] != 0) {
        PlaceEdge(faces[4][1], faces[0][1], 4);
    }
    if (faces[0][3] != 0 && faces[1][1] != 0) {
        PlaceEdge(faces[1][1], faces[0][3], 1);
    }
    if (faces[0][5] != 0 && faces[3][1] != 0) {
        PlaceEdge(faces[3][1], faces[0][5], 3);
    }
    if (faces[0][7] != 0 && faces[2][1] != 0) {
        PlaceEdge(faces[2][1], faces[0][7], 2);
    }
    int counter = 0;
    for (int i = 1; i < 5; i++) {
        if (faces[i][3] == faces[i][5] && faces[i][3] == i) {
            counter++;
        }
    }
    if (counter != 4) {
        ThirdStage();
    }
}

void RubiksCube::FourthStage() {
    int counter = 0;
    for (int i = 1; i < 8; i += 2) {
        if (faces[0][i] == 0) {
            counter++;
        }
    }
    if (counter == 4) {
        return;
    }
    if (counter == 2) {
        if (faces[0][1] == 0) {
            if (faces[0][3] == 0) {
                VToCross(2);
            } else if (faces[0][5] == 0) {
                VToCross(1);
            } else if (faces[0][7] == 0) {
                StickToCross(1);
            }
        } else if (faces[0][3] == 0) {
            if (faces[0][5] == 0) {
                StickToCross(2);
            } else if (faces[0][7] == 0) {
                VToCross(3);
            }
        } else {
            VToCross(4);
        }
    } else {
        StickToCross(1);
        rotateUp(true);
        rotateUp(true);
        VToCross(1);
    }
}

void RubiksCube::Solve() {
    std::cout << "\nStage 1.1: ";
    WhiteCross();
    std::cout << '\n' << "Stage 1.2: ";
    EndFirstStage();
    std::cout << '\n' << "Stage 2: ";
    SecondStage();
    std::cout << '\n' << "Stage 3: ";
    ThirdStage();
    std::cout << '\n' << "Stage 4: ";
    FourthStage();
    std::cout << '\n' << "Stage 5: ";
    FifthStage();
    std::cout << '\n' << "Stage 6: ";
    SixthStage();
    std::cout << '\n' << "Stage 7: ";
    SeventhStage();
    std::cout << '\n';
}

void RubiksCube::StickToCross(int front) {
    rotateFront(true, front);
    PifPuf(true, front, 0);
    rotateFront(false, front);
}

void RubiksCube::VToCross(int front) {
    rotateFront(true, front);
    PifPuf(true, front, 0);
    PifPuf(true, front, 0);
    rotateFront(false, front);
}

void RubiksCube::FifthStage() {
    for (int i = 0; i < 4; i++) {
        if (faces[0][6] != 0) {
            if (faces[1][2] == 0) {
                for (int j = 0; j < 4; j++) {
                    PifPuf(true, 4, 1);
                }
            } else {
                for (int j = 0; j < 2; j++) {
                    PifPuf(true, 4, 1);
                }
            }
        }
        rotateUp(false);
    }
    for (int w : faces[5]) {
        if (w != 5) {
            throw std::logic_error("Invariant error: Broken corner");
        }
    }
}

void RubiksCube::SixthStage() {
    while (CheckSixthStage() < 2) {
        rotateUp(true);
    }
    if (CheckSixthStage() == 2) {
        if (faces[1][1] == 1) {
            if (faces[2][1] == 2) {
                SixthRotation(3);
            } else if (faces[3][1] == 3) {
                SixthRotation(1);
                rotateUp(true);
                rotateUp(true);
                SixthRotation(1);
                rotateUp(false);
            } else if (faces[4][1] == 4) {
                SixthRotation(2);
            }
        } else if (faces[2][1] == 2) {
            if (faces[3][1] == 3) {
                SixthRotation(4);
            } else if (faces[4][1] == 4) {
                SixthRotation(2);
                rotateUp(true);
                rotateUp(true);
                SixthRotation(2);
                rotateUp(false);
            }
        } else {
            SixthRotation(1);
        }
    }
}

int RubiksCube::CheckSixthStage() {
    int counter = 0;
    for (int i = 1; i < 5; i++) {
        if (faces[i][1] == i) {
            counter++;
        }
    }
    return counter;
}

void RubiksCube::SixthRotation(int front) {
    rotateRight(true, front);
    rotateUp(true);
    rotateRight(false, front);
    rotateFront(false, front);
    rotateRight(true, front);
    rotateUp(true);
    rotateRight(false, front);
    rotateUp(false);
    rotateRight(false, front);
    rotateFront(true, front);
    rotateRight(true, front);
    rotateRight(true, front);
    rotateUp(false);
    rotateRight(false, front);
    rotateUp(false);
}

void RubiksCube::SeventhStage() {
    for (int i = 0; i < 4; i++) {
        if (isSolved()) {
            return;
        }
        int left = findCorner();
        int front = 0, top = 0;
        if (left == 1 || left == 0) front = 3, top = 4;
        if (left == 2) front = 4, top = 1;
        if (left == 3) front = 1, top = 2;
        if (left == 4) front = 2, top = 3;
        rotateFront(true, top);
        rotateFront(true, top);
        rotateDown(true);
        rotateFront(true, top);
        rotateFront(true, top);
        rotateDown(false);
        rotateFront(true, front);
        rotateFront(true, front);
        rotateFront(true, top);
        rotateFront(true, top);
        rotateUp(false);
        rotateFront(true, top);
        rotateFront(true, top);
        rotateUp(true);
        rotateFront(true, front);
        rotateFront(true, front);
    }
    throw std::logic_error("Invariant error Seventh stage");
}

int RubiksCube::findCorner() {
    int j = 2;
    for (int i = 1; i < 5; i++) {
        if (faces[i][2] == i && faces[j][0] == j) {
            return i;
        }
        j = (j == 4) ? 1 : j + 1;
    }
    return 0;
}

void RubiksCube::Shuffle() {
    std::cout << "\nShuffle: ";
    srand(time(nullptr));
    int plus = 1 + rand()%(4);
    for (int i = 0; i < plus; i++) {
        rotateFront(true,  2);
    }
    plus = 1 + rand()%(4);
    for (int i = 0; i < plus; i++) {
        rotateLeft(false, 2);
    }
    plus = 1 + rand()%(4);
    for (int i = 0; i < plus; i++) {
        rotateRight(true, 2);
    }
    plus = 1 + rand()%(4);
    for (int i = 0; i < plus; i++) {
        rotateBack(true, 2);
    }
    plus = 1 + rand()%(4);
    for (int i = 0; i < plus; i++) {
        rotateRight(true, 2);
    }
    std::cout << "\nDone.\n";
}

void RubiksCube::facesToColour() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            colour[0][i][j][0] = makeColour(faces[2][i + (6 - j * 3)])[0];
            colour[0][i][j][1] = makeColour(faces[2][i + (6 - j * 3)])[1];
            colour[0][i][j][2] = makeColour(faces[2][i + (6 - j * 3)])[2];

            colour[1][i][j][0] = makeColour(faces[4][(2 - i) + (6 - j * 3)])[0];
            colour[1][i][j][1] = makeColour(faces[4][(2 - i) + (6 - j * 3)])[1];
            colour[1][i][j][2] = makeColour(faces[4][(2 - i) + (6 - j * 3)])[2];

            colour[2][i][j][0] = makeColour(faces[3][(6 - i * 3) + j])[0];
            colour[2][i][j][1] = makeColour(faces[3][(6 - i * 3) + j])[1];
            colour[2][i][j][2] = makeColour(faces[3][(6 - i * 3) + j])[2];

            colour[3][i][j][0] = makeColour(faces[1][(6 - i * 3) + (2 - j)])[0];
            colour[3][i][j][1] = makeColour(faces[1][(6 - i * 3) + (2 - j)])[1];
            colour[3][i][j][2] = makeColour(faces[1][(6 - i * 3) + (2 - j)])[2];

            colour[4][i][j][0] = makeColour(faces[0][i + (6 - j * 3)])[0];
            colour[4][i][j][1] = makeColour(faces[0][i + (6 - j * 3)])[1];
            colour[4][i][j][2] = makeColour(faces[0][i + (6 - j * 3)])[2];

            colour[5][i][j][0] = makeColour(faces[5][i + j * 3])[0];
            colour[5][i][j][1] = makeColour(faces[5][i + j * 3])[1];
            colour[5][i][j][2] = makeColour(faces[5][i + j * 3])[2];
        }
    }
}

std::vector<GLfloat> RubiksCube::makeColour(int face) {
    std::vector<GLfloat> colourRGB (3);
    if (face == 0) {
        colourRGB[0] = 1.0;
        colourRGB[1] = 1.0;
        colourRGB[2] = 0.0;
    }
    if (face == 1) {
        colourRGB[0] = 1.0;
        colourRGB[1] = 0.55;
        colourRGB[2] = 0.0;
    }
    if (face == 2) {
        colourRGB[0] = 0.0;
        colourRGB[1] = 0.0;
        colourRGB[2] = 1.0;
    }
    if (face == 3) {
        colourRGB[0] = 1.0;
        colourRGB[1] = 0.0;
        colourRGB[2] = 0.0;
    }
    if (face == 4) {
        colourRGB[0] = 0.0;
        colourRGB[1] = 1.0;
        colourRGB[2] = 0.0;
    }
    if (face == 5) {
        colourRGB[0] = 1.0;
        colourRGB[1] = 1.0;
        colourRGB[2] = 1.0;
    }
    return colourRGB;
}

void RubiksCube::draw() {
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            glBegin(GL_POLYGON);
            glColor3f(colour[0][i + 1][j + 1][0], colour[0][i + 1][j + 1][1], colour[0][i + 1][j + 1][2]);
            glVertex3f(0.5 / 3 + i * (0.5 / 1.5), -0.5 / 3 + j * (0.5 / 1.5), -0.5 / 3);
            glVertex3f(0.5 / 3 + i * (0.5 / 1.5), 0.5 / 3 + j * (0.5 / 1.5), -0.5 / 3);
            glVertex3f(-0.5 / 3 + i * (0.5 / 1.5), 0.5 / 3 + j * (0.5 / 1.5), -0.5 / 3);
            glVertex3f(-0.5 / 3 + i * (0.5 / 1.5), -0.5 / 3 + j * (0.5 / 1.5), -0.5 / 3);
            glEnd();

            glBegin(GL_POLYGON);
            glColor3f(colour[1][i + 1][j + 1][0], colour[1][i + 1][j + 1][1], colour[1][i + 1][j + 1][2]);
            glVertex3f(0.5 / 3 + i * (0.5 / 1.5), -0.5 / 3 + j * (0.5 / 1.5), 0.5 / 3 + 2 * (0.5 / 1.5));
            glVertex3f(0.5 / 3 + i * (0.5 / 1.5), 0.5 / 3 + j * (0.5 / 1.5), 0.5 / 3 + 2 * (0.5 / 1.5));
            glVertex3f(-0.5 / 3 + i * (0.5 / 1.5), 0.5 / 3 + j * (0.5 / 1.5), 0.5 / 3 + 2 * (0.5 / 1.5));
            glVertex3f(-0.5 / 3 + i * (0.5 / 1.5), -0.5 / 3 + j * (0.5 / 1.5), 0.5 / 3 + 2 * (0.5 / 1.5));
            glEnd();
        }

        for (int j = 0; j < 3; j++) {
            glBegin(GL_POLYGON);
            glColor3f(colour[2][i + 1][j][0], colour[2][i + 1][j][1], colour[2][i + 1][j][2]);
            glVertex3f(0.5 / 3 + (0.5 / 1.5), -0.5 / 3 + i * (0.5 / 1.5), -0.5 / 3 + j * (0.5 / 1.5));
            glVertex3f(0.5 / 3 + (0.5 / 1.5), 0.5 / 3 + i * (0.5 / 1.5), -0.5 / 3 + j * (0.5 / 1.5));
            glVertex3f(0.5 / 3 + (0.5 / 1.5), 0.5 / 3 + i * (0.5 / 1.5), 0.5 / 3 + j * (0.5 / 1.5));
            glVertex3f(0.5 / 3 + (0.5 / 1.5), -0.5 / 3 + i * (0.5 / 1.5), 0.5 / 3 + j * (0.5 / 1.5));
            glEnd();

            glBegin(GL_POLYGON);
            glColor3f(colour[3][i + 1][j][0], colour[3][i + 1][j][1], colour[3][i + 1][j][2]);
            glVertex3f(-0.5 / 3 - (0.5 / 1.5), -0.5 / 3 + i * (0.5 / 1.5), 0.5 / 3 + j * (0.5 / 1.5));
            glVertex3f(-0.5 / 3 - (0.5 / 1.5), 0.5 / 3 + i * (0.5 / 1.5), 0.5 / 3 + j * (0.5 / 1.5));
            glVertex3f(-0.5 / 3 - (0.5 / 1.5), 0.5 / 3 + i * (0.5 / 1.5), -0.5 / 3 + j * (0.5 / 1.5));
            glVertex3f(-0.5 / 3 - (0.5 / 1.5), -0.5 / 3 + i * (0.5 / 1.5), -0.5 / 3 + j * (0.5 / 1.5));
            glEnd();

        }

        for (int j = 0; j < 3; j++) {
            glBegin(GL_POLYGON);
            glColor3f(colour[4][i + 1][j][0], colour[4][i + 1][j][1], colour[4][i + 1][j][2]);
            glVertex3f(0.5 / 3 + i * (0.5 / 1.5), 0.5 / 3 + (0.5 / 1.5), 0.5 / 3 + j * (0.5 / 1.5));
            glVertex3f(0.5 / 3 + i * (0.5 / 1.5), 0.5 / 3 + (0.5 / 1.5), -0.5 / 3 + j * (0.5 / 1.5));
            glVertex3f(-0.5 / 3 + i * (0.5 / 1.5), 0.5 / 3 + (0.5 / 1.5), -0.5 / 3 + j * (0.5 / 1.5));
            glVertex3f(-0.5 / 3 + i * (0.5 / 1.5), 0.5 / 3 + (0.5 / 1.5), 0.5 / 3 + j * (0.5 / 1.5));
            glEnd();

            glBegin(GL_POLYGON);
            glColor3f(colour[5][i + 1][j][0], colour[5][i + 1][j][1], colour[5][i + 1][j][2]);
            glVertex3f(0.5 / 3 + i * (0.5 / 1.5), -0.5 / 3 - (0.5 / 1.5), -0.5 / 3 + j * (0.5 / 1.5));
            glVertex3f(0.5 / 3 + i * (0.5 / 1.5), -0.5 / 3 - (0.5 / 1.5), 0.5 / 3 + j * (0.5 / 1.5));
            glVertex3f(-0.5 / 3 + i * (0.5 / 1.5), -0.5 / 3 - (0.5 / 1.5), 0.5 / 3 + j * (0.5 / 1.5));
            glVertex3f(-0.5 / 3 + i * (0.5 / 1.5), -0.5 / 3 - (0.5 / 1.5), -0.5 / 3 + j * (0.5 / 1.5));
            glEnd();
        }
    }
}