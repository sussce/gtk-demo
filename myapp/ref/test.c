#include <stdio.h>

int id_atomic;

typedef struct _T T;
typedef struct _L L;
typedef struct _Node Node;
typedef struct _Prop Prop;

struct _T {
  T* prev;
  T* next;
  T* sibling;
};
struct _L {
  L* next;
};
struct _Node {
  int id;
  Prop* props;

  T* t; 
};
struct _Prop {
  char* name;
  char* value;

  L* l;
};

#define t_init(t)

int main (int argc, char** argv) {
  T root,a,b,c,d,b1,c1,d1,b2,c2;
  Node na, nb, nc, nd, nb1, nc1, nd1, nb2, nc2;
  //;

  na.t = &a;
  
  root = t_init();
  
  root.next = &a;

  a.prev = &root;
  a.next = &b;  
  b.next = &c;
  c.next = &d;

  a.sibling = &b1;
  b1.next = &c1;
  c1,next = &d1;

  b1.sibling = &b2;
  b2.next = &c2;
  
  return 0;
}
