//
//  move.h
//  Sokoban Solver
//
//  Created by Nis Sarup on 30/10/12.
//  Copyright (c) 2012 Nis Sarup. All rights reserved.
//

#ifndef __Sokoban_Solver__move__
#define __Sokoban_Solver__move__

#include <iostream>

using namespace std;

class Move {
    int delta_x;
    int delta_y;
    
public:
    string name;
    char symbol;
    
    Move();
    Move(string name, char symbol, int delta_x, int delta_y);
    void get_new_pos(int& old_x, int& old_y, int& new_x, int& new_y);
    void get_new_pos(int& old_x, int& old_y, int& new_x, int& new_y, int& repeats);
};

#endif /* defined(__Sokoban_Solver__move__) */
