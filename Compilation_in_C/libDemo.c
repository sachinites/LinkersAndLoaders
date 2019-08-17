#include "include/tree.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
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

#if 1
    /*Testing Non-Recursive Traversals*/
    printAllTraversals(tree);
    PostorderNR(tree);
    InorderNR(tree);
    PreorderNR(tree);
#endif

return 0;

}
