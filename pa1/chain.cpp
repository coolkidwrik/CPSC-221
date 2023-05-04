#include "chain.h"
#include <cmath>
#include <iostream>


// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain() {
  clear();
}

/**
 * Inserts a new node after the node pointed to by p in the
 * chain (so p->next is the new node) and returns a pointer to
 * the newly created node.
 * If p is NULL, inserts a new head node to the chain.
 * This function **SHOULD** create a new Node and increase length_.
 *
 * @param p = The new node should be pointed to by p->next.
 *            If p is NULL, the new node becomes the head of the chain.
 * @param ndata = The data to be inserted.
 */
Chain::Node * Chain::insertAfter(Node * p, const Block &ndata) {
  Node* nn = new Node(ndata);

  if (head_ == NULL) head_ = nn;
  else if (p == NULL) {
    nn->next = head_;
    head_->prev = nn;
    head_=nn;
  } 
  else {
    nn->prev = p;
    nn->next = p->next;
    p->next = nn;
    if (nn->next != NULL) nn->next->prev = nn;
  }

  length_++;
  return nn;
}

/**
 * Swaps the position in the chain of the two nodes pointed to
 * by p and q.
 * If p or q is NULL or p==q, do nothing.
 * Change the chain's head pointer if necessary.
 */
void Chain::swap(Node *p, Node *q) {
  if (p == NULL || q == NULL || p == q) return;

  if (p == head_) head_ = q;
  else if (q == head_) head_ = p;

  Node* t = p->next;
  p->next = q->next;
  q->next = t;

  if (p->next != NULL) p->next->prev = p;
  if (q->next != NULL) q->next->prev = q;

  t = p->prev;
  p->prev = q->prev;
  q->prev = t;

  if (p->prev != NULL) p->prev->next = p;
  if (q->prev != NULL) q->prev->next = q; 
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear() {
  Node* n;
  while (head_ != NULL) {
    n = head_;
    head_->prev = NULL;
    head_ = head_->next;
    n->next = NULL;
    delete n;
  }
  n = NULL;
}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const & other) {
  head_ = NULL;
  length_ = 0;

  if (other.head_ == NULL) return;
  
  Node* h = new Node(other.head_->data);
  Node* p = h;
  Node* c;
  head_ = h;
  length_++;

  for (Node* n = other.head_->next; n != NULL; n = n->next) {
    c = new Node(n->data);
    p->next = c;
    c->prev = p;
    p = c;
    c = c->next;
    length_++;
  }
}

/* Modifies the current chain:
 * 1) Find the node with the first (leftmost) block in the unscrambled
 *    image and move it to the head of the chain.
 *	This block is the one whose closest match (to the left) is the
 *	largest.  That is, the distance (using distanceTo) to this block
 *	is big for all other blocks.
 *	For each block B, find the distanceTo B from every other block
 *	and take the minimum of these distances as B's "value".
 *	Choose the block B with the maximum value over all blocks and
 *	swap its node to the head of the chain.
 *
 * 2) Starting with the (just found) first block B, find the node with
 *    the block that is the closest match to follow B (to the right)
 *    among the remaining blocks, move (swap) it to follow B's node,
 *    then repeat to unscramble the chain/image.
 */
 
void Chain::unscramble() {
  double maxPoints = 0;
  Node* nodeToChange;
  double score;
  double currScore;

  for (Node* left = head_; left != NULL; left = left->next) {
    score = 1000000;

    for (Node* right = head_; right != NULL; right = right->next) {
      if (left != right) {
        currScore = right->data.distanceTo(left->data);
        if (currScore < score) score = currScore;
      }
    }
    if (score > maxPoints) {
      nodeToChange = left;
      maxPoints = score;
    }
  }

  swap(head_, nodeToChange);

  for (Node* n = head_; n->next != NULL; n = n->next) {
    score = 1000000;

    for (Node* n2 = n; n2 != NULL; n2 = n2->next) {
      currScore = n->data.distanceTo(n2->data);
      if (currScore < score) {
        score = currScore;
        nodeToChange = n2;
      }
    }

    swap(n->next, nodeToChange);
  }
}
