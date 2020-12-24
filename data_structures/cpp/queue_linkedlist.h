class queue {
  private:
    struct Node {
      int data;
      Node* next;
    };

    Node* _head;
    Node* _tail; 

  public:
    queue();
    ~queue();

    void enqueue(int value);
    int dequeue();

    bool isEmpty() const;

    unsigned size() const;
    int head() const;
    int tail() const;
};
