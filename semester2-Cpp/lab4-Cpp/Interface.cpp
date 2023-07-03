#include "Interface.h"
#include <iostream>
#include <fstream>

Interface::Interface(RubiksCube &cube_) {
    cube = cube_;
}

void Interface::start(int argc, char *argv[]) {
    std::cout << "HELLO!!" << "\n";
    std::string command;
    while (std::cin >> command) {
        if (command == "read") {
            read();
        } else if (command == "print") {
            print();
        } else if (command == "shuffle") {
            shuffle();
        } else if (command == "rotate") {
            rotate();
        } else if (command == "solve") {
            solve();
        } else if (command == "help") {
            help();
        } else if (command == "end") {
            end();
            return;
        } else {
            std::cout << "Sorry, no matching command found. Try typing \"help\"\n";
        }
    }
}

void Interface::shuffle() {
    std::cout << "Randomized: ";
    cube.Shuffle();
    std::cout << "Done\n";
}

void Interface::read() {
    std::cout << "Enter file name: ";
    std::string fileRead;
    std::cin >> fileRead;
    std::ifstream in(fileRead);
    in >> cube;
    std::cout << "Done\n";
}

void Interface::print() {
    std::cout << "Print to console? (y/n)";
    std::string answer;
    std::cin >> answer;
    if (answer == "y") {
        std::cout << "Current state:\n\n" << cube;
    } else {
        std::cout << "Enter file name:";
        std::string filePrint;
        std::cin >> filePrint;
        std::ofstream out(filePrint);
        out << cube;
        std::cout << "Done";
    }
    std::cout << '\n';
}

void Interface::solve() {
    cube.Solve();
    std::cout << "Done\n";
}

void Interface::help() {
    std::cout << "Enter command. List of commands:\n" <<
              "read = reads cube's state from a file\n" <<
              "print = prints cube's state to a console or a file\n" <<
              "shuffle = shuffles the cube\n" <<
              "rotate = rotates the cube\n" <<
              "solve = solves a cube and prints steps to a console\n" <<
              "help = prints list of commands\n\n" <<
              "Colors as numbers:\n" <<
              "0 - yellow\n1 - orange\n2 - blue\n3 - red\n4 - green\n5 - white\n";
}

void Interface::end() {
    std::cout << "Good Bye!";
}

void Interface::rotate() {
    std::string face;
    std::cin >> face;
    if (face == "U") {
        cube.rotateUp(true);
    }
    if (face == "U'") {
        cube.rotateUp(false);
    }
    if (face == "D") {
        cube.rotateDown(true);
    }
    if (face == "D'") {
        cube.rotateDown(false);
    }
    if (face == "F") {
        cube.rotateFront(true, 2);
    }
    if (face == "F'") {
        cube.rotateFront(false, 2);
    }
    if (face == "B") {
        cube.rotateBack(true, 2);
    }
    if (face == "B'") {
        cube.rotateBack(false, 2);
    }
    if (face == "R") {
        cube.rotateRight(true, 2);
    }
    if (face == "R'") {
        cube.rotateRight(false, 2);
    }
    if (face == "L") {
        cube.rotateLeft(true, 2);
    }
    if (face == "L'") {
        cube.rotateLeft(false, 2);
    }
    std::cout << "Done\n";
}
