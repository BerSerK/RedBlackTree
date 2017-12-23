#include <iostream>
#include <cstdlib>
#include "StreamStatic.h"

using namespace std;

int main(int argc, char *argv[]){
  StaticTree tree(10000);
  StaticNode* z, *x;
  int index;
  double key;
  index =0;
  while (cin >> key) {
    if (index >= tree.length_k){
      x = tree.DeleteNode(tree.nodes[ (index)%(tree.length_k) ]);
      tree.pool_push(x);
    }
    z = tree.pool_pop();
    if (z == NULL){
      cout << "pool too small." << endl;
    }
    z -> key = key;
    z -> index = index;
    z -> size = 1;
    tree.nodes[index % tree.length_k] = z;

    tree.InsertNode(z);
    cout << index << ":" << key << "\t"
	 << tree.Minimum(tree.root) -> key << "\t"
	 << tree.Maximum(tree.root) -> key << "\t" << endl;
    for (int i = 1; i <= 10; i++) {
      z = tree.Select(tree.root, i);
      cout << i <<":" <<  z -> key << "\t";
    }
    cout << endl;
    
    index ++;
  }
  return 0;
}
