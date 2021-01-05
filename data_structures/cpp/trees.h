#ifndef _TREES_H_
#define _TREES_H_

/* augmented node */
struct augNode {
  int data;
  int count;
  
  augNode* parent;
  augNode* left;
  augNode* right;
};

/* simple node */
struct Node {
  int data;
  
  Node* parent;
  Node* left;
  Node* right;
};

/* simple BST */
class BST {
  private:
    Node* root;
  public:
    BST();
    ~BST();

    void insert(const int p);
    void remove(const int p);
    bool have(const int p) const;

    int findMin() const;
    int findMax() const;
};

/* augmented BST */
class augBST {
  private:
    augNode* root;
  public:
    augBST();
    ~augBST();

    void insert(const int p);
    void remove(const int p);
    bool have(const int p) const;

    int findMin() const;
    int findMax() const;
};


#endif
