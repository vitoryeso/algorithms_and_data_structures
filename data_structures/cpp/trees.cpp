#include "./trees.h"
#include <iostream>

void copyNode(Node* n1, Node* n2) {
  n1->data = n2->data;
  n1->parent = n2->parent;
  n1->left = n2->left;
  n1->right = n2->right;
}

void copyNode(augNode* n1, augNode* n2) {
  n1->data = n2->data;
  n1->count = n2->count;
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

augBST::augBST() {
  this->root = nullptr;
}

augBST::~augBST() {
  if (this->root != nullptr) {
    this->root = nullptr;
  }
}

void augBST::insert(const int p) {
  /* the new node will always have this content */
  augNode newNode;

  newNode.count = 0;
  newNode.data = p;
  newNode.left = nullptr;
  newNode.right = nullptr;

  /* empty BST */
  if (this->root == nullptr) {
    newNode.parent = nullptr;
    this->root = new augNode;
    copyNode(this->root, &newNode);
    return;
  }

  augNode* prov;
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
        prov->right = new augNode;
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
        prov->left = new augNode;
        copyNode(prov->left, &newNode);
        break;
      }
    }
  }

  /* inverse path to increase the counter */
  while (prov->parent != nullptr) {
    prov->count++;
    prov = prov->parent;
  }
  prov->count++; // root node
}

void augBST::remove(const int p) {
  //
}

bool augBST::have(const int p) const {
  augNode* prov = this->root;

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

int augBST::findMin() const {
  if (this->root == nullptr) return 0;
  augNode* prov = this->root;  

  while(prov->left != nullptr) prov = prov->left;
  return prov->data;
}

int augBST::findMax() const {
  if (this->root == nullptr) return 0;
  augNode* prov = this->root;  

  while(prov->right != nullptr) prov = prov->right;
  return prov->data;
}
