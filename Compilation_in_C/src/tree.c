#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "../include/tree.h"
#include "../common.h"

char enableLogging = FALSE;
tree_t* 
init_tree(void)
{
    tree_t *tree = calloc(1,sizeof(tree_t));
    if(!tree)
        return NULL;
    tree->root = NULL;
    tree->stack = get_new_stack();
    return tree;
}

tree_node_t*
getNodePtrBST(tree_t *tree, int elem){
    if(!tree || !tree->root) return NULL;
    if(tree->root->data == elem) return tree->root;
    tree_node_t *root = tree->root;
    do{
        if(elem > root->data)
            root = root->right;
        else
            root = root->left;
    } while(root && root->data!= elem);
    return root;
}

char isNodeVisited(tree_node_t *node)
{
    return node->isVisited;
}

void markNodeVisited(tree_node_t *node)
{
    node->isVisited = TRUE;
}


void markNodeNotVisited(tree_node_t *node)
{
    node->isVisited = FALSE;
}

tree_node_t* init_tree_node(int n)
{
    tree_node_t *node = calloc(1, sizeof(tree_node_t));
    if(!node)   return NULL;
    node->data = n;
    node->isVisited = FALSE;
    return node;
}

int
add_tree_node_by_value(tree_t *tree, int n)
{
    tree_node_t *root = NULL, *parent = NULL;;
    if(!tree) return FAILURE;
    tree_node_t *node = init_tree_node(n);
    if(!tree->root){
        tree->root = node;
        return SUCCESS;
    }

    root = tree->root;

    while(root){
            parent = root;
            root = (n <= root->data) ? root->left : root->right;
    } // while ends

    if(n <= parent->data)
        parent->left = node;
    else
        parent->right = node;

    return SUCCESS;
}

int
add_tree_node(tree_t *tree, tree_node_t *node)
{
    tree_node_t *root = NULL, *parent = NULL;;
    if(!tree) return FAILURE;
    if(!tree->root){
        tree->root = node;
        return SUCCESS;
    }

    root = tree->root;

    while(root){
            parent = root;
            root = (node->data <= root->data) ? root->left : root->right;
    } // while ends

    if(node->data <= parent->data)
        parent->left = node;
    else
        parent->right = node;

    return SUCCESS;
}


static void 
_printInorder(tree_node_t *root)
{
    if(!root)    return;
    _printInorder(root->left);
    printf("%-3d ", root->data);
    _printInorder(root->right);
}

void 
Inorder(tree_t *tree)
{
    if(!tree){
        printf("\n%s(): uninitialsed tree", __FUNCTION__);
        return;
    }
    if(!tree->root){
        printf("\n%s(): empty tree", __FUNCTION__);
        return;
    }
    
    printf("InOrder :\n");
    _printInorder(tree->root);
    printf("\n");
}

static void 
_printPreorder(tree_node_t *root)
{
    if(!root)    return;
    printf("%-3d ", root->data);
    _printPreorder(root->left);
    _printPreorder(root->right);
}

void 
Preorder(tree_t *tree)
{
    if(!tree){
        printf("\n%s(): uninitialsed tree", __FUNCTION__);
        return;
    }
    if(!tree->root){
        printf("\n%s(): empty tree", __FUNCTION__);
        return;
    }

    printf("PreOrder :\n");
    _printPreorder(tree->root);
    printf("\n");
}

static void 
_printPostorder(tree_node_t *root)
{
    if(!root)    return;
    _printPostorder(root->left);
    _printPostorder(root->right);
    printf("%-3d ", root->data);
}

void 
Postorder(tree_t *tree)
{
    if(!tree){
        printf("\n%s(): uninitialsed tree", __FUNCTION__);
        return;
    }
    if(!tree->root){
        printf("\n%s(): empty tree", __FUNCTION__);
        return;
    }
    printf("postOrder :\n");
    _printPostorder(tree->root);
    printf("\n");
}

void
printAllTraversals(tree_t *tree)
{
    Postorder(tree);
    Inorder(tree);
    Preorder(tree);
}


void PostorderNR(tree_t *tree)
{

/*Thsi is Non-Recursive method of post-order traversal of a tree using stack
1. use of flag is used in the tree node to keep the track if the node is visited
2. if you dont want to change the tree node data structure, may be use some other method, like use 2 stacks.
3. The use of "isVisited" flag is to decide the follow up action to be taken when the new element is exposed to the top of the stack after popping out its children:
    * whether we need to push the right child of the element because its left child has been processed
    * whenther we need to process the element itself (pop-out) because its right child has been processed.
*/

    if(!tree)
        return;
    if(!tree->root)
        return;

    int output[MAX_STACK_SIZE], index = 0, i =0;
    tree_node_t *root = tree->root, *curr_node = NULL;
    stack_t *stack = get_new_stack();

    if(enableLogging) printf("%s() trace BEGIN \n", __FUNCTION__);
    PUSH(stack, tree->root);

    while(!isStackEmpty(stack))
    {

        if(root->left){
            PUSH(stack, root->left);
            root->isVisited = FALSE;
            root=root->left;
            continue;
        }

        if(root->right){
            PUSH(stack, root->right);
            root->isVisited = TRUE;
            root=root->right;
            continue;
        }

        curr_node = pop(stack);
        if(enableLogging) {
            printf("element %d pop-out of the stack, line : %d, %s()\n", 
                       curr_node->data, __LINE__, __FUNCTION__);
            printStack(stack);
        }
        output[index] = curr_node->data;
        index++;

        ITERATE:
            root = getTopElem(stack);
            if(!root)
                break;

            if(isNodeVisited(root) == FALSE){
                markNodeVisited(root);
                if(root->right)
                {
                    PUSH(stack, root->right);
                    root = root->right;
                    continue;
                }
                else
                {
                   curr_node = pop(stack);
                   if(enableLogging) {
                       printf("element %d pop-out of the stack, line : %d, %s()\n",curr_node->data, __LINE__,
                       __FUNCTION__);
                       printStack(stack);
                   }
                   output[index] = curr_node->data;
                   index++;
                   goto ITERATE;
                }
            }
            else 
            {
                   curr_node = pop(stack);
                   if(enableLogging){
                       printf("element %d pop-out of the stack, line : %d, %s()\n",curr_node->data,
                       __LINE__, __FUNCTION__);
                       printStack(stack);
                   }
                   output[index] = curr_node->data;
                   index++;
                   goto ITERATE;
            }

            } // while ends

    free_stack(stack);
    stack = NULL;
    printf("%s :\n", __FUNCTION__);
    for(i = 0 ; i < index; i++)
        printf("%-3d ", output[i]); 
    printf("\n");
    if(enableLogging) printf("%s() trace END \n", __FUNCTION__);
    return;
}

void InorderNR(tree_t *tree)
{

/*Thsi is Non-Recursive method of In-order traversal of a tree using stack
1. use of flag is used in the tree node to keep the track if the node is visited
2. if you dont want to change the tree node data structure, may be use some other method, like use 2 stacks.
3. The use of "isVisited" flag is to decide the follow up action to be taken when the new element is exposed to the top of the stack after popping out its children:
    * whether we need to push the right child of the element because its left child has been processed
    * whenther we need to process the element itself (pop-out) because its right child has been processed.
*/

    if(!tree)
        return;
    if(!tree->root)
        return;

    int output[MAX_STACK_SIZE], index = 0, i =0;
    tree_node_t *root = tree->root, *curr_node = NULL;
    stack_t *stack = get_new_stack();

    if(enableLogging) printf("%s() trace BEGIN \n", __FUNCTION__);
    PUSH(stack, tree->root);

    while(!isStackEmpty(stack))
    {

        if(root->left){
            PUSH(stack, root->left);
            root->isVisited = FALSE; // node above me in the stack is my left child
            root=root->left; 
            continue;
        }

        curr_node = getTopElem(stack);
        output[index] = curr_node->data;
        index++;

        markNodeVisited(curr_node);

        if(root->right){
            PUSH(stack, root->right);
            root->isVisited = TRUE; // node above me in the stack is my right child
            root=root->right;
            continue;
        }

        curr_node = pop(stack); // its a leaf
        if(enableLogging) {
            printf("element %d pop-out of the stack, line : %d\n",curr_node->data, __LINE__);
            printStack(stack);
        }

ITERATE:
        root = getTopElem(stack);
        if(!root)
            break;

        if(isNodeVisited(root) == FALSE){
            markNodeVisited(root);
            output[index] = root->data;
            index++;
            if(root->right)
            {
                PUSH(stack, root->right);
                root = root->right;
                continue;
            }
            else
            {
                POP(stack); 
                goto ITERATE;
            }
        }

        POP(stack);
        goto ITERATE;
    } // while ends

    free_stack(stack);
    stack = NULL;
    printf("%s :\n", __FUNCTION__);
    for(i = 0 ; i < index; i++)
        printf("%-3d ", output[i]); 
    printf("\n");
    if(enableLogging) printf("%s() trace END \n", __FUNCTION__);
    return;
}
        
void PreorderNR(tree_t *tree)
{

/*Thsi is Non-Recursive method of In-order traversal of a tree using stack
1. use of flag is used in the tree node to keep the track if the node is visited
2. if you dont want to change the tree node data structure, may be use some other method, like use 2 stacks.
3. The use of "isVisited" flag is to decide the follow up action to be taken when the new element is exposed to the top of the stack after popping out its children:
    * whether we need to push the right child of the element because its left child has been processed
    * whenther we need to process the element itself (pop-out) because its right child has been processed.
*/

    if(!tree)
        return;
    if(!tree->root)
        return;

    int output[MAX_STACK_SIZE], index = 0, i =0;
    tree_node_t *root = tree->root;
    stack_t *stack = get_new_stack();

    if(enableLogging) printf("%s() trace BEGIN \n", __FUNCTION__);
    PUSH(stack, tree->root);

    while(!isStackEmpty(stack))
    {
        root = getTopElem(stack);
        output[index] = root->data;
        index++;

        if(root->left){
            PUSH(stack, root->left);
            root->isVisited = FALSE;
            root=root->left;
            continue;
        }

        if(root->right){
            PUSH(stack, root->right);
            root->isVisited = TRUE;
            root=root->right;
            continue;
        }

        POP(stack); // its a leaf

ITERATE:
        root = getTopElem(stack);
        if(!root)
            break;

        if(isNodeVisited(root) == FALSE){
            markNodeVisited(root);
            if(root->right)
            {
                PUSH(stack, root->right);
                root = root->right;
                continue;
            }
            else
            {
                POP(stack); 
                goto ITERATE;
            }
        }

        POP(stack);
        goto ITERATE;
    } // while ends

    free_stack(stack);
    stack = NULL;
    printf("%s :\n", __FUNCTION__);
    for(i = 0 ; i < index; i++)
        printf("%-3d ", output[i]); 
    printf("\n");
    if(enableLogging) printf("%s() trace END \n", __FUNCTION__);
    return;
}

void swapchilds(tree_node_t *node)
{
    if (!node) return;
    tree_node_t *temp = NULL;
    temp = node->left;
    node->left = node->right;
    node->right = temp;
    return;
}

static void 
_MirrorImageByInorder(tree_node_t *root)
{
    if(!root) return;
    _MirrorImageByInorder(root->left);
    swapchilds(root);
    _MirrorImageByInorder(root->right);
    return;
}

void MirrorImageByInorder(tree_t *tree)
{
    if(!tree || !tree->root) return;
    _MirrorImageByInorder(tree->root);
    return;
}


static void 
_MirrorImageByPreorder(tree_node_t *root)
{
    if(!root) return;
    swapchilds(root);
    _MirrorImageByPreorder(root->left);
    _MirrorImageByPreorder(root->right);
    return;
}

void MirrorImageByPreorder(tree_t *tree)
{
    if(!tree || !tree->root) return;
    _MirrorImageByPreorder(tree->root);
    return;
}


static void 
_MirrorImageByPostorder(tree_node_t *root)
{
    if(!root) return;
    _MirrorImageByPostorder(root->left);
    _MirrorImageByPostorder(root->right);
    swapchilds(root);
    return;
}

void MirrorImageByPostorder(tree_t *tree)
{
    if(!tree || !tree->root) return;
    _MirrorImageByPostorder(tree->root);
    return;
}



int maxSumPath = 0;
static int _MaxSumPath(tree_node_t *root){
    if(!root) return 0;

    int l = _MaxSumPath(root->left);
    int r = _MaxSumPath(root->right);
    
    int valuetobeReturnedtoParent = MAX (MAX(l + root->data, r + root->data), 
                                         MAX(r + root->data, root->data));
    int recycleSum  = l + root->data + r;
    
    maxSumPath = MAX(valuetobeReturnedtoParent, recycleSum);
    return valuetobeReturnedtoParent;
    
}

int MaxSumPath(tree_t *tree){
   if(!tree || !tree->root) 
       printf("MxsSumPath = %d\n", 0);

    _MaxSumPath(tree->root);
    printf("MaxSumPath = %d\n", maxSumPath);
    maxSumPath = 0;
   
}

static 
tree_node_t* _removeHalfNodes(tree_node_t *root){
    if(!root) return NULL;
    tree_node_t *lchild = _removeHalfNodes(root->left);
    root->left = lchild;
    tree_node_t *rchild = _removeHalfNodes(root->right);
    root->right = rchild;
    // case 1 : if my both childs are NULL, i will return my pointer to my parent
    if(!lchild && !rchild) return root;
    //case 2 : if my both children are not NULL, i will return my pointer to my parent
    else if(lchild && rchild) return root;
    //case 3 : now i am a half node, i am return my only kid
    else if(lchild && !rchild) 
        return lchild;
    else
        return rchild;
}

tree_t* removeHalfNodes(tree_t *tree){
    if(!tree)
        return NULL;
    if(!tree->root)
        return tree;
#define DUMMY_TREE_NODE_VAL 0
     tree_node_t *dummy = init_tree_node(DUMMY_TREE_NODE_VAL);
     dummy->right = tree->root;
    _removeHalfNodes(dummy);
     tree->root = dummy->right;
     return tree;
}

int frameno = 0;
tree_node_t* constructAllBST(int arr[], int index1, int index2)
{
     frameno++;
    if(enableLogging) 
        printf("%s():frame:%d called for index1 = %d, index2 = %d\n", __FUNCTION__, frameno, index1, index2);
     
     int i = 0;
     tree_node_t *root = NULL;

     if(index1 == index2 && index1 > -1){
         if(enableLogging)
             printf("   returning node with value %d for loop [%d,%d] from frame = %d\n", arr[index1], index1, index2, frameno);
         frameno--;
         return init_tree_node(arr[index1]);
    }

    if(index2 <= index1 ) {
        if(enableLogging)
            printf("returning NULL\n");
        frameno--;
        return NULL;
    }

    if(index1 >= index2) {
        if(enableLogging)
            printf("returning NULL\n");
        frameno--;
        return NULL;
    }
    
    for(i = index1 ; i <= index2; i++){
        root = init_tree_node(arr[i]);
        if(enableLogging)
            printf("root selected is : i = %d, value = %d for loop [%d,%d], frameno = %d\n", 
                i, arr[i], index1, index2, frameno);

            root->left = constructAllBST(arr, index1, i-1);
            root->right = constructAllBST(arr, i+1, index2);
            if(frameno == 1)
            {
                tree_t *tree = init_tree();
                tree->root = root;
                Inorder(tree);
            }
            else
            {
                    if(enableLogging)
                        printf("returning from frame %d with root node = %d\n", frameno, root->data);
                    frameno--;
                }
    }
}


static void
apply_gdb(tree_t *tree)
{
    return;
}

static tree_node_t*
_LowestCommonAncestor(tree_node_t *root, int n1, int n2)
{
    if(!root) return NULL;
    tree_node_t *lchild = _LowestCommonAncestor(root->left, n1, n2);
    tree_node_t *rchild = _LowestCommonAncestor(root->right, n1, n2);

    // processinf leaf node
    if(!lchild && !rchild){
        if(root->data == n1 || root->data ==n2)
            return root;
        else
            return NULL;
    }

    else if(!lchild && rchild){
            if(root->data == n1 || root->data == n2)
                return root;
            else
                return rchild;
    }

    else if(lchild && !rchild){
        if(root->data == n1 || root->data == n2)
            return root;
        else
            return lchild;
    }

    else if(lchild && rchild)
            return root;
    }


/*Given values of two nodes in a Binary Search Tree, write a c program to find the Lowest Common Ancestor (LCA).*/
tree_node_t* LowestCommonAncestor(tree_t *tree, int n1, int n2)
{
    if(!tree || !tree->root)
        return NULL;
    tree_node_t *ancestor = _LowestCommonAncestor(tree->root, n1, n2);
    return ancestor;
}

static tree_node_t*
_rightThreadedBST(tree_node_t *root)
{
    tree_node_t* lchild = NULL;
    tree_node_t* rchild = NULL;

    if(!root) return NULL;
    lchild = _rightThreadedBST(root->left);
    rchild = _rightThreadedBST(root->right);

    // processinf leaf
    if(!lchild & !rchild) return root;

    else if(!lchild && rchild)
        return rchild;
        
    else if(lchild && !rchild){
        if(!lchild->right){
        lchild->right = root;
        return root;
        }
    }

    else if(lchild && rchild)
    {
        if(!lchild->right)
            lchild->right = root;
        return rchild;
    }

}

tree_node_t* 
rightThreadedBST(tree_t *tree)
{
    if(!tree || !tree->root)
        return NULL;
    return _rightThreadedBST(tree->root);
}


static tree_node_t*
_leftThreadedBST(tree_node_t *root)
{
    tree_node_t* lchild = NULL;
    tree_node_t* rchild = NULL;

    if(!root) return NULL;
    lchild = _leftThreadedBST(root->left);
    rchild = _leftThreadedBST(root->right);

    // processinf leaf
    if(!lchild & !rchild) return root;

    else if(!lchild && rchild){
        if(!rchild->left)
            rchild->left = root;
        return root;
    }

    else if(lchild && !rchild)
        return lchild;

    else if(lchild && rchild)
    {
        if(!rchild->left)
            rchild->left = root;
        return lchild;
    }

}

tree_node_t* 
leftThreadedBST(tree_t *tree)
{
    if(!tree || !tree->root)
        return NULL;
    return _leftThreadedBST(tree->root);
}

int
getFirstNodebyPostOrder(tree_t *tree)
{
    stack_t *stack = tree->stack;
    if(isStackEmpty(stack) == FALSE) assert(0);
    if(!tree || !tree->root) return -1;
    tree_node_t *root = tree->root;

    do{
        PUSH(stack, root);
        if(root->left){
            markNodeNotVisited(root);
            root = root->left;
            continue;
        }

        if(root->right){
            markNodeVisited(root);
            root = root->right;
            continue;
        }

        root = pop(stack);
        if(enableLogging) 
            printf("element %d pop-out of the stack, line : %d, %s()\n", root->data, __LINE__, __FUNCTION__);

        return root->data;

    }while(1);

}

int
getNextNodebyPostOrder(stack_t *stack)
{
    if(!stack) assert(0);
    if(isStackEmpty(stack)) return -1;
    tree_node_t *root = NULL;

    PROCESS_STACK_TOP:
        root = getTopElem(stack);
        if(isNodeVisited(root) == FALSE){
            markNodeVisited(root);
            if(root->right){
                root = root->right;
                if(enableLogging)
                    printf("unconditional jump from %s():%d to label PUSH_INTO_STACK\n", __FUNCTION__, __LINE__);
                goto PUSH_INTO_STACK;
            }

            root = pop(stack);
            if(enableLogging)
                printf("element %d pop-out of the stack, line : %d, %s()\n", root->data, __LINE__, __FUNCTION__);
            return root->data;
        }

        if(isNodeVisited(root) == TRUE){
            root = pop(stack);
            if(enableLogging)
                printf("element %d pop-out of the stack, line : %d, %s()\n", root->data, __LINE__, __FUNCTION__);
            return root->data;
        }

        PUSH_INTO_STACK:
        do{
            PUSH(stack, root);
            if(root->left){
                markNodeNotVisited(root);
                root = root->left;
                continue;
            }

            markNodeVisited(root);

            if(root->right){
                root = root->right;
                continue;
            }

            break;
        } while(1);
        
        if(enableLogging)
            printf("unconditional jump from %s():%d to label PROCESS_STACK_TOP\n", __FUNCTION__, __LINE__);
        goto PROCESS_STACK_TOP;
        return 0;
}

static void
_undoRightThreadedBST(tree_t *tree)
{
    stack_t *stack = tree->stack;
    tree_node_t *root = tree->root, *curr = NULL;
    PUSH(stack, root);

    PUSH_INTO_STACK:

    while(root->left){
        root = root->left;
        PUSH(stack,root);
    }

    STACK_CHECK:
    if(root->right){
        if(isPresentinStack(stack, root->right) == FALSE){
            root = root->right;
            PUSH(stack, root);
            goto PUSH_INTO_STACK;
        }

        root->right = NULL;

        POP:
        curr = pop(stack);
        if(enableLogging)
            printf("element %d pop-out of the stack, line : %d, %s()\n", curr->data, __LINE__, __FUNCTION__);
            root = getTopElem(stack);
            if(!root) return;

            if(root->right == curr){
                goto POP;
            }

            else if (root->left == curr){
                goto STACK_CHECK;
            }
    }
    // stack may be left with some nodes after last element in inorder traversal is processed
    reset_stack(stack);
 }

void undoRightThreadedBST(tree_t *tree)
{
    if(!tree) return;
    if(!tree->root) return;
    reset_stack(tree->stack);
    _undoRightThreadedBST(tree);
    }


static void
_undoLeftThreadedBST(tree_t *tree)
{
    stack_t *stack = tree->stack;
    tree_node_t *root = tree->root, *curr = NULL;
    PUSH(stack, root);

    STACK_CHECK:
        if(root->left){
        if(isPresentinStack(stack, root->left) == FALSE){
            root = root->left;
            PUSH(stack,root);
            goto STACK_CHECK;
        }
        else
        {
            root->left = NULL;
            
            POP:
            curr =  pop(stack);
            if(enableLogging)
                printf("element %d pop-out of the stack, line : %d, %s()\n", curr->data, __LINE__, __FUNCTION__);
            root = getTopElem(stack);
            if(!root) return;

            if(root->right == curr){
                goto POP;
            }
            else if(root->left == curr){
                goto PUSH_INTO_STACK;
            }

         }
    }

        PUSH_INTO_STACK:
        if(root->right){
            root = root->right;
            PUSH(stack, root);
            goto STACK_CHECK; 
        }
        goto POP;
}

void undoLeftThreadedBST(tree_t *tree)
{
    if(!tree) return;
    if(!tree->root) return;
    reset_stack(tree->stack);
    _undoLeftThreadedBST(tree);
}

/*Used when recursive calls needs to return two nodes to its ancestors
instead of one */

typedef struct pack{
    tree_node_t *successor_node; // node whose successor is to be found
    tree_node_t *predecessor_node; // node whose predecessor is to be found
} pack_t;

static int
_isPackEmpty(pack_t *pack)
{
    if(pack->successor_node || pack->predecessor_node)
        return FALSE;
     return TRUE;
}

static pack_t 
_constuctCompleteThreadedBST(tree_node_t *root)
{
      pack_t pack;
      pack.successor_node = NULL;
      pack.predecessor_node = NULL;
      
      if(!root){
          /* return empty pack*/
        return pack;
      }

      pack_t lchild_pack = _constuctCompleteThreadedBST(root->left);
      pack_t rchild_pack = _constuctCompleteThreadedBST(root->right);

      /*Examining the pack returned from left and right child*/
      
      // cas 1 : if i am leaf, that is both packs are empty

      if(_isPackEmpty(&lchild_pack) && _isPackEmpty(&rchild_pack)){
          pack.successor_node = root;
          pack.predecessor_node = root;
          return pack;
      }
      
      // case 2 : if left pack is empty, but right pack is not

      else if(_isPackEmpty(&lchild_pack) && !_isPackEmpty(&rchild_pack)){
         rchild_pack.predecessor_node->left = root;
         pack.successor_node = rchild_pack.successor_node;
         pack.predecessor_node = root;
         return pack;
      }

      // case 3 : if right pack is empty, but left pack is not

      else if(!_isPackEmpty(&lchild_pack) && _isPackEmpty(&rchild_pack)){
          lchild_pack.successor_node->right = root;
          pack.successor_node = root;
          pack.predecessor_node = lchild_pack.predecessor_node;
          return pack;
      }

    // case 4 : if right pack and left both are not empty

    else if(!_isPackEmpty(&lchild_pack) && !_isPackEmpty(&rchild_pack)){
         lchild_pack.successor_node->right = root;
         pack.predecessor_node = lchild_pack.predecessor_node;
         rchild_pack.predecessor_node->left = root;
         pack.successor_node = rchild_pack.successor_node;
         return pack;
    }
}
void* constuctCompleteThreadedBST(tree_t *tree)
{
    if(!tree) return NULL;
    if(!tree->root) return NULL;
    _constuctCompleteThreadedBST(tree->root); 
    return NULL;
}


static pack_t
_BSTtreeToDoublyLinkedList(tree_node_t* root)
{
      pack_t pack;
      pack.successor_node = NULL;
      pack.predecessor_node = NULL;
      
      if(!root)
        return pack; /* return empty pack*/

    pack_t lchild_pack = _BSTtreeToDoublyLinkedList(root->left);
    pack_t rchild_pack = _BSTtreeToDoublyLinkedList(root->right);

   /*case 1 : i am a leaf node, then my lchild_pack and rchild_pack will be empty, then
   my successor and predecessor both will exist in the tree above me*/
   if(_isPackEmpty(&lchild_pack) && _isPackEmpty(&rchild_pack)){
        pack.successor_node = root; // find successor_node of root (that is myself)
        pack.predecessor_node = root; // find predecessor_node of root (that is myself)
        return pack;
   }

   /*case 2 : If i am a node with only left child, left pack will not be empty but right pack will be*/
   else if(!_isPackEmpty(&lchild_pack) && _isPackEmpty(&rchild_pack)){
       /*Do the Double Bonding because i am the successor node of lchild_pack.successor_node*/
       lchild_pack.successor_node->right = root;
       root->left = lchild_pack.successor_node;

       pack.successor_node = root; // my right child is NULL, definitely my successor would exist above me
       pack.predecessor_node = lchild_pack.predecessor_node; //  predecessor of predecessor_node would exist in tree above me
       return pack;
   }

   /*case 3 : If i am a node with only right child, right pack will not be empty but left will be */
   else if(_isPackEmpty(&lchild_pack) && !_isPackEmpty(&rchild_pack)){
       /*Do the Double Bonding because i am the predecessor node of rchild_pack.predecessor_node*/
       rchild_pack.predecessor_node->left = root;
       root->right = rchild_pack.predecessor_node;

       pack.successor_node = rchild_pack.successor_node;// successor of successor_node would exist in tree above me
       pack.predecessor_node = root;// my left child is NULL, definitely my predecessor would exist above me
       return pack;
   }

   /*case 4 : If i am the node with both my left and right child, i will recieve packs from both the children*/
   else if(!_isPackEmpty(&lchild_pack) && !_isPackEmpty(&rchild_pack)){
       /*Do the double bonding with lchild_pack.successor_node because i am the successor of it*/
       lchild_pack.successor_node->right = root;
       root->left = lchild_pack.successor_node;

       /*Do the double bonding with rchild_pack.predecessor_node because i am the predecessor of it*/
       rchild_pack.predecessor_node->left = root;
       root->right = rchild_pack.predecessor_node;

       /* Still the successor of rchild_pack.successor_node is not known, so pass to the ancestor*/
       pack.successor_node = rchild_pack.successor_node;

       /* Still the predecessor of lchild_pack.predecessor_node is not known, so pass to the ancestor*/
       pack.predecessor_node = lchild_pack.predecessor_node;
       return pack;
   }
}


tree_node_t* 
BSTtreeToDoublyLinkedList (tree_t *tree)
{
    if(!tree) return NULL;
    if(!tree->root) return NULL;
    _BSTtreeToDoublyLinkedList(tree->root);
    // returning node of the doubly linked list which was the root of the tree before tree was converted to doubly LL
    return tree->root; 
}

int
DepthOfTreeNode(tree_t *tree, int elem)
{
    if(!tree || !tree->root) return -1;
    int depth = 0;
    tree_node_t *root = tree->root;
    if(root->data == elem) return depth;

    do
    {
        if(elem > root->data){
            depth++;
            root = root->right;
        }
        else{
             depth++;
             root = root->left;
        }
    } while(root && root->data != elem);

    if(!root)
        return -1;
    return depth;
}


void 
gotoEndOfList(tree_node_t **node)
{
    if(!node || !*node) assert(0);
    while((*node)->right){
        *node = (*node)->right;
    }
}

static tree_node_t*
_allDescendantNodesAtDistance(const tree_node_t *root, tree_node_t *elem, const int D, int d)
{
    if(!elem) return NULL;

    if(d < D){
        tree_node_t* lchild = _allDescendantNodesAtDistance(root, elem->left, D, d+1);
        tree_node_t* rchild = _allDescendantNodesAtDistance(root, elem->right, D, d+1);

        /* create linked list of nodes, This will temper original tree*/

        /* case 1 , if lchild and rchild both are not null*/
        if(lchild && rchild){
            tree_node_t* Istnode = lchild;
            gotoEndOfList(&lchild);
            lchild->right = rchild;
            rchild->left = lchild;
            return Istnode;
        }

        /*case 2 : when lchild is NULL but rchild is not*/

        else if(!lchild && rchild){
            return rchild;
        }

        /*case 3 : when rchild is NULL but lchild is not*/

        else if(lchild && !rchild){
            return lchild;
        }
        
        /*case 4, both are NULL*/
        else
            return NULL;
    }
    else if(d == D){
        elem->left = NULL;
        elem->right = NULL;
        return elem;
    }
}


tree_node_t*
allDescendantNodesAtDistance(tree_t *tree, int elem, int distance){
    if(!tree || !tree->root) return NULL;
    tree_node_t *node_elem = getNodePtrBST(tree, elem); // O(log n)
    if(!node_elem) return NULL;
    if(distance == 0) return node_elem;
    return _allDescendantNodesAtDistance(tree->root, node_elem, distance, 0);
}

static tree_node_t*
_getNthAncestor(tree_node_t *root, const int elem, int *level)
{
    tree_node_t *node_ptr = NULL;
    if(!root) return NULL;

    if(elem < root->data){
        node_ptr = _getNthAncestor(root->left, elem, level);
        if(*level == 0){
            if(node_ptr) 
                return node_ptr;
            else
                return root;
        }
        (*level)--;
        return NULL;
    }

    else if (elem > root->data){
        node_ptr = _getNthAncestor(root->right, elem, level);
        if(*level == 0){
            if(node_ptr) 
                return node_ptr;
            else
                return root;
        }
        (*level)--;
        return NULL;
    }

    else
        node_ptr = root;

    if(node_ptr->data == elem){
        if(*level == 0)
            return node_ptr;
         (*level)--;
          return NULL;
    }
}


tree_node_t*
getNthAncestor(tree_t *tree, int elem, int distance)
{
    if(!tree || !tree->root) return NULL;
    tree_node_t *node_elem = getNodePtrBST(tree, elem); // O(log n)
    if(!node_elem) return NULL;
    if(distance == 0) return node_elem;
    int level = distance;
    return _getNthAncestor(tree->root, elem, &level);
 }
#if 1 
int 
main(int argc, char **argv){
    int rc = SUCCESS;
    if(argc > 1)
        enableLogging = TRUE;
    tree_t *tree = init_tree();
    add_tree_node_by_value(tree,  50);
    add_tree_node_by_value(tree,  30);
    add_tree_node_by_value(tree,  10);
    add_tree_node_by_value(tree,  3);
    add_tree_node_by_value(tree,  4);
    add_tree_node_by_value(tree,  35);
    add_tree_node_by_value(tree,  32);
    add_tree_node_by_value(tree,  40);
    add_tree_node_by_value(tree,  38);
    add_tree_node_by_value(tree,  100);
    add_tree_node_by_value(tree,  75);
    add_tree_node_by_value(tree,  200);
    add_tree_node_by_value(tree,  150);
    add_tree_node_by_value(tree,  120);

#if 0 
    /*Testing Non-Recursive Traversals*/
    printAllTraversals(tree);
    PostorderNR(tree);
    InorderNR(tree);
    PreorderNR(tree);
#endif

#if 0
    /*Testing Mirror images*/
    printAllTraversals(tree);
    MirrorImageByPostorder(tree);
    MirrorImageByPostorder(tree);
    printAllTraversals(tree);
#endif

    /*Testing Maximum Sum path*/
#if 0
    MaxSumPath(tree);
#endif

    /*Testing halfnode removal*/
#if 0
    removeHalfNodes(tree);
    printAllTraversals(tree);
#endif

#if 0
    /*Testing constructAllBST*/
    int arr[] = {0,1,2,3,4,5};
    constructAllBST(arr, 0, 5);
#endif

#if 0
/*Testing Lowest Common Ancestor*/
    printf("LowestCommonAncestor = %d\n", (LowestCommonAncestor(tree, 1,   38))->data);
    printf("LowestCommonAncestor = %d\n", (LowestCommonAncestor(tree, 1,  120))->data);
    printf("LowestCommonAncestor = %d\n", (LowestCommonAncestor(tree, 35,  30))->data);
    printf("LowestCommonAncestor = %d\n", (LowestCommonAncestor(tree, 100,100))->data);
    printf("LowestCommonAncestor = %d\n", (LowestCommonAncestor(tree, 75,  75))->data);
#endif

#if 0
    Postorder(tree);
    printf("node#1 = %d\n", getFirstNodebyPostOrder(tree));
    int i = 2, N = 8;
    for(i = 2; i <= N; i++){
        printf("node#%d = %d\n", i, getNextNodebyPostOrder(tree->stack));
    }
#endif

#if 0
    /*Testing threaded Biary tree*/
    /*Either execute rightThreadedBST Or leftThreadedBT*/
    rightThreadedBST(tree);
    //leftThreadedBST(tree);
    apply_gdb(tree); 
#endif

/* Checking undoRightThreadedBST */
#if 0
    rightThreadedBST(tree);
    undoRightThreadedBST(tree);
    printAllTraversals(tree);
#endif

/* Checking undoiLeftThreadedBST */
#if 0
    leftThreadedBST(tree);
    undoLeftThreadedBST(tree);
    printAllTraversals(tree);
#endif

/*Testing constuctCompleteThreadedBST*/
#if 0
    constuctCompleteThreadedBST(tree);
    apply_gdb(tree);
#endif

    /*Testing BSTtreeToDoublyLinkedList*/
#if 0
    tree_node_t *start_node = BSTtreeToDoublyLinkedList(tree);
    // bring the start_node to first node of the doubly linked list
     while(start_node->left){
        start_node = start_node->left;
    }
    printf("printing forward:\n");
    while(start_node->right){
        printf("%d ", start_node->data);
        start_node = start_node->right;
    }
    printf("%d\n", start_node->data);

    printf("printing backwards:\n");
    while(start_node->left){
        printf("%d ", start_node->data);
        start_node = start_node->left;
    }
    printf("%d\n", start_node->data);
    
#endif

/* Tesintg DepthOfTreeNode() */
#if 0
    printf("depth of node %d  = %d\n", 233, depthoftreenode(tree, 233));
    printf("depth of node %d  = %d\n", 38, depthoftreenode(tree, 38));
    printf("depth of node %d  = %d\n", 120, depthoftreenode(tree, 120));
#endif

    /* Tesintg getNodePtrBST() */
#if 0
    //printf("elem = %d, node_ptr_value = %d\n", 233, getNodePtrBST(tree, 233)->data);
    printf("elem = %d, node_ptr_value = %d\n", 38, getNodePtrBST (tree,   38)->data);
    printf("elem = %d, node_ptr_value = %d\n", 120, getNodePtrBST(tree, 120)->data);
#endif

    /* Testing allDescendantNodesAtDistance */
#if 0
    tree_node_t* list = allDescendantNodesAtDistance(tree, 50, 1);
    if(!list){
        printf("NO such elements\n");
        return rc;
    }

    while(list->right){
        printf("%d -> ", list->data);
        list = list->right;
    }
        printf("%d\n", list->data);
#endif

/* Testing getNthAncestor()*/
#if 1
    tree_node_t *ancestor = getNthAncestor(tree, 38, 4);
    if(ancestor)
        printf("Ancestor node of 38 at height 4  = %d\n", ancestor->data);
    else
         printf("Ancestor node is NULL\n");
    ancestor = getNthAncestor(tree, 38, 3);
    if(ancestor)
        printf("Ancestor node of 38 at height 3  = %d\n", ancestor->data);
    else
         printf("Ancestor node is NULL\n");
#endif
    return rc;
}
#endif
