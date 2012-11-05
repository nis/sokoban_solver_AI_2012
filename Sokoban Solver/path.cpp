//
//  path.cpp
//  Sokoban Solver
//
//  Created by Nis Sarup on 30/10/12.
//  Copyright (c) 2012 Nis Sarup. All rights reserved.
//

#include "path.h"
#include "move.h"

// Used for sorting the list of paths.
bool Path::operator< (const Path& path) const {
    
    // Priority: heuristic_score > placement_score > path_length
    if (this->heuristic_score == path.heuristic_score) {
        if (this->placement_score == path.placement_score) {
            return this->path_length < path.path_length;
        } else {
            return this->placement_score < path.placement_score;
        }
    } else {
        return this->heuristic_score < path.heuristic_score;
    }
    
    // Priority: path_length > heuristic_score > placement_score NO GOOD!
//    if (this->path_length == path.path_length) {
//        if (this->heuristic_score == path.heuristic_score) {
//            return this->placement_score < path.placement_score;
//        } else {
//            return this->heuristic_score < path.heuristic_score;
//        }
//    } else {
//        return this->path_length < path.path_length;
//    }
}

void Path::print_path() {
    for (int i = 0; i < this->sequence.size(); i++) {
        cout << this->sequence[i].symbol << "";
    }
    cout << endl;
}

bool Path::is_valid_move(Move& move, vector<Move>& moves) {
    int valid_move = 1;
    int x, y, new_x, new_y;
    this->map.get_position_of_man(x, y);
    
    // Can we go where we want to?
    move.get_new_pos(x, y, new_x, new_y);
    char new_position_char = this->map.get_path(new_x, new_y);
    
    if (new_position_char != '.') {
        valid_move = 0;
    } else {
        
        // Are we pushing a diamond?
        if (this->map.get_object(new_x, new_y) == 'J') {
            
            // Check if we can move the diamond
            int triple_x, triple_y;
            move.get_new_pos(new_x, new_y, triple_x, triple_y);
            if (this->map.get_object(triple_x, triple_y) == 'J' || this->map.get_area(triple_x, triple_y) == 'X') {
                // We can't push more than one diamond in RL, so no need to implement it in the solver.
                // We can't push them through walls either.
                valid_move = 0;
            } else {
                
                // Check if the diamond is being pushed into a corner.
                // But only if it's not being pushed onto a goal.
                if (this->map.get_area(triple_x, triple_y) != 'G') {
                    // Four corners to check. This will also check if the diamond is pushed into a dead end.
                    
                    int up_x, up_y, left_x, left_y, down_x, down_y, right_x, right_y;
                    for (int jj = 0; jj < moves.size(); jj++) {
                        if (moves[jj].name == "Up") {
                            moves[0].get_new_pos(triple_x, triple_y, up_x, up_y);
                        }
                        if (moves[jj].name == "Down") {
                            moves[2].get_new_pos(triple_x, triple_y, down_x, down_y);
                        }
                        if (moves[jj].name == "Left") {
                            moves[3].get_new_pos(triple_x, triple_y, left_x, left_y);
                        }
                        if (moves[jj].name == "Right") {
                            moves[1].get_new_pos(triple_x, triple_y, right_x, right_y);
                        }
                    }
                    
                    
                    if (this->map.get_area(up_x, up_y) == 'X' && this->map.get_area(right_x, right_y) == 'X') {
                        valid_move = 0;
                        if (this->map.get_area(down_x, down_y) == 'X' && this->map.get_area(right_x, right_y) == 'X') {
                            valid_move = 0;
                            if (this->map.get_area(down_x, down_y) == 'X' && this->map.get_area(left_x, left_y) == 'X') {
                                valid_move = 0;
                                if (this->map.get_area(up_x, up_y) == 'X' && this->map.get_area(left_x, left_y) == 'X') {
                                    valid_move = 0;
                                }
                            }
                        }
                    }
                    
                    // Check if we are locking two diamonds next to each other
                    // XX
                    // JJ // No good
                    // XX
                    // .X
                    // JJ // Good
                    // .X
                    // X.
                    // JJ // Good
                    // X.
                    // And in the other direction as well.
                    int quad_x, quad_y;
                    move.get_new_pos(triple_x, triple_y, quad_x, quad_y);
                    
                    if (this->map.get_object(quad_x, quad_y) == 'J') {
                        int qup_x, qup_y, qleft_x, qleft_y, qdown_x, qdown_y, qright_x, qright_y;
                        for (int jj = 0; jj < moves.size(); jj++) {
                            if (moves[jj].name == "Up") {
                                moves[0].get_new_pos(quad_x, quad_y, qup_x, qup_y);
                            }
                            if (moves[jj].name == "Down") {
                                moves[2].get_new_pos(quad_x, quad_y, qdown_x, qdown_y);
                            }
                            if (moves[jj].name == "Left") {
                                moves[3].get_new_pos(quad_x, quad_y, qleft_x, qleft_y);
                            }
                            if (moves[jj].name == "Right") {
                                moves[1].get_new_pos(quad_x, quad_y, qright_x, qright_y);
                            }
                        }
                        
                        // Horizontal diamonds
                        if (this->map.get_path(qup_x, qup_y) != '.' && this->map.get_path(qdown_x, qdown_y) != '.' && (this->map.get_path(up_x, up_y) != '.' || this->map.get_path(down_x, down_y) != '.')) {
                            valid_move = 0;
                        }
                        
                        // Vertical diamonds
                        if (this->map.get_path(qleft_x, qleft_y) != '.' && this->map.get_path(qright_x, qright_x) != '.' && (this->map.get_path(left_x, left_y) != '.' || this->map.get_path(right_x, right_y) != '.')) {
                            valid_move = 0;
                        }
                    }
                    
                }
            }
        }
    }
    return valid_move;
}

void Path::add_valid_next_moves(vector<Move>& moves, list<Path>& paths) {
    
    for (int i = 0; i < moves.size(); i++) {
        if (this->is_valid_move(moves[i], moves)) {
            Path new_path(this->map);
            
            // Move
            new_path.sequence = this->sequence; // Add the old sequence to the new path
            new_path.move(moves[i]);
            
            // Calculate stats
            new_path.heuristic_score = new_path.map.calculate_diamond_placing_score();
            new_path.path_length = (int)new_path.sequence.size();
            
            paths.push_back(new_path);
        }
    }
}

void Path::move(Move move) {
    this->sequence.push_back(move);
    this->map.move_man(move);
    this->placement_score = this->map.update_placement();
}

Path::Path() {
    
}

Path::Path(Map map) {
    this->map = map;
}