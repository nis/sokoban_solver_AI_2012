//
//  map.cpp
//  Sokoban Solver
//
//  Created by Nis Sarup on 29/10/12.
//  Copyright (c) 2012 Nis Sarup. All rights reserved.
//

#include "map.h"
#include <fstream>
#include <sstream>
#include "move.h"

int Map::update_placement() {
    int score = 0;
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (this->get_object(i, j) == 'M') {
                score = get_placement(i, j) + 1;
                set_placement(i, j, score);
            }
        }
    }
    return score;
}

int Map::calculate_diamond_placing_score() {
    int score = 0;
    
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (this->get_object(i, j) == 'J') {
                score += this->get_heuristic(i, j);
            }
        }
    }
    
    return score;
}

void Map::move_man(Move move) {
    int x, y, new_x, new_y;
    this->get_position_of_man(x, y);
    move.get_new_pos(x, y, new_x, new_y);
    
    // First move diamond if needed.
    if (this->get_object(new_x, new_y) == 'J') {
        int diamond_new_x, diamond_new_y;
        move.get_new_pos(new_x, new_y, diamond_new_x, diamond_new_y);
        
        if (this->get_area(diamond_new_x, diamond_new_y) == 'X') {
            cout << "Diamond pushed into wall!" << endl;
        }
        
        this->set_object(new_x, new_y, ' ');
        
        // Check if the diamond is in goal
        if (this->get_area(new_x, new_y) == 'G') {
            this->set_object(diamond_new_x, diamond_new_y, 'J');
        } else {
            this->set_object(diamond_new_x, diamond_new_y, 'J');
        }
    }
    
    this->set_object(x, y, ' ');
    this->set_object(new_x, new_y, 'M');
}

void Map::get_position_of_man(int& x, int& y) {
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (this->get_object(i, j) == 'M') {
                x = i;
                y = j;
            }
        }
    }
}

void Map::build_heuristics() {
    // Set goal
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (this->get_area(i, j) == 'G') {
                this->set_heuristic(i , j, 0);
            }
        }
    }
    
    int c = 0;
    
    while (c < this->world_map.size() * 1) {
        for (int i = 0; i < this->height; i++) {
            for (int j = 0; j < this->width; j++) {
                if (this->get_heuristic(i, j) > 0 && this->get_path(i, j) != 'X' && this->get_path(i, j) != ' ') {
                    if (i - 1 < this->world_map.size() && i - 1 >= 0) {
                        if (this->get_heuristic(i - 1, j) < this->get_heuristic(i , j)) {
                            this->set_heuristic(i , j, this->get_heuristic(i - 1, j) + 1);
                        }
                    }
                    if (i + 1 < this->world_map.size() && i + 1 >= 0) {
                        if (this->get_heuristic(i + 1, j) < this->get_heuristic(i , j)) {
                            this->set_heuristic(i , j, this->get_heuristic(i + 1, j) + 1);
                        }
                    }
                    if (j - 1 < this->world_map[0].size() && j - 1 >= 0) {
                        if (this->get_heuristic(i, j - 1) < this->get_heuristic(i , j)) {
                            this->set_heuristic(i , j, this->get_heuristic(i, j - 1) + 1);
                        }
                    }
                    if (j + 1 < this->world_map[0].size() && j + 1 >= 0) {
                        if (this->get_heuristic(i, j + 1) < this->get_heuristic(i , j)) {
                            this->set_heuristic(i, j, this->get_heuristic(i, j + 1) + 1);
                        }
                    }
                }
            }
        }
        c++;
    }
}

void Map::import(string path) {
    ifstream infile(path);
    string line;
    
    if (!infile) {
        cout << "Unable to open file datafile.txt" << endl;
    } else {
        getline( infile , line );
        stringstream ss (stringstream::in | stringstream::out);
        ss << line;
        ss >> this->width >> this->height >> this->number_of_diamonds;
        this->init_map(this->height, this->width);
        
        int line_end;
        int row = 0;
        while (row <= this->height) {
            getline( infile , line );
            if (line != "") {
                if (line.size() <= this->width + 1) {
                    line_end = (int)line.size() - 1;
                }
                for (int i = 0; i < line_end; i++) {
                    char temp_char = line[i];
                    if (temp_char == 'X' || temp_char == ' ' || temp_char == 'G' || temp_char == 'J' || temp_char == '.' || temp_char == 'M') {
                        
                        // Areas
                        if (temp_char == 'X' || temp_char == 'G') {
                            this->set_area(row, i, temp_char);
                        }
                        
                        // Objects
                        if (temp_char == 'M' || temp_char == 'J') {
                            this->set_object(row, i, temp_char);
                        }
                        
                        // Paths
                        if (temp_char == '.' || temp_char == 'G' || temp_char == 'M' || temp_char == 'J') {
                            this->set_path(row, i, '.');
                        }
                    }
                }
            }
            row++;
        }
        
        this->build_heuristics();
        this->update_placement();
    }
}

void Map::set_area(int x, int y, char value) {
    this->areas[x][y] = value;
}

char Map::get_area(int x, int y) {
    return this->areas[x][y];
}

void Map::set_object(int x, int y, char value) {
    this->objects[x][y] = value;
}

char Map::get_object(int x, int y) {
    return this->objects[x][y];
}

int Map::get_heuristic(int x, int y) {
    return this->heuristics[x][y];
}

void Map::set_heuristic(int x, int y, int value) {
    this->heuristics[x][y] = value;
}

int Map::get_placement(int x, int y) {
    return this->placement[x][y];
}

void Map::set_placement(int x, int y, int value) {
    this->placement[x][y] = value;
}

char Map::get_path(int x, int y) {
    return this->world_map[x][y];
}

void Map::set_path(int x, int y, char value) {
    this->world_map[x][y] = value;
}

Map::Map() {
}

Map::Map(int width, int height) {
    this->init_map(width, height);
}

Map::Map(string file) {
    this->import(file);
}

void Map::print_viewable() {
    cout << "Map:" << endl << "-------------------------------------" << endl;
    
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (this->get_area(i, j) == 'G') {
                if (get_object(i, j) == 'J') {
                    cout << 'H' << "\t";
                } else {
                    cout << this->get_area(i, j) << "\t";
                }
                
            } else if (this->get_object(i, j) == 'M' || this->get_object(i, j) == 'J') {
                cout << this->get_object(i, j) << "\t";
            } else if (this->get_path(i, j) == '.') {
                cout << this->get_path(i, j) << "\t";
            } else {
                cout << ' ' << "\t";
            }
            
        }
        cout << endl;
    }
    cout << "-------------------------------------" << endl << endl;
}

void Map::print_map() {
    cout << "Map:" << endl << "-------------------------------------" << endl;
    
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            cout << this->get_path(i, j) << "\t";
        }
        cout << endl;
    }
    cout << "-------------------------------------" << endl << endl;
}

void Map::print_objects() {
    cout << "Objects:" << endl << "-------------------------------------" << endl;
    
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
                cout << this->get_object(i, j) << "\t";
        }
        cout << endl;
    }
    cout << "-------------------------------------" << endl << endl;
}

void Map::print_areas() {
    cout << "Areas:" << endl << "-------------------------------------" << endl;
    
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            cout << this->get_area(i, j) << "\t";
        }
        cout << endl;
    }
    cout << "-------------------------------------" << endl << endl;
}

void Map::print_heuristics() {
    cout << "Heuristics:" << endl << "-------------------------------------" << endl;
    
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (this->get_heuristic(i, j) == 99) {
                cout << " \t";
            } else {
                cout << this->get_heuristic(i, j) << "\t";
            }
            
        }
        cout << endl;
    }
    cout << "-------------------------------------" << endl << endl;
}

void Map::print_placement() {
    cout << "Heuristics:" << endl << "-------------------------------------" << endl;
    
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (this->get_placement(i, j) == 0) {
                cout << " \t";
            } else {
                cout << this->get_placement(i, j) << "\t";
            }
            
        }
        cout << endl;
    }
    cout << "-------------------------------------" << endl << endl;
}

void Map::init_map(int width, int height) {
    for (int i = 0; i < width; i++) {
        vector<char> row(height);
        for (int j = 0; j < height; j++) {
            row[j] = ' ';
        }
        this->world_map.push_back(row);
    }
    
    for (int i = 0; i < width; i++) {
        vector<int> row(height);
        for (int j = 0; j < height; j++) {
            row[j] = 99;
        }
        this->heuristics.push_back(row);
    }
    
    for (int i = 0; i < width; i++) {
        vector<int> row(height);
        for (int j = 0; j < height; j++) {
            row[j] = 0;
        }
        this->placement.push_back(row);
    }
    
    for (int i = 0; i < width; i++) {
        vector<char> row(height);
        for (int j = 0; j < height; j++) {
            row[j] = ' ';
        }
        this->objects.push_back(row);
    }
    
    for (int i = 0; i < width; i++) {
        vector<char> row(height);
        for (int j = 0; j < height; j++) {
            row[j] = ' ';
        }
        this->areas.push_back(row);
    }
}