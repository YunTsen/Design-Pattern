#ifndef ITERATOR_H
#define ITERATOR_H

//在定義類別時，如果只是需要使用到某個類別來宣告指標或是參考，
//但不涉及類別的生成或操作等訊息，則可以作該類別的前置宣告（Forward declaration），而不用含入該類別的定義
class Node;

class Iterator
{
public:
  virtual void first() = 0;
  virtual  Node* currentItem() = 0;
  virtual void next() = 0;
  virtual bool isDone() = 0;
};

#endif