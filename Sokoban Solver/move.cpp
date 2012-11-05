//
//  move.cpp
//  Sokoban Solver
//
//  Created by Nis Sarup on 30/10/12.
//  Copyright (c) 2012 Nis Sarup. All rights reserved.
//

#include "move.h"

Move::Move() {
    
}

Move::Move(string name, char symbol, int delta_x, int delta_y) {
    this->name = name;
    this->symbol = symbol;
    this->delta_x = delta_x;
    this->delta_y = delta_y;
}

void Move::get_new_pos(int& old_x, int& old_y, int& new_x, int& new_y, int& repeats) {
    for (int i = 0; i < repeats; i++) {
        this->get_new_pos(old_x, old_y, new_x, new_y);
    }
}

void Move::get_new_pos(int& old_x, int& old_y, int& new_x, int& new_y) {
    new_x = old_x + this->delta_x;
    new_y = old_y + this->delta_y;
}