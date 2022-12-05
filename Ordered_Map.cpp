#include "Ordered_Map.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

//NOTES
//balanced BST created by Maria Cardei for AVL Project 1 in cop3530
//code inspired from code presented in class and found at https://www.onlinegdb.com/Omm3kM-y6
//code additionally inspired from COP3530 Course Lectures

//functions aiding in destructor
void Ordered_Map::destruct(TreeNode* node){
    //does post-order-like traversal to delete nodes (LRRo).. happens when destructor is called
    if (node == nullptr){
        return;
    }
    //left
    if (node->left != nullptr)
        destruct(node->left);
    //right
    if (node->right != nullptr)
        destruct(node->right);
    //root
    delete node;
}

Ordered_Map::~Ordered_Map(){
    // This will be called in main to deallocate memory
    //automatically called since instance of ordered_map is local on the stack
    destruct(this->root);
}

//rotate functions
Ordered_Map::TreeNode* Ordered_Map::rotateLeft(Ordered_Map::TreeNode* grandparent){
    //rotate left in a right right case
    TreeNode* parent = grandparent->right;
    grandparent->right = parent->left;
    parent->left = grandparent;

    //update heights here
    grandparent->height = max(findHeight(grandparent->left), findHeight(grandparent->right)) + 1;
    parent->height = max(findHeight(parent->left), findHeight(parent->right)) + 1;

    return parent;
}

Ordered_Map::TreeNode* Ordered_Map::rotateRight(Ordered_Map::TreeNode* grandparent){
    //rotation in case of a left left case
    TreeNode* parent = grandparent->left;
    grandparent->left = parent->right;
    parent->right = grandparent;

    //update heights here
    grandparent->height = max(findHeight(grandparent->left), findHeight(grandparent->right)) + 1;
    parent->height = max(findHeight(parent->left), findHeight(parent->right)) + 1;

    return parent;
}

//Returns the height of a node!
int Ordered_Map::findHeight(TreeNode* node){
    //double checks it's not a nullptr
    if (node == nullptr){
        return 0;
    }
    else {
        return node->height;
    }
}

int Ordered_Map::bf(TreeNode* node){
    //find balance factor
    if (node == nullptr){
        return 0;
    }
    else {
        return findHeight(node->left) - findHeight(node->right);
    }
}

//search name helper function
set<string>* Ordered_Map::searchNameExists(Ordered_Map::TreeNode *helpRoot, const string& name) {
    //get names in preorder order and return in a set, to then be printed out by function calling this one
    if(helpRoot == nullptr)
        return nullptr;
    else
    {
        //root.. only add to the set if it is a node we want based on name input/parameter
        if (helpRoot->name == name){
            return &helpRoot->values;
        }
        //left
        else if (name < helpRoot->name)
            return searchNameExists(helpRoot->left, name);
            //if node name is bigger than that of the roots
        else
            return searchNameExists(helpRoot->right, name);
    }
}

set<string> Ordered_Map::findValues(const string &name) {
    set<string>* items_in_set = searchNameExists(this->root, name);

    //code for debugging !
//    if (items_in_set == nullptr) {
//        cout << "Attemped to searchName for " << name << ": no values for this name found." << endl;
//    }
//    else {
//        set<string>::iterator itr;
//        for (itr = items_in_set->begin(); itr != items_in_set->end(); itr++)
//        {
//            cout << *itr << " ";
//        }
//    }
    return *items_in_set;
}

set<string>* Ordered_Map::bracketSearchName(Ordered_Map::TreeNode *helpRoot, const string& name){
    //get names in preorder order and return in a set, to then be printed out by function calling this one
    if(helpRoot == nullptr)
        return nullptr;
    else
    {
        //root.. only add to the set if it is a node we want based on name input/parameter
        if (helpRoot->name == name){
            return &helpRoot->values;
        }
            //left
        else if (name < helpRoot->name)
            return searchNameExists(helpRoot->left, name);
            //if node name is bigger than that of the roots
        else
            return searchNameExists(helpRoot->right, name);
    }
}

//functions for inserting.. can only insert with brackets []
Ordered_Map::TreeNode* Ordered_Map::helperInsert(TreeNode* helpRoot, const string& name) {
    //case where the tree is empty
    if (helpRoot == nullptr)
        return new TreeNode(name);
        //if node id is smaller than that of the roots
    else if (name < helpRoot->name)
        helpRoot->left = helperInsert(helpRoot->left, name);
        //if node id is bigger than that of the roots
    else
        helpRoot->right = helperInsert(helpRoot->right, name);

    //update height!
    helpRoot->height = 1 + max(findHeight(helpRoot->left), findHeight(helpRoot->right));
    //check balance factor
    int balance = bf(helpRoot);

    //if tree is right heavy..
    if (balance < -1){
        //if trees right subtree is left heavy
        if (bf(helpRoot->right) >= 1){
            //right left -> rotate right then left
            helpRoot->right = rotateRight(helpRoot->right);
            return rotateLeft(helpRoot);
        }
        else {
            //right right -> rotate left
            return rotateLeft(helpRoot);
        }
    }
        //if tree is left heavy
    else if (balance > 1){
        //if left subtree is right heavy
        if (bf(helpRoot->left) <= -1){
            //left right -> rotate left then right
            helpRoot->left = rotateLeft(helpRoot->left);
            return rotateRight(helpRoot);
        }
        else {
            //left left -> right rotation
            return rotateRight(helpRoot);
        }
    }
    return helpRoot;
}

set<string>& Ordered_Map::operator[](const string& name){
    set<string>* pset = bracketSearchName(root, name);
    if (pset != nullptr){
        return *pset;
    }
    else {
        //root is nullptr
        root = helperInsert(root, name);
        pset = bracketSearchName(root, name);
        return *pset;
    }
}