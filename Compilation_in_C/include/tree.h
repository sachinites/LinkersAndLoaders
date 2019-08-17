#include <assert.h>
#include "stack.h"
#define SUCCESS 0
#define FAILURE -1
#define TRUE 1
#define FALSE 0

typedef struct tree_node{
  struct tree_node *left;
  struct tree_node *right;
  int data;
  char isVisited;
} tree_node_t;

typedef struct tree{
    tree_node_t *root;
    stack_t *stack;
} tree_t;

int
add_tree_node_by_value(tree_t *tree, int n);
int
add_tree_node(tree_t *tree, tree_node_t *node);
tree_t* init_tree(void);
tree_node_t* init_tree_node(int n);
char isNodeVisited(tree_node_t *node);
void markNodeVisited(tree_node_t *node);
void markNodeNotVisited(tree_node_t *node);
void Inorder(tree_t *tree);
void Preorder(tree_t *tree);
void Postorder(tree_t *tree);
void printAllTraversals(tree_t *tree);
void PreorderNR(tree_t *tree);
void PostorderNR(tree_t *tree);
void InorderNR(tree_t *tree);
void MirrorImageByInorder(tree_t *tree);
void MirrorImageByPostorder(tree_t *tree);
void MirrorImageByPreorder(tree_t *tree);
void swapchilds(tree_node_t *node);
int MaxSumPath(tree_t *tree);
tree_t* removeHalfNodes(tree_t *tree);
tree_node_t* constructAllBST(int arr[], int index1, int index2);
int calulateTreeBreadth(tree_t *tree);
tree_node_t* LowestCommonAncestor(tree_t *tree, int n1, int n2);
tree_node_t* rightThreadedBST(tree_t *tree);
tree_node_t* leftThreadedBST(tree_t *tree);
void undoRightThreadedBST(tree_t *tree);  /*TC: O(nlog(n)), SC: O(log(n))*/ 
void undoLeftThreadedBST(tree_t *tree);  /*TC: O(nlog(n)), SC: O(log(n))*/
tree_node_t* BSTtreeToDoublyLinkedList (tree_t *tree);/*TC: O(n), SC: O(logn)*/

void* constuctCompleteThreadedBST(tree_t *tree);
int
getFirstNodebyPostOrder(tree_t *tree);
int
getNextNodebyPostOrder();

int
DepthOfTreeNode(tree_t *tree, int elem);

tree_node_t* 
allDescendantNodesAtDistance(tree_t *tree, int elem, int distance);

tree_node_t *
getNthAncestor(tree_t *tree, int elem, int distance);
tree_node_t*
getNodePtrBST(tree_t *tree, int elem);


#define PUSH(stack, node)                                                             \
do{                                                                                   \
    if(!push((stack), (node))) {                                                      \
        if(enableLogging){                                                            \
            printf("element %d pushed into stack, line : %d, %s()\n",                 \
                                           (node)->data, __LINE__, __FUNCTION__);     \
            printStack(stack);}                                                       \
   }else                                                                              \
        assert(0);                                                                    \
} while (0) 


#define POP(stack)                                                                    \
do{                                                                                   \
tree_node_t *_node = NULL;                                                            \
    if(_node = pop(stack)){                                                           \
        if(enableLogging) {                                                           \
            printf("element %d pop out of the stack, line : %d, %s()\n",              \
                                               _node->data, __LINE__, __FUNCTION__);  \
            printStack(stack);}                                                       \
    }else                                                                             \
        assert(0);                                                                    \
} while (0) 

/* Utility Funtions*/
void 
gotoEndOfList(tree_node_t **node);
