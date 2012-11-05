//
//  map.h
//  Sokoban Solver
//
//  Created by Nis Sarup on 29/10/12.
//  Copyright (c) 2012 Nis Sarup. All rights reserved.
//

#ifndef __Sokoban_Solver__map__
#define __Sokoban_Solver__map__

#include <iostream>
#include "vector"
#include "move.h"

using namespace std;

class Map {
private:
    vector<vector<char>> world_map;
    vector<vector<int>> heuristics;
    vector<vector<int>> placement;
    vector<vector<char>> objects;
    vector<vector<char>> areas;
    int number_of_diamonds;
    int width;
    int height;
    
public:
    Map();
    Map(int width, int height);
    Map(string file);
    
    void set_path(int x, int y, char value);
    char get_path(int x, int y);
    
    void set_heuristic(int x, int y, int value);
    int get_heuristic(int x, int y);
    
    void set_placement(int x, int y, int value);
    int get_placement(int x, int y);
    
    void set_object(int x, int y, char value);
    char get_object(int x, int y);
    
    void set_area(int x, int y, char value);
    char get_area(int x, int y);
    
    void init_map(int width, int height);
    
    void print_map();
    void print_heuristics();
    void print_objects();
    void print_areas();
    void print_placement();
    void print_viewable();
    
    void import(string path);
    void build_heuristics();
    void get_position_of_man(int& x, int& y);
    void move_man(Move move);
    int calculate_diamond_placing_score();
    
    int update_placement();
};


#endif /* defined(__Sokoban_Solver__map__) */
