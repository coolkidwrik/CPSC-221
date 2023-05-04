#include "HexCell.h"

#include <cmath>

HexCell::HexCell(int Q, int R) {
    q = Q;
    r = R;
    path_dist_from_start = 0;
    for (int i = 0; i < 6; i++) {
        neighbours[i] = nullptr;
        links[i] = nullptr;
    }
}

void HexCell::link(HexCell* cell) {
    if (distance(cell) == 1) {
        int Q = cell->q, R = cell->r;
        if (Q-q == 0 && R-r == -1) {
            links[0] = cell;
            cell->links[0+3] = this;
        } 
        if (Q-q == 1 && R-r == -1) {
            links[1] = cell; 
            cell->links[1+3] = this;
        }
        if (Q-q == 1 && R-r == 0) {
            links[2] = cell;
            cell->links[2+3] = this;
        }
        if (Q-q == 0 && R-r == 1) {
            links[3] = cell; 
            cell->links[3-3] = this;
        } 
        if (Q-q == -1 && R-r == 1) {
            links[4] = cell; 
            cell->links[4-3] = this; 
        }
        if (Q-q == -1 && R-r == 0) {
            links[5] = cell; 
            cell->links[5-3] = this; 
        }
    }
}

int HexCell::get_neighbour_side(const HexCell* cell) const {
    for (int i = 0; i < 6; i++) if (neighbours[i] == cell) return i;
    return -1;
}

bool HexCell::is_linked(const HexCell* cell) const {
    if (cell == nullptr) return false;
    for (HexCell* l : links) {
        if (l == cell) return true;
    }
    return false;
}

int HexCell::distance(const HexCell* cell) const {
    if (cell == nullptr) return -1;
    return (abs(q - cell->q) + abs(r - cell->r) + abs(q+r - (cell->q + cell->r))) / 2;
}

bool HexCell::is_edge() const {
    for (auto n : neighbours) {
        if (n == nullptr) return true;
    }
    return false;
}