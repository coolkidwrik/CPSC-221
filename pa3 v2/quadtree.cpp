
/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */
#include <iostream>
#include "quadtree.h"
using namespace std;

// Node constructor, given.
quadtree::Node::Node(pair<int, int> ul, int d, RGBAPixel a, double v)
    : upLeft(ul), dim(d), avg(a), var(v), NW(nullptr), NE(nullptr), SE(nullptr), SW(nullptr) {}

// quadtree destructor, given.
quadtree::~quadtree() {
    clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree& other) {
    copy(other);
}
// quadtree assignment operator, given.
quadtree& quadtree::operator=(const quadtree& rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

quadtree::quadtree(PNG& imIn) {
    /* Your code here! */
    int logNewEdge = log2(min(imIn.width(), imIn.height()));
    int newEdge = pow(2, logNewEdge);
    imIn.resize(newEdge, newEdge);
    stats s(imIn);
    edge = newEdge;
    root = buildTree(s, {0,0}, log2(edge));
}

quadtree::Node* quadtree::buildTree(stats& s, pair<int, int> ul, int dim) {
    /* Your code here! */
    RGBAPixel avg = s.getAvg(ul, dim);
    double var = s.getVar(ul, dim);
    Node* nn = new Node(ul, dim, avg, var);
    if (dim == 0) {
        nn->NW = NULL;
        nn->NE = NULL;
        nn->SW = NULL;
        nn->SE = NULL;
        return nn;
    } else {
        int scale = pow(2, dim-1);
        pair<int, int> nep = {ul.first + scale, ul.second}; // north-east pair
        pair<int, int> swp = {ul.first, ul.second + scale}; // south-west pair
        pair<int, int> sep = {ul.first + scale, ul.second + scale}; // south-east pair
        nn->NW = buildTree(s, ul, dim - 1); // ul = nwp (north-west pair)
        nn->NE = buildTree(s, nep, dim - 1);
        nn->SW = buildTree(s, swp, dim - 1);
        nn->SE = buildTree(s, sep, dim -1);
        return nn;
    }
}

PNG quadtree::render() const {
    /* Your code here! */
    PNG p(edge, edge);
    vector<Node*> stack;
    stack.push_back(root);
    Node* currNode; 
    while (stack.size() != 0) {
        currNode = stack.back();
        stack.pop_back();
        if (currNode->NW == NULL) {
            int xOffset = currNode->upLeft.first;
            int yOffset = currNode->upLeft.second;
            for (int x = xOffset; x < xOffset + pow(2, currNode->dim); x++)
                for (int y = yOffset; y < yOffset + pow(2, currNode->dim); y++) {
                    *(p.getPixel(x, y)) = currNode->avg;
                    // cout << "LEAF AT: " << x << ", " << y << endl;
                }
        }
        else {
            stack.push_back(currNode->SW);
            stack.push_back(currNode->NW);
            stack.push_back(currNode->SE);
            stack.push_back(currNode->NE);
        }
    }
    return p;
}

int quadtree::idealPrune(const int leaves) const {
    // int tol = 0;
    // int pmin = 10000000;
    // while (pruneSize(tol) > leaves) {
    //     tol += 1000;
    //     // pmin = pruneSize(tol);
    // }
    // tol -= 1000;
    // // pmin = pruneSize(tol);

    // while (pruneSize(tol) > leaves) {
    //     tol += 100;
    //     // pmin = pruneSize(tol);
    // }
    // tol -= 100;
    // // pmin = pruneSize(tol);

    // while (pruneSize(tol) > leaves) {
    //     tol += 10;
    //     // pmin = pruneSize(tol);
    // }
    // tol -= 10;
    // pmin = pruneSize(tol);

    // while (pruneSize(tol) > leaves) {
    //     tol++;
    //     // pmin = pruneSize(tol);
    // }
    // return tol;
    int tol = 0;
    int pmin = 10000000;
    while (pruneSize(tol) > leaves) {
        tol += 1000;
    }
    if (tol-1000 >= 0){
        tol -= 1000;
    }
   

    while (pruneSize(tol) > leaves) {
        tol += 100;
      
    }
    if (tol-100 >= 0){
        tol -= 100;
    }


    while (pruneSize(tol) > leaves) {
        tol += 10;
  
    }
    if (tol-10 >= 0){
        tol -= 10;
    }
  

    while (pruneSize(tol) > leaves) {
        tol++;
        
    }
    return tol;
}

int quadtree::pruneSize(const int tol) const {
    /* Your code here! */
    return traverse(root, tol);
}

int quadtree::traverse(Node* n, const int tol) const {
    int count = 0;
    // if (n == NULL) return 0;
    
    // int count = traverse(n->NW, tol) + traverse(n->NE, tol) + traverse(n->SW, tol) + traverse(n->SE, tol);
    // if (prunable(n, tol)) {
    //     return 1 + count;
    // } else {
    //     return count;  // 0 + val
    // }
    if (n == NULL) return 0;

    if (n->NW ==NULL){
        return 1;
    }
    if (prunable(n, tol)) {
        return 1;
    } 
    else {
        count += traverse(n->NW, tol);
        count += traverse(n->NE, tol);
        count += traverse(n->SW, tol);
        count += traverse(n->SE, tol);
    }

    return count;
}

void quadtree::prune(const int tol) {
    pruneh(root, tol);
}

void quadtree::pruneh(Node* node, const int tol) {
    if (node == NULL) return;
    if (prunable(node, tol)) {
        clear_node(node->NW);
        clear_node(node->NE);
        clear_node(node->SW);
        clear_node(node->SE);
        node->NW = nullptr;
        node->NE = nullptr;
        node->SW = nullptr;
        node->SE = nullptr;
    } 
    else {
        pruneh(node->NW, tol);
        pruneh(node->NE, tol);
        pruneh(node->SW, tol);
        pruneh(node->SE, tol);
    }
}

void quadtree::clear() {
    /* your code here */
    clear_node(root);
}

// helper for clear
void quadtree::clear_node(Node* root) {
    if (root != NULL) {
        clear_node(root->NW);
        root->NW = nullptr;
        clear_node(root->NE);
        root->NE = nullptr;
        clear_node(root->SW);
        root->SW = nullptr;
        clear_node(root->SE);
        root->SE = nullptr;
        delete root;
    }
}

void quadtree::copy(const quadtree& orig) {
    /* your code here */
    root = copy_node(orig.root);
    edge = orig.edge;
}

//helper for copy
quadtree::Node* quadtree::copy_node(const Node* n) {
    if (n == NULL) {
        return nullptr;
    } else {
        Node* nn = new Node(n->upLeft, n->dim, n->avg, n->var);
        nn->NW = copy_node(n->NW);
        nn->NE = copy_node(n->NE);
        nn->SW = copy_node(n->SW);
        nn->SE = copy_node(n->SE);
        return nn;
    }
}

