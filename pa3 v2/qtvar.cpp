#include "qtvar.h"

bool qtvar::prunable(Node * root, const int tol) const {
    /* Your code here! */
    // A node is pruned if its variance is less than tolerance.
    return root->var < tol;
}
