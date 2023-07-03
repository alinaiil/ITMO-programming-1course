#include "rubikscube.h"
#pragma once
#include <iostream>

class Interface {
private:
    RubiksCube cube;
public:
    explicit Interface(RubiksCube& cube_);

    void start(int argc, char **argv);

    void shuffle();

    void read();

    void print();

    void rotate();

    void solve();

    static void help();

    void end();
};