/**
 * @file   StreamStatic.h
 * @author alex <yeshiwei.math@gmail.com>
 * @date   Thu Jun 20 00:49:51 2013
 * 
 * @brief  利用红黑树实现的求动态数据流最新N个数中的第k小个数的快速算法。每次k都可以取不同的值。
 * 
 * 
 */

#ifndef __STREAMSTATIC_H__
#define __STREAMSTATIC_H__

#include <vector>

using namespace std;

enum Color{
  BLACK,
  RED
};


class StaticNode{
 public:
  double key;			/**< 数据值 */
  int index;			/**< 数据下标，第index个数据 */
  int size;			/**< 红黑树中这个节点作为根的子树的大小 */
  Color color;			/**< 红黑树中这个节点的颜色 */
  StaticNode *left, *right, *parent; /**< 红黑树中这个节点的左儿子，右儿子，父亲 */
};

class StaticTree{
 public:
  StaticTree(int length_k);	/**< 只有这一个构造函数 表示只考虑最近 length_k个数*/
  ~StaticTree();

  /** 
   * 求节点z的后继节点，就是key比 z大的节点中的最小key的节点
   * 
   * @param z 
   * 
   * @return 
   */
  StaticNode* Successor(StaticNode *z);	

  /** 
   *  插入一个节点，用于新增一个数据 
   * 
   * @param z 
   */
  void InsertNode(StaticNode *z); 

  /** 
   * 删除一个节点，用于删除倒数第k个数据， 未实现，下面这个就够了。
   * 
   * @param index 被删除节点的下标。
   */
  void DeleteNode(int index);	

  /** 
   * 删除一个节点。用于删除倒数第k个数据
   * 
   * @param z 被删除节点的指针。
   * 
   * @return 返回树中从内存意义上被删除的那个节点。可能删除了别的节点，把被删除节点的信息copy到z上了。
   */
  StaticNode* DeleteNode(StaticNode *z);
  
  /** 
   * 求以 z为根的子树中的包含最大key的节点。
   * 
   * @param z 所求子树的根。
   * 
   * @return 最大key的节点的指针。
   */
  StaticNode* Maximum(StaticNode *z); 

  /** 
   * 求以 z为根的子树中的包含最小key的节点。
   * 
   * @param z 所求子树的根。
   * 
   * @return 最小key的节点的指针。
   */
  StaticNode* Minimum(StaticNode *z);
  
  /** 
   * 取出第以 x为根的子树中第i小的数
   * 
   * @param x 子树的根
   * @param i 求第i小的数
   * 
   * @return 子树中key第i小的节点的指针。
   */
  StaticNode* Select(StaticNode*x, int i); 

  /** 
   * 取出第i小的数
   * 
   * @param i 求第i小的数
   * 
   * @return 子树中key第i小的key值。
   */
  double Select(int i); 

  /** 
   * 取出第i大的数
   * 
   * @param i 求第i大的数
   * 
   * @return 子树中key第i大的key值。
   */
  double SelectLarge(int i); 

  StaticNode* pool_pop();	/**< 从事先分配的内存池中取一个空节点 */
  void pool_push(StaticNode *z); /**< 把从红黑树中删除掉的节点放回内存池中 */


  StaticNode* poolroot; int poolsize; /**< 内存池的相关信息 */

  int length_k;			/**< 所考虑的长度 */
  vector <StaticNode*> nodes;   /**< 用于保存最后k个数据在树中的位置以便将来删除。 */
  StaticNode* root;		/**< 红黑树的根。 */

 private:
  //@{ 红黑树实现的内部子程序。    
  StaticNode* nil;
  void LeftRotate(StaticNode* x);
  void RightRotate(StaticNode* x);
  void InsertFixup(StaticNode* z);
  void DeleteFixup(StaticNode* x);

  StaticNode* pool;
  //@}
};

#endif
