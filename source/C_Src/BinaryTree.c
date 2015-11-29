// dumb code with smart data structure is better than smart code with dumb data structure
#include "queue.h"
#include "stack.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"
#include <stdio.h>
#include "BinaryTree.h"
#include "commonOperations.h"
#define MAX_Q_SIZE 500
#define INT_MAX 100
struct bnode* newBNode(int data)
{
    struct bnode* node = malloc(sizeof(struct bnode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}
struct bQueue {
    struct bnode** node; // To maintain an array of binary nodes
    int front;
    int rear;
    int size;
    unsigned capacity;
};

struct bQueue* createBQueue(unsigned capacity)
{
    struct bQueue* queue = malloc(sizeof(struct bQueue));
    queue->front = 0;
    queue->rear = capacity - 1;
    queue->size = 0;
    queue->capacity = capacity;
    queue->node = (struct bnode**)malloc(sizeof(struct bnode*) * capacity);
    // an array of binary nodes is created..
    return queue;
}
void enqueueBNode(struct bQueue* queue, struct bnode* node)
{
    if(queue->size == queue->capacity) { // Full queue
	//		queue->front=queue->rear=node;
	printf("Full queue..");
	return;
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->node[queue->rear] = node;
    queue->size += 1;
    //	queue=queue->rear;
}
struct bnode* dequeueBNode(struct bQueue* queue)
{
    if(queue->size == 0) {
	printf("Emtpy queue..");
	return NULL;
    }
    struct bnode* item = queue->node[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size -= 1;
    return item;
}
int isCompleteBT(struct node* root)
{

    // Defn:A complete binary tree is a binary tree in which every level,
    // except possibly the last, is completely filled,
    // and all nodes are as far left as possible. See following examples.
    /*
     * The following trees are examples of Complete Binary Trees
1
/   \
2     3

   1
/    \
2       3
/
4

   1
/    \
2      3
/  \    /
4    5  6
     * */
    // Base Case
    if(root == NULL) {
	return 1; // It is a complete binary tree
    }
    struct bQueue* queue = createBQueue(10);
    int flag = 0; // set the flag to false.
    enqueueBNode(queue, root);
    while((queue->size) != 0) { // up until the size of queue is empty, do the following..
	struct bnode* temp_node = dequeueBNode(queue);
	// full node has both left and right.
	if(temp_node->left) {
	    if(flag) {
		/* If we have seen a non full node, and we see a node
		 * with non-empty right child, then the given tree is not
		 * a complete Binary Tree
		 *       1
		 *    2     3
		 * 	4      5    -> Not a complete binary tree.
		 * */
		return 0; // Not a complete binary tree.
	    }
	    enqueueBNode(queue, temp_node->left); // enqueue left node.
	} else {
	    flag = 1; // Non - full node.
	}
	// For a leaf node in a complete binary tree, temp_node->left is the last one,
	// even if it is of flag set to true, it will not be returned as false.
	if(temp_node->right) {
	    if(flag) {
		/* If we have seen a non full node, and we see a node
		 * with non-empty left child, then the given tree is not
		 * a complete Binary Tree
		 *        1
		 *    2       3
		 * 	     4    -> Not a complete binary tree.
		 * */
		return 0;
	    }
	    enqueueBNode(queue, temp_node->right);
	} else {
	    flag = 1;
	}
    }
    // If we reach here, then it s a complete binary tree
    return 1;
}

struct bStack {
    struct bnode* elem;
    struct bStack* next;
};
void pushBStack(struct bStack** top_ref, struct bnode* data)
{

    struct bStack* temp = (struct bStack*)malloc(sizeof(struct bStack));

    temp->elem = data;

    temp->next = *(top_ref);

    *(top_ref) = temp;
}
struct bnode* popBStack(struct bStack** top_ref)
{

    struct bStack* top = *(top_ref);
    if(top == NULL) {

	printf("Stack underflow..");
	return NULL;
    }
    //	struct bStack* temp = top;

    struct bnode* res = top->elem;

    *(top_ref) = top->next;

    free(top);

    return res;
}
void inorderUsingStack(struct bnode* root)
{
    // The best way to remeber this prblem is to know how the recursion of inorder tree works
    /*
     * 1) printInOrder(root->left)
     * 2) print root->data
     * 3) printInOrder(root->right)
     *
     * Push the left elements to stack
     * If the element is NULL, then return
     * pop the element from stack
     * print the Data
     * Go to the right of element
     * Proceed the above steps again.
     * */

    // Initialize stack
    struct bStack* stack = NULL;
    struct bnode* current = root;
    int isDone = 0; // flag to set if the stack is empty
    while(!isDone) {
	if(current != NULL) {
	    // If the element is not null, push it to the stack
	    pushBStack(&stack, current); // push the N element
	    current = current->left; // traverse left (L)
	} else {
	    if(stack != NULL) {
		current = popBStack(&stack);
		printf("%d ", current->data); // print the N Element.
		current = current->right;
	    } else {
		isDone = 1;
		// if the stack is empty, that means, all the elements were traversed.
		// or if there is no binary tree, then the traversal is done.
	    }
	}
    }
}
// Start of threaded binary tree
/* The idea of threaded binary trees is to make inorder traversal faster and do it without stack and
 * without recursion.
 *
 * A binary tree is made threaded by making all right child (leaf) pointers
 * that would normally be NULL point to the inorder successor of the node (if it exists).
 *
 * Inorder successor -> LNR
 * L's successor is N, R's Successor is N's parent
 *
 * Two types of threaded binary tree
 * 1) Single Threaded -> right child pointing to inorder successor
 * 2) Double Threaded -> left child pointer to inorder predecessor, right child pointer to inorder successor
 * The predecessor threads are useful for reverse inorder traversal and postorder traversal.
 *
 * */
struct thbnode {
    int data;
    struct thbnode* left, *right;
    int rightThreaded; // boolean true if the right child points to the inorder successor.
};
struct thbnode* leftMost(struct thbnode* node)
{
    if(node == NULL)
	return;
    else {
	while(node->left != NULL) { // if node!=NULL check used, then at the end we will be getting node as NULL.
	    node = node->left;
	}
	return node; // returns the left most node.
    }
}
/*
 *
 * Inorder Successor of a node:-
 *
 * in a threaded binary tree,
 * if the rightthreaded enabled, then the right child points to the successor
 * else, traverse to the leftMost element in the right subtree.
 *
 *
 * */
void inOrderThreaded(struct thbnode* node)
{

    // Refer image stored in this project.
    struct thbnode* cur = leftMost(node);
    while(cur != NULL) {
	printf("%d ", cur->data);
	if(cur->rightThreaded) { // if right threaded enabled, go to the inorder successor stored in right
	    // child pointer
	    cur = cur->right;
	} else {
	    // else, find the left Most element in the right subtree,  then proceed with this loop.
	    cur = leftMost(cur->right);
	}
    }
}
// End of threaded binary tree
int size(struct bnode* node)
{
    // Size of binary node = size(node->left)+1+size(node->right)
    if(node == NULL)
	return 0;
    else {
	return size(node->left) + 1 + size(node->right);
    }
}

/****
 *  Properties of Binary Tree:
 *
 *  1) For a binary tree
 * 		Leaf = Internal Nodes + 1
 *
 *  2) For a k-ary tree
 * 		Leaf = (k-1)*Internal Nodes+1;
 *
 * 	3) Level -> descending from root, [i.e root is at level 1]
 * 	   Number of elements at level = 2^(L - 1) = 2^(1-1) = 2^0 = 1
 *

 * 4) Height -> leaf to root path, [i.e leaf is at height 1]
 *
 * 	  Maximum number of nodes in a binary tree of height Ã¢â‚¬ËœhÃ¢â‚¬â„¢ is (2^h) Ã¢â‚¬â€œ 1.
          Here height of a tree is maximum number of nodes on root to leaf path.

        Height of a leaf node is considered as 1.
        Height of parent of a leaf node is considered as 2. (2^2)-1 => 3
        Height of grand parent of a leaf node is considered as 3. (2^3)-1 => 7
 *
 *  5) A full binary tree -> a node should have 2 or 0 children
 *
 * 	 			18
            /     \
          40       30
                   /  \
                 100   40
 *
 * 	 6) A Complete binary tree ->  A Binary Tree is complete Binary Tree if all levels are completely filled
 * 	    except possibly the last level and the last level has all keys as left as possible
 *
 * Example ,	18
           /       \
         15         30
        /  \        /  \
      40    50    100   40
     /  \   /
    8   7  9
 *
 * ****/
void printInOrder(struct bnode* node)
{
    // In order Traversal -> LNR
    if(node == NULL)
	return;
    printInOrder(node->left);
    printf("%d ", node->data);
    printInOrder(node->right);
}
void printPreOrder(struct bnode* node)
{
    // pre order Traversal -> NLR
    // used to create a copy of the tree.
    if(node == NULL)
	return;
    printf("%d ", node->data);

    printPreOrder(node->left);

    printPreOrder(node->right);
}

void printPostOrder(struct bnode* node)
{
    // pre order Traversal -> LRN
    // can be used to delete a tree.
    if(node == NULL)
	return;

    printPostOrder(node->left);

    printPostOrder(node->right);

    printf("%d ", node->data);
}
int max(int a, int b)
{
    return (a > b) ? a : b;
}
int maxDepthOfTree(struct bnode* root)
{   // Complexity: o(n)
    /*determines max depth or height of a tree.
     * recursively calculate the maxDepth of Left and right subtree
     * Get the max of the left and subtree maxDepth.
     * that becomes the max depth of tree*/

    /*
     *
     * Recursively calculate height of left and right subtrees of a node and assign height to the node as max of the
 heights of two children plus 1. See below pseudo code and program for details.

    Algorithm:

    maxDepth()
    1. If tree is empty then return 0
    2. Else
 (a) Get the max depth of left subtree recursively  i.e.,
      call maxDepth( tree->left-subtree)
 (a) Get the max depth of right subtree recursively  i.e.,
      call maxDepth( tree->right-subtree)
 (c) Get the max of max depths of left and right
      subtrees and add 1 to it for the current node.
     max_depth = max(max dept of left subtree,
                         max depth of right subtree)
                         + 1
 (d) Return max_depth

     * */

    if(root == NULL)
	return 0;

    int leftDepth = maxDepthOfTree(root->left);
    int rightDepth = maxDepthOfTree(root->right);

    return max(leftDepth, rightDepth) + 1;
}

int identicalTrees(struct bnode* a, struct bnode* b)
{

    if(a == NULL && b == NULL) {
	return 1; // If reached the end, then the trees are identical.
    }
    if(a != NULL && b != NULL) { // Very important, data should be not null
	return (a->data == b->data) && identicalTrees(a->left, b->left) && identicalTrees(a->right, b->right);
    }

    return 0; // if reached here, they they are not identical.
}

void deleteAllTreeNodes(struct bnode* root)
{
    // Post order traversal is the best way to delete a tree.
    // Because, in that only left and right child were traversed first.
    // Rest traversal can also be used, however we need extra space.
    // We can achieve the deletion in post order with time complexity of o(n).
    if(root == NULL) // Base Case
	return;
    deleteAllTreeNodes(root->left);
    deleteAllTreeNodes(root->right);
    printf("Deleting the Node %d\n", root->data);
    free(root);
}
void deleteTree(struct bnode** root)
{
    deleteAllTreeNodes(*root);
    *root = NULL; // Need to set the root to NULL
}

void constructMirror(struct bnode* root)
{
    /*	Mirror of a Tree: Mirror of a Binary Tree T is another Binary Tree M(T) with left
     * and right children of all non-leaf nodes interchanged.
    */
    /* Change a tree so that the roles of the  left and
        right pointers are swapped at every node.

     So the tree...
           4
          / \
         2   5
        / \
       1   3

     is changed to...
           4
          / \
         5   2
            / \
           3   1
    */
    if(root == NULL) {
	return;
    } else {
	struct bnode* temp;
	// Calling the mirror on left and righ subtree.
	constructMirror(root->left);
	constructMirror(root->right);

	// swapping the node.
	temp = root->left;
	root->left = root->right;
	root->right = temp;
    }
}
struct DllNode* convertTreeToList(struct bnode* root)
{
    // Converts the binary tree to doubly linked list.
    /*Prblm Statement:-
     * Write a recursive function treeToList(Node root) that takes an ordered binary tree and rearranges the internal
     pointers to make a circular doubly linked list out of the tree nodes.
     *
     * TheÃ¢â‚¬ï¿½previousÃ¢â‚¬ï¿½ pointers should be stored in the Ã¢â‚¬Å“smallÃ¢â‚¬ï¿½ field and the Ã¢â‚¬Å“nextÃ¢â‚¬ï¿½ pointers should be stored in the
     Ã¢â‚¬Å“largeÃ¢â‚¬ï¿½ field.
     *
     * The list should be arranged so that the nodes are in increasing order.

             * Return the head pointer to the new list.
     *
     * */
    // converts the tree to list.
    if(root == NULL)
	return NULL;
    // recursively call upon the left and righ subtree.
    struct DllNode* aList = convertTreeToList(root->left);
    struct DllNode* bList = convertTreeToList(root->right);

    // root element left and right must point to itself.
    //  1 -> next = 1
    //  1 -> prev =1, becomes a standalone element.
    struct DllNode* currElem = malloc(sizeof(struct DllNode));
    currElem->next = currElem;
    currElem->prev = currElem;
    currElem->data = root->data;

    // append two dlls in sorted increasing order.
    aList = appendTwoDll(aList, currElem); // aList appended with root list
    aList = appendTwoDll(aList, bList); // aList again appended with blist.

    return aList;
}

void printRootToLeafPaths(struct bnode* root, int path[], int pathLen)
{

    if(root == NULL) { // leaf node's right and left child will be null.
	// so here, path should not be printed. since the root to leaf path will be printed twice.
	//			printArray(path,pathLen);
	return;
    }
    path[pathLen++] = root->data; // append the node data to path.
    if(root->left == NULL && root->right == NULL) { // if both left and right child is empty.
	printArray(path, pathLen); // print the array
	return;
    }
    printRootToLeafPaths(root->left, path, pathLen);
    printRootToLeafPaths(root->right, path, pathLen);
}
void printPaths(struct bnode* root)
{
    int path[1000];
    printRootToLeafPaths(root, path, 0);
}
void printLevelOrderTraversal(struct bnode* root)
{//Doesn't print the tree line by line
    // O(n) Complexity.
    // use a queue for level order traversal.
    struct bQueue* queue = createBQueue(100);
    struct bnode* cur = root; // first element, no need to put in a queue.
    while(cur) {
	printf("%d ", cur->data);
	if(cur->left) {
	    enqueueBNode(queue, cur->left);
	}
	if(cur->right) {
	    enqueueBNode(queue, cur->right);
	}
	cur = dequeueBNode(queue);
    }
}
int getLeafCount(struct bnode* root)
{
    if(root == NULL) // for internal node having only one pointer (either left or right)
	return 0;
    if(root->left == NULL && root->right == NULL) {
	return 1; // leaf node
    }
    return getLeafCount(root->left) + getLeafCount(root->right);
}
void alternateLevelSpiralTraversal(struct bnode* root)
{
    struct bStack* s1 = NULL; // If not assigned null, we will have problem in push and pop operation.
    struct bStack* s2 = NULL;
    // stack 1 -> for storing the data from left to right.
    // stack 2 -> for storing the data from right to left nodes.
    pushBStack(&s1, root); // only 1 node
    while(s1 != NULL || s2 != NULL) {
	// if either of the stack is not empty
	while(s1 != NULL) {
	    struct bnode* cur = popBStack(&s1);
	    printf("%d ", cur->data);
	    if(cur->right) // Very very important.
		pushBStack(&s2, cur->right); // right to left traversal for the next level.
	    if(cur->left)
		pushBStack(&s2, cur->left);
	}
	while(s2 != NULL) {
	    struct bnode* cur = popBStack(&s2);
	    printf("%d ", cur->data); // Left to right traversal for the next level.
	    if(cur->left)
		pushBStack(&s1, cur->left);
	    if(cur->right)
		pushBStack(&s1, cur->right);
	}
    }
}
void printGivenLevel(struct bnode* root, int level, int itr)
{
    if(root == NULL)
	return;
    if(level == 1) {
	// only if the level is 1,
	printf("%d", root->data);
	return;
    }
    if(level > 1) {
	if(itr) {
	    printGivenLevel(root->left, level - 1, itr);
	    printGivenLevel(root->right, level - 1, itr);
	} else {
	    printGivenLevel(root->right, level - 1, itr);
	    printGivenLevel(root->left, level - 1, itr);
	}
    }
}

void printSpiral(struct bnode* root)
{
    // takes O(n^2) time -> to print alternative levels in spiral fashion.
    int height = maxDepthOfTree(root);
    int i;
    int itr = 0; // 0 means right to left traversal.
    // 1 means left to right traversal.
    for(i = 1; i <= height; i++) {
	printGivenLevel(root, i, itr);
	itr = !itr;
    }
}

int isChildrenSumParentValue(struct bnode* root)
{
    int left_data = 0;
    int right_data = 0;
    if(root == NULL || (root->left == NULL && root->right == NULL)) {
	return 1; // leaf node, then return bool 1.
    } else {
	if(root->left) {
	    left_data = root->left->data;
	}
	if(root->right) {
	    right_data = root->right->data;
	}
	// check if the parent's value is equal to child's value
	// and also, traverse the check to left and right subtree.
	return (root->data == left_data + right_data) && isChildrenSumParentValue(root->left) &&
	       isChildrenSumParentValue(root->right);
    }
}
void incrementNode(struct bnode* root, int diff)
{
    if(root->left) {
	root->left->data = root->left->data + diff;
	incrementNode(root->left, diff);
    } else if(root->right) {
	root->right->data = root->right->data + diff;
	incrementNode(root->right, diff);
    }
}
// Converting a tree to hold children sum property
void convertTreeToChildremSuM(struct bnode* root)
{
    // Time complexity: O(n^2), Worst case complexity is for a skewed tree such that nodes are in decreasing order from
    // root to leaf.
    /*Traverse given tree in post order to convert it,
     *
     * i.e., first change left and right children to hold the children sum property then change the parent node.

            Let difference between nodeÃ¢â‚¬â„¢s data and children sum be diff.

            diff = nodeÃ¢â‚¬â„¢s children sum - nodeÃ¢â‚¬â„¢s data

            If diff is 0 then nothing needs to be done.

            If diff > 0 ( nodeÃ¢â‚¬â„¢s data is smaller than nodeÃ¢â‚¬â„¢s children sum) increment the nodeÃ¢â‚¬â„¢s data by diff.

            If diff < 0 (nodeÃ¢â‚¬â„¢s data is greater than the node's children sum) then increment one childÃ¢â‚¬â„¢s data.
             * We can choose to increment either left or right child if they both are not NULL.
             *
             * Let us always first increment the left child.
             *
             * Incrementing a child changes the subtreeÃ¢â‚¬â„¢s children sum property
             * 			so we need to change left subtree also.
             *
             * So we recursively increment the left child.
             *
             * If left child is empty then we recursively call increment() for right child.
             * Let us run the algorithm for the given example.
             *
             * First convert the left subtree (increment 7 to 8).
             *              50
/     \
/         \
8             2
/ \             /\
/     \          /   \
3        5      1      30

Then convert the right subtree (increment 2 to 31)

50
/    \
/        \
8            31
/ \           / \
/     \       /     \
3       5    1       30

Now convert the root, we have to increment left subtree for converting the root.

50
/    \
/        \
19           31
/ \           /  \
/     \       /      \
14      5     1       30

Please note the last step Ã¢â‚¬â€œ we have incremented 8 to 19, and to fix the subtree we have incremented 3 to 14.
*/
    if(root == NULL || (root->left == NULL && root->right == NULL)) {
	// for an internal node with 1 children, or a leaf node
	return;
    } else {
	// proceed on for the child
	convertTreeToChildremSuM(root->left);
	convertTreeToChildremSuM(root->right);
	int left_data = 0, right_data = 0;
	if(root->left) {
	    left_data = root->left->data;
	}
	if(root->right) {
	    right_data = root->right->data;
	}
	// diff = children's sum - node's data.
	int diff = left_data + right_data - root->data;
	if(diff > 0) {
	    root->data = root->data + diff;
	}
	if(diff < 0) {
	    incrementNode(root, -diff); // to make it positive
	}
    }
}
int diameterOfTree(struct bnode* root)
{

    // Time complexity: takes O(n^2) time.
    /* 	The diameter of a tree (sometimes called the width) is the number of nodes on the longest path
     * 	between two leaves in the tree.
     *
     * 	The diameter of a tree T is the ----largest [MAx]---- of the following quantities:

    * the diameter of TÃ¢â‚¬â„¢s left subtree
    * the diameter of TÃ¢â‚¬â„¢s right subtree
    * the longest path between leaves that goes through the root of T
    * (this can be computed from the heights of the subtrees of T)
     *   -- i.e height of left tree + height of right tree +1 [passing through]
     * */
    if(root == NULL)
	return 0; // Base Case.
    int leftTreeHeight = maxDepthOfTree(root->left); // maxDepth and maximum height both are equal
    int rightTreeHeight = maxDepthOfTree(root->right);

    int leftDiameter = diameterOfTree(root->left);
    int rightDiameter = diameterOfTree(root->right);

    return max(max(leftTreeHeight + rightTreeHeight + 1, leftDiameter), rightDiameter);
}
int diameterOfTreeOptimized(struct bnode* root, int* height)
{
    int lh = 0, rh = 0;
    if(root == NULL) {
	*height = 0;
	return 0;
    }
    int leftDiameter = diameterOfTreeOptimized(root->left, &lh);
    int rightDiameter = diameterOfTreeOptimized(root->right, &rh);

    *height = max(lh, rh) + 1; // max of left height, right height + 1
    // provides the height to parent.

    return max(max(lh + rh + 1, leftDiameter), rightDiameter); // as per diameter rule.
}

int isHeightBalancedTree(struct bnode* root)
{
    // Time complexity: if in case skewed tree, takes o(n^2) time, similar to diameterOfTree

    /*
     * Consider a height-balancing scheme where following conditions should be checked to determine if a binary tree is
     balanced.

             * An empty tree is height-balanced. A non-empty binary tree T is balanced if:
     [similar to the diameter of tree calculation]
     * 1) Left subtree of T is balanced

     * 2) Right subtree of T is balanced

     * 3) The difference between heights of left subtree and right subtree is not more than 1.

     * The above height-balancing scheme is used in AVL trees.
     * */
    if(root == NULL) {
	return 1; // must return 1
    }
    //---------- This block itself getting iterated for n nodes in the tree.
    int leftHeight = maxDepthOfTree(root->left); // takes o(n) time
    int rightHeight = maxDepthOfTree(root->right); // takes o(n) time
    //----------
    if(abs(leftHeight - rightHeight) <= 1 && isHeightBalancedTree(root->left) &&
       isHeightBalancedTree(root->right)) // Not more than 1, left subtree and right subtree are balanced, then balanced
    {
	return 1;
    } else {
	return 0;
    }
}
int isHeightBalancedTreeOptimized(struct bnode* root, int* height)
{
    // Time complexity -> o(n) time.
    int lh = 0, rh = 0;
    if(root == NULL) {
	*height = 0;
	return 1; // leaf is always balanced .
    }
    int leftBalanced = isHeightBalancedTreeOptimized(root->left, &lh);
    int rightBalanced = isHeightBalancedTreeOptimized(root->right, &rh);

    *height = max(lh, rh) + 1; // provides the height to parent.

    return leftBalanced && rightBalanced && abs(lh - rh) <= 1;
}
void printInOrderWithoutRecurAndStack(struct bnode* root)
{
    // morris traversal - going to make use of threaded binary tree, in order successor. Similarly
    /*	In this traversal, we first create links to Inorder successor and print the data using these links,
     *  and finally revert the changes to restore original tree.
     *
     * Although the tree is modified through the traversal, it is reverted back to its original shape after the
     *completion.
     * Unlike Stack based traversal, no extra space is required for this traversal.
     *
     * */
    struct bnode* current = root;
    struct bnode* predece;
    if(root == NULL)
	return;
    while(current != NULL) {
	if(current->left == NULL) { // in threaded binary tree, current->right always point to the inorder sucessor
	    // for the leaf node, print the data
	    printf("%d", current->data); // N node
	    current = current->right; // inorder successor -> would point to N node, if the above line is L.
	} else { // Executs for internal node
	    // if we have a left subtree then find the inorder predecessor
	    /*to find inorder predecessor*/
	    predece = current->left; // left subtree.
	    while(predece->right != NULL && predece->right != current) {
		// predece->right != Current -- to check if it's not already linked
		// predece->right != current is false when printing inorder successor.
		predece = predece->right;
	    }
	    // here we assume, we have reached the inorder predecessor.
	    if(predece->right == NULL) {
		// if predecessor's right pointer is not assigned.
		predece->right = current; // set the successor to the right's pointer.
		// L - Assign successor for left subtree
		current = current->left;
	    } else { // revert back the assignment we have done., and also printing the data.
		predece->right = NULL;
		// N - Node
		printf("%d", current->data); // print the data [here the N node]
		// R- Right, assign succeessor for right.
		current = current->right; // Assign the successor for right subtree.
	    }
	}
    }
}
int hasPathSum(struct bnode* root, int sum)
{
    if(root == NULL) // Base case, if the tree itself is NULL
	return sum == 0; // if reached the end of the tree, then return sum == 0

    int res = 0;
    int subSum = sum - root->data;

    if(subSum == 0 && root->left == NULL && root->right == NULL) {
	return 1; // for the leaf node, if sum is 0, return 1;
    }

    if(root->left) {
	res = res || hasPathSum(root->left, subSum); // if path sum found in left tree itself, then res =1
    }
    if(root->right) {
	res = res || hasPathSum(root->right, subSum); // no need to find in right tree when res is already 1.
    }
    return res;
}
int search(char arr[], int start, int end, char data)
{
    int i;
    for(i = start; i <= end; i++) {
	if(arr[i] == data)
	    return i;
    }
}
struct bnode* constructTreeFromInAndPreOrder(char in[], char pre[], int inStart, int inEnd)
{
    // InStart, InEnd -> Inorder start and end index
    static int preIndex = 0; // if assigned static, the data will be taken from memory only

    if(inStart > inEnd) {
	return NULL; // basic case
    }
    struct bnode* node = newBNode(pre[preIndex++]); // increment the preIndex, for the next recursion call to take
                                                    // effect
    // N Node
    if(inStart == inEnd)
	return node; // this is the element to be returned. inStart and inEnd are equal

    int inIndex = search(in, inStart, inEnd, node->data);
    // must have to pass inStart and inEnd index.
    // L Node
    node->left = constructTreeFromInAndPreOrder(in, pre, inStart, inIndex - 1); // inIndex need to be excluded.
    // R Node
    node->right = constructTreeFromInAndPreOrder(in, pre, inIndex + 1, inEnd);

    return node;
}
void doubleTree(struct bnode* root)
{
    /*	To create Double tree of the given tree,
     *
     * create a new duplicate for each node, and insert the duplicate as the left child of the original node.

So the treeÃ¢â‚¬Â¦

2
/ \
1   3

is changed toÃ¢â‚¬Â¦

   2
  / \
 2   3
/   /
1   3
/
1

     *
     * Algorithm:
    Recursively convert the tree to double tree in postorder fashion.
     * For each node, first convert the left subtree of the node, then right subtree,
*	  finally create a duplicate node of the node and fix the left child of the node and left child of left child.
     * */
    if(root == NULL)
	return;
    // post order fashion
    doubleTree(root->left); // L Node
    doubleTree(root->right); // R Node
    struct bnode* oldLeft = root->left;
    root->left = newBNode(root->data);
    root->left->left = oldLeft;
}
int getWidth(struct bnode* root, int level)
{
    /*
     *  consider the following tree in a bottom up fashion.
     *
     * To find the level 3 node's width, we had to traverse downwards and find the number of elements at that level,
     * Traverse downwards -> reducing the level at each traversal.
     *
     * 3 Levels
     *
     * Starting the call as getWidth (root, level 3)
     *
     *      	root(r3)   1
     *
     *    left(r2)    right(r1)   2
     *
     * left (r1)  right(r1)      right(r1) 3
     *
     * r1 denotes returning 1.
     *
     *
     *
     *
     * */
    // Gets width of the tree.
    if(root == NULL)
	return 0; // null return 0;
    if(level == 1)
	return 1; // Level 1 means the root node reached in stack,
    if(level > 1) {
	return getWidth(root->left, level - 1) + getWidth(root->right, level - 1);
    }
}
int getMaxWidth(struct bnode* root)
{
    /*
     * Let us consider the below example tree.

1
/  \
2    3
/  \     \
4    5     8
  /  \
 6    7

For the above tree,
width of level 1 is 1,
width of level 2 is 2,
width of level 3 is 3
width of level 4 is 2.

So the maximum width of the tree is 3.
     * */
    int max = 0, i, curWidth;
    int height = maxDepthOfTree(root);
    // takes O(n^2) time.
    for(i = 1; i <= height; i++) {
	curWidth = getWidth(root, i);
	if(max < curWidth) {
	    max = curWidth;
	}
    }
    return max;
}
int getWidthUsingQueue(struct bnode* root)
{

    struct bQueue* queue = createBQueue(10);
    int tempSize, max = 0;
    if(root == NULL)
	return;
    enqueueBNode(queue, root); // root node enqueued.

    while(queue->size != 0) {
	tempSize = queue->size;
	if(max < tempSize) {
	    max = tempSize;
	}
	while(tempSize != 0) { // for each level, temp size becomes zero.
	    root = dequeueBNode(queue); // dequeue the node
	    if(root->left) {
		enqueueBNode(queue, root->left);
	    }
	    if(root->right) {
		enqueueBNode(queue, root->right);
	    }
	    tempSize--;
	}
    }
    return max;
}
int getWidthUsingRecurrence(struct bnode* root, int count[], int level)
{

    if(root) {
	count[level]++;
	getWidthUsingRecurrence(root->left, count, level + 1);
	getWidthUsingRecurrence(root->right, count, level + 1);
    }
}
int getMaxWidthByUsingLevelArray(struct bnode* root)
{

    int height = maxDepthOfTree(root);
    int* count = (int*)calloc(sizeof(int), height); // calloc initializes the array to 0 by default.
    getWidthUsingRecurrence(root, count, 0);

    int i, max = 0;
    int curWidth;
    for(i = 0; i < height; i++) {
	curWidth = count[i];
	if(max < curWidth) {
	    max = curWidth;
	}
    }
    return max;
}
int isStructSame(struct bnode* root1, struct bnode* root2)
{

    if(root1 == NULL && root2 == NULL) {
	return 1;
    }
    if(root1 != NULL && root2 != NULL) {
	return isStructSame(root1->left, root2->left) && isStructSame(root1->right, root2->right);
    }
    return 0;
}
int isFoldable(struct bnode* root)
{
    /*
     * A tree can be folded if left and right subtrees of the tree
    are structure wise mirror image of each other. An empty tree is considered as foldable.

    Consider the below trees:
    (a) and (b) can be folded.
    (c) and (d) cannot be folded.

(a)
   10
 /    \
7      15
 \    /
  9  11

(b)
    10
   /  \
  7    15
 /      \
9       11

(c)
    10
   /  \
  7   15
 /    /
5   11

(d)

     10
   /   \
  7     15
/  \    /
9   10  12

     *
     *
     * */
    if(root == NULL)
	return 0;
    constructMirror(root->left); // construct mirror of the tree.
    // swaps the node.
    int isFold = isStructSame(root->left, root->right);

    constructMirror(root->left); // changed to the original tree.

    return isFold;
}

int isStructureWiseMirror(struct bnode* a, struct bnode* b)
{
    // Structure wise mirror of each other.
    if(a == NULL && b == NULL)
	return 1;
    if(a == NULL || b == NULL) // if one or the other is null, return 0
	return 0;

    return isStructureWiseMirror(a->left, b->right) && isStructureWiseMirror(a->right, b->left);
}
int isFoldableByCheckingMirror(struct bnode* root)
{
    if(root == NULL)
	return 0;
    return isStructureWiseMirror(root->left,
                                 root->right); // is left and right tree structure wise mirror of each other.?
}
void printKDistantNodes(struct bnode* root, int level)
{
    /*
     * Given a root of a tree, and an integer k. Print all the nodes which are at k distance from root.

            For example, in the below tree, 4, 5 & 8 are at distance 2 from root.

        1
      /   \
    2      3
  /  \    /
4     5  8

     *
     * */
    if(root == NULL)
	return;
    if(level == 0) {
	printf("%d\t", root->data);
    } else {
	printKDistantNodes(root->left, level - 1);
	printKDistantNodes(root->right, level - 1);
    }
}
int getLevelUtilOfANodeInTree(struct bnode* root, int data, int level)
{
    if(root == NULL)
	return 0;
    if(root->data == data) {
	return level;
    }
    int downLevel = getLevelUtilOfANodeInTree(root->left, data, level + 1); // if found in the left subtree.
    if(downLevel != 0)
	return downLevel;
    downLevel = getLevelUtilOfANodeInTree(root->right, data, level + 1); // if found in the right subtree.
    return downLevel;
}
int getLevel(struct bnode* node, int data)
{
    return getLevelUtilOfANodeInTree(node, data, 1);
}
int printAncestors(struct bnode* root, int target)
{
    if(root == NULL) {
	return 0;
    }
    if(root->data == target)
	return 1; // if the data found, then return true.
    if(printAncestors(root->left, target) || printAncestors(root->right, target)) {
	printf("%d ", root->data);
	return 1;
    }
    return 0;
}
int sum(struct bnode* root)
{
    // sum of a node -> sum of left subtree + data + sum of right subtree
    if(root == NULL)
	return 0;
    else {
	return sum(root->left) + root->data + sum(root->right);
    }
}
int isSuMTree(struct bnode* root)
{
    // notice the subtle difference between sum tree and children sum
    // SuM Tree -> calculates the sum of left subtree and right subtree, finds if it equals to the node data
    // children SuM -> calculates the immediate left node data and right node data, see if sum of those data equals to
    // the node data.
    // Takes O(n^2) time complexity
    if(root == NULL || (root->left == NULL && root->right == NULL)) {
	return 1;
    }
    int ls = 0;
    int rs = 0;
    if(isSuMTree(root->left) && isSuMTree(root->right)) {
	// if both the left and right subtree are sum tree, then check for current node -> sum property
	ls = sum(root->left);
	rs = sum(root->right);
	//		if(ls+rs)
	return (ls + rs == root->data);

    } else {
	return 0;
    }
}
int isLeafNode(struct bnode* root)
{
    if(root == NULL)
	return 0;
    if(root->left == NULL && root->right == NULL)
	return 1;
    return 0;
}
int isSuMTreeTricky(struct bnode* root)
{
    // Takes O(n) time complexity.

    /*The method 2 uses following rules to get the sum directly.
    1) If the node is a leaf node then sum of subtree rooted with this node is equal to value of this node.
    2) If the node is not a leaf node then sum of subtree rooted with this node is twice the value of this node
     * (Assuming that the tree rooted with this node is SumTree).
     *
     * */
    if(root == NULL || isLeafNode(root)) {
	return 1; // if either a leaf node or the tree itself is null, then return success.
    }
    int ls, rs;
    ls = 0;
    rs = 0;
    if(isSuMTreeTricky(root->left) && isSuMTreeTricky(root->right)) {
	// only if the left subtree and right subtree are sum tree.
	if(root->left == NULL) {
	    ls = 0; // left subtree sum is 0
	} else if(isLeafNode(root->left)) {
	    ls = root->left->data;
	} else if(root->left) {
	    ls = 2 * (root->left->data);
	}

	if(root->right == NULL) {
	    rs = 0; // left subtree sum is 0
	} else if(isLeafNode(root->right)) {
	    rs = root->right->data;
	} else if(root->right) {
	    // assuming that the subtree satisfies the property
	    // ie ls+rs == root->data means 2*root->data
	    rs = 2 * (root->right->data);
	}

	return (ls + rs == root->data);
    }
}
int isSubTreeBinaryTree(struct bnode* T, struct bnode* S)
{
    // Time worst case complexity of above solution is O(mn) where m and n are number of nodes in given two trees.
    if(T == NULL)
	return 0;
    if(S == NULL)
	return 1;
    if(identicalTrees(T, S))
	return 1;
    // see identical for the left
    return identicalTrees(T->left, S) || identicalTrees(T->right, S);
}
void storeInOrder(struct bnode* T, char inT[], int* i)
{
    if(T == NULL) {
	inT[(*i)++] = '$';
	return;
    }
    storeInOrder(T->left, inT, i);
    inT[(*i)++] = (char)T->data;
    storeInOrder(T->right, inT, i);
}
void storePreOrder(struct bnode* T, char preT[], int* i)
{
    if(T == NULL) {
	preT[(*i)++] = '$';
	return;
    }
    preT[(*i)++] = (char)T->data;
    storePreOrder(T->left, preT, i);
    storePreOrder(T->right, preT, i);
}
int isSubTreeBinaryTreeUsingInAndPreOrder(struct bnode* T, struct bnode* S)
{
    /*
     * Tree S is a subtree of T if both inorder and preorder traversals of S
     * are substrings of inorder and preorder traversals of T respectively.
     *
     * we just need to find if the inorder traversal of S is a substring of T.
     *
     *  Inorder and Preorder traversals of Binary Tree take O(n) time.
     *
     * The above algorithm doesn't work for cases where a tree is present
in another tree, but not as a subtree. Consider the following example.

    Tree1   T =>    c a $ x b
      x
    /    \
  a       b
 /
c


    Tree2	  S =>	  c a $ x $ b d
      x
    /    \
  a       b
 /         \
c            d

Inorder and Preorder traversals of the big tree or Tree2 are.

Inorder and Preorder traversals of small tree or Tree1 are

The Tree2 is not a subtree of Tree1, but inS[] and preS[] are
subarrays of inT[] and preT[] respectively.
     *
     * */
    int m, n;
    m = 0;
    n = 0;
    char inT[INT_MAX], inS[INT_MAX];
    char preT[INT_MAX], preS[INT_MAX];
    storeInOrder(T, inT, &m);
    storeInOrder(S, inS, &n);
    inT[m] = '\0';
    inS[n] = '\0'; // must have to end the char array with null

    if(strstr(inT, inS) == NULL)
	return 0;
    printf("\n%s %s", inT, inS);
    m = 0;
    n = 0;
    storePreOrder(T, preT, &m);
    storePreOrder(S, preS, &n);
    preT[m] = '\0';
    preS[n] = '\0';
    printf("\n%s %s", preT, preS);
    return (strstr(preT, preS) != NULL); // if and only if the sub string can be found in parent
}
struct connNode {
    int data;
    struct connNode* left;
    struct connNode* right;
    struct connNode* nextRight;
};
struct connNode* newConnNode(int data)
{
    struct connNode* node = (struct connNode*)malloc(sizeof(struct connNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->nextRight = NULL;

    return (node);
}
void connectNodeAtSameLevelUsingBFS()
{
    //	we can connect node at the same level using bfs
    // Following is the algorithm
    /**
     * 1. enqueue the root node
     * 2. while (queue is not empty){
     *		2.1. get size of the queue as temp_size
                    2.2  while (temp_size!=0)
                     * {
                     * 		dequeue the element.
                     * 		element dequeued's left and right subtree are again added to the queue
                     * 		temp_size--;
                     * 		if(temp_size==0)
                     * 		{
                     * 			element->nextRight = NULL;
                     * 		}else{
                     * 			element->nextRight = frontOfQueue;
                     *		}
                     * }
     *
     * }
     *
     * /
    */
}
void connectNodePreOrderUtil(struct connNode* node)
{
    /*In this method we set nextRight in Pre Order fashion to make sure that the nextRight of parent is set before its
     children.
     *
     * When we are at node p, we set the nextRight of its left and right children.
     *
     * Since the tree is complete tree, nextRight of pÃ¢â‚¬â„¢s left child (p->left->nextRight) will always be pÃ¢â‚¬â„¢s right child,
     * and nextRight of pÃ¢â‚¬â„¢s right child (p->right->nextRight) will always be left child of pÃ¢â‚¬â„¢s nextRight
     * (if p is not the rightmost node at its level).

            If p is the rightmost node, then nextRight of pÃ¢â‚¬â„¢s right child will be NULL.
     *
     * */
    // Connects node at the same level
    // Like pre order, N L R is taken care.
    // Takes o(n) complexity
    if(!node) // Null return
	return;

    // we have covered the root node, now go for left subtree
    if(node->left) {
	node->left->nextRight = node->right; // left's adjacent node is always right
    }
    if(node->right) {
	// if the right subtree's parent node's adjacent is not null, then get the left element
	// else null.
	node->right->nextRight = (node->nextRight) ? node->nextRight->left : NULL;
    }
    connectNodePreOrderUtil(node->left); // go further down for left subtree
    connectNodePreOrderUtil(node->right);
    /*doesn't hold for binary trees that aren't complete
     *
     * Let us consider following tree as an example.
     * In Method 2, we set the nextRight pointer in pre order fashion.
     * When we are at node 4, we set the nextRight of its children which are 8 and 9
     * (the nextRight of 4 is already set as node 5). nextRight of 8 will simply be set as 9,
     * but nextRight of 9 will be set as NULL which is incorrect.
     * We canÃ¢â‚¬â„¢t set the correct nextRight, because when we set nextRight of 9,
     * we only have nextRight of node 4 and ancestors of node 4,
     * we donÃ¢â‚¬â„¢t have nextRight of nodes in right subtree of root.
     *
     * 9's next right should be 10

        1
      /    \
    2        3
   / \      /  \
  4   5    6    7
 / \           / \
8   9        10   11

     * */
}
void connectNodeAtSameLevelUsingPreOrder(struct connNode* node)
{
    // holds only for the complete binary tree, in which all the nodes are as left as possible and at the same level.
    node->nextRight = NULL; // the right child of root node is always null
    connectNodePreOrderUtil(node);
}
struct connNode* getNextRightInLevel(struct connNode* root)
{
    root = root->nextRight; // adjacent node at parent's level.
    while(root != NULL) {
	if(root->left != NULL) // if the parent has the left tree, then return that.
	    return root->left;
	if(root->right != NULL)
	    return root->right;
	root = root->nextRight; // traverse through the adjacent nodes.
    }
    return NULL;
}
void connectNodesUtilRecurByLevelWise(struct connNode* node)
{
    if(!node) // if null then return as such.
	return;

    if(node->nextRight != NULL) {
	/* Before setting nextRight of left and right children, set nextRight
of children of other nodes at same level (because we can access
children of other nodes using p's nextRight only) */
	// finish lining up by level wise.
	connectNodesUtilRecurByLevelWise(node->nextRight);
    }

    if(node->left) {
	// if the node has a left child, then check if the node has right child too.
	if(node->right) {
	    node->left->nextRight = node->right;
	    node->right->nextRight = getNextRightInLevel(node);
	} else {
	    node->left->nextRight = getNextRightInLevel(node);
	}
	/* Recursively call for next level nodes.  Note that we call only
for left child. The call for left child will call for right child */
	connectNodesUtilRecurByLevelWise(node->left);
    } /* If left child is NULL then first node of next level will either be
  p->right or getNextRight(p) */
    else if(node->right) {
	node->right->nextRight = getNextRightInLevel(node); // get next right from level.
	connectNodesUtilRecurByLevelWise(node->right); // proceed for the right child
    } else {
	connectNodesUtilRecurByLevelWise(
	    getNextRightInLevel(node)); // proceed for the next adjacent node, got from level wise order.
    }
}
void populateInOrderSuccessor(struct connNode* node)
{
    // Takes O(N) complexity
    // populate inorder successor for next pointer.
    static struct connNode* next = NULL; // get that from memory

    if(node) {
	/*
	 * 				   N next
	 * 				/    \
	 * 			 N			R
	 *       /   \
	 * 		L		 R
	 *
	 * */
	populateInOrderSuccessor(node->right);

	node->nextRight = next;

	next = node;

	populateInOrderSuccessor(node->left);
    }
}
int convertTreeToSuMTree(struct bnode* root)
{

    // converting tree to sum tree steps
    /*
     * 1) A node's value is equal to sum of left subtree and right subtree.

     **/
    if(root == NULL)
	return 0; // if leaf node's child then return 0

    int old_data = root->data; // this old data need to returned for parent.

    root->data = convertTreeToSuMTree(root->left) + convertTreeToSuMTree(root->right);
    // always the node's current data should be equal to the sum of left and right subtree.

    return old_data + root->data;
    // old data + left and right subtree -> let us achieve this conversion of sum tree
    // in O(N) time.
}
int printPathEqualToNode(struct bnode* root, struct bnode* node)
{
    if(root == NULL)
	return 0;
    if(root == node || printPathEqualToNode(root->left, node) || printPathEqualToNode(root->right, node)) {
	printf(" %d ", root->data); // print the ancestor and current node as well
	return 1;
    }
    return 0;
}
// This function Sets the target_leaf_ref to refer the leaf node of the maximum
// path sum.  Also, returns the max_sum using max_sum_ref
void getTargetLeaf(struct bnode* root, struct bnode** target_leaf, int curSuM, int* maxSuM)
{
    // root is the current node
    // sets the target leaf node, so that we can print the path
    // current sum is the sum of nodes in the traversal
    // maxSuM always holds the maximum obtained through so far.
    if(root == NULL)
	return;

    curSuM = curSuM + root->data; // add the current nodes' data to current sum
    if(root->left == NULL && root->right == NULL) {
	// if reached leaf node, then check if the curSuM is greater than the maxSuM obtained so far, if then
	// set the target leaf to this node, maxSuM to cursum
	if(curSuM > (*maxSuM)) {
	    *maxSuM = curSuM;
	    *target_leaf = root;
	}
    }
    getTargetLeaf(root->left, target_leaf, curSuM, maxSuM);
    getTargetLeaf(root->right, target_leaf, curSuM, maxSuM);
}
int getMaxIndexInArray(int arr[], int start, int end)
{
    int i, max = 0;
    for(i = start + 1; i <= end; i++) {
	if(max < arr[i])
	    max = i;
    }
    return max;
}
struct bnode* constructSpecialBinaryTreeFromInorder(int arr[], int start, int end)
{
    // Special binary tree can be constructed from in order traversal.
    // Property of special binary tree: Node's element value is larger than both the left and right subtree.
    // i.e to the left and to the right, all elements will be smaller.
    // for example, let the arrray is {1,5,10,40,30,15,28,20}
    // Find max, Max element is 40, that becomes the root
    //          40
    //    /           \
	//   {1,5,10}    {30,15,28,20}
    // We do recursively the above steps for left and right subtree.

    if(start > end)  //          10
	return NULL; //      {1,5} NULL

    // Get max index from the array.
    int index = getMaxIndexInArray(arr, start, end);

    struct bnode* root = newBNode(arr[index]);
    // if there is only one element left, then that element should be returned.
    if(start == end)
	return root;

    root->left = constructSpecialBinaryTreeFromInorder(arr, start, index - 1);
    root->right = constructSpecialBinaryTreeFromInorder(arr, index + 1, end);
}
struct bnode* constructSpecialBinaryTreeFromPreOrder(int preArr[], char preLN[], int* preArrIndex, int n)
{
    /*
     * Properties of special binary tree:-
     *
     * 1) Tree has either 0 or two children
     *
     * 2) Array preLN given having only two possible values, L / N
     * The value L corresponds to a leaf node in the binary tree.
     * The value N corresponds to internal node having two children.
     *
     * Arrive at a function that constructs binary tree using the above properties.
     * */
    // Input: pre[] = {10,30,20,5,15}
    // preLN[] = {'N','N','L','L','L'}
    int preLNIndex = *preArrIndex;
    if(preLNIndex == n) // if all nodes are constructed return NULL.
	return NULL;
    struct bnode* root = newBNode(preArr[preLNIndex]);
    (*preArrIndex)++; // incrementing the preArrIndex, to advance next element.

    if(preLN[preLNIndex] == 'N') {
	root->left = constructSpecialBinaryTreeFromPreOrder(preArr, preLN, preArrIndex, n);
	root->right = constructSpecialBinaryTreeFromPreOrder(preArr, preLN, preArrIndex, n);
    }
    // if L only the element will be returned.
    return root;
}
void printBoundaryLeft(struct bnode* root)
{
    // need to ensure top down pattern
    // so print the data, before recursively calling for left or right subtree.
    if(root) { // only if root exists

	// for leaf node, root->left is null and root->right is null, so root->data will not be printed, where
	// root is the leaf node in this case.

	if(root->left) { // if left node exists, traverse left most always.
	    printf("%d ", root->data);
	    printBoundaryLeft(root->left);
	} else if(root->right) {
	    // if right node exists, traverse right node, and thereafter proceed printing data.
	    printf("%d ", root->data);
	    printBoundaryLeft(root->right);
	}
	// do nothing for leaves.
    }
}
void printBoundaryRight(struct bnode* root)
{
    // Need to ensure bottom up manner, so traverse left or right subtree, after then print the node data.
    if(root) {
	if(root->right) { // print right first, because that's the boundary.
	    printBoundaryRight(root->right);
	    printf("%d ", root->data);
	} else if(root->left) {
	    printBoundaryRight(root->left);
	    printf("%d ", root->data);
	} // else do nothing for leaf.
    }
}
void printLeaves(struct bnode* root)
{
    if(root == NULL)
	return;
    printLeaves(root->left);
    // In Order traversal.
    if(root->left == NULL && root->right == NULL)
	printf("%d ", root->data);
    printLeaves(root->right);
}
void boundaryTraversalBinaryTree(struct bnode* root)
{
    /**
     * 				    20
     *
     * 			 8             		22
     *
     *   	4 		12							25
     *
     * 			10		14
     * printing boundary traversal takes the following steps:-
     *
     * 1. Print the left boundary in a top down manner. printBoundaryLeft() method
     * 2. print the leaf nodes from left to right, this again subdivided into two steps
     * 		printLeaves() method
     * 		2.1 print the leaf nodes of left subtree from left to right
     * 		2.2 print the leaf nodes of right subtree from right to left
     * 2. print the right boundary in a bottom up manner. printBoundaryRight() method
     *
     * One thing we must make sure is, the data not getting printed / duplicated twice in the traversal.
     * eg. left most node is also leaf node of the tree.
     *
     * */

    if(root) {
	printf("%d ", root->data);

	printBoundaryLeft(root->left);

	printLeaves(root->left); // don't call on root itself, because, it evens prints the repeated data.
	printLeaves(root->right);

	printBoundaryRight(root->right);
    }
}

struct bnode* constructFullTreeFromPreAndPostOrder(int pre[],
                                                   int post[],
                                                   int* preIndex,
                                                   int postLowIndex,
                                                   int postHighIndex,
                                                   int size)
{
    // prefix : N1 N2 L2 R2 R1
    // postfix: L2 R2 N2 R1 N1
    // N1 becomes the root element,find N2 in postfix, left to N2 in postfix becomes left subtre, right to  N2 becomes
    // right subtree
    // L2 R2 Next preIndex is L2, construct L2 Node and postLowIndex==postHighIndex, hence this element itself will be
    // returned.

    // full tree is the one which has 0 or two children.
    // if it is not a full tree,, then it's hard to construct binary tree from pre and post.
    if(*preIndex >= size || postLowIndex > postHighIndex) {
	// base case, if consntructed fully then return null
	return NULL;
    }
    struct bnode* root = newBNode(pre[*(preIndex)]); // for the root node
    ++(*preIndex);
    if(postLowIndex == postHighIndex) // if there is only one node left, then return that node.
	return root;

    int i;
    for(i = postLowIndex; i <= postHighIndex; i++) {
	// find the next PreIndex element in Post Arr
	// when it's found, then left to that element in Post becomes left subtree, and right to element
	// becomes right subtree.
	if(pre[*(preIndex)] == post[i])
	    break;
    }
    // Now i would contain the index of next free element, main point to note that is i element is not constructed as of
    // now.
    // so we need to pass postLowIndex,i and i+1,postHighIndex
    if(postLowIndex <= postHighIndex) { // makes sure the traversal is proper.
	root->left = constructFullTreeFromPreAndPostOrder(
	    pre, post, preIndex, postLowIndex, i, size); // before i, becomes left subtree
	root->right = constructFullTreeFromPreAndPostOrder(pre, post, preIndex, i + 1, postHighIndex - 1, size);
	// postHighIndex - 1 is must, otherwise, we will not get output
	// reason, postHighIndex -1 eliminates the root element already constructed.
    }
    return root;
    /*Example:
     *
     * input: pre {1,2,4,8,9,5,3,6,7}
     * post {8,9,4,5,2,6,7,3,1}
     *
     * Now take 1 element in pre
     * 	create node		1
     * Now look out for the second element in pre but don't create node
     * Look out node for next pre element is 2
     *
     * Find the element 2 in post
     *
     * let i be the index found for  element 2
     * root->left = construct tree (postLowIndex,i) {8,9,4,5,2}
     * root->right = construct tree (i+1,posthighIndex-1) {6,7,3}
     * 1 is rejected because it's already constructed. [POST : LRN]
     *
     *
     * */
}

void iterativePreOrderTraversal(struct bnode* root)
{

    struct bStack* stack = NULL;
    /*
     * 	1) Create an empty stack nodeStack and push root node to stack.
            2) Do following while nodeStack is not empty.
                    Ã¢â‚¬Â¦.a) Pop an item from stack and print it.
                    Ã¢â‚¬Â¦.b) Push right child of popped item to stack
                    Ã¢â‚¬Â¦.c) Push left child of popped item to stack
     *
     * */
    pushBStack(&stack, root); // N
    while(stack != NULL) {
	struct bnode* currNode = popBStack(&stack);
	printf("%d ", currNode->data);
	if(currNode->right) { // R
	    pushBStack(&stack, currNode->right);
	}
	if(currNode->left) { // L, so that on next iteration, we will get the left element to print.
	    pushBStack(&stack, currNode->left);
	}
    }
    // output: 10 8 3 5 2 2
}
void printPreOrderWithoutStackAndRecur(struct bnode* root)
{
    struct bnode* current = root;
    struct bnode* predece;
    while(current) { // as long as the root is not null.

	if(current->left == NULL) {
	    printf("%d ", current->data); // NLR, if L is null, print node data,then traverse right.
	    current = current->right; // points the in order successor, sometimes Node n.
	} else {
	    predece = current->left; // go to left subtree, and find the predecessor.
	    while(predece->right != NULL && predece->right != current) {
		// predece->right!=current because, we have mapped inorder successor of this predecessor node to
		// current.
		predece = predece->right;
	    }
	    if(predece->right == NULL) { // if the inorder successor is not set.
		printf("%d ", current->data); // print the Node visited.
		predece->right = current;
		current = current->left; // traverse left.
	    } else {
		predece->right = NULL; // revert back to original.
		current = current->right; // traverse right.
	    }
	}
    }
}
void insertNodeToCompleteBinaryTreeUsingList(struct bnode** root, int data, struct bQueue* queue)
{
    struct bnode* testNode = newBNode(data);
    if(!(*root)) { // if root of the tree not assigned.
	// if empty.
	*root = testNode;
    } else {
	struct bnode* frontNode = queue->node[queue->front];
	if(!frontNode->left) {
	    frontNode->left = testNode;
	} else if(!frontNode->right) {
	    frontNode->right = testNode;
	}
	if(frontNode && frontNode->left && frontNode->right) {
	    // if the frontnode has both left and right child, after adding up the right
	    // then dequeue, this should not happen on the next insertion., so put it in if block.
	    dequeueBNode(queue);
	}
    }
    enqueueBNode(queue, testNode);
}
void createCompleteBinaryTreeUsingLinkedList()
{
    /**
     * A complete binary tree is a binary tree where each level Ã¢â‚¬ËœlÃ¢â‚¬â„¢ except the last has 2^l nodes
     * and the nodes at the last level are all left aligned.
     * Complete binary trees are mainly used in heap based data structures.

             * The nodes in the complete binary tree are inserted from left to right in one level at a time.
            If a level is full, the node is inserted in a new level.

    Below are some of the complete binary trees.

           1
          / \
         2   3

            1
           / \
          2   3
         / \  /
        4  5 6

    Below binary trees are not complete:

         1
        / \
       2   3
      /    /
      4   5

           1
          / \
         2   3
        / \  /
       4  5 6
      /
     7

    Complete binary trees are generally represented using -----arrays-----.
     * The array representation is better because it doesnÃ¢â‚¬â„¢t contain any empty slot.
     * Given parent index i, its left child is given by 2 * i + 1
     * and its right child is given by 2 * i + 2.
     * So no extra space is wasted and space to store left and right pointers is saved.
     * 	*/

    /* How to achieve creation of complete binary tree using linked list.
     * Goal:
     * 	1) we should provide a function that always adds the node at the last level and should be
     * 		always as left as possible.
     * Soln: A queue data structure can be used to achieve creation of complete binary tree.
     * Algorithm:
     * 1) initialize the queue to root node.
     * 2) Create a new node temp
     * 3) while the queue is not empty
                   {
                           front = getTheFrontElement of queue

                           if( front doesn't have left child )
                            set front -> left =temp;
                           else if(front doesn't have right child)
                            set front -> right =temp;

                            if(bothchildexists)
                             dequeue(), so that next level will be traversed for inserting data.
                   }
            4) enqueue the node created.
     *
     * */
    struct bnode* root = NULL;
    struct bQueue* queue = createBQueue(15);
    int i;

    for(i = 1; i <= 12; ++i)
	insertNodeToCompleteBinaryTreeUsingList(&root, i, queue);
    printLevelOrderTraversal(root);
}

void iterativePostOrderTraversal(struct bnode* root)
{
    struct bStack* stack1 = NULL;
    struct bStack* stack2 = NULL;
    /*Going to make use of reverse pre order traversal. [NRL]
    as we push the NRL to stack, when popping elements from stack, LRN will be printed, which is the post
    order traversal.*/
    /*
     *  algorithm:
     * 	1) push the node to stack 1.
     * 2) while the stack 1 is not empty, do the following
     * 		-- pop the node from stack 1 and push it to stack 2 //N is pushed
     * 	    -- push the left and right elements of popped node to stack 1 //on the next iteration, R element is pushed
     * to stack 2
     * 3)Now we would have had NRL, popping the elements from stack 2, we will get LRN, the post order traversal.
     * */
    pushBStack(&stack1, root);
    while(stack1 != NULL) {
	struct bnode* poppedElem = popBStack(&stack1);
	pushBStack(&stack2, poppedElem);
	if(poppedElem->left) {
	    pushBStack(&stack1, poppedElem->left);
	}
	if(poppedElem->right) {
	    pushBStack(&stack1, poppedElem->right);
	}
    }
    while(stack2 != NULL) {
	struct bnode* tempElem = popBStack(&stack2);
	printf("%d ", tempElem->data);
    }
}
void iterativePostOrderTraversalUsingSingleStack(struct bnode* root)
{
    struct bStack* stack = NULL;
    do { // do until the stack is not empty
	/*
	 * 1) push the root's right element and root element to stack//  RN
	 * 2) set the root element to left child of root, so that left subtree is processed next.//L
	 * 3) Repeat the steps as long as left element exists
	 *
	 * 4) root = pop the element from the stack
	 * 5) check if root->right exists and equals to peek of the stack, ensures that right tree is processed first
	 * , before proceeding to the root element. then
	 * 	 -- pop the right element of root
	 *   -- push the root element to stack
	 *   -- Set root = root->right, so that right tree is processed next for printing.
	 * 6) else, print the element, set root = NULL
	 * */
	while(root) {
	    // step 1, 2 & 3
	    if(root->right)
		pushBStack(&stack, root->right);
	    pushBStack(&stack, root);
	    root = root->left;
	}
	// Reverses RN to NR
	root = popBStack(&stack); // step 4//N element is popped up
	if(root->right && stack != NULL && stack->elem == root->right) { // peek of the element
	    // step 5
	    popBStack(&stack); // pops up the right child
	    pushBStack(&stack, root); // pushes the root element.
	    root = root->right; // in the next iteration, pushes the right child
	} else {
	    printf("%d", root->data);
	    root = NULL;
	}

    } while(stack != NULL);
    /*	example,
     * 				1
     *
     * 		 2				3
     *
     * 4		5		6		7
     *
     * 1. Right child of 1 exists.
Push 3 to stack. Push 1 to stack. Move to left child.
    Stack: 3, 1

2. Right child of 2 exists.
Push 5 to stack. Push 2 to stack. Move to left child.
    Stack: 3, 1, 5, 2

3. Right child of 4 doesn't exist. '
Push 4 to stack. Move to left child.
    Stack: 3, 1, 5, 2, 4

4. Current node is NULL.
Pop 4 from stack. Right child of 4 doesn't exist.
Print 4. Set current node to NULL.
    Stack: 3, 1, 5, 2

5. Current node is NULL.
Pop 2 from stack. Since right child of 2 equals stack top element,
pop 5 from stack. Now push 2 to stack.
Move current node to right child of 2 i.e. 5
    Stack: 3, 1, 2

6. Right child of 5 doesn't exist. Push 5 to stack. Move to left child.
    Stack: 3, 1, 2, 5

7. Current node is NULL. Pop 5 from stack. Right child of 5 doesn't exist.
Print 5. Set current node to NULL.
    Stack: 3, 1, 2
     *
     * */
}
void reverseLevelOrderTraversal(struct bnode* root)
{

    int h = maxDepthOfTree(root);

    // same as printGivenLevel, spiral, but just start from h, moving down to 0
    int i, itr;
    itr = 1;
    for(i = h; i >= 1; i--) {
	printGivenLevel(root, i, itr);
    }
    struct bQueue* queue = createBQueue(20);
    enqueueBNode(queue, root);
    struct bStack* stack = NULL;
    while(queue->size != 0) {
	// instead of printing the data push it to stack,
	struct bnode* temp = dequeueBNode(queue);
	pushBStack(&stack, temp);
	if(temp->right) {
	    enqueueBNode(queue, temp->right);
	} // Make sure we push the right subtree, so that popping will be in order.
	if(temp->left) {
	    enqueueBNode(queue, temp->left);
	}
    }
    while(stack != NULL) {
	printf("%d ", popBStack(&stack)->data);
    }
}
void convertListToBinary(struct node* list, struct bnode** root)
{
    // Trick to convert list to binary node is to traverse the list and enqueue the nodes as we traverse,

    // Head element is always the root, root's left and right child will be at positions 2*i+1,    2*i+2

    // fetch the next two elements in list and store it as left and right child of root.
    // repeat this by dequeing the node and storing the left,right child.
    if(list == NULL)
	return;

    struct bnode* newNode = newBNode(list->data); // root element construction
    struct bQueue* queue = createBQueue(20);

    *root = newNode;
    enqueueBNode(queue, newNode); // enqueue the root node

    list = list->next; // move next

    while(list) {
	struct bnode* parent = dequeueBNode(queue);

	// next element in the list moved to queue
	struct bnode* leftChild = newBNode(list->data);
	enqueueBNode(queue, leftChild);
	struct bnode* rightChild = NULL;
	list = list->next;
	if(list) {
	    rightChild = newBNode(list->data);
	    enqueueBNode(queue, rightChild);
	    list = list->next;
	}
	parent->left = leftChild;
	parent->right = rightChild;
    }
}
struct bnode* binTreeToDllListUtil(struct bnode* root)
{
    // converts the binary tree to dll list [stores the inorder traversal of binary tree]

    if(root == NULL)
	return NULL;

    if(root->left) {
	// call recursively on the left subtree
	struct bnode* leftTree = binTreeToDllListUtil(root->left);
	// left subtree is obtained.
	// Get the predecessor
	for(; leftTree->right != NULL; leftTree = leftTree->right)
	    ;
	// Leftree is the predecessor
	leftTree->right = root; // point the predecessor next to root
	root->left = leftTree; // set the prev pointer of root to predecessor
    }
    if(root->right) {
	struct bnode* rightTree = binTreeToDllListUtil(root->right);
	// right subtree is obtained
	// find the successor
	for(; rightTree->left != NULL; rightTree = rightTree->left)
	    ;

	root->right = rightTree; // set the next pointer of root to successor
	rightTree->left = root; // set the prev pointer of successor to root.
    }

    return root;
    // return the node
}
struct bnode* binTreeToDllList(struct bnode* root)
{

    if(root) {

	root = binTreeToDllListUtil(root);

	while(root->left) { // as the left tree exists, do the following
	    root = root->left;
	}
    }
    return root;
}
int isIsoMorphic(struct bnode* n1, struct bnode* n2)
{
    // Two trees are called isomorphic if one of them can be obtained from other by a series of flips,
    // i.e. by swapping left and right children of a number of nodes.
    // Any number of nodes at any level can have their children swapped. means that a level of node may not have been
    // flipped at all.

    if(n1 == NULL && n2 == NULL)
	return 1; // isomorphic true

    if(n1 == NULL || n2 == NULL)
	return 0; // isomorphic false

    if(n1->data != n2->data)
	return 0; // if the data are not equal return false

    return (isIsoMorphic(n1->left, n2->left) &&
            isIsoMorphic(n1->right, n2->right)) || // subtree rooted at this node has not been flipped
           (isIsoMorphic(n1->left, n2->right) &&
            isIsoMorphic(n1->right, n2->left)); // subtree rooted at this node has been flipped.
}
int iterativeFindHeight(struct bnode* root)
{

    /*How to find height without recursion? We can use level order traversal to find height without recursion.
     * The idea is to traverse level by level. Whenever move down to a level, increment height by 1
     * (height is initialized as 0).
     * Count number of nodes at each level, stop traversing when count of nodes at next level is 0.
     *
     * */

    struct bQueue* queue = createBQueue(20);
    enqueue(queue, root);
    int height = 0; // initialize the height to 0
    while(1) {
	// traverse level by level
	int nodeCount = queue->size; // number of nodes at this level
	if(nodeCount == 0)
	    return height;
	height++; // this level is going to be traversed, so increment height.

	while(nodeCount > 0) {
	    // as long as the number of nodes at this level is 0, enqueue all the next level nodes to queue.
	    struct bnode* current = dequeueBNode(queue); // makes sure to dequeue all the nodes of current level.
	    if(current->left) {
		enqueueBNode(queue, current->left);
	    }
	    if(current->right) {
		enqueueBNode(queue, current->right);
	    }
	    nodeCount--;
	}
    }
}
void fixPrevPtrUsingInOrder(struct bnode* root)
{
    /*
     * // Changes left pointers to work as previous pointers in converted DLL
     * // The function simply does inorder traversal of Binary Tree and updates
     * // left pointer using previously visited node
     *
     * */
    static struct bnode* prev = NULL;
    if(root) { // for example LNR, previous visited node of N is L and previous visited node of R is N.
	fixPrevPtrUsingInOrder(root->left);
	root->left = prev; // root->left becomes the prev pointer here.
	prev = root; // as we traverse inorder, store the prev pointer
	fixPrevPtrUsingInOrder(root->right);
    }
}
struct bnode* fixNextPtrUsingInOrder(struct bnode* root)
{
    if(!root)
	return NULL;
    // find right most node or the last node

    while(root && root->right) {
	root = root->right;
    }

    // as we found the right most node, we can use prev pointer to traverse to head of the list.
    struct bnode* next = NULL;
    // as we found the right most node, traverse back in the list, that becomes the next.
    while(root && root->left) { // 25 is the right most node
	next = root; // 25 is the next element
	root = root->left; // Set the root to back of the next element //36 is the predecessor of 25
	root->right = next; // This back element has to set it's right to next element //set the next of 36 to 25
    }
    return root;
    // returns head of the list
}
struct bnode* BTTToDLL(struct bnode* root)
{
    /*
     * 			10
     *
     * 		12				25
     *
     * 25 		30		36

            dll list is 25 -> 12 -> 30 -> 10 -> 36 -> 25
     *
     * */
    fixPrevPtrUsingInOrder(root);

    return fixNextPtrUsingInOrder(root); // returns the head of dll list.
}
void printAncestorsWithoutRecursion(struct bnode* root, int key)
{
    // how to achieve printing ancestor without recursion - we will make use of iterative post order traversal
    // LRN, We will check if the node is found in left subtree then right, if not found then break.

    // trick here is to keep track of ancestors of a node by pushing them to stack.
    // Atlast, the contents of stack printed will be the ancestors of node found.
    struct bStack* stack = NULL;
    while(1) {

	while(root && root->data != key) { // if the left subtree exists, push it to the stack, traverse down all the
	                                   // way
	    pushBStack(&stack, root); // N node will be pushed, so that if node not found,
	    // we can traverse right subtree
	    root = root->left;
	}
	if(root && root->data == key) { // N - as we find the key node, then break out of recursion.
	    break;
	}

	if(stack != NULL && stack->elem->right == NULL) { // if we couldn't find the right subtree to traverse on
	    // and also the data not found so far

	    // if stack's top element's right child doesn't exist.
	    root = popBStack(&stack);
	    /*
	     *  If the popped node is right child of top, then remove the top
	            as well. Left child of the top must have processed before.
	            Consider the following tree for example and key = 3.  If we
	            remove the following loop, the program will go in an
	            infinite loop after reaching 5.
	                      1
	                    /   \
	              2     3
	                    \
  4
  \
	                      5
	     *
	     * */
	    while(stack != NULL &&
	          stack->elem->right == root) { // if the stack's top has this popped up element as right,
		// then pop it again.
		root = popBStack(&stack);
		// pop until the stack is not empty and stack's top element's right is root.
	    }
	}
	root = (stack == NULL) ? NULL : stack->elem->right; // by the end, go to the right subtree to find the data
    }

    // By this block, ancestors would have been populated in stack.
    // we just need to print the contents of stack by popping them up.
    while(stack != NULL) {
	printf("%d ", popBStack(&stack)->data);
    }
}
int findOddAndEvenLevelDiff(struct bnode* root)
{
    // This method is to find the difference between the sum of odd and even level nodes.
    /*
     * Algorithm:
     * 1) We are going to make use of recursion and '-' symbol to calculate the odd and even level diff
     * 2) Formula is root->data - findOddAndEvenLevelDiff(root->left) - findOddAndEvenLevelDiff(root->right)
     * 3) Internally ->
     * 	 	-- root->data -
     * 				findOddAndEvenLevelDiff(root->left) [root->left->data -
     *findEvenAndEvenlevelDiff(root->left->left)]
     *
     * 	    -- Now this becomes, root->data + root->left->left->data - root->left->data
     * 		-- i.e level 1 + level 3 - level 2
     * */
    if(root == NULL)
	return 0;
    // Difference for root is root's data - difference for left subtree
    // - difference for right subtree
    return root->data - findOddAndEvenLevelDiff(root->left) - findOddAndEvenLevelDiff(root->right);
    /*			      5
                                    /   \
                              2     6
                            /  \     \
                      1    4     8
                    /     / \
              3     7   9
     *
     * use this tree -> paper work results in -9 as output
     * */
}
int findOddAndEvenLevelDiffUsingLevelOrder(struct bnode* root)
{

    struct bQueue* queue = createBQueue(20);
    enqueueBNode(queue, root);
    enqueueBNode(queue, NULL); // separator for a level.
    int odd_sum = 0;
    int even_sum = 0;
    int level = 1;
    // Example: 1 NULL 2 3
    /*	1 is dequeued and 2 and 3 were added.
     *  NULL is dequeued, next level starts. and also enqueue NULL so that 2 3 NULL will exist in queue.
     *
     * */
    while(queue->size != 0) {
	// as the queue size is not empty, do the following.
	struct bnode* current = dequeueBNode(queue);

	if(current == NULL) { // the marker, we are starting the new level.
	    if(queue->size == 0) // if not breaked, then this will run infinitely.
		break;
	    // we are starting the new level, so increment level
	    level++;
	    enqueueBNode(queue, NULL);
	} else { // if this is same level as the previous node.
	    if(level & 1)
		odd_sum += current->data;
	    else
		even_sum += current->data;
	    if(current->left)
		enqueueBNode(queue, current->left);
	    if(current->right)
		enqueueBNode(queue, current->right);
	}
    }
    return odd_sum - even_sum;
}
int searchInInorder(int in[], int x, int n)
{
    int i;
    for(i = 0; i < n; i++) {
	if(in[i] == x)
	    return i;
    }
    return -1;
}
void printPostOrderFromInAndPre(int in[], int pre[], int n)
{
    // instead of constructing the tree, we can directly print the post order from in and pre order

    int root = searchInInorder(in, pre[0], n); // always the first element in pre order is the N node.
    // pre[0] is the N
    // Left subtree
    if(root != 0) { // if the left subtree is not empty
	printPostOrderFromInAndPre(in, pre + 1, root);
	// pre+1 is the root element of left subtree L
	// in is the arr to be searched -> from index 0 to root for left subtree
    }

    // Right subtree
    if(root != n - 1) { // if the right subtree is not empty
	printPostOrderFromInAndPre(in + root + 1, pre + root + 1, n - root - 1);
	// pre+root+1 here is the root element of right subtree.. [pre+root-1 is the end of left subtree..]
	// in+root+1 -> provides new array from in+root+1 address location.
	// n-root-1 -> provides the last index to search for pre+1 in in+root+1 array.
    }

    printf("%d ", pre[0]); // print the node N
}

int findDepthOfOddLeaves(struct bnode* root, int level)
{

    if(root == NULL)
	return 0;
    // level - current level
    if(root->left == NULL && root->right == NULL && level & 1) {
	return level; // if it is a leaf and it's an odd level, then return that level
    }
    return max(findDepthOfOddLeaves(root->left, level + 1),
               findDepthOfOddLeaves(root->right, level + 1)); // move to next level, left
    // and right subtree, find depth of odd leaves from there.
    // please note that we return only the depth of odd leaves, if it's an even depth, we simply ignore them.
}
int checkIfLeavesAtSameLevel(struct bnode* root, int level, int* leafLevel)
{
    // In preorder fashion, NLR -> as we go down the tree increment the level.
    // As we find the first leaf node, we store the first leaf level
    // upon finding other leaves, we return if the those leaf levels are equal to the one first found
    if(root == NULL)
	return 1;

    if(root->left == NULL && root->right == NULL) {
	// leaf node
	if(*leafLevel == 0) // if it's not already set
	    *leafLevel = level;
	return (*leafLevel == level);
    } // N

    // LR
    return checkIfLeavesAtSameLevel(root->left, level + 1, leafLevel) &&
           checkIfLeavesAtSameLevel(root->right, level + 1, leafLevel);
}
int printLeftViewBinTree(struct bnode* root)
{

    int maxLevel = 0;
    printLeftViewBinTreeUsingRecursive(root, 1, &maxLevel);
    struct bQueue* queue = createBQueue(20);
    int level = 0;
    int isLevelVisited = 0;
    enqueueBNode(queue, root);
    enqueueBNode(queue, NULL);

    while(queue->size != 0) {
	// We can print the right view -> check if the peek of queue is NULL, print current->data
	struct bnode* current = dequeueBNode(queue);
	if(current == NULL) {
	    if(queue->size == 0)
		break;
	    level++;
	    isLevelVisited = 0; // new level, so level not visited
	    enqueueBNode(queue, NULL);
	} else {
	    if(!isLevelVisited) {
		printf("%d ", current->data);
		isLevelVisited = 1; // print the first level data.
	    }
	    if(current->left) {
		enqueueBNode(queue, current->left);
	    }
	    if(current->right) {
		enqueueBNode(queue, current->right);
	    }
	}
    }
}
void printLeftViewBinTreeUsingRecursive(struct bnode* root, int level, int* maxLevel)
{
    // Algorithm: track the max level as we traverse, if the node's level we are traversing is higher,then print it's
    // data, save it as maxlevel
    // Use pre order fashion
    if(root == NULL)
	return;
    if(*maxLevel < level) {
	*maxLevel = level;
	printf("%d ", root->data);
    }
    printLeftViewBinTreeUsingRecursive(root->left, level + 1, maxLevel);
    printLeftViewBinTreeUsingRecursive(root->right, level + 1, maxLevel);
}
struct bnode* pruneNodesWithSuMLess(struct bnode* root, int k, int* sum)
{
    /*
     * Algorithm: To delete a node if the sum of it's path is less than the value k.
     * 1) Calculate sum of nodes from root to leaf path.
     * 2) Delete/free the node if this node's value causes trouble,
     * 	   i.e, delete the path if the sum of node in it's path is less than k
     * 		At the end, maintains the sum>=k, in a binary tree.
     *
     *  Since the path is from root to leaf and nodes are deleted in bottom up manner,
            a node is deleted only when all of its descendants are deleted.
     * Therefore, when a node is deleted, it must be a leaf in the current Binary Tree.
     *
     *
     * */
    if(root == NULL)
	return NULL;
    int lSum = *sum + root->data;
    int rSum = lSum;

    // To traverse Bottom up manner, we must visit left and right subtree
    root->left = pruneNodesWithSuMLess(root->left, k, &lSum);
    root->right = pruneNodesWithSuMLess(root->right, k, &rSum);

    *sum = max(lSum, rSum); // this make sure, if the node we are to delete, doesn't fall under any other path

    if(*sum < k) { // if the sum of it's path is less than k value, then free the node.
	free(root);
	return NULL;
    }
    return root;
    // prune tree
    /*
     *
     * Consider the following Binary Tree
     1
 /      \
2        3
/   \     /  \
4     5   6    7
/ \    /       /
8   9  12      10
/ \           \
13  14         11
 /
15

For input k = 20, the tree should be changed to following
(Nodes with values 6 and 8 are deleted)
     1
 /      \
2        3
/   \        \
4     5        7
\    /       /
9  12      10
/ \           \
13  14         11
 /
15

For input k = 45, the tree should be changed to following.
 1
/
2
/
4
\
9
\
14
/
15
     *
     * */
}
struct bnode* pruneNodesBySubtractNodeData(struct bnode* root, int sum)
{ // sum is the k, we will send. sum<=k need to be deleted.
    if(root == NULL)
	return;
    root->left = pruneNodesBySubtractNodeData(root->left, sum - root->data);
    root->right = pruneNodesBySubtractNodeData(root->right, sum - root->data);
    // as we reach the leaf node, if sum is greater than the leaf node, free that node.
    if(root->left == NULL &&
       root->right == NULL) { // the internal node converted to a leaf node, left and righ subtree is null
	// then this method will be executed, otherwise, the internal node will exist as such.
	if(sum > root->data) {
	    free(root);
	    return NULL;
	}
    }
    return root;
}
struct bnode* extractLeavesToDllList(struct bnode* root, struct bnode** head_ref)
{
    // takes o(n) time - does a single traversal
    if(root == NULL)
	return NULL;

    if(root->left == NULL && root->right == NULL) {
	// construct dll list by extracting leave nodes from binary tree.
	// Leaf's parent node left and right subtree should be set to null

	root->right = *head_ref; // next ptr of curr is head
	if(*head_ref != NULL) {
	    (*head_ref)->left = root;
	} // dll's head node's prev pointer set to curr node
	*head_ref = root; // head of dll is set to root.

	return NULL; // this ensures that it's parent node is set to null.
    }
    root->right = extractLeavesToDllList(root->right, head_ref); // right tree has to be traversed first,
    // since construction of node in dll list happends at the head.
    root->left = extractLeavesToDllList(root->left, head_ref);

    return root;
}
void deepestLeftLeafNode(struct bnode* root, int level, int* maxLevel, int isLeft, struct bnode** resptr)
{
    // level- node we are in, maxLevel -> to store the max level we are traversing
    // isLeft -> boolean to traverse only the left most node...
    // resPtr is  the resulting data we are going to send.
    if(root == NULL)
	return;

    if(isLeft && !root->left && !root->right && level > (*maxLevel)) {
	*maxLevel = level;
	*resptr = root;
	return;
    }
    deepestLeftLeafNode(root->left, level + 1, maxLevel, 1, resptr);
    deepestLeftLeafNode(root->right, level + 1, maxLevel, 0, resptr);
}
void getNextRightAfterKey(struct bnode* root, int key)
{
    /*Given a Binary tree and a key in the binary tree, find the node right to the given key.
     * If there is no node on right side, then return NULL. Expected time complexity is O(n) where n is the number of
    nodes in the given binary tree.

    For example, consider the following Binary Tree. Output for 2 is 6, output for 4 is 5.
     * Output for 10, 6 and 5 is NULL.

              10
           /      \
          2         6
       /   \         \
     8      4          5
     *
     * */
    struct bQueue* queue = createBQueue(20);
    int level = 0;
    enqueueBNode(queue, root);
    while(queue->size != 0) {
	int nodeCount = queue->size;
	while(nodeCount > 0) {
	    struct bnode* current = dequeueBNode(queue);
	    if(current->data == key && queue->size >= 1) {
		// if the current node data is equal to key's then check if the next node exists in the queue
		printf("Next Right after key is %d", queue->node[queue->front]->data);
	    }
	    if(current->left) {
		enqueueBNode(queue, current->left);
	    }
	    if(current->right) {
		enqueueBNode(queue, current->right);
	    }
	    nodeCount--;
	}
    }
}
int sumOfAllNumbersFromRootToLeafPath(struct bnode* root, int keyData)
{
    // NLR
    /*
     *
     * For example consider the following Binary Tree.

                                      6
                                  /      \
                                3          5
                              /   \          \
                             2     5          4
                                  /   \
                                 7     4
    There are 4 leaves, hence 4 root to leaf paths:
    Path                    Number
    6->3->2                   632
    6->3->5->7               6357
    6->3->5->4               6354
    6->5>4                    654
    Answer = 632 + 6357 + 6354 + 654 = 13997
     *
     * */
    int sum = 0;
    if(root == NULL)
	return;
    keyData = (keyData * 10) + root->data;
    if(!root->left && !root->right) {
	//		sum+=keyData+root->data;
	return keyData;
    }

    return sumOfAllNumbersFromRootToLeafPath(root->left, keyData) +
           sumOfAllNumbersFromRootToLeafPath(root->right, keyData);
}
void binTreeToDllListUsingInorder(struct bnode* root, struct bnode** head)
{
    // LNR.
    if(root == NULL)
	return;

    static struct bnode* prev = NULL; // mae this as static.

    binTreeToDllListUsingInorder(root->left, head);

    if(prev == NULL) // using  prev is null, then set head to root.
	*head = root;
    else { // rootleft is prev, prev right is next.
	root->left = prev;
	prev->right = root;
    }
    prev = root; // important set previous to the root element.

    binTreeToDllListUsingInorder(root->right, head);
}
struct bnode* findLCA(struct bnode* root, int n1, int n2)
{
    // this method assumes that data is found in left or right subtree.
    // if n1 || n2 is not found in the tree itself, still we will return the data, instead of returning NULL ideally.
    if(root == NULL)
	return NULL;

    if(root->data == n1 || root->data == n2) {
	return root;
    }
    /*Note that the above method assumes that keys are present in Binary Tree.
     * If one key is present and other is absent,
     * then it returns the present key as LCA (Ideally should have returned NULL).
     *
     * */
    struct bnode* leftLCA = findLCA(root->left, n1, n2); // find lca in left
    struct bnode* rightLCA = findLCA(root->right, n1, n2); // find lca in right.

    if(leftLCA && rightLCA)
	return root; // if lca is found in left and right subtree, then we return the root

    return (leftLCA != NULL) ? leftLCA :
                               rightLCA; // if only one  found in left / righ subtree alone, then return that lca.
}
struct bnode* findLCAByValidation(struct bnode* root, int n1, int n2, int* v1, int* v2)
{
    // v1 and v2 validates - n1 and n2 exists in the tree respectively.
    if(root == NULL)
	return NULL;

    if(root->data == n1) { // if found, set validated to true.
	*v1 = 1;
	return root;
    }
    if(root->data == n2) {
	*v2 = 1;
	return root;
    }
    struct bnode* leftLCA = findLCAByValidation(root->left, n1, n2, v1, v2);
    struct bnode* rightLCA = findLCAByValidation(root->right, n1, n2, v1, v2);

    if(leftLCA && rightLCA)
	return root;

    return (leftLCA != NULL) ? leftLCA : rightLCA;
}
int findBNode(struct bnode* root, int data)
{
    if(root == NULL)
	return 0;
    if(root->data == data || findBNode(root->left, data) || findBNode(root->right, data)) {
	return 1;
    }
    return 0;
}
struct bnode* findLCAByValidationUtil(struct bnode* root, int n1, int n2)
{
    int v1 = 0;
    int v2 = 0;

    struct bnode* result = findLCAByValidation(root, n1, n2, &v1, &v2);
    if((v1 && v2) || (v1 && findBNode(root, n2)) || (v2 && findBNode(root, n1)))
	// if both keys found, then no problem, else if one node is validated,
	// find the n2 node in root, if found, then we can return result.
	return result;
    return NULL;
}
int findDistance(struct bnode* root, int data)
{
    // finds the number of edges between root to data.
    if(root == NULL)
	return -1;
    if(root->data == data)
	return 0; // as we found the data, return 0

    int left = findDistance(root->left, data);
    int right = findDistance(root->right, data);

    if(left != -1)
	return left + 1;
    else if(right != -1)
	return right + 1;
    else
	return -1;
}
int getDistanceBetweenTwoNodes(struct bnode* root, int n1, int n2)
{
    // Distance between two nodes -> nothing but the number of edges between n1 and n2

    /*
     *
     * Algorithm:
     * 1) Get the LCA node of n1 and n2
     * 2) Find the distance between LCA-n1 and LCA-n2
     *
     * */
    struct bnode* lca = findLCAByValidationUtil(root, n1, n2);
    if(lca) { // if both nodes are present.
	return findDistance(lca, n1) + findDistance(lca, n2);
    }
}
int printKDistantNodesFromLeaf(struct bnode* root, int k)
{

    // link:http://www.geeksforgeeks.org/print-nodes-distance-k-leaf-node/

    if(root == NULL)
	return 0;

    if(root->left == NULL && root->right == NULL)
	return 0;
    int leftDist = printKDistantNodesFromLeaf(root->left, k);
    int rightDist = printKDistantNodesFromLeaf(root->right, k);

    if(leftDist + 1 == k ||
       rightDist + 1 == k) // if the left or right's leaf distance is equal to k then print the data.
	printf("%d ", root->data);

    int min = leftDist;
    // we need to return only the min distance.
    if(rightDist < min)
	min = rightDist;
    return min + 1; // include the node's existence also.
    // an another approach: is to store the path from root to leaf.
    // As we reach the leaf, we need to print the path[pathlen-k-1] and also need to keep track
    // if the node is already visited. if it's already visited, then no need to print the data.
}
int checkIfHeighBalancedLikeRBTree(struct bnode* root, int* maxh, int* minh)
{
    //	For every node, length of the longest leaf to node path has not more than
    // twice the nodes on shortest path from node to leaf.
    if(root == NULL) {
	*maxh = 0;
	*minh = 0;
	return 1; // is balanced set to true.
    }
    int lmaxh, lminh, rmaxh, rminh;
    int lbalanced = checkIfHeighBalancedLikeRBTree(root->left, &lmaxh, &lminh);
    int rbalanced = checkIfHeighBalancedLikeRBTree(root->right, &rmaxh, &rminh);

    if(!lbalanced || !rbalanced) {
	return 0;
    }
    // max height of current node is max of left and right tree.
    // min height of current node is min of left and right tree.

    *maxh = max(lmaxh, rmaxh) + 1; // maximum height of left and right subtree
    *minh = ((lminh < rminh) ? lminh : rminh) + 1; // minimum height of left and right.
    // add the current node to max h and min height.
    return ((*maxh) <= (2 * (*minh)));
    /*
     *
     12                                        40
  \                                     /    \
   14                                10      100
     \                                        /  \
      16                                     60   150
Cannot be a Red-Black Tree              It can be Red-Black Tree
with any color assignment				  --maxheight<=2*minheight
14->
lmaxh, lminh =0
rmaxh -> , rminh ->
Max height of 12 is 3						 maxheight of 40 is 2
Min height of 12 is 1						 minheight of 10 is 1
Min height of 14 is 1
Max height of 14 is 2

      10
    /   \
  5     100
       /   \
      50   150
     /
    40
It can also be Red-Black Tree
     *
     *
     * */
}
int printNodesAtKPositionFromTargetNode(struct bnode* root, struct bnode* target, int k)
{
    // time complexity is o(n)
    /*
     * 					20
    * 				/		\
     * 			8				22
     *     /     \
     * 	4			12
     * 			  /   \
     * 			10		14
     * Consider the tree shown in diagram

Input: target = pointer to node with data 8.
   root = pointer to node with data 20.
   k = 2.
Output : 10 14 22

If target is 14 and k is 3, then output
should be "4 20"
     *
     * */

    // for the target node.
    if(root == NULL)
	return -1; // if not found, very important..
    if(root == target) {
	// print the nodes at position k from target
	printKDistantNodes(root, k);
	return 0;
    }
    int dl = printNodesAtKPositionFromTargetNode(root->left, target, k);
    // if the target node found in left subtree, dl is the distance from target node to current node
    if(dl != -1) { // if only found.
	// dl+1 indicates the current node distance from target.
	if(dl + 1 == k) { // if the current node is at distant k, then print this node as well
	    printf("%d ", root->data);
	} else { // traverse right, dl+2 is the distance from target.
	    // we need to print the nodes at level k - (dl+2)
	    printKDistantNodes(root->right, k - dl - 2);
	}
	return dl + 1; // return dl+1, including this node, provides data to parent, so that they can proceed with
	               // printing the data
    }
    int dr = printNodesAtKPositionFromTargetNode(root->right, target, k);
    if(dr != -1) {
	// if the target node found in the right subtree, and dr+1 is the distance from  current node to target node
	if(dr + 1 == k) {
	    printf("%d ", root->data);
	} else {
	    printKDistantNodes(root->left, k - dr - 2); // go to the left .
	}
	return dr + 1;
    }
    return -1; // if node itself is not found from left and right subtree.
}
void findMaxAndMinFromBTreeVOrder(struct bnode* root, int* min, int* max, int horLevel)
{

    if(root == NULL) {
	return 0;
    }
    if(horLevel < (*min)) {
	*min = horLevel;
    }
    if(horLevel > (*max)) {
	*max = horLevel;
    }
    findMaxAndMinFromBTreeVOrder(root->left, min, max, horLevel - 1);
    findMaxAndMinFromBTreeVOrder(root->right, min, max, horLevel + 1);
}
void printVerticalLine(struct bnode* root, int lineno, int horLevel)
{
    if(root == NULL)
	return;
    if(lineno == horLevel) // if the line number does equal to horLevel, then print the data.
	printf("%d", root->data);
    printVerticalLine(root->left, lineno - 1, horLevel); // line no traverses left wards
    printVerticalLine(root->right, lineno + 1, horLevel); // line no traverses right wards.
}
void printBinaryTreeInVerticalOrder(struct bnode* root)
{
    // Takes o(w*n) complexity where w is width of binary tree and n is number of nodes in binary tree
    // Time efficient implementation would be make use
    // of hashmap<level,arraylist<struct bnode>>
    // for each level, we create or append data to the array list.
    int min = 0;
    int max = 0;
    findMaxAndMinFromBTreeVOrder(root, &min, &max, 0);
    // now that we found the max and min, we need to print the elements that fall under
    int i;
    for(i = min; i <= max; i++) {
	printf("\n");
	printVerticalLine(root, 0, i); // print i vertical nodes.
    }
}
int searchUtilInOrder(int in[], int start, int end, int data)
{
    int i;
    for(i = start; i <= end; i++) {
	if(in[i] == data)
	    return i;
    }
    return -1;
}
int* extractNodesForLevelArr(int in[], int inSize, int level[], int n)
{
    int i, j;
    j = 0;
    int* levelArr = (int*)malloc(sizeof(int) * inSize); // inSize elements should be in level
    for(i = 0; i < n; i++) {
	if(searchUtilInOrder(in, 0, inSize - 1, level[i]) != -1) { // this search is must

	    // as we iterate throught the level array, it checks if the level's element exists in inorder
	    // if exists
	    // inSize-1 is must -> since inSize will be 4, but we will be iterating from 0..3
	    levelArr[j++] = level[i];
	}
    }
    return levelArr;
}
struct bnode* constructBTreeFromInAndLevelOrder(int in[], int level[], int inStart, int inEnd, int n)
{
    /*	An upper bound on time complexity of above method is O(n3).
     * In the main recursive function, extractNodes() is called which takes O(n2) time.
     *
     * */
    /*
     * Let us consider the above example.

in[] = {4, 8, 10, 12, 14, 20, 22};
level[] = {20, 8, 22, 4, 12, 10, 14};

In a Levelorder sequence, the first element is the root of the tree.
So we know Ã¢â‚¬â„¢20Ã¢â‚¬â„¢ is root for given sequences.
* By searching Ã¢â‚¬â„¢20Ã¢â‚¬â„¢ in Inorder sequence, we can find out all elements on left side of Ã¢â‚¬Ëœ20Ã¢â‚¬â„¢ are in left subtree and
elements on right are in right subtree. So we know below structure now.

         20
       /    \
      /      \
{4,8,10,12,14}  {22}

    Let us call {4,8,10,12,14} as left subarray in Inorder traversal and {22} as right subarray in Inorder traversal.
    In level order traversal, keys of left and right subtrees are not //consecutive.//
* So we extract all nodes from level order traversal which are in left subarray of Inorder traversal.
To construct the left subtree of root, we recur for the extracted elements from level order traversal and left subarray
of inorder traversal.
* In the above example, we recur for following two arrays.
     * */
    if(inStart > inEnd)
	return NULL;

    struct bnode* newNode = newBNode(level[0]);
    // the first node in level is the root node in this recursion.
    if(inStart == inEnd)
	return newNode;

    int inIndex = searchUtilInOrder(in, inStart, inEnd, newNode->data);
    // Extract all nodes from level order traversal which are in left subarray of inorder traversal
    int* llevel = extractNodesForLevelArr(in, inIndex, level, n);

    // Extract all nodes from level order traversal which are in right subarray of inorder traversal
    // from inIndex+1, the size of inIndex+1 arr is n-inIndex-1
    int* rlevel = extractNodesForLevelArr(in + inIndex + 1, n - inIndex - 1, level, n);

    newNode->left = constructBTreeFromInAndLevelOrder(
        in, llevel, inStart, inIndex - 1, n); // inIndex-1 indicates that this node is done
    newNode->right = constructBTreeFromInAndLevelOrder(in, rlevel, inIndex + 1, inEnd, n);

    return newNode;
}
int findMaxSuMOfPathBetweenTwoLeaves(struct bnode* root, int* maxSum)
{
    // maxSum stores the maximum path between leaves.
    // returns the max path sum for the subtree rooted under current node.

    /**
     * We can find the maximum sum using single traversal of binary tree. The idea is to maintain two values in
    recursive calls
    1) Maximum root to leaf path sum for the subtree rooted under current node.
    2) The maximum path sum between leaves (desired output).

    For every visited node X, we find the maximum root to leaf sum in left and right subtrees of X.
     * We add the two values with X->data, and compare the sum with maximum path sum found so far.
     * */
    if(root == NULL)
	return 0;
    if(!root->left && !root->right)
	return root->data;
    // Find maximum sum in left and right subtree. Also find
    // maximum root to leaf sums in left and right subtrees
    // and store them in ls and rs
    int lSum = findMaxSuMOfPathBetweenTwoLeaves(
        root->left, maxSum); // maximum sum in left subtree that passed through the left subtree node
    int rSum = findMaxSuMOfPathBetweenTwoLeaves(root->right, maxSum);

    /* Find the maximum path sum passing through current root.
 Note that every path will pass through some node and
 recursive function is called for every node
     * */
    int sum = lSum + rSum + root->data;

    if((*maxSum) < (sum))
	*maxSum = sum; // maximum path sum between leaves.

    return max(lSum, rSum) + root->data;
}
void reverseAlternateLevelsOfPerfectBTree(struct bnode* root, int* arr, int* index, int level)
{
    /*
     *
     * Given tree:
           a
        /     \
       b       c
     /  \     /  \
    d    e    f    g
   / \  / \  / \  / \
   h  i j  k l  m  n  o

Modified tree:
                            a
        /     \
       c       b
     /  \     /  \
    d    e    f    g
   / \  / \  / \  / \
  o  n m  l k  j  i  h
       *
       * A tricky solution is to do two inorder traversals.
       *
       * Following are steps to be followed.
1) Traverse the given tree in inorder fashion and store all odd level nodes in an auxiliary array.
* For the above example given tree, contents of array become {h, i, b, j, k, l, m, c, n, o}

2) Reverse the array. The array now becomes {o, n, c, m, l, k, j, b, i, h}

3) Traverse the tree again inorder fashion.
* While traversing the tree, one by one take elements from array and store elements from array to every
* odd level traversed node.
For the above example, we traverse Ã¢â‚¬ËœhÃ¢â‚¬â„¢ first in above array and replace Ã¢â‚¬ËœhÃ¢â‚¬â„¢ with Ã¢â‚¬ËœoÃ¢â‚¬â„¢.
* Then we traverse Ã¢â‚¬ËœiÃ¢â‚¬â„¢ and replace it with n.
     *
     * */
    if(root == NULL)
	return;
    reverseAlternateLevelsOfPerfectBTree(root->left, arr, index, level + 1);
    if(level % 2 != 0) {
	arr[*index] = root->data;
	(*index)++;
    }
    reverseAlternateLevelsOfPerfectBTree(root->right, arr, index, level + 1);
}
void modifyBTreeByReversing(struct bnode* root, int* arr, int* index, int level)
{
    if(root == NULL)
	return;
    modifyBTreeByReversing(root->left, arr, index, level + 1);
    if(level % 2 != 0) {
	root->data = arr[*index];
	(*index)++;
    }
    modifyBTreeByReversing(root->right, arr, index, level + 1);
}
void reverseArr(int* arr, int n)
{
    int l = 0, r = n - 1;
    int temp;
    while(l < r) {
	temp = arr[l];
	arr[l] = arr[r];
	arr[r] = temp;
	l++;
	r--;
    }
}
void reverseAlternate(struct bnode* root)
{
    // takes o(n) time
    int* arr = (int*)malloc(sizeof(int) * 20);
    int index = 0;
    reverseAlternateLevelsOfPerfectBTree(root, arr, &index, 0);

    // here index will be the size of arr.
    reverseArr(arr, index); // reversing the array, so that construction of btree happens in order of reversed level.

    index = 0;
    // so that this will be used.
    modifyBTreeByReversing(root, arr, &index, 0);
}
int isSibling(struct bnode* root, struct bnode* a, struct bnode* b)
{

    if(root == NULL)
	return 0; // if root is null, then return 0.

    return (root->left == a && root->right == b) || // if the elements are siblings, i.e. having the same parent..
           (root->left == b && root->right == a) || (isSibling(root->left, a, b)) || // traverse the left subtree..
           (isSibling(root->right, a, b)); // traverse the right subtree..
}
int checkIfNodesAreCousin(struct bnode* root, struct bnode* a, struct bnode* b)
{
    // A node is said to be a cousion of the other only when they are in the same level but having different parent
    if((getLevel(root, a->data) == getLevel(root, b->data)) && !isSibling(root, a, b)) {
	printf("The given two nodes are cousin of each other..");
    } else {
	printf("Not cousion of each other..");
    }
}
void serializeBTree(struct bnode* root,FILE *fp){
	// serialize Binary tree, storing the data into a file.
	//PreOrder Traversal -> N L R, marking the null pointers as -1, including for the leaf node.
	if(root==NULL){
		fprintf(fp,"%d ",-1);
		return ;
	}
	fprintf(fp,"%d ",root->data);//N
	serializeBTree(root->left,fp);//L
	serializeBTree(root->right,fp);//R
}
void deSerializeBTree(struct bnode **root,FILE *fp){
	// deserialize Binary tree, from the file read the data to a node
	//PreOrder Traversal -> N L R, marking the null pointers as -1, including for the leaf node.
	int val;
	
	/* Read next item from file. If theere are no more items or next
	 * item is marker, then return
	 * */
	
	if(!fscanf(fp,"%d",&val)||val==-1){//if not able to read the file / null pointer has been reached.
		return;
	}
	
	*root=newBNode(val);//N
	deSerializeBTree(&(*root)->left,fp);//L
	deSerializeBTree(&(*root)->right,fp);//R
}
void printNodesBetweenTwoLevel(struct bnode* root,int low,int high){
	
	struct bQueue* queue = createBQueue(15);
	enqueueBNode(queue,root);
	enqueueBNode(queue,NULL);//end of first level
	struct bnode* cur = NULL;
	int level=1;
	while(queue->size!=0){//as long as the queue is not empty
		cur = dequeueBNode(queue);//dequeue the node
		if(cur==NULL)//if the node dequeued is marker, then the level has ended.
		{
			level++;//increment the level.
			
			if(queue->size==0||level>high)//if the queue is empty
			//if the level is greater than high, then just return.
				break ;
			
			enqueueBNode(queue,NULL);
				
			continue; //this is required, because we don't want to proceed further.
		}
		
		if(level>=low){
			//if the level is greater than the low level.
			printf("%d ",cur->data);
		}
		if(cur->left){
			enqueueBNode(queue,cur->left);
		}
		if(cur->right){
			enqueueBNode(queue,cur->right);
		}
		
	}
	
	
}
int closestDistToLeaf(struct bnode* root){
	//Returns the closest distance to leaf node from root, This closest traversal, goes towards the left and right subtree
	//but it doesn't go through the ancestors, for which the following method should be used.
	if(root==NULL)
		return INT_MAX; //not a leaf node
	if(root->left==NULL&&root->right==NULL)
		return 0;//a leaf node
	return 1+(getMin(closestDistToLeaf(root->left),closestDistToLeaf(root->right)));//return the min distance to leaf from root node.
}
int findClosestLeafNodeAmongAncestorsAlso(struct bnode* root,int key,struct bnode *ancestors[],int index)
{
		//does a preorder traversal, stores the node in ancestors
		if(root==NULL)
			return INT_MAX; //if only one node, max is returned so that min is chosen [i.e the leaf]
		
		if(root->data==key){
			//if the key is found in root.
			int res = closestDistToLeaf(root); //get the closest distance to leaf node using left and right subtree traversal
			int i;
			for(i=index-1;i>=0;i--){
					//going backwards, ancestors
				res = getMin(res,index-i+closestDistToLeaf(ancestors[i]));
				/*
				 * 	   A
					 /    \    
                                        B       C
					     /   \  
					    E     F   
					   /       \
					  G         H
					 / \       /
                                        I   J     K

	Closest leaf to 'H' is 'K', so distance is 1 for 'H'
	Closest leaf to 'C' is 'B', so distance is 2 for 'C'
	 * 			ancestors[] = {A,B,C,E,F}
	 * 			For example, F is the node data, then index is 4 i traverses from 3 to 0, ancestors[3] is E, i.e for 
	 * 			a right node, left and node data will always be the ancestor, 
	 * 			in order to find the closest leaf, we will have to find the closestDistToLeaf of every ancestors.
	 * 			ancestors[3] is E, index-i + closest(ancestors[3]) => 4-3 => 1 (from F to C) + closest(ancestors[3]) 
				 * 
				 * 
				 * */
				 
			}
			return res;
		}
		ancestors[index]=root; //N
		return getMin(findClosestLeafNodeAmongAncestorsAlso(root->left,key,ancestors,index+1),
		findClosestLeafNodeAmongAncestorsAlso(root->right,key,ancestors,index+1));
	
}
void printSpecificLevelOrder(struct bnode* root){
   
    /*
     i.e.Given a perfect binary tree,
     *  print nodes in level order but nodes should be from left and right side alternatively. Here 1st and 2nd levels are trivial.
    While 3rd level: 4(left), 7(right), 5(left), 6(right) are printed.
    While 4th level: 8(left), 15(right), 9(left), 14(right), .. are printed.
    While 5th level: 16(left), 31(right), 17(left), 30(right), .. are printed.
    */
    
    /*
     The standard level order traversal idea will slightly change here. Instead of processing ONE node at a time, 
     * we will process TWO nodes at a time. And while pushing children into queue,
     * the enqueue order will be: 1st node’s left child, 2nd node’s right child, 1st node’s right child and 2nd node’s left child.
     */
    if(root==NULL)
        return ;
    printf("%d ",root->data);
    
    if(root->left==NULL){
        return ;
    }else{
        printf("%d %d ", root->left->data, root->right->data);
    }
    
    struct bQueue* queue = createBQueue(70);
    
    //We are going to process two nodes, root->left->left , then root->right->right
    enqueueBNode(queue,root->left);
    enqueueBNode(queue,root->right);
    
    while(queue->size!=0){//unless the queue is empty
       
        struct bnode* leftNode = dequeueBNode(queue);
        struct bnode* rightNode = dequeueBNode(queue);
        
        printf("%d %d ",leftNode->left->data,rightNode->right->data);
        printf("%d %d ",leftNode->right->data,rightNode->left->data);//if we are printing backwards, then these elements can be pushed to stack
        
        if(leftNode->left->left!=NULL){//given it's a perfect binary tree, we are not searching for rightNode
            enqueueBNode(queue,leftNode->left);
            enqueueBNode(queue,rightNode->right);
            enqueueBNode(queue,leftNode->right);
            enqueueBNode(queue,rightNode->left);
        }
    }

}
void convertLeftRightToDownRight(struct bnode* root){
    /*
     * Given a binary tree in left-right representation as below

                               1
                            /    \
                           2	  3
                               /    \
                              4      5
                             /     /   \
                            6    7      8 

Convert the structure of the tree to down-right representation like the below tree.

                        1
                        |
                        2 – 3
                            |
                            4 — 5
                            |   |
                            6   7 – 8 

The conversion should happen in-place, 
     * 
     * i.e., left child pointer should be used as down pointer and right child pointer should be used as right sibling pointer.
     */
    if(root==NULL) return ;//Base Case
    
    //First the child should be processed, If we were to process parent first, then we may not have the pointer to right subtre.
    convertLeftRightToDownRight(root->left);
    convertLeftRightToDownRight(root->right);

    if(root->left==NULL)
        root->left = root -> right; //point left pointer to the sibling
    else{
        root->left->right = root->right;//Child's right pointer should point to the sibling.
    }
    root->right = NULL; //set the right pointer to NULL.
}

void downRightTraversal(struct bnode *root) {
    if (root != NULL) {
        printf("%d ",root->data);
        downRightTraversal(root->right);//This will be ignored for the second level. 
        //For third level onwards, Left right will be recursively processed.
        downRightTraversal(root->left);
    }
}
void printLevelOrderByLine(struct bnode* root){
    
    struct bQueue* queue = createBQueue(100);
    if(root==NULL) return ;
    
    enqueueBNode(queue,root);
    
    while(1){
        
        int currLevelCount = queue->size;//for this level, number of nodes.
        if(currLevelCount==0){
            break ; //if the queue is empty then break from the loop.
        }
        while(currLevelCount>0){
            struct bnode* curr = dequeueBNode(queue);
            printf("%d ",curr->data);
            if(curr->left!=NULL){
                enqueueBNode(queue,curr->left);
            }
            if(curr->right!=NULL) {
                enqueueBNode(queue, curr->right);
            }
            currLevelCount--;//Important
        }
        //end of the level
        printf("\n");
    }
}
//-------------------- End of Function Definitions----------------------//

//-------------------- Start of Test Data Methods-----------------------//

void identicalTreeTestData()
{
    struct bnode* root1 = newBNode(1);
    struct bnode* root2 = newBNode(1);
    root1->left = newBNode(2);
    root1->right = newBNode(3);
    root1->left->left = newBNode(4);
    root1->left->right = newBNode(5);

    root2->left = newBNode(2);
    root2->right = newBNode(3);
    root2->left->left = newBNode(4);
    root2->left->right = newBNode(5);

    if(identicalTrees(root1, root2))
	printf("Both tree are identical.");
    else
	printf("Trees are not identical.");
}
void sizeTestData()
{
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);

    printf("Size of the tree is %d", size(root));
}

void traveresalTestData()
{
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);

    printf("\n Preorder traversal of binary tree is \n");
    printPreOrder(root);

    printf("\n Inorder traversal of binary tree is \n");
    printInOrder(root);

    printf("\n Postorder traversal of binary tree is \n");
    printPostOrder(root);
}
void isCompleteBinaryTreeTestData()
{
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    root->right->right = newBNode(6);
    /*
     *  Let us construct the following Binary Tree which
         is not a complete Binary Tree
               1
             /   \
            2     3
           / \     \
          4   5     6

     * */
    if(isCompleteBT(root))
	printf("Complete Binary Tree");
    else
	printf("NOT Complete Binary Tree");
}
void inOrderStackTestData()
{
    /* Constructed binary tree is
      1
    /   \
  2      3
/  \
4     5
*/
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);

    inorderUsingStack(root);
}
void maxDepthTreeTestData()
{
    struct bnode* root = newBNode(1);

    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);

    printf("Hight of tree is %d", maxDepthOfTree(root));
}
void deleteTreeTestData()
{
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);

    // Note that we pass the address of root here
    deleteTree(&root);
    printf("\n Tree deleted ");
}
void mirrorTreeTestData()
{ // time complexity: o(n)
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);

    /* Print inorder traversal of the input tree */
    printf("\n Inorder traversal of the constructed tree is \n");
    printInOrder(root);

    /* Convert tree to its mirror */
    constructMirror(root);

    /* Print inorder traversal of the mirror tree */
    printf("\n Inorder traversal of the mirror tree is \n");
    printInOrder(root);
}
void printAllPathsTestData()
{
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);

    /* Print all root-to-leaf paths of the input tree */
    printPaths(root);
}
void treeToCircularListTestData()
{
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    /*
     * 		1
     *   2   3
     * 4  5
     *
     * 4 2 5 1 3 -> Inorder successor
     * */
    struct DllNode* head = convertTreeToList(root);
    printDllList(head);
}
void levelOrderTraversalTestData()
{
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);

    printf("Level Order traversal of binary tree is \n");
    printLevelOrderTraversal(root);
}
void LeafCountTestData()
{
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    printf("%d ", getLeafCount(root));
}

void alternateLevelSpiralTraversalTestData()
{
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(7);
    root->left->right = newBNode(6);
    root->right->left = newBNode(5);
    root->right->right = newBNode(4);
    alternateLevelSpiralTraversal(root);
}
void printSpiralTestData()
{
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(7);
    root->left->right = newBNode(6);
    root->right->left = newBNode(5);
    root->right->right = newBNode(4);
    printf("Spiral Order traversal of binary tree is \n");
    printSpiral(root);
}
void isChildrenSumParentValueTestData()
{
    struct bnode* root = newBNode(10);
    root->left = newBNode(8);
    root->right = newBNode(2);
    root->left->left = newBNode(3);
    root->left->right = newBNode(5);
    root->right->right = newBNode(2);
    if(isChildrenSumParentValue(root))
	printf("The given tree satisfies the children sum property ");
    else
	printf("The given tree does not satisfy the children sum property ");
}
void convertTreeToChildremSuMTestData()
{
    struct bnode* root = newBNode(50);
    root->left = newBNode(7);
    root->right = newBNode(2);
    root->left->left = newBNode(3);
    root->left->right = newBNode(5);
    root->right->left = newBNode(1);
    root->right->right = newBNode(30);

    printf("\n Inorder traversal before conversion ");
    printInOrder(root);

    convertTreeToChildremSuM(root);

    printf("\n Inorder traversal after conversion ");
    printInOrder(root);
}
void diameterOfTreeTestData()
{
    /* Constructed binary tree is
       1
     /   \
   2      3
 /  \
4     5
*/
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    int height = 0;
    printf("Diameter of the given binary tree is %d\n", diameterOfTree(root));
    printf("Optimized diameter of the given binary tree is %d\n", diameterOfTreeOptimized(root, &height));
}
void isHeightBalancedTreeTestData()
{
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    root->left->left->left = newBNode(8);
    int height = 0;
    //    if(isHeightBalancedTree(root))
    if(isHeightBalancedTreeOptimized(root, &height))
	printf("Tree is balanced");
    else
	printf("Tree is not balanced");
}
void morrisInOrderTraversalTestData()
{
    /* Constructed binary tree is
       1
     /   \
   2      3
 /  \
4     5
*/
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);

    printInOrderWithoutRecurAndStack(root);
}
void hasPathSumTestData()
{
    // Time complexity: O(n) time
    int sum = 21;

    /* Constructed binary tree is
              10
            /   \
          8      2
        /  \    /
      3     5  2
    */
    struct bnode* root = newBNode(10);
    root->left = newBNode(8);
    root->right = newBNode(2);
    root->left->left = newBNode(3);
    root->left->right = newBNode(5);
    root->right->left = newBNode(2);

    if(hasPathSum(root, sum))
	printf("There is a root-to-leaf path with sum %d", sum);
    else
	printf("There is no root-to-leaf path with sum %d", sum);
}
void constructTreeFromInAndPreOrderTestData()
{
    /*
     *      A
   /   \
 /       \
B         C
/ \        /
/     \    /
D       E  F
* */
    // D 68 66 69 65 70 67
    char in[] = { 'D', 'B', 'E', 'A', 'F', 'C' };
    char pre[] = { 'A', 'B', 'D', 'E', 'C', 'F' };
    int len = sizeof(in) / sizeof(in[0]);
    struct bnode* root = constructTreeFromInAndPreOrder(in, pre, 0, len - 1);

    /* Let us test the built tree by printing Insorder traversal */
    printf("\n Inorder traversal of the constructed tree is \n");
    printInOrder(root);
}
void doubleTreeTestData()
{
    /* Constructed binary tree is
        1
      /   \
    2      3
  /  \
4     5
*/
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);

    printf("Inorder traversal of the original tree is \n");
    printInOrder(root);

    doubleTree(root);

    printf("\n Inorder traversal of the double tree is \n");
    printInOrder(root);
}
void getMaxWidthTestData()
{
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    root->right->right = newBNode(8);
    root->right->right->left = newBNode(6);
    root->right->right->right = newBNode(7);

    /*
     Constructed bunary tree is:
            1
          /  \
         2    3
       /  \     \
      4   5     8
                /  \
               6   7
    */
    printf("Maximum width is %d \n", getMaxWidth(root));
    printf("Maximum width using queue is %d\n", getWidthUsingQueue(root));
    printf("Maximum width using array is %d\n", getMaxWidthByUsingLevelArray(root));
}
void isFoldableTestData()
{
    /* The constructed binary tree is
             1
           /   \
          2     3
          \    /
           4  5
      */
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->right->left = newBNode(4);
    root->left->right = newBNode(5);

    //  if(isFoldable(root) == 1)
    if(isFoldableByCheckingMirror(root)) {
	printf("\n tree is foldable");
    } else {
	printf("\n tree is not foldable");
    }
}
void printKDistantNodesTestData()
{
    /* Constructed binary tree is
1
/   \
2      3
/  \    /
4     5  8
*/
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    root->right->left = newBNode(8);

    printKDistantNodes(root, 2);
}
void getLevelOfANodeInTreeTestData()
{
    struct bnode* root;
    int x;

    /* Constructing tree given in the above figure */
    root = newBNode(3);
    root->left = newBNode(2);
    root->right = newBNode(5);
    root->left->left = newBNode(1);
    root->left->right = newBNode(4);

    for(x = 1; x <= 5; x++) {
	int level = getLevel(root, x);
	if(level)
	    printf(" Level of %d is %d\n", x, getLevel(root, x));
	else
	    printf(" %d is not present in tree \n", x);
    }
}
void printAncestorsTestData()
{
    /* Construct the following binary tree
          1
        /   \
      2      3
    /  \
  4     5
 /
7
*/
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    root->left->left->left = newBNode(7);

    printAncestors(root, 7);
    printf("\n");
    printAncestorsWithoutRecursion(root, 7);
}
void sumTreeTestData()
{
    struct bnode* root = newBNode(26);
    root->left = newBNode(10);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(6);
    root->right->right = newBNode(3);
    //    if(isSuMTree(root))
    if(isSuMTreeTricky(root))
	printf("The given tree is a SumTree ");
    else
	printf("The given tree is not a SumTree ");
}
void isSubTreeBinaryTreeTestData()
{
    /* Construct the following tree
         26
       /   \
     10     3
   /    \     \
 4      6      3
  \
   30
*/
    struct bnode* T = newBNode(26);
    T->right = newBNode(3);
    T->right->right = newBNode(3);
    T->left = newBNode(10);
    T->left->left = newBNode(4);
    T->left->left->right = newBNode(30);
    T->left->right = newBNode(6);

    /* Construct the following tree
          10
        /    \
      4      6
       \
        30
    */
    struct bnode* S = newBNode(10);
    S->right = newBNode(6);
    S->left = newBNode(4);
    S->left->right = newBNode(30);

    if(isSubTreeBinaryTree(T, S))
	//	if(isSubTreeBinaryTreeUsingInAndPreOrder(T,S))
	printf("Tree S is subtree of tree T");
    else
	printf("Tree S is not a subtree of tree T");

    // the below code takes O(N) time
    struct bnode* aNode = newBNode('a');
    aNode->left = newBNode('b');
    aNode->right = newBNode('d');
    aNode->left->left = newBNode('c');
    //    aNode->right->right = newBNode('e');
    // uncommenting that, we will get no: s is not a subtree of T
    struct bnode* bNodeS = newBNode('a');
    bNodeS->left = newBNode('b');
    bNodeS->left->left = newBNode('c');
    bNodeS->right = newBNode('d');

    if(isSubTreeBinaryTreeUsingInAndPreOrder(aNode, bNodeS))
	printf("Yes: S is a subtree of T");
    else
	printf("No: S is NOT a subtree of T");
}
void connectNodeAtSameLevelUsingPreOrderTestData()
{
    /* Constructed binary tree is
        10
      /   \
    8      2
  /
3
*/
    struct connNode* root = newConnNode(10);
    root->left = newConnNode(8);
    root->right = newConnNode(2);
    root->left->left = newConnNode(3);

    // Populates nextRight pointer in all nodes
    connectNodeAtSameLevelUsingPreOrder(root);

    // Let us check the values of nextRight pointers
    printf("Following are populated nextRight pointers in the tree "
           "(-1 is printed if there is no nextRight) \n");
    printf("nextRight of %d is %d \n", root->data, root->nextRight ? root->nextRight->data : -1);
    printf("nextRight of %d is %d \n", root->left->data, root->left->nextRight ? root->left->nextRight->data : -1);
    printf("nextRight of %d is %d \n", root->right->data, root->right->nextRight ? root->right->nextRight->data : -1);
    printf("nextRight of %d is %d \n",
           root->left->left->data,
           root->left->left->nextRight ? root->left->left->nextRight->data : -1);

    /* Constructed binary tree is
                  10
                /   \
              8      2
            /         \
          3            90
        */
    root = newConnNode(10);
    root->left = newConnNode(8);
    root->right = newConnNode(2);
    root->left->left = newConnNode(3);
    root->right->right = newConnNode(90);

    // Populates nextRight pointer in all nodes
    connectNodesUtilRecurByLevelWise(root);

    // Let us check the values of nextRight pointers
    printf("Following are populated nextRight pointers in the tree "
           //    printf("Following are populated nextRight pointers in the tree "
           "(-1 is printed if there is no nextRight) \n");
    printf("nextRight of %d is %d \n", root->data, root->nextRight ? root->nextRight->data : -1);
    printf("nextRight of %d is %d \n", root->left->data, root->left->nextRight ? root->left->nextRight->data : -1);
    printf("nextRight of %d is %d \n", root->right->data, root->right->nextRight ? root->right->nextRight->data : -1);
    printf("nextRight of %d is %d \n",
           root->left->left->data,
           root->left->left->nextRight ? root->left->left->nextRight->data : -1);
    printf("nextRight of %d is %d \n",
           root->right->right->data,
           root->right->right->nextRight ? root->right->right->nextRight->data : -1);
}
void populateInOrderSuccessorTestData()
{

    /* Constructed binary tree is
          10
        /   \
      8      12
    /
  3
*/
    struct connNode* root = newConnNode(10);
    root->left = newConnNode(8);
    root->right = newConnNode(12);
    root->left->left = newConnNode(3);

    // Populates nextRight pointer in all nodes
    populateInOrderSuccessor(root);

    // Let us see the populated values
    struct connNode* ptr = root->left->left;
    while(ptr) {
	// -1 is printed if there is no successor
	printf("Next of %d is %d \n", ptr->data, ptr->nextRight ? ptr->nextRight->data : -1);
	ptr = ptr->nextRight;
    }
}
void convertTreeToSuMTreeTestData()
{
    struct bnode* root = NULL;
    int x;
    // output should be 0 4 0 20 0 12 0
    /* Constructing tree given in the above figure */
    root = newBNode(10);
    root->left = newBNode(-2);
    root->right = newBNode(6);
    root->left->left = newBNode(8);
    root->left->right = newBNode(-4);
    root->right->left = newBNode(7);
    root->right->right = newBNode(5);

    convertTreeToSuMTree(root);

    // Print inorder traversal of the converted tree to test result of toSumTree()
    printf("Inorder Traversal of the resultant tree is: \n");
    printInOrderWithoutRecurAndStack(root);
}
void printMaxPathTestData()
{
    /*Given a Binary Tree,
     * find the maximum sum path from a leaf to root.
For example, in the following tree,
there are three leaf to root paths 8->-2->10, -4->-2->10 and 7->10.
* The sums of these three paths are 16, 4 and 17 respectively.
The maximum of them is 17 and the path for maximum is 7->10.

             10
          /      \
                    -2         7
              /   \
            8     -4

     *
     * */
    struct bnode* root = NULL;

    /* Constructing tree given in the above figure */
    root = newBNode(10);
    root->left = newBNode(-2);
    root->right = newBNode(7);
    root->left->left = newBNode(8);
    root->left->right = newBNode(-4);

    printf("Following are the nodes on the maximum sum path \n");

    struct bnode* target_leaf;
    int max_sum = 0;

    // find the target leaf and maximum sum
    getTargetLeaf(root, &target_leaf, 0, &max_sum);

    // print the path from root to the target leaf
    printPathEqualToNode(root, target_leaf);

    printf("\nSum of the nodes is %d ", max_sum);
}
void constructSpecialBinaryTreeFromInorderTestData()
{
    int inorder[] = { 5, 10, 40, 30, 28 };
    int size = sizeof(inorder) / sizeof(inorder[0]);
    struct bnode* root = constructSpecialBinaryTreeFromInorder(inorder, 0, size - 1);
    printf("Inorder traversal of constructed special binary tree is..");
    printInOrder(root);
}
void constructSpecialBinaryTreeFromPreOrderTestData()
{
    int pre[] = { 10, 30, 20, 5, 15 };
    char preLN[] = { 'N', 'N', 'L', 'L', 'L' };
    int n = sizeof(pre) / sizeof(pre[0]);
    int startIndex = 0;
    struct bnode* root = constructSpecialBinaryTreeFromPreOrder(pre, preLN, &startIndex, n);
    printf("Constructed special binary tree from pre order is .."); // output 20 30 5 10 15
    printInOrder(root);
    //	constructSpecialBinaryTreeFromPreOrder()
}
void boundaryTraversalBinaryTreeTestData()
{

    struct bnode* root = newBNode(20);
    root->left = newBNode(8);
    root->left->left = newBNode(4);
    root->left->right = newBNode(12);
    root->left->right->left = newBNode(10);
    root->left->right->right = newBNode(14);
    root->right = newBNode(22);
    root->right->right = newBNode(25);
    boundaryTraversalBinaryTree(root);
    // Boundary output: 20 8 4 10 14 25 22
}
void constructFullTreeFromPreAndPostOrderTestData()
{
    int pre[] = { 1, 2, 4, 8, 9, 5, 3, 6, 7 };
    int post[] = { 8, 9, 4, 5, 2, 6, 7, 3, 1 };
    int size = sizeof(pre) / sizeof(pre[0]);
    int preStartIndex = 0;
    struct bnode* root = constructFullTreeFromPreAndPostOrder(pre, post, &preStartIndex, 0, size - 1, size);
    printf("After constructing the full tree from pre and post order..");
    printInOrder(root);
}
void iterativePreOrderTraversalTestData()
{
    /* Constructed binary tree is
       10
     /   \
   8      2
 /  \    /
3     5  2
*/
    struct bnode* root = newBNode(10);
    root->left = newBNode(8);
    root->right = newBNode(2);
    root->left->left = newBNode(3);
    root->left->right = newBNode(5);
    root->right->left = newBNode(2);
    iterativePreOrderTraversal(root);
}
void morrisPreOrderTraversalTestData()
{
    struct bnode* root = NULL;

    root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);

    root->left->left = newBNode(4);
    root->left->right = newBNode(5);

    root->right->left = newBNode(6);
    root->right->right = newBNode(7);

    root->left->left->left = newBNode(8);
    root->left->left->right = newBNode(9);

    root->left->right->left = newBNode(10);
    root->left->right->right = newBNode(11);

    printPreOrderWithoutStackAndRecur(root);

    printf("\n");
    printPreOrder(root);
}
void createCompleteBinaryTreeUsingLinkedListTestData()
{
    createCompleteBinaryTreeUsingLinkedList();
}
void iterativePostOrderTraversalTestData()
{
    // Let us construct the tree shown in above figure
    struct bnode* root = NULL;
    root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    root->right->left = newBNode(6);
    root->right->right = newBNode(7);

    iterativePostOrderTraversal(root);
    iterativePostOrderTraversalUsingSingleStack(root);

    // prints 4 5 2 6 7 3 1
}
void reverseLevelOrderTraversalTestData()
{
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    root->right->left = newBNode(6);
    root->right->right = newBNode(7);

    printf("Level Order traversal of binary tree is \n");
    reverseLevelOrderTraversal(root);
}
void convertListToBinaryTestData()
{
    // converts the list to binary tree
    // create a linked list shown in above diagram
    struct node* head = NULL;
    push(&head, 36); /* Last node of Linked List */
    push(&head, 30);
    push(&head, 25);
    push(&head, 15);
    push(&head, 12);
    push(&head, 10); /* First node of Linked List */

    struct bnode* root;
    convertListToBinary(head, &root);

    printf("Inorder Traversal of the constructed Binary Tree is: \n");
    printInOrder(root);
    /*
     * Inorder Traversal of the constructed Binary Tree is:
    25 12 30 10 36 15
     *
     * */
}
void binTreeToDllListTestData()
{
    // Let us create the tree shown in above diagram
    struct bnode* root = newBNode(10);
    root->left = newBNode(12);
    root->right = newBNode(15);
    root->left->left = newBNode(25);
    root->left->right = newBNode(30);
    root->right->left = newBNode(36);

    // Convert to DLL
    struct bnode* head = binTreeToDllList(root);

    // Print the converted list
    while(head != NULL) {
	printf("%d ", head->data);
	head = head->right;
    }

    // convert tree to doubly linked list
    // Let us create the tree shown in above diagram
    struct bnode* root1 = newBNode(10);
    root1->left = newBNode(12);
    root1->right = newBNode(15);
    root1->left->left = newBNode(25);
    root1->left->right = newBNode(30);
    root1->right->left = newBNode(36);

    printf("\n\t\tInorder Tree Traversal\n\n");
    printInOrder(root1);

    head = BTTToDLL(root1);
    //	head=NULL;
    //	binTreeToDllListUsingInorder(root,&head);
    printf("\n\n\t\tDLL Traversal\n\n");

    printf("\n");
    // Print the converted list
    while(head != NULL) {
	printf("%d ", head->data);
	head = head->right;
    }

    // Let us create the tree shown in above diagram
    struct bnode* root2 = newBNode(10);
    root2->left = newBNode(12);
    root2->right = newBNode(15);
    root2->left->left = newBNode(25);
    root2->left->right = newBNode(30);
    root2->right->left = newBNode(36);

    // Convert to DLL
    struct bnode* head1 = NULL;
    binTreeToDllListUsingInorder(root2, &head1);
    printf("\n\nType 3: DLLTraversal using bin inorder..\n\n");
    // Print the converted list
    while(head1 != NULL) {
	printf("%d ", head1->data);
	head1 = head1->right;
    }
}
void isIsoMorphicTestData()
{
    // Let us create trees shown in above diagram
    struct bnode* n1 = newBNode(1);

    n1->left = newBNode(2);
    n1->right = newBNode(3);
    n1->left->left = newBNode(4);
    n1->left->right = newBNode(5);
    n1->right->left = newBNode(6);
    n1->left->right->left = newBNode(7);
    n1->left->right->right = newBNode(8);

    struct bnode* n2 = newBNode(1);
    n2->left = newBNode(3);
    n2->right = newBNode(2);
    n2->right->left = newBNode(4);
    n2->right->right = newBNode(5);
    n2->left->right = newBNode(6);
    n2->right->right->left = newBNode(8);
    n2->right->right->right = newBNode(7);

    if(isIsoMorphic(n1, n2))
	printf("Given trees are isomorphic to each other..");
    else
	printf("given trees are not isomorphic to each other..");
}
void interpretationOnArrayOfDigitTestData()
{
    // please find the problem and it's solution at Java project.
}
void iterativeFindHeightTestData()
{
    // Let us create binary tree shown in above diagram
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);

    printf("Height of tree is %d ", iterativeFindHeight(root));
}
void printLevelDiffTestData()
{
    struct bnode* root = newBNode(5);
    root->left = newBNode(2);
    root->right = newBNode(6);
    root->left->left = newBNode(1);
    root->left->right = newBNode(4);
    root->left->right->left = newBNode(3);
    root->right->right = newBNode(8);
    root->right->right->right = newBNode(9);
    root->right->right->left = newBNode(7);
    printf("%d is the required difference\n", findOddAndEvenLevelDiff(root));
    // we can alternatively use the queue for printing the diff between odd and even level node data.
    printf("%d - using level order\n", findOddAndEvenLevelDiffUsingLevelOrder(root));
}
void printPostOrderFromInAndPreTestData()
{
    int in[] = { 4, 2, 5, 1, 3, 6 };
    int pre[] = { 1, 2, 4, 5, 3, 6 };
    int n = sizeof(in) / sizeof(in[0]);
    //   cout << "Postorder traversal " << endl;
    printf("Post order traversal..\n");
    printPostOrderFromInAndPre(in, pre, n);

    //   return 0;
}
void findDepthOfOddLeavesTestData()
{
    int level = 1, depth = 0;
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->right->left = newBNode(5);
    root->right->right = newBNode(6);
    root->right->left->right = newBNode(7);
    root->right->right->right = newBNode(8);
    root->right->left->right->left = newBNode(9);
    root->right->right->right->right = newBNode(10);
    root->right->right->right->right->left = newBNode(11);
    printf("Depth of odd leaves is %d", findDepthOfOddLeaves(root, level));
}
void checkIfLeavesAtSameLevelTestData()
{
    // Does a simple traversal, so takes O(n) time..
    // Let us create tree shown in thirdt example
    struct bnode* root = newBNode(12);
    root->left = newBNode(5);
    root->left->left = newBNode(3);
    root->left->right = newBNode(9);
    root->left->left->left = newBNode(1);
    root->left->right->left = newBNode(1);
    int leafLevel = 0;
    if(checkIfLeavesAtSameLevel(root, 1, &leafLevel))
	printf("Leaves are at same level\n");
    else
	printf("Leaves are not at same level\n");
}
void printLeftViewBinTreeTestData()
{
    struct bnode* root = newBNode(12);
    root->left = newBNode(10);
    root->right = newBNode(30);
    root->right->left = newBNode(25);
    root->right->right = newBNode(40);
    printLeftViewBinTree(root);
}
void pruneTreeTestData()
{
    int k = 45;
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    root->right->left = newBNode(6);
    root->right->right = newBNode(7);
    root->left->left->left = newBNode(8);
    root->left->left->right = newBNode(9);
    root->left->right->left = newBNode(12);
    root->right->right->left = newBNode(10);
    root->right->right->left->right = newBNode(11);
    root->left->left->right->left = newBNode(13);
    root->left->left->right->right = newBNode(14);
    root->left->left->right->right->left = newBNode(15);

    printf("Tree before truncation\n");
    printInOrder(root);
    int Sum = 0;
    //    root = pruneNodesWithSuMLess(root, k,&Sum); // k is 45
    root = pruneNodesBySubtractNodeData(root, k);
    printf("\n\nTree after truncation\n");
    printInOrder(root);
}
void extractLeavesToDllListTestData()
{
    struct bnode* head = NULL;
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    root->right->right = newBNode(6);
    root->left->left->left = newBNode(7);
    root->left->left->right = newBNode(8);
    root->right->right->left = newBNode(9);
    root->right->right->right = newBNode(10);

    printf("Inorder Trvaersal of given Tree is:\n");
    printInOrder(root);

    root = extractLeavesToDllList(root, &head);

    printf("\nExtracted Double Linked list is:\n");
    while(head != NULL) {
	printf("%d ", head->data);
	head = head->right;
    }

    printf("\nInorder traversal of modified tree is:\n");
    printInOrder(root);
}
void deepestLeftLeafNodeTestData()
{

    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->right->left = newBNode(5);
    root->right->right = newBNode(6);
    root->right->left->right = newBNode(7);
    root->right->right->right = newBNode(8);
    root->right->left->right->left = newBNode(9);
    root->right->right->right->right = newBNode(10);
    int maxlevel = 0;
    struct bnode* result = NULL;

    deepestLeftLeafNode(root, 0, &maxlevel, 0, &result);
    if(result) {
	printf("Deepest left leaf node..%d ", result->data);
    } else {
	printf("Deepest leaf node not found..");
    }
}
void getNextRightAfterKeyTestData()
{
    struct bnode* root = newBNode(10);
    root->left = newBNode(2);
    root->right = newBNode(6);
    root->right->right = newBNode(5);
    root->left->left = newBNode(8);
    root->left->right = newBNode(4);
    getNextRightAfterKey(root, 2);
    getNextRightAfterKey(root, 10);
}
void sumOfAllNumbersFromRootToLeafPathTestData()
{
    struct bnode* root = newBNode(6);
    root->left = newBNode(3);
    root->right = newBNode(5);
    root->right->right = newBNode(7);
    root->left->left = newBNode(2);
    root->left->right = newBNode(5);
    root->right->right = newBNode(4);
    root->left->right->left = newBNode(7);
    root->left->right->right = newBNode(4);
    printf("sum of all numbers %d", sumOfAllNumbersFromRootToLeafPath(root, 0));
}
void findLCATestData()
{
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    root->right->left = newBNode(6);
    root->right->right = newBNode(7);
    printf("LCA(4, 5) = %d ", findLCA(root, 4, 5)->data);
    printf("\nLCA(4, 6) = %d", findLCA(root, 4, 6)->data);
    printf("\nLCA(3, 4) = %d", findLCA(root, 3, 4)->data);
    printf("\nLCA(2, 4) = %d", findLCA(root, 2, 4)->data);

    struct bnode* lca = findLCAByValidationUtil(root, 4, 5);
    if(lca != NULL)
	printf("\nLCA(4, 5) = %d", lca->data);
    else
	printf("Keys are not present ");

    lca = findLCAByValidationUtil(root, 4, 10);
    if(lca != NULL)
	printf("\nLCA(4, 10) = %d", lca->data);
    else
	printf("\nKeys are not present ");
}
void getDistanceBetweenTwoNodesTestData()
{

    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    root->right->left = newBNode(6);
    root->right->right = newBNode(7);
    root->right->left->right = newBNode(8);
    printf("Dist(4, 5) = %d", getDistanceBetweenTwoNodes(root, 4, 5));
    printf("\nDist(4, 6) = %d", getDistanceBetweenTwoNodes(root, 4, 6));
    printf("\nDist(3, 4) = %d", getDistanceBetweenTwoNodes(root, 3, 4));
    printf("\nDist(2, 4) = %d", getDistanceBetweenTwoNodes(root, 2, 4));
    printf("\nDist(8, 5) = %d", getDistanceBetweenTwoNodes(root, 8, 5));
}
void printKDistantNodesFromLeafTestData()
{
    // Let us create binary tree given in the above example
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    root->right->left = newBNode(6);
    root->right->right = newBNode(7);
    root->right->left->right = newBNode(8);

    printf("Nodes at distance 2 are: ");
    printKDistantNodesFromLeaf(root, 2);
}
void checkIfHeighBalancedLikeRBTreeTestData()
{
    struct bnode* root = newBNode(10);
    /*root->left = newBNode(5);
    root->right = newBNode(100);
    root->right->left = newBNode(50);
    root->right->right = newBNode(150);
    root->right->left->left = newBNode(40);*/
    // not balanced
    root->left = newBNode(12);
    root->left->right = newBNode(14);
    int maxh, minh, balanced;
    balanced = checkIfHeighBalancedLikeRBTree(root, &maxh, &minh);
    balanced ? printf("Balanced") : printf("Not Balanced");
}
void printNodesAtKPositionFromTargetNodeTestData()
{
    /* Let us construct the tree shown in above diagram */
    struct bnode* root = newBNode(20);
    root->left = newBNode(8);
    root->right = newBNode(22);
    root->left->left = newBNode(4);
    root->left->right = newBNode(12);
    root->left->right->left = newBNode(10);
    root->left->right->right = newBNode(14);
    struct bnode* target = root->left->right;
    printNodesAtKPositionFromTargetNode(root, target, 2);
}
void verticalOrderTraversalTestData()
{
    // Create binary tree shown in above figure
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    root->right->left = newBNode(6);
    root->right->right = newBNode(7);
    root->right->left->right = newBNode(8);
    root->right->right->right = newBNode(9);

    printf("Vertical order traversal is \n");
    printBinaryTreeInVerticalOrder(root);
}
void constructBTreeFromInAndLevelOrderTestData()
{
    int in[] = { 4, 8, 10, 12, 14, 20, 22 };
    int level[] = { 20, 8, 22, 4, 12, 10, 14 };
    int n = sizeof(in) / sizeof(in[0]);
    struct bnode* root = constructBTreeFromInAndLevelOrder(in, level, 0, n - 1, n);

    /* Let us test the built tree by printing Insorder traversal */
    printf("Inorder traversal of the constructed tree is \n");
    printInOrder(root);
}
void findMaxSuMOfPathBetweenTwoLeavesTestData()
{
    struct bnode* root = newBNode(-15);
    root->left = newBNode(5);
    root->right = newBNode(6);
    root->left->left = newBNode(-8);
    root->left->right = newBNode(1);
    root->left->left->left = newBNode(2);
    root->left->left->right = newBNode(6);
    root->right->left = newBNode(3);
    root->right->right = newBNode(9);
    root->right->right->right = newBNode(0);
    root->right->right->right->left = newBNode(4);
    root->right->right->right->right = newBNode(-1);
    root->right->right->right->right->left = newBNode(10);
    int res = -1;
    findMaxSuMOfPathBetweenTwoLeaves(root, &res);
    printf("Max pathSum of the given binary tree is %d", res);
}
void reverseAlternateTestData()
{
    struct bnode* root = newBNode('a');
    root->left = newBNode('b');
    root->right = newBNode('c');
    root->left->left = newBNode('d');
    root->left->right = newBNode('e');
    root->right->left = newBNode('f');
    root->right->right = newBNode('g');
    root->left->left->left = newBNode('h');
    root->left->left->right = newBNode('i');
    root->left->right->left = newBNode('j');
    root->left->right->right = newBNode('k');
    root->right->left->left = newBNode('l');
    root->right->left->right = newBNode('m');
    root->right->right->left = newBNode('n');
    root->right->right->right = newBNode('o');

    printf("Inorder Traversal of given tree\n");
    printInOrder(root);

    reverseAlternate(root);

    printf("\n\nInorder Traversal of modified tree\n");
    printInOrder(root);
}
void checkIfNodesAreCousinTestData()
{
    struct bnode* root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    root->left->right->right = newBNode(15);
    root->right->left = newBNode(6);
    root->right->right = newBNode(7);
    root->right->left->right = newBNode(8);

    struct bnode* Node1, *Node2;
    Node1 = root->left->left;
    Node2 = root->right->right;

    checkIfNodesAreCousin(root, Node1, Node2);
}
void serializeBTreeAndDeserializeTestData(){
	//given a binary search tree, using preorder traversal + bst property we can get the tree.
	
	//given a complete binary tree, using level order traversal would do to get the tree
	
	//given a full binary tree, we will be having only 0 or 2 children, hence a pre order traversal with a pointer to leaf or internal node will help us construct the binary tree.
	
	//For a general binary tree, we can use pre order traversal + marker for null pointer -> to serialize and deserialize the binary tree.
	
	// Let us construct a tree shown in the above figure
    struct bnode *root        = newBNode(20);
    root->left               = newBNode(8);
    root->right              = newBNode(22);
    root->left->left         = newBNode(4);
    root->left->right        = newBNode(12);
    root->left->right->left  = newBNode(10);
    root->left->right->right = newBNode(14);
 
    // Let us open a file and serialize the tree into the file
    FILE *fp = fopen("tree.txt", "w");
    if (fp == NULL)
    {
        puts("Could not open file");
        return 0;
    }
    serializeBTree(root, fp);
    fclose(fp);
 
    // Let us deserialize the storeed tree into root1
    struct bnode *root1 = NULL;
    fp = fopen("tree.txt", "r");
    deSerializeBTree(&root1, fp);
 
    printf("Inorder Traversal of the tree constructed from file:\n");
    printInOrder(root1);
}
void printNodesBetweenTwoLevelTestData(){
	// Let us construct the BST shown in the above figure
    struct bnode *root        = newBNode(20);
    root->left               = newBNode(8);
    root->right              = newBNode(22);
    root->left->left         = newBNode(4);
    root->left->right        = newBNode(12);
    root->left->right->left  = newBNode(10);
    root->left->right->right = newBNode(14);
 
    printf("Level Order traversal between given two levels is");
    printNodesBetweenTwoLevel(root, 2, 3);
}
int findClosestLeafNodeTestData(struct bnode* root,int key){
	struct bnode* ancestors[100];
	return findClosestLeafNodeAmongAncestorsAlso(root,key,ancestors,0);
}
void findClosestLeafNodeAmongAncestorsAlsoTestData(){
	 // Let us construct the BST shown in the above figure
    struct bnode *root        = newBNode('A');
    root->left               = newBNode('B');
    root->right              = newBNode('C');
    root->right->left        = newBNode('E');
    root->right->right       = newBNode('F');
    root->right->left->left  = newBNode('G');
    root->right->left->left->left  = newBNode('I');
    root->right->left->left->right = newBNode('J');
    root->right->right->right      = newBNode('H');
    root->right->right->right->left = newBNode('K');
 
    char k = 'H';
    printf("\nDistace of the closest key from %c is %d",k,findClosestLeafNodeTestData(root, k));
    k = 'C';
    printf("\nDistace of the closest key from %c is %d",k,findClosestLeafNodeTestData(root, k));
    k = 'E';
    printf("\nDistace of the closest key from %c is %d",k,findClosestLeafNodeTestData(root, k));
    k = 'B';
    printf("\nDistace of the closest key from %c is %d",k,findClosestLeafNodeTestData(root, k));
	
}
void printSpecificLevelOrderTestData(){

    //Perfect Binary Tree of Height 4
    struct bnode *root = newBNode(1);

    root->left = newBNode(2);
    root->right = newBNode(3);

    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    root->right->left = newBNode(6);
    root->right->right = newBNode(7);

    root->left->left->left = newBNode(8);
    root->left->left->right = newBNode(9);
    root->left->right->left = newBNode(10);
    root->left->right->right = newBNode(11);
    root->right->left->left = newBNode(12);
    root->right->left->right = newBNode(13);
    root->right->right->left = newBNode(14);
    root->right->right->right = newBNode(15);

    root->left->left->left->left = newBNode(16);
    root->left->left->left->right = newBNode(17);
    root->left->left->right->left = newBNode(18);
    root->left->left->right->right = newBNode(19);
    root->left->right->left->left = newBNode(20);
    root->left->right->left->right = newBNode(21);
    root->left->right->right->left = newBNode(22);
    root->left->right->right->right = newBNode(23);
    root->right->left->left->left = newBNode(24);
    root->right->left->left->right = newBNode(25);
    root->right->left->right->left = newBNode(26);
    root->right->left->right->right = newBNode(27);
    root->right->right->left->left = newBNode(28);
    root->right->right->left->right = newBNode(29);
    root->right->right->right->left = newBNode(30);
    root->right->right->right->right = newBNode(31);

    printf("Specific Level Order traversal of binary tree is \n");
    printSpecificLevelOrder(root);


}
void convertLeftRightToDownRightTestData() {
    // Let us create binary tree shown in above diagram
    /*
           1
         /   \
        2     3
             / \
            4   5
           /   /  \
          6   7    8
     */
    struct bnode *root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->right->left = newBNode(4);
    root->right->right = newBNode(5);
    root->right->left->left = newBNode(6);
    root->right->right->left = newBNode(7);
    root->right->right->right = newBNode(8);

    convertLeftRightToDownRight(root);

    printf("Traversal of the tree converted to down-right form\n");
    downRightTraversal(root);
    
}
void printLevelOrderByLineTestData() {
    // Let us create binary tree shown in above diagram
    struct bnode *root = newBNode(1);
    root->left = newBNode(2);
    root->right = newBNode(3);
    root->left->left = newBNode(4);
    root->left->right = newBNode(5);
    root->right->right = newBNode(6);

    printLevelOrderByLine(root);

}
void binaryTreeTestData()
{
    printLevelOrderByLineTestData();
//    convertLeftRightToDownRightTestData();
//    printSpecificLevelOrderTestData();
//	findClosestLeafNodeAmongAncestorsAlsoTestData();
	//	printNodesBetweenTwoLevelTestData();
	//	serializeBTreeAndDeserializeTestData();
	//	checkIfNodesAreCousinTestData();
	//  reverseAlternateTestData();
    //	findMaxSuMOfPathBetweenTwoLeavesTestData();
    //	constructBTreeFromInAndLevelOrderTestData();
    //	verticalOrderTraversalTestData();
    //	printNodesAtKPositionFromTargetNodeTestData();
    //	checkIfHeighBalancedLikeRBTreeTestData();
    //	printKDistantNodesFromLeafTestData();
    //	getDistanceBetweenTwoNodesTestData();
    //	findLCATestData();
    //	sumOfAllNumbersFromRootToLeafPathTestData();
    //	getNextRightAfterKeyTestData();
    //	deepestLeftLeafNodeTestData();
    //	extractLeavesToDllListTestData();
    //	pruneTreeTestData();
    //	printLeftViewBinTreeTestData();
    //	checkIfLeavesAtSameLevelTestData();
    //	findDepthOfOddLeavesTestData();
    //	printPostOrderFromInAndPreTestData();
    //	printLevelDiffTestData();
    //	iterativeFindHeightTestData();
    //	isIsoMorphicTestData();
    //	binTreeToDllListTestData();
    //	convertListToBinaryTestData();
    //	reverseLevelOrderTraversalTestData();
    //	iterativePostOrderTraversalTestData();
    //	createCompleteBinaryTreeUsingLinkedListTestData();
    //	morrisPreOrderTraversalTestData();
    //	iterativePreOrderTraversalTestData();
    //	constructFullTreeFromPreAndPostOrderTestData();
    //	boundaryTraversalBinaryTreeTestData();
    //	constructSpecialBinaryTreeFromPreOrderTestData();
    //	constructSpecialBinaryTreeFromInorderTestData();
    //	printMaxPathTestData();
    //	convertTreeToSuMTreeTestData();
    //	populateInOrderSuccessorTestData();
    //	connectNodeAtSameLevelUsingPreOrderTestData();
    //	isSubTreeBinaryTreeTestData();
    //	sumTreeTestData();
    //	printAncestorsTestData();
    //	getLevelOfANodeInTreeTestData();
    //	printKDistantNodesTestData();
    //	isFoldableTestData();
    //	getMaxWidthTestData();
    //	doubleTreeTestData();
    //	constructTreeFromInAndPreOrderTestData();
    //	hasPathSumTestData();
    //	morrisInOrderTraversalTestData();
    //	isHeightBalancedTreeTestData();
    //	diameterOfTreeTestData();
    //	convertTreeToChildremSuMTestData();
    //	isChildrenSumParentValueTestData();
    //	printSpiralTestData();
    //	alternateLevelSpiralTraversalTestData();
    //	LeafCountTestData();
    //	levelOrderTraversalTestData();
    //	treeToCircularListTestData();
    //	printAllPathsTestData();
    //	mirrorTreeTestData();
    //	deleteTreeTestData();
    //	maxDepthTreeTestData();
    //	identicalTreeTestData();
    //	isCompleteBinaryTreeTestData();
    //	traveresalTestData();
    //	inOrderStackTestData();
    //	sizeTestData();
}
