//
//  path.h
//  Sokoban Solver
//
//  Created by Nis Sarup on 30/10/12.
//  Copyright (c) 2012 Nis Sarup. All rights reserved.
//

#ifndef __Sokoban_Solver__path__
#define __Sokoban_Solver__path__

#include <iostream>
#include "map.h"
#include "move.h"
#include "list"
#include "vector"

using namespace std;

class Path {
private:
    Move next_move;
    
public:
    Map map;
    vector<Move> sequence;
    
    int heuristic_score = 1000;
    int path_length = 0;
    int placement_score = 0;
    
    Path();
    Path(Map map);
    bool is_valid_move(Move& move, vector<Move>& moves);
    void move(Move move);
    
    void add_valid_next_moves(vector<Move>& moves, list<Path>& paths);
    
    bool operator< (const Path& path) const;
    
    void print_path();
};

#endif /* defined(__Sokoban_Solver__path__) */
