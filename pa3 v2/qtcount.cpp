#include "qtcount.h"

bool qtcount::prunable(Node * root, const int tol) const {
    /* Your code here */
    RGBAPixel r = root->avg;
    vector<Node*> stack;
    Node* currNode;
    stack.push_back(root);
    while (stack.size() != 0) {
        currNode = stack.back();
        stack.pop_back();
        if (currNode->NW != NULL) {
            stack.push_back(currNode->NW);
            stack.push_back(currNode->NE);
            stack.push_back(currNode->SW);
            stack.push_back(currNode->SE);
        }
        else {
            RGBAPixel pix = currNode->avg;
            if ((pow((r.r - pix.r), 2) + pow((r.g - pix.g), 2) + pow((r.b - pix.b), 2)) > tol) return false;
        }
    }
    return true;
};
