#include "StreamStatic.h"
#include <cstdlib>
#include <iostream>


StaticTree::StaticTree(int length_k) {
  nil = new StaticNode;
  nil -> color = BLACK;
  nil -> size = 0;
  nil -> key = 1 << 31 - 1;
  root = nil;
  this -> length_k = length_k;
  nodes.resize(length_k);
  pool = new StaticNode[length_k + 1];
  poolroot = pool;
  for(int i = 0; i < length_k; i ++) {
    pool[i].right = &pool[i +1];
    pool[ i + 1].parent = &pool[i];
  }
  poolsize = length_k +1;
}

StaticTree::~StaticTree() {
  delete nil;
}

StaticNode* StaticTree::pool_pop() {
  if (poolsize > 0) {
    poolroot = poolroot -> right;
    poolsize --;
    return poolroot -> parent;
  }else{
    return NULL;
  }
}
void StaticTree::pool_push(StaticNode* z) {
  poolsize ++;
  poolroot -> parent = z;
  z -> right = poolroot;
  poolroot = z;
}
void StaticTree::LeftRotate(StaticNode* x) {
  StaticNode *y = x->right;
  x -> right = y -> left;
  if (y -> left != nil) y -> left -> parent = x;/// There was not if in the book, this will introduce an segment fault with 'input4.txt';
  y -> parent = x -> parent;
  if ( x -> parent == nil ) {
    root = y;
  }else if ( x == x -> parent -> left) {
    x -> parent -> left = y;
  }else{
    x -> parent -> right = y;
  }
  y -> left = x;
  x -> parent = y;
  y -> size = x -> size;
  x -> size = x -> left -> size + x -> right ->size + 1;
  //cout<<"left rotate"<< x -> left ->size << x-> right ->size << y -> left ->size<<y -> right ->size<<endl;
}

void StaticTree::RightRotate(StaticNode* y) {
  StaticNode *x = y->left;
  y -> left = x -> right;
  if (x -> right != nil) x -> right -> parent = y;
  x -> parent = y -> parent;
  if ( y -> parent == nil ) {
    root = x;
  }else if ( y == y -> parent -> right) {
    y -> parent -> right = x;
  }else{
    y -> parent -> left = x;
  }
  x -> right = y;
  y -> parent = x;
  x -> size = y -> size;
  y -> size = y -> right -> size + y -> left ->size + 1;
  //cout<<"right rotate"<< y -> left ->size << y-> right ->size << x -> left ->size<< x -> right ->size<<endl;
}


void StaticTree::InsertNode(StaticNode* z) {
  StaticNode * y = nil, * x = root;
  while(x != nil) {
    x -> size = x -> size + 1;
    y = x;
    if ( z -> key <  x -> key) {
      x = x -> left;
    }else{
      x = x -> right;
    }
  }
  z -> parent = y;
  if ( y == nil ) {
    root = z;
  }else if ( z -> key < y -> key) {
    y -> left = z;
  }else{
    y -> right = z;
  }
  z -> left = nil;
  z -> right = nil;
  z -> color = RED;
  z -> size = 1;
  InsertFixup(z);
}

void StaticTree::InsertFixup(StaticNode* z) {
  StaticNode *y;
  while(z -> parent -> color == RED) {
    if ( z -> parent == z -> parent -> parent -> left) {
      y = z -> parent -> parent ->right;
      if (y -> color == RED) {
	z -> parent -> color = BLACK;
	y -> color = BLACK;
	z -> parent -> parent -> color = RED;
	z = z -> parent -> parent;
      }else{
	if ( z == z -> parent -> right) {
	  z = z -> parent;
	  LeftRotate(z);
	}
	z -> parent -> color = BLACK;
	z -> parent -> parent -> color = RED;
	RightRotate(z -> parent -> parent);
      }
    }else{
      y = z -> parent -> parent ->left;
      if (y -> color == RED) {
	z -> parent -> color = BLACK;
	y -> color = BLACK;
	z -> parent -> parent -> color = RED;
	z = z -> parent -> parent;
      }else{
	if ( z == z -> parent -> left) {
	  z = z -> parent;
	  RightRotate(z);
	}
	z -> parent -> color = BLACK;
	z -> parent -> parent -> color = RED;
	LeftRotate(z -> parent -> parent);
      }
    }
  }
  root->color = BLACK;
}

StaticNode* StaticTree::DeleteNode(StaticNode *z) {
  StaticNode *y, *x;
  if ( z -> left == nil || z -> right == nil) {
    y = z;
    //cout<<"case 1"<<endl;
  }else{
    y = Successor(z);
    //cout<<"case 2"<<endl;
  }
  if ( y -> left != nil) {
    x = y -> left;
  }else{
    x = y -> right;
  }
  x -> parent = y -> parent;

  if ( y -> parent == nil) {
    root = x;
  }else if ( y  == y -> parent -> left) {
    y -> parent -> left = x;
  }else{
    y -> parent -> right = x;
  }

  if ( y != z) {
    z -> key = y -> key;
    ///copy y's satellite data to z.
    z -> index = y -> index;
    nodes[ y -> index % (length_k)] = z;
  }

  z = y; 
  while(z != root) {
    z = z -> parent;
    z -> size = z -> size - 1;
  }
  if ( y -> color == BLACK) {
    DeleteFixup(x);
  }
  return y;
}

void StaticTree::DeleteFixup(StaticNode* x) {
  StaticNode* w;

  while( x != root && x -> color == BLACK) {
    if (x == x -> parent -> left) {
      w = x -> parent -> right;
      if ( w -> color == RED) {
	w -> color = BLACK;
	x -> parent -> color = RED;
	LeftRotate( x -> parent);
	w = x -> parent -> right;
      }
      if ( w -> left -> color == BLACK && w -> right -> color == BLACK) {
	w -> color = RED;
	x = x -> parent;
      }else{
	if ( w -> right -> color == BLACK) {
	  w -> left -> color = BLACK;
	  w -> color = RED;
	  RightRotate(w);
	  w = x -> parent -> right;
	}
	w -> color = x -> parent -> color;
	x -> parent -> color = BLACK;
	w -> right -> color = BLACK;
	LeftRotate(x -> parent);
	x = root;
      }
    }else{
      w = x -> parent -> left;
      if ( w -> color == RED) {
	w -> color = BLACK;
	x -> parent -> color = RED;
	RightRotate( x -> parent);
	w = x -> parent -> left;
      }
      if ( w -> right -> color == BLACK && w -> left -> color == BLACK) {
	w -> color = RED;
	x = x -> parent;
      } else {
	if ( w -> left -> color == BLACK) {
	  w -> right -> color = BLACK;
	  w -> color = RED;
	  LeftRotate(w);
	  w = x -> parent -> left;
	}
	
	w -> color = x -> parent -> color;
	x -> parent -> color = BLACK;
	w -> left -> color = BLACK;
	RightRotate(x -> parent);
	x = root;
      }

    }
  }
  x -> color = BLACK;
}

StaticNode* StaticTree::Select(StaticNode* x, int i) {
  int r = x ->left -> size + 1;
  //std::cout <<i<<" " <<r << " "<<x->left->size <<" "<<x->right->size<<endl;
  if (i > root -> size) {
    return nil;
  }
  if ( i == r) {
    return x;
  }else if (i < r) {
    return Select( x -> left, i);
  }else{
    return Select( x -> right, i - r);
  }
}

double StaticTree::Select(int i) {
  return Select(root, i) -> key;
}

double StaticTree::SelectLarge(int i) {
  return Select(root, root -> size - i + 1) -> key;
}

StaticNode* StaticTree::Successor(StaticNode *x) {
  if (x -> right != nil) {
    return Minimum(x -> right);
  }
  StaticNode *y = x -> parent;
  while( y != nil && x == y -> right) {
    x = y;
    y = y -> parent;
  }
  return y;
}

StaticNode* StaticTree::Minimum(StaticNode *x) {
  while(x -> left != nil) {
    x = x -> left;
  }
  return x;
}

StaticNode* StaticTree::Maximum(StaticNode *x) {
  while(x -> right != nil) {
    x = x -> right;
  }
  return x;
}
