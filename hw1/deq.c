#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deq.h"
#include "error.h"

// indices and size of array of node pointers
typedef enum {Head,Tail,Ends} End;

typedef struct Node {
  struct Node *np[Ends];        // next/prev neighbors
  Data data;
} *Node;

typedef struct {
  Node ht[Ends];                // head/tail nodes
  int len;
} *Rep;

static Rep rep(Deq q) {
  if (!q) ERROR("zero pointer");
  return (Rep)q;
}

// switch between the head and tail
static End opposite(End e) {
  return e == Head ? Tail : Head;
}

// add data to the selected end
static void put(Rep r, End e, Data d) {
  End o=opposite(e);

  // make space for the new node
  Node n=(Node)malloc(sizeof(*n));

  if (!n)
    ERROR("malloc() failed");

  // add the data and connect the new node
  n->data=d;
  n->np[e]=0;
  n->np[o]=r->ht[e];

  // connect the old end to the new node
  if (r->ht[e])
    r->ht[e]->np[e]=n;
  else
    // if the queue is empty, this node is both ends
    r->ht[o]=n;

  // make the new node the selected end and update the length
  r->ht[e]=n;
  r->len++;
}

// find a node using its index
static Node node_at(Rep r, End e, int i) {
  // make sure the queue is not empty
  if (!r->ht[e])
    ERROR("zero pointer");

  // make sure the index is valid
  if (i < 0 || i >= r->len)
    ERROR("index out of range");

  End o=opposite(e);
  Node n=r->ht[e];

  // move through the queue until the index is found
  while (i--)
    n=n->np[o];

  return n;
}

// return the data at an index
static Data ith(Rep r, End e, int i) {
  return node_at(r,e,i)->data;
}

// remove a node and return its data
static Data unlink_node(Rep r, Node n) {
  if (!n)
    return 0;

  // save the nodes on each side
  Node toward_head=n->np[Head];
  Node toward_tail=n->np[Tail];

  // reconnect the head side
  if (toward_head)
    toward_head->np[Tail]=toward_tail;
  else
    // there was no node toward the head, so the head was removed
    r->ht[Head]=toward_tail;

  // reconnect the tail side
  if (toward_tail)
    toward_tail->np[Head]=toward_head;
  else
    r->ht[Tail]=toward_head;

  // save the data before removing the node
  Data d=n->data;
  free(n);
  r->len--;

  return d;
}

// remove data from the selected end
static Data get(Rep r, End e) {
  // make sure the queue is not empty
  if (!r->ht[e])
    ERROR("zero pointer");

  return unlink_node(r,r->ht[e]);
}

// find and remove matching data
static Data rem(Rep r, End e, Data d) {
  End o=opposite(e);

  // search from the selected end
  for (Node n=r->ht[e]; n; n=n->np[o])
    if (n->data == d)
      return unlink_node(r,n);

  // return zero if the data was not found
  return 0;
}

extern Deq deq_new() {
  Rep r=(Rep)malloc(sizeof(*r));
  if (!r) ERROR("malloc() failed");
  r->ht[Head]=0;
  r->ht[Tail]=0;
  r->len=0;
  return r;
}

extern int deq_len(Deq q) { return rep(q)->len; }

extern void deq_head_put(Deq q, Data d) {        put(rep(q),Head,d); }
extern Data deq_head_get(Deq q)         { return get(rep(q),Head);   }
extern Data deq_head_ith(Deq q, int i)  { return ith(rep(q),Head,i); }
extern Data deq_head_rem(Deq q, Data d) { return rem(rep(q),Head,d); }

extern void deq_tail_put(Deq q, Data d) {        put(rep(q),Tail,d); }
extern Data deq_tail_get(Deq q)         { return get(rep(q),Tail);   }
extern Data deq_tail_ith(Deq q, int i)  { return ith(rep(q),Tail,i); }
extern Data deq_tail_rem(Deq q, Data d) { return rem(rep(q),Tail,d); }

extern void deq_map(Deq q, DeqMapF f) {
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail])
    f(n->data);
}

extern void deq_del(Deq q, DeqMapF f) {
  if (f) deq_map(q,f);
  Node curr=rep(q)->ht[Head];
  while (curr) {
    Node next=curr->np[Tail];
    free(curr);
    curr=next;
  }
  free(q);
}

extern Str deq_str(Deq q, DeqStrF f) {
  char *s=strdup("");
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail]) {
    char *d=f ? f(n->data) : n->data;
    char *t; asprintf(&t,"%s%s%s",s,(*s ? " " : ""),d);
    free(s); s=t;
    if (f) free(d);
  }
  return s;
}
