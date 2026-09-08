#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "deq.h"

// keep track of items visited and freed
static int mapped_count;
static int freed_count;

// count an item when it is visited
static void count_item(Data d) {
  assert(d != 0);
  mapped_count++;
}

// free an item and count it
static void free_item(Data d) {
  free(d);
  freed_count++;
}

// change an integer into a string
static Str int_str(Data d) {
  char *s=0;

  if (asprintf(&s,"%d",*(int *)d) < 0)
    return 0;

  return s;
}

// make a new integer
static int *new_int(int value) {
  int *p=malloc(sizeof(*p));

  assert(p != 0);

  *p=value;
  return p;
}

// test an empty queue
static void test_empty_queue(void) {
  Deq q=deq_new();

  // check the starting length
  assert(deq_len(q) == 0);
  // try removing data that is not there
  assert(deq_head_rem(q,(Data)1) == 0);

  // check the empty string
  char *s=deq_str(q,0);

  assert(s != 0);
  assert(strcmp(s,"") == 0);

  free(s);
  deq_del(q,0);
}

// test the queue with strings
static void test_string_data(void) {
  Deq q=deq_new();

  char *a="alpha";
  char *b="beta";
  char *c="gamma";
  char *missing="missing";

  // add data from both ends
  deq_head_put(q,b);             // beta
  deq_head_put(q,a);             // alpha beta
  deq_tail_put(q,c);             // alpha beta gamma

  // check the length
  assert(deq_len(q) == 3);

  // check the order from the head
  assert(deq_head_ith(q,0) == a);
  assert(deq_head_ith(q,1) == b);
  assert(deq_head_ith(q,2) == c);

  // check the order from the tail
  assert(deq_tail_ith(q,0) == c);
  assert(deq_tail_ith(q,1) == b);
  assert(deq_tail_ith(q,2) == a);

  // check the queue string
  char *s=deq_str(q,0);

  assert(strcmp(s,"alpha beta gamma") == 0);
  free(s);

  // make sure every item is visited
  mapped_count=0;
  deq_map(q,count_item);
  assert(mapped_count == 3);

  // remove data from the middle
  assert(deq_head_rem(q,b) == b);
  assert(deq_len(q) == 2);

  // try removing data that is not there
  assert(deq_tail_rem(q,missing) == 0);
  assert(deq_len(q) == 2);

  // remove the last two items
  assert(deq_tail_get(q) == c);
  assert(deq_head_get(q) == a);
  assert(deq_len(q) == 0);

  deq_del(q,0);
}

// test one item and repeated data
static void test_duplicate_and_singleton(void) {
  Deq q=deq_new();

  char *x="x";
  char *y="y";

  // add and remove the only item
  deq_tail_put(q,x);
  assert(deq_head_get(q) == x);
  assert(deq_len(q) == 0);

  // add the same data more than once
  deq_tail_put(q,x);             // x
  deq_tail_put(q,x);             // x x
  deq_tail_put(q,y);             // x x y

  // remove an x starting from the tail
  assert(deq_tail_rem(q,x) == x);
  assert(deq_len(q) == 2);

  assert(deq_head_ith(q,0) == x);
  assert(deq_head_ith(q,1) == y);

  // add another x to the head
  deq_head_put(q,x);             // x x y

  // remove an x starting from the head
  assert(deq_head_rem(q,x) == x);
  assert(deq_len(q) == 2);

  // remove y from the queue
  assert(deq_head_rem(q,y) == y);
  assert(deq_len(q) == 1);

  deq_del(q,0);
}

// test data made with malloc
static void test_owned_data(void) {
  Deq q=deq_new();

  // make and add three integers
  int *one=new_int(1);
  int *two=new_int(2);
  int *three=new_int(3);

  deq_tail_put(q,one);
  deq_tail_put(q,two);
  deq_tail_put(q,three);

  // check the integer string
  char *s=deq_str(q,int_str);

  assert(s != 0);
  assert(strcmp(s,"1 2 3") == 0);

  free(s);

  // free and count all three integers
  freed_count=0;
  deq_del(q,free_item);

  assert(freed_count == 3);
}

int main(void) {
  // run all the tests
  test_empty_queue();
  test_string_data();
  test_duplicate_and_singleton();
  test_owned_data();

  puts("All queue tests passed.");

  return 0;
}
