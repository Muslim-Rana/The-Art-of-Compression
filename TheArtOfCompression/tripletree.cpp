/**
 * @file        tripletree.cpp
 * @description Student-implemented functions of a ternary tree for CPSC 221 PA3.
 *
 *              THIS FILE WILL BE SUBMITTED FOR GRADING
 */

#include "tripletree.h"

 /**
      * Constructor that builds a TripleTree out of the given PNG.
      *
      * The TripleTree represents the subimage from (0,0) to (w-1, h-1) where
      * w-1 and h-1 are the largest valid image coordinates of the original PNG.
      * Every node corresponds to a rectangle of pixels in the original PNG,
      * represented by an (x,y) pair for the upper left corner of the
      * square and two integers for the number of pixels on the width and
      * height dimensions of the rectangular region the node defines.
      *
      * The node's three children correspond to a partition
      * of the node's rectangular region into three approximately equal-size strips.
      *
      * If the rectangular region is taller than it is wide, the region is divided
      * into horizontal strips:
      *  +-------+
      *  |   A   |
      *  |       |
      *  +-------+
      *  |   B   |
      *  |       |    (Split tall)
      *  +-------+
      *  |   C   |
      *  |       |
      *  +-------+
      *
      * If the rectangular region is wider than it is tall, the region is divided
      * into vertical strips:
      *  +---------+---------+---------+
      *  |    A    |    B    |    C    |
      *  |         |         |         |    (Split wide)
      *  +---------+---------+---------+
      *
      * Your regions are not guaranteed to have dimensions exactly divisible by 3.
      * If the dimensions of your rectangular region are 3p x q or q x 3p where 3p
      * represents the length of the long side, then your rectangular regions will
      * each have dimensions p x q (or q x p)
      *
      * If the dimensions are (3p+1) x q, subregion B gets the extra pixel of size
      * while subregions A and C have dimensions p x q.
      *
      * If the dimensions are (3p+2) x q, subregions A and C each get an extra pixel
      * of size, while subregion B has dimensions p x q.
      *
      * If the region to be divided is a square, then apply the Split wide behaviour.
      *
      * Every leaf in the constructed tree corresponds to a pixel in the PNG.
      *
      * @param imIn - the input image used to construct the tree
      */
TripleTree::TripleTree(PNG& imIn) {
    root = BuildNode(imIn, pair<unsigned int, unsigned int>(0,0), imIn.width(), imIn.height());
}

/**
 * Render returns a PNG image consisting of the pixels
 * stored in the tree. It may be used on pruned trees. Draws
 * every leaf node's rectangle onto a PNG canvas using the
 * average color stored in the node.
 *
 * You may want a recursive helper function for this.
 */
PNG TripleTree::Render() const {
    PNG image(root->width, root->height);
    renderHelper(image, root);
    return image;
}

/*
 * Prune function trims subtrees as high as possible in the tree.
 * A subtree is pruned (cleared) if all of its leaves are within
 * tol of the average color stored in the root of the subtree.
 * Pruning criteria should be evaluated on the original tree, not
 * on a pruned subtree. (we only expect that trees would be pruned once.)
 *
 * You may want a recursive helper function for this.
 *
 * @param tol - maximum allowable RGBA color distance to qualify for pruning
 */
void TripleTree::Prune(double tol) {
    // add your implementation below
	pruneHelper(root, tol);
}

/**
 * Rearranges the tree contents so that when rendered, the image appears
 * to be mirrored horizontally (flipped over a vertical axis).
 * This may be called on pruned trees and/or previously flipped/rotated trees.
 *
 * You may want a recursive helper function for this.
 */
void TripleTree::FlipHorizontal() {
    // add your implementation below
    root = flipHelper(root);
    
	
}

/**
 * Rearranges the tree contents so that when rendered, the image appears
 * to be rotated 90 degrees counter-clockwise.
 * This may be called on pruned trees and/or previously flipped/rotated trees.
 *
 * You may want a recursive helper function for this.
 */
void TripleTree::RotateCCW() {
    // add your implementation below
    rotateHelper(root);
	
}

/*
 * Returns the number of leaf nodes in the tree.
 *
 * You may want a recursive helper function for this.
 */
int TripleTree::NumLeaves() const {
    // replace the line below with your implementation
    return numLeavesHelper(root);
}

/**
     * Destroys all dynamically allocated memory associated with the
     * current TripleTree object. To be completed for PA3.
     * You may want a recursive helper function for this one.
     */
void TripleTree::Clear() {
    // add your implementation below
	clearHelper(root);
}

/**
 * Copies the parameter other TripleTree into the current TripleTree.
 * Does not free any memory. Called by copy constructor and operator=.
 * You may want a recursive helper function for this one.
 * @param other - The TripleTree to be copied.
 */
void TripleTree::Copy(const TripleTree& other) {
    // add your implementation below

    //no memory freeing yet
    root = copyHelper(other.root);
}

/**
 * Private helper function for the constructor. Recursively builds
 * the tree according to the specification of the constructor.
 * @param im - reference image used for construction
 * @param ul - upper left point of node to be built's rectangle.
 * @param w - width of node to be built's rectangle.
 * @param h - height of node to be built's rectangle.
 */
Node* TripleTree::BuildNode(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
    if (w == 0 || h == 0) {
        return nullptr;
    }
    if (w == 1 && h == 1) {
        Node* node = new Node(ul, w, h);
        node->avg = *im.getPixel(ul.first, ul.second);
        return node;
    }

    if (h > w) {
        Node* node = new Node(ul, w, h);
        Node* A = nullptr;
        Node* B = nullptr;
        Node* C = nullptr;


        if (h % 3 == 0) {
            A = BuildNode(im, ul, w, h/3);
            B = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second + (h/3)), w, h/3);
            C = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second + 2 * (h/3)), w, h/3);  
        }
        else if (h % 3 == 1) {
            A = BuildNode(im, ul, w, h/3);
            B = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second + (h/3)), w, (h/3) + 1);
            C = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second + (2 * (h/3)) + 1), w, h/3);  
        }
        else if (h % 3 == 2) {
            A = BuildNode(im, ul, w, h/3 + 1);
            B = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second + (h/3) + 1), w, h/3);
            C = BuildNode(im, pair<unsigned int, unsigned int>(ul.first, ul.second + (2 * (h/3)) + 1), w, (h/3) + 1);  
        }



        node->A = A;
        node->B = B;
        node->C = C;
        
        if (B != nullptr) {
            unsigned int avgR = (A->width * A->height * A->avg.r) + (B->width * B->height * B->avg.r) + (C->width * C->height * C->avg.r);
            unsigned int avgG = (A->width * A->height * A->avg.g) + (B->width * B->height * B->avg.g) + (C->width * C->height * C->avg.g);
            unsigned int avgB = (A->width * A->height * A->avg.b) + (B->width * B->height * B->avg.b) + (C->width * C->height * C->avg.b);
            unsigned int avgA = (A->width * A->height * A->avg.a) + (B->width * B->height * B->avg.a) + (C->width * C->height * C->avg.a);
            unsigned int totalSize = node->width * node->height;

            node->avg = RGBAPixel(avgR/totalSize, avgG/totalSize, avgB/totalSize, avgA/totalSize);
        }
        else {
            unsigned int avgR = (A->width * A->height * A->avg.r) + (C->width * C->height * C->avg.r);
            unsigned int avgG = (A->width * A->height * A->avg.g) + (C->width * C->height * C->avg.g);
            unsigned int avgB = (A->width * A->height * A->avg.b) + (C->width * C->height * C->avg.b);
            unsigned int avgA = (A->width * A->height * A->avg.a) + (C->width * C->height * C->avg.a);
            unsigned int totalSize = node->width * node->height;

            node->avg = RGBAPixel(avgR/totalSize, avgG/totalSize, avgB/totalSize, avgA/totalSize);
        }
        

        return node;
    }
    else {
        Node* node = new Node(ul, w, h);
        Node* A = nullptr;
        Node* B = nullptr;
        Node* C = nullptr;

        if (w % 3 == 0) {
            A = BuildNode(im, ul, w/3, h);
            B = BuildNode(im, pair<unsigned int, unsigned int>(ul.first + (w/3), ul.second), w/3, h);
            C = BuildNode(im, pair<unsigned int, unsigned int>(ul.first + (2 * (w/3)), ul.second), w/3, h);  
        }
        else if (w % 3 == 1) {
            A = BuildNode(im, ul, w/3, h);
            B = BuildNode(im, pair<unsigned int, unsigned int>(ul.first + (w/3), ul.second), (w/3) + 1, h);
            C = BuildNode(im, pair<unsigned int, unsigned int>(ul.first + (2 * (w/3)) + 1, ul.second), (w/3), h);  
        }
        else if (w % 3 == 2) {
            A = BuildNode(im, ul, w/3 + 1, h);
            B = BuildNode(im, pair<unsigned int, unsigned int>(ul.first + (w/3) + 1, ul.second), w/3, h);
            C = BuildNode(im, pair<unsigned int, unsigned int>(ul.first + (2 * (w/3)) + 1, ul.second), (w/3) + 1, h);  
        }

        node->A = A;
        node->B = B;
        node->C = C;
        
        if (B != nullptr) {
            unsigned int avgR = (A->width * A->height * A->avg.r) + (B->width * B->height * B->avg.r) + (C->width * C->height * C->avg.r);
            unsigned int avgG = (A->width * A->height * A->avg.g) + (B->width * B->height * B->avg.g) + (C->width * C->height * C->avg.g);
            unsigned int avgB = (A->width * A->height * A->avg.b) + (B->width * B->height * B->avg.b) + (C->width * C->height * C->avg.b);
            unsigned int avgA = (A->width * A->height * A->avg.a) + (B->width * B->height * B->avg.a) + (C->width * C->height * C->avg.a);
            unsigned int totalSize = node->width * node->height;

            node->avg = RGBAPixel(avgR/totalSize, avgG/totalSize, avgB/totalSize, avgA/totalSize);
        }
        else {
            unsigned int avgR = (A->width * A->height * A->avg.r) + (C->width * C->height * C->avg.r);
            unsigned int avgG = (A->width * A->height * A->avg.g) + (C->width * C->height * C->avg.g);
            unsigned int avgB = (A->width * A->height * A->avg.b) + (C->width * C->height * C->avg.b);
            unsigned int avgA = (A->width * A->height * A->avg.a) + (C->width * C->height * C->avg.a);
            unsigned int totalSize = node->width * node->height;

            node->avg = RGBAPixel(avgR/totalSize, avgG/totalSize, avgB/totalSize, avgA/totalSize);
        }
        

        return node;

    }


    return nullptr;
    


}

/* ===== IF YOU HAVE DEFINED PRIVATE MEMBER FUNCTIONS IN tripletree_private.h, IMPLEMENT THEM HERE ====== */

void TripleTree::renderHelper(PNG& image, Node* node) const {
    if (node == nullptr) {
        return;
    }
    if (node->A == nullptr && node->B == nullptr && node->C == nullptr) {
        for (unsigned int x = 0; x < node->width; x++) {
            for (unsigned int y = 0; y < node->height; y++) {
                *(image.getPixel(node->upperleft.first + x, node->upperleft.second + y)) = node->avg;
            }
        }
    }
    
    renderHelper(image, node->A);
    renderHelper(image, node->B);
    renderHelper(image, node->C);
    
}


Node* TripleTree::copyHelper(Node* otherRoot) {
    if (otherRoot == nullptr) {
        return nullptr;
    }
    if (otherRoot->A == nullptr && otherRoot->B == nullptr && otherRoot->C == nullptr) {
        Node* newNode = new Node(otherRoot->upperleft, otherRoot->width, otherRoot->height);
        newNode->avg = otherRoot->avg;
        return newNode;
    }

    Node* childA = copyHelper(otherRoot->A);
    Node* childB = copyHelper(otherRoot->B);
    Node* childC = copyHelper(otherRoot->C);

    Node* newNode = new Node(otherRoot->upperleft, otherRoot->width, otherRoot->height);
    newNode->avg = otherRoot->avg;
    newNode->A = childA;
    newNode->B = childB;
    newNode->C = childC;

    
    return newNode;
}


Node* TripleTree::flipHelper(Node* curr) {
    if (curr == nullptr) {
        return nullptr;
    }

    Node* newA = curr->A;
    Node* newB = curr->B;
    Node* newC = curr->C;

    if (curr->width >= curr->height && curr->C != nullptr && curr->A != nullptr) {
        Node* temp = curr->A;
        newA = curr->C;
        newC = temp;
        copyHeights(curr->A, curr->C);
    }

    curr->A = flipHelper(newA);
    curr->B = flipHelper(newB);
    curr->C = flipHelper(newC);


    return curr;
}

void TripleTree::copyHeights(Node* node1, Node* node2) {
    if (node1 == nullptr || node2 == nullptr) {
        return;
    }

    pair<unsigned int, unsigned int> temp = node1->upperleft;
    node1->upperleft = node2->upperleft;
    node2->upperleft = temp;

    copyHeights(node1->A, node2->A);
    copyHeights(node1->B, node2->B);
    copyHeights(node1->C, node2->C);
}


void TripleTree::rotateHelper(Node*& curr) {
    if (curr == nullptr) {
        return;
    }

    if (curr->width == curr->height) {
        if (curr->A && curr->A->width < curr->A->height) {
            Node* temp = curr->A;
            curr->A = curr->C;
            curr->C = temp;
        }


    }
    else if (curr->width > curr->height) {
        Node* temp = curr->A;
        curr->A = curr->C;
        curr->C = temp;
    }

    unsigned int cord1 = curr->upperleft.first;
    unsigned int cord2 = curr->upperleft.second;
    unsigned temp = curr->width;
    curr->width = curr->height;
    curr->height = temp;

    curr->upperleft = {cord2, root->height - cord1 - curr->height};

    if (curr->A) {
        rotateHelper(curr->A);
        rotateHelper(curr->B);
        rotateHelper(curr->C);
    }
    
}


void TripleTree::clearHelper(Node*& curr) {
    if (curr == nullptr) {
        return;
    } 

    clearHelper(curr->A);
    clearHelper(curr->B);
    clearHelper(curr->C);
    
    delete curr;
    curr = nullptr;
}

int TripleTree::numLeavesHelper(Node* curr) const {
    if (curr == nullptr) {
        return 0;
    }
    else if (curr->A == nullptr && curr->B == nullptr && curr->C == nullptr) {
        return 1;
    }

    return numLeavesHelper(curr->A) + numLeavesHelper(curr->B) + numLeavesHelper(curr->C);
}

void TripleTree::pruneHelper(Node* curr, double tol) {
    if (curr == nullptr) {
        return;
    }

    if (curr->A != nullptr && isZapable(curr, curr->avg, tol)) {
        clearHelper(curr->A);
        clearHelper(curr->B);
        clearHelper(curr->C);
    }
    else {
        pruneHelper(curr->A, tol);
        pruneHelper(curr->B, tol);
        pruneHelper(curr->C, tol);
    }


}

bool TripleTree::isZapable(Node* curr, RGBAPixel avg, double tol) {
    if (curr == nullptr) {
        return true;
    }

    if (curr->width == 1 && curr->height == 1) {
        return curr->avg.distanceTo(avg) <= tol;
    }

    return isZapable(curr->A, avg, tol) && isZapable(curr->B, avg, tol) && isZapable(curr->C, avg, tol);
}

  

