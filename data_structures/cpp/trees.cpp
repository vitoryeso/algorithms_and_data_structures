#include "./trees.h"
#include <iostream>

void copyNode(Node* n1, Node* n2) {
  n1->data = n2->data;  
  n1->parent = n2->parent;  
  n1->left = n2->left;  
  n1->right = n2->right;  
}

BST::BST() {
  this->root = nullptr;
}

BST::~BST() {
  if (this->root != nullptr) {
    this->root = nullptr;
  }
}

void BST::insert(const int p) {
  /* the new node will always have this content */
  Node newNode;
  newNode.data = p;
  newNode.left = nullptr;
  newNode.right = nullptr;

  /* empty BST */
  if (this->root == nullptr) {
    newNode.parent = nullptr;
    this->root = new Node;
    copyNode(this->root, &newNode);
    return;
  }

  Node* prov;
  prov = this->root;

  while (true) {
    /* go right */
    if (p >= prov->data) {
      /* keep going */
      if (prov->right != nullptr) {
        prov = prov->right;
      }
      /* insert and stop */
      else {
        newNode.parent = prov;
        prov->right = new Node;
        copyNode(prov->right, &newNode);
        break;
      }
    }
    /* go left */
    else if (p < prov->data) {
      /* keep going */
      if (prov->left != nullptr) {
        prov = prov->left;
      }
      /* insert and stop */
      else {
        newNode.parent = prov;
        prov->left = new Node;
        copyNode(prov->left, &newNode);
        break;
      }
    }
  }
}

void BST::remove(const int p) {
  //
}

bool BST::have(const int p) const {
  Node* prov = this->root;

  while (prov != nullptr) {
    /* finded */
    if (prov->data == p) return true;

    /* go right */
    else if (p > prov->data) {
      prov = prov->right;
    }

    /* go left */
    else {
      prov = prov->left;
    }
  }

  return false;
}

int BST::findMin() const {
  if (this->root == nullptr) return 0;
  Node* prov = this->root;  

  while(prov->left != nullptr) prov = prov->left;
  return prov->data;
}

int BST::findMax() const {
  if (this->root == nullptr) return 0;
  Node* prov = this->root;  

  while(prov->right != nullptr) prov = prov->right;
  return prov->data;
}
