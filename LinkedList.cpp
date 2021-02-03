#include "LinkedList.h"
#include "Arduino.h"
LinkedList::LinkedList() {
}

void LinkedList::Add(int data1, int data2) {
  Node* node;
  node->data1 = data1;
  node->data2 = data2;
  
  if(p_index) {
    p_head = p_index = node;
  } else {
    p_index->next = node;
    p_index = p_index->next;
  }
}

void LinkedList::Remove(Node* node) {
  Node* n = p_head;
  Node* pre;
  while(n && n->next) {
    pre = n;
    n = n->next;
  }
  if(n) {
    if(pre)
      pre->next = n->next;
    delete n;
  }
}

Node* LinkedList::Head() {
  return p_head;
}
