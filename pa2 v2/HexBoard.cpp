#include "HexBoard.h"

#include <iostream>

#include "queue.h"
#include "stack.h"

HexBoard::HexBoard(int radius) {
    this->radius = radius;
    start_coord = {0,0};
    create_board();
    configure_cells();
    longest_path_dist = 0;
    vector<pair<int, int>> solution;
    map<pair<int, int>, HexCell*> cells;
    pair<int,  int> end_coord;
}

void HexBoard::create_board() {
    for (int q = -(radius-1); q <= radius-1; q++) {
        int r1 = max(-(radius-1), -q - (radius -1));
        int r2 = min((radius-1), -q + radius-1);
        for (int r = r1; r <= r2; r++) {
            HexCell* new_cell = new HexCell(q, r);
            cells[{q,r}] = new_cell;
        }
    }
}

void HexBoard::configure_cells() {
    for (auto c : cells) {
        HexCell* cell = c.second;
        int q = cell->q;
        int r = cell->r;
        if ((abs(q) + abs(q + r-1) + abs(r-1))/2 < radius) 
        cell->neighbours[0] = cells[{q, r-1}];       
        if ((abs(q+1) + abs(q + r) + abs(r-1))/2 < radius) 
        cell->neighbours[1] = cells[{q+1, r-1}];
        if ((abs(q+1) + abs(q+1 + r) + abs(r))/2 < radius) 
        cell->neighbours[2] = cells[{q+1, r}];
        if ((abs(q) + abs(q + r+1) + abs(r+1))/2 < radius) 
        cell->neighbours[3] = cells[{q, r+1}];
        if ((abs(q-1) + abs(q + r) + abs(r+1))/2 < radius) 
        cell->neighbours[4] = cells[{q-1, r+1}];
        if ((abs(q-1) + abs(q-1 + r) + abs(r))/2 < radius) 
        cell->neighbours[5] = cells[{q-1, r}];
    }
}

void HexBoard::generate_maze(pair<int, int> start_coord, double branch_prob) {
    this->start_coord = start_coord;
    map<pair<int, int>, bool> visited;
    Stack<HexCell*> stack;
    stack.push(cells[start_coord]);
    visited.insert({start_coord, false});
    HexCell *end_cell = nullptr;

    while (!stack.isEmpty()) {
        HexCell* curr_cell, *neigh_cell;
        curr_cell = stack.peek();
        neigh_cell = chooseNeighbour(curr_cell, visited, branch_prob);
        if (neigh_cell != nullptr) {
            curr_cell->link(neigh_cell);
            visited.insert({{neigh_cell->q, neigh_cell->r}, false});
            neigh_cell->path_dist_from_start = curr_cell->path_dist_from_start+1;
            if (neigh_cell->path_dist_from_start > longest_path_dist) {
                longest_path_dist = neigh_cell->path_dist_from_start;
            }
            if (neigh_cell->is_edge() && (end_cell == nullptr || neigh_cell->path_dist_from_start > end_cell->path_dist_from_start)) {
                end_cell = neigh_cell;
                end_coord = {neigh_cell->q, neigh_cell->r};
            }
            stack.push(neigh_cell);
        } 
        else stack.pop();
    }
}

void HexBoard::solve_maze() {
    pair<int, int> curr_coord;
    Quack<pair<int, int>> duckie;
    map<pair<int, int> , bool> visited;
    HexCell* curr_cell;
    bool check = true;
    duckie.pushR(start_coord);
    visited.insert({start_coord, false});

    while (true) {
        curr_coord = duckie.peekR();
        if (curr_coord == end_coord) break;
        curr_cell = cells[curr_coord];
        check = false;
        for (HexCell* link : curr_cell->links) {
            if (link != nullptr && visited.find({link->q, link->r}) == visited.end()) {
                check = true;
                duckie.pushR({link->q, link->r});
                visited.insert({{link->q, link->r}, false});
                break;
            }
        }
        if (!check) duckie.popR();
    }

    while (!duckie.isEmpty()) {
        solution.push_back(duckie.peekL());
        duckie.popL();
    }
}