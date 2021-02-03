#ifndef LinkedList_h
#define LinkedList_h

typedef struct Node {
  Node* next;
  int data1;
  int data2;
};

class LinkedList
{
  public:
    LinkedList(); // constructor
    
    void Add(int data1, int data2);
    void Remove(Node* node);
    
    Node* Head();
    
  private:
    Node* p_head;
    Node* p_index;
};

#endif
