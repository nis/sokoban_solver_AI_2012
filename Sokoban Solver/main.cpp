//
//  main.cpp
//  Sokoban Solver
//
//  Created by Nis Sarup on 29/10/12.
//  Copyright (c) 2012 Nis Sarup. All rights reserved.
//

#include <stdlib.h>
#include <iostream>
#include "list"
#include "vector"
#include "map.h"
#include "move.h"
#include "path.h"

vector<Move> make_moves();
void run_solver(list<Path>& paths, vector<Move>& moves, list<Path>& succesful_paths, int& max_path_length, int& max_iterations, int& max_solutions, int& sort_every);

using namespace std;

void run_solver(list<Path>& paths, vector<Move>& moves, list<Path>& succesful_paths, int& max_path_length, int& max_iterations, int& max_solutions, int& sort_every) {
    int update_counter = 0;
    int sort_counter = 0;
    int i;
    for (i = 0; i < max_iterations; i++) {
        // Is the front path a succesfull path?
        if (paths.front().heuristic_score == 0) {
            succesful_paths.push_back(paths.front());
            
            // Have we got enough solutions?
            if (succesful_paths.size() >= max_solutions) {
                succesful_paths.sort();
                cout << "Max solutions found: " << succesful_paths.size() << endl;
                break;
            }
            
            paths.pop_front();
        } else {
            // Have we reached the maximum path length?
            if (paths.front().path_length < max_path_length) {
                paths.front().add_valid_next_moves(moves, paths);
                paths.pop_front();
                
                if (sort_counter >= sort_every) {
                    paths.sort();
                    sort_counter = 0;
                } else {
                    sort_counter++;
                }
            } else {
                // No need to sort if we have just discarded the front path.
                paths.pop_front();
            }
        }
        
        if (update_counter == 1000) {
            system( "clear" );
            cout << "Solutions found: " << succesful_paths.size() << endl;
            cout << "Tree size: " << paths.size() << endl;
            cout << "Current stat: " << paths.front().heuristic_score << "x" << paths.front().path_length << endl;
            paths.front().map.print_viewable();
            cout << flush;
            update_counter = 0;
        }
        update_counter++;
    }
    
    if (i >= max_iterations) {
        cout << "Max iterations reached." << endl;
    }
}

int main(int argc, const char * argv[])
{
    // Import and build the map:
    string map_file = "/Users/tamen/Documents/Archive/Skole/SDU/7Semester/AI/Documents/map_one_diamond.txt";
    map_file = "/Users/tamen/Documents/Archive/Skole/SDU/7Semester/AI/Documents/my_map_5_diamonds.txt";

    Map map(map_file);
    
    // Got the moves like Jagger!
    vector<Move> moves = make_moves();
    
    // Initiate the search tree
    list<Path> paths;
    Path start(map);
    paths.push_back(start);
    
    // Solutions go here
    list<Path> succesful_paths;
    
    // Parameters
    int max_path_length = 50;
    int max_iterations = 100000;
    int max_solutions = 10;
    int sort_every = 10; // How often should we sort the paths.
    
    // Run! Forrest, run!
    run_solver(paths, moves, succesful_paths, max_path_length, max_iterations, max_solutions, sort_every);
    
    // Lets see the solutions
    int solutions_size = (int)succesful_paths.size();
    succesful_paths.front().print_path();
//    for (int i = 1; i <= solutions_size; i++) {
//        cout << "Solution " << i <<  ". Path length: " << succesful_paths.front().path_length << endl;
//        succesful_paths.pop_front();
//    }
    
    std::cout << "Done!" << endl;
    return 0;
}

vector<Move> make_moves() {
    vector<Move> moves;
    Move up("Up", 'u', -1, 0);
    Move right("Right", 'r', 0, 1);
    Move left("Left", 'l', 0, -1);
    Move down("Down", 'd', 1, 0);
    moves.push_back(up);
    moves.push_back(right);
    moves.push_back(down);
    moves.push_back(left);
    return moves;
}