#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

// new RB-tree
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;

  return p;
}

// delete RB-tree
void free_walk(node_t *x, node_t *nil){
  if (x != nil){
    free_walk(x->left, nil);
    free_walk(x->right, nil);
    free(x);
  }
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free_walk(t->root, t->nil);
  free(t->nil);
  free(t);
}

// insert node
void left_rotate(rbtree *t, node_t *x){
  node_t *y = x->right;
  
  // beta
  x->right = y->left;
  if (y->left != t->nil){
    y->left->parent = x;
  }
  // x.p ~~ y
  y->parent = x->parent;
  if (x->parent == t->nil){
    t->root = y;
  }
  else if (x == x->parent->left) {
     x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }
  // x ~~ y
  y->left = x;
  x->parent = y;
}
void right_rotate(rbtree *t, node_t *y){
  node_t *x = y->left;

  // beta
  y->left = x->right;
  if (x->right != t->nil){
    x->right->parent = y;
  }
  // x.p ~~ y
  x->parent = y->parent;
  if (y->parent == t->nil){
    t->root = x;
  }
  else if (y == y->parent->left) {
     y->parent->left = x;
  }
  else {
    y->parent->right = x;
  }
  // x ~~ y
  x->right = y;
  y->parent = x;
}

void rbtree_insert_fixup(rbtree *t, node_t *z){
  node_t *y; // z의 삼촌 y.
  while (z->parent->color == RBTREE_RED) {
    if (z->parent == z->parent->parent->left){
      y = z->parent->parent->right;
      if (y->color == RBTREE_RED) {   // 경우 1 : 적색 삼촌.
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else{                           // 흑색 삼촌. 
        if (z == z->parent->right) {  // 경우 2 : 흑색 삼촌. z는 오른쪽 자식.
          z = z->parent;  // 경우 3가 되기 위해(=왼쪽자식이 되기위해) 회전하기전 미리 z포인터를 조정.
          left_rotate(t, z);
        }
        // 경우 3 : 흑색 삼촌. z는 왼쪽 자식.
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    else{ // z->parent == z->parent->parent->right
      y = z->parent->parent->left;
      if (y->color == RBTREE_RED) {   // 경우 1 : 적색 삼촌.
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else{                           // 흑색 삼촌. 
        if (z == z->parent->left) {   // 경우 2 : 흑색 삼촌. z는 왼쪽 자식.
          z = z->parent;  // 경우 3가 되기 위해(=왼쪽자식이 되기위해) 회전하기전 미리 z포인터를 조정.
          right_rotate(t, z);
        }
        // 경우 3 : 흑색 삼촌. z는 오른쪽 자식.
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *y = t->nil;
  node_t *x = t->root;
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;

  while (x != t->nil) {   // 왼쪽 오른쪽 따지며 리프까지 내려감.
    y = x;
    if (z->key < x->key) {
      x = x->left;
    }
    else {
      x = x->right;
    }
  }

  // z ~~ z.p와 관계 설정
  z->parent = y;          // x의 자리에 z가 들어가게 됨.
  if (y == t->nil) {      // t가 빈 트리였을 경우.
    t->root = z;
  }
  else if (z->key < y->key) {
    y->left = z;
  }
  else {
    y->right = z;
  }
  // z ~~ (z.left, z.right) 관계 설정 : 리프노드가 되므로 nil로 설정.
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  rbtree_insert_fixup(t, z);
  
  return z;
}

// find node
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *x = t->root;
  while (x != t->nil && key != x->key) {
    if (key < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }
  if (x == t->nil) { return NULL; }
  else { return x; }
}

// min, max
node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *x = t->root;
  while (x->left != t->nil) {
    x = x->left;
  }
  return x;
}
node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *x = t->root;
  while (x->right != t->nil) {
    x = x->right;
  }
  return x;
}

// erase node
// node_t *subtree_min(node_t *x, node_t *nil){
//   while (x->left != nil) {
//     x = x->left;
//   }
//   return x;
// }
void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  if (u->parent == t->nil) {
    t->root = v;
  }
  else if (u == u->parent->left) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}
void rbtree_erase_fixup(rbtree *t, node_t *x){
  node_t *w;    // w는 x의 sibling.
  while (x != t->root && x->color == RBTREE_BLACK) {
    if (x == x->parent->left) {
      w = x->parent->right;
      // w 는 RED
      if (w->color == RBTREE_RED) { // 경우 1 : w가 RED인 경우.
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      // w 는 BLACK
      // 경우 2 : w는 BLACK. w의 자식 둘다 BLACK.
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        // 경우 3 : w는 BLACK. w 왼쪽자식 RED, 오른쪽자식 BLACK.
        if (w->right->color == RBTREE_BLACK) {
        w->left->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        right_rotate(t,w);
        w = x->parent->right;
        }
        // 경우 4 : w는 BLACK. w의 오른쪽자식 RED.
        w->color = x->parent->color;
        x->parent->color = w->right->color = RBTREE_BLACK;
        left_rotate(t,x->parent);
        x = t->root;
      }
    }
    else {  // x == x->parent->right
      w = x->parent->left;
      // w 는 RED
      if (w->color == RBTREE_RED) { // 경우 1 : w가 RED인 경우.
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      // w 는 BLACK
      // 경우 2 : w는 BLACK. w의 자식 둘다 BLACK.
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        // 경우 3 : w는 BLACK. w 오른쪽자식 RED, 왼쪽자식 BLACK.
        if (w->left->color == RBTREE_BLACK) {
        w->right->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        left_rotate(t,w);
        w = x->parent->left;
        }
        // 경우 4 : w는 BLACK. w의 왼쪽자식 RED.
        w->color = x->parent->color;
        x->parent->color = w->left->color = RBTREE_BLACK;
        right_rotate(t,x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}
int rbtree_erase(rbtree *t, node_t *z) {
  // TODO: implement erase
  node_t * y = z;
  node_t * x;
  color_t y_original_color = y->color;
  // 자식 하나일 때
  if (z->left == t->nil) {
    x = z->right;
    rbtree_transplant(t, z, z->right);
  }
  else if (z->right == t->nil) {
    x = z->left;
    rbtree_transplant(t, z, z->left);
  }
  // 자식 둘일 때
  else {
    rbtree subt = {z->right, t->nil};
    y = rbtree_min(&subt);
    y_original_color = y->color;
    x = y->right; // y는 어떤 서브트리의 minimum이므로 왼쪽 자식이 없다.
    if (y->parent == z) {
      x->parent = y;
    }
    else {
      rbtree_transplant(t, y, y->right);  // z.p = r 이 일어나는 순간.
      y->right = z->right;
      y->right->parent = y;
    }
    rbtree_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;  // y의 color를 z가 원래 갖고있던 color로 강제.
  }

  if (y_original_color == RBTREE_BLACK) {
    rbtree_erase_fixup(t,x);
  }

  return 0;
}

// to array
void ascending_array_walk(node_t *x, node_t *nil, key_t *arr, int *idx, const size_t n){
  if (x != nil && *idx < n) {
    ascending_array_walk(x->left, nil, arr, idx, n);
    arr[(*idx)++] = x->key;
    ascending_array_walk(x->right, nil, arr, idx, n);
  }
}
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  int * idx = (int *)calloc(1, sizeof(int));
  *idx = 0;
  ascending_array_walk(t->root, t->nil, arr, idx, n);
  free(idx);
  return 0;
}
