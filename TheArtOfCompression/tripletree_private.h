/**
 * @file        tripletree_private.h
 * @description student declarations of private functions
 *              for TripleTree, CPSC 221 PA3
 *
 *              THIS FILE WILL BE SUBMITTED.
 *
 *              Simply declare your function prototypes here.
 *              No other scaffolding is necessary.
 */

 // begin your declarations below
 void renderHelper(PNG& image, Node* node) const;

 Node* copyHelper(Node* otherRoot);

 Node* flipHelper(Node* curr);

 void copyHeights(Node* node1, Node* node2);

 void rotateHelper(Node*& curr);

 void clearHelper(Node*& curr);

 int numLeavesHelper(Node* curr) const;

 void pruneHelper(Node* curr, double tol);

 bool isZapable(Node* curr, RGBAPixel avg, double tol);



