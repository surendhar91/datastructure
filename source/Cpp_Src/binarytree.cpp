#include<map>
#include<vector>
#include<stdlib.h>
#include"binaryTreeImp.h"
#include<queue>
#include<stack>
using namespace std;
struct treeNode{
	int data;
	struct treeNode *left;
	struct treeNode *right;
};
struct thbnode{
	int data;
	bool isThreaded;
	struct thbnode* left;
	struct thbnode* right;
};
struct treeNode* createTreeNode(int data){
	struct treeNode* node = (struct treeNode*)malloc(sizeof(struct treeNode));
	node->data=data;
	node->left=NULL;
	node->right=NULL;
	return node;
}
struct thbnode* createThreadedNode(int data){
	struct thbnode* node = (struct thbnode*)malloc(sizeof(struct thbnode));
	node->data=data;
	node->left=NULL;
	node->right=NULL;
	return node;
}
void verticalSuMUtil(struct treeNode *root,int verticalKey,map<int,int> &hMap){//reference is very very important
	if(root==NULL)
		return ;//Base case
    //Do inorder traversal, moving left, verticalKey = verticalKey-1
	//moving right vertical Key = vertical Key +1
	verticalSuMUtil(root->left,verticalKey-1,hMap);
	
	int prevSum = hMap[verticalKey];
	hMap[verticalKey] = prevSum + root->data;//for this vertical key, update the sum with root data.
	verticalSuMUtil(root->right,verticalKey+1,hMap);
}
void verticalSuMBinaryTree(struct treeNode* node){
	//achieves in O(n) time.
	/* Create following Binary Tree
              1
            /    \
          2        3
         / \      / \
        4   5    6   7
  
        */
	if (node == NULL) { return; }
  
        // Creates an empty hashMap hM
    map<int, int> hM;
  
        // Calls the VerticalSumUtil() to store the vertical sum values in hM
    verticalSuMUtil(node, 0, hM);
  
        // Prints the values stored by VerticalSumUtil()
    
	   for( map<int,int>::iterator ii=hM.begin(); ii!=hM.end(); ++ii)
	   {
	       cout << (*ii).first << ": " << (*ii).second << endl;
	   }
    
	
}
void verticalSuMBinaryTreeTestData(){
	struct treeNode* root = createTreeNode(1);
	root->left=createTreeNode(2);
    root->right = createTreeNode(3);
	root->left->left = createTreeNode(4);
	root->left->right = createTreeNode(5);
	root->right->left =createTreeNode(6);
    root->right->right = createTreeNode(7);
    verticalSuMBinaryTree(root);


}
bool findPath(struct treeNode* root,vector<int> &path,int key){
	//find path given a key
	if(root==NULL)
		return false;//because, if we reached here then the data is not found..
		return false;//because, if we reached here then the data is not found..
	
	path.push_back(root->data);
	
	if(root->data==key)
		return true;//if the key is found.
	
	if(findPath(root->left,path,key)||findPath(root->right,path,key))
		return true;//see if the path can be found in left / right subtree.
	
	path.pop_back();//must, if the left traversal of tree doesn't find the key intended, then pop the already added elements.
	
	return false;// if we reached this then we have to return false.
	
}
int findLCA(struct treeNode* root,int n1,int n2){
	//returns the key value of least common ancestor.
	
	//we are going to find the lca
	//Least common ancestor - ancestor of n1 and n2 which is farthest away from root.
	vector<int> path1,path2;
	
	if(!findPath(root,path1,n1)||!findPath(root,path2,n2))//if any one is not found, then return -1
		return -1;
	
	int i;
	for(i=0;i<path1.size()&&i<path2.size();i++){
		//before the mismatch, we need to return the data
		if(path1[i]!=path2[i])
			break;
	}
	return path1[i-1];//common ancestor just before the mismatch.
}
void populateInorderNodesInQueue(struct thbnode* root,queue<struct thbnode *> *que){
	if(root==NULL)
		return ;
	if(root->left)
		populateInorderNodesInQueue(root->left,que);
	que->push(root);
	if(root->right)
		populateInorderNodesInQueue(root->right,que);
}
void convertBinaryTreeToThreadedTree(struct thbnode* root,queue<struct thbnode *> *que){
	/*	     1
            / \
           2   3
          / \ / \
         4  5 6  7   */
	 
	if(root==NULL)
		return ;
	if(root->left)
	convertBinaryTreeToThreadedTree(root->left,que);//L
	
	que->pop();//pop up the element. [used to be the root element] -> N
	if(root->right)//R
		convertBinaryTreeToThreadedTree(root->right,que);
	else{
		//if the right element is not found, then map the right to front of the queue.
		root->right = que->front();
		root->isThreaded = true;
	}
}

struct RandNode{
    int data;
    struct RandNode *left,*right,*random;
};

RandNode* createRandNode(int data){
    RandNode* newNode = new RandNode;//just like class we can create this..
    newNode->data=data;
    newNode->left = newNode->right = newNode->random = NULL;
    return newNode;
}

RandNode* copyLeftRightNode(RandNode* treeNode){
    
    if(treeNode==NULL)
        return NULL;
    RandNode* orgLeftNode   = treeNode->left; //A --> B B is the left node
    //Always treeNode->left will point to the clone node hereafter.
    treeNode->left          = createRandNode(treeNode->data);//A --> CA [A is the treeNode clone the node as CA and make it as left]]
    treeNode->left->left    = orgLeftNode; // A--> CA --> B
    
    if(orgLeftNode!=NULL){//only if the org left node exists, we have to proceed
        orgLeftNode->left = copyLeftRightNode(orgLeftNode);//do the copyLeftRightNode recursively for the tree below..
        //Proceed for B's left.. 
    }
    treeNode->left->right    = copyLeftRightNode(treeNode->right);//The cloned CA now has to point to CC..
    //Clone->right = treeNode->right [Means do the same for the right subtree..]
    return treeNode->left; //The clone node
    

}

void copyRandPointers(RandNode* treeNode, RandNode* cloneNode){

    if(treeNode==NULL)
        return ;
    
    if(treeNode->random!=NULL){
        cloneNode->random = treeNode->random->left; //Here left means the cloned node. [which is random again..]
    }else{
        cloneNode->random = NULL;
    }
    
    if(treeNode->left!=NULL&&cloneNode->left!=NULL){//Ensures the cloneNode exists first and the left element of tree node exists.
        //treeNode->left is CA
        //cloneNode->left is B
        copyRandPointers(treeNode->left->left,cloneNode->left->left);//Move down the tree.
        // B and CB 
    }
    // C and CC
    copyRandPointers(treeNode->right,cloneNode->right);
    
}
void restoreTreeLeftNode(RandNode* treeNode, RandNode* cloneNode){
    if(treeNode==NULL)
        return ;
    if(cloneNode->left){
        RandNode* orgNode = cloneNode->left;//B
        RandNode* cloneLeft = orgNode->left;
        treeNode->left  = treeNode->left->left;//go to the next node down..
        cloneNode->left = cloneLeft;//go to the next clone node down..
    }else{
        treeNode->left  = NULL;
    }
    restoreTreeLeftNode(treeNode->left,cloneNode->left);
    restoreTreeLeftNode(treeNode->right,cloneNode->right);

}
RandNode* cloneTree(RandNode* treeNode)
{
    if(treeNode==NULL)
        return NULL;
    RandNode* cloneNode = copyLeftRightNode(treeNode);
    copyRandPointers(treeNode,cloneNode);
    restoreTreeLeftNode(treeNode,cloneNode);
    
    return cloneNode;
}
void printInOrderRandNode(RandNode* node){
    if(node==NULL)
        return ;
    //LNR
    printInOrderRandNode(node->left);
    
    /* then print data of Node and its random */
    cout<<"["<<node->data<<" ";
    if(node->random==NULL)
        cout<<"NULL";
    else
        cout<<node->random->data;
    cout<<"], ";

    printInOrderRandNode(node->right);
}
void cloneTreeTestData(){
    /*
     Method 2 (Temporarily Modify the Given Binary Tree)

    1. Create new nodes in cloned tree and insert each new node in original tree between the left pointer edge of corresponding node 
     * in the original tree (See the below image).
    i.e. if current node is A and it’s left child is B ( A — >> B ), 
     * then new cloned node with key A wil be created (say cA) and it will be put as A — >> cA — >> B (B can be a NULL or a non-NULL left child). Right child pointer will be set correctly i.e. if for current node A, right child is C in original tree (A — >> C) then corresponding cloned nodes cA and cC will like cA —- >> cC

    Binary_Tree(1)

    2. Set random pointer in cloned tree as per original tree
    i.e. if node A’s random pointer points to node B, then in cloned tree, 
     * cA will point to cB (cA and cB are new node in cloned tree corresponding to node A and B in original tree)

    3. Restore left pointers correctly in both original and cloned tree
     
     */
    RandNode *tree = createRandNode(10);
    RandNode *n2 = createRandNode(6);
    RandNode *n3 = createRandNode(12);
    RandNode *n4 = createRandNode(5);
    RandNode *n5 = createRandNode(8);
    RandNode *n6 = createRandNode(11);
    RandNode *n7 = createRandNode(13);
    RandNode *n8 = createRandNode(7);
    RandNode *n9 = createRandNode(9);
    tree->left = n2;
    tree->right = n3;
    tree->random = n2;
    n2->left = n4;
    n2->right = n5;
    n2->random = n8;
    n3->left = n6;
    n3->right = n7;
    n3->random = n5;
    n4->random = n9;
    n5->left = n8;
    n5->right = n9;
    n5->random = tree;
    n6->random = n9;
    n9->random = n8;

    /*  Test No 7
        Node *tree = newNode(1);
        tree->left = newNode(2);
        tree->right = newNode(3);
        tree->left->random = tree;
        tree->right->random = tree->left;
     */
    cout << "Inorder traversal of original binary tree is: \n";
    printInOrderRandNode(tree);

    RandNode *clone = cloneTree(tree);

    cout << "\n\nInorder traversal of cloned binary tree is: \n";
    printInOrderRandNode(clone);
    
}
struct thbnode* leftMost(struct thbnode *root){
	
	while(root!=NULL&&root->left!=NULL)//root->left is left tree..
		root=root->left;
	return root;
}
void inorderUsingThreadedTree(struct thbnode* root){
	if(root==NULL)
		return ;
	else{
		struct thbnode* cur = leftMost(root);
		while(cur!=NULL){
		cout<<cur->data;//Left most
		if(cur->isThreaded)//threaded means, no right pointer, hence pointing to inorder successor
			cur=cur->right;// Traverse to inorder successor,means N
		else{
			cur=leftMost(cur->right);//find the left most in the right subtree.
		}
		}
		}
	
}
bool iterativeSearchLevelOrder(struct treeNode* root, int data){
    
    queue<struct treeNode *> que;
    que.push(root);
    while(!que.empty()){
        struct treeNode* curr = que.front();
        que.pop();
        if(curr->data==data)
            return true;
        if(curr->left)
            que.push(curr->left);
        if(curr->right)
            que.push(curr->right);
    
    }
    return false;

}
bool iterativeSearchPreOrder(struct treeNode* root,int data){
    
    stack<struct treeNode *> stak;
    stack.push(root);
    while(!stak.empty()){
        struct treeNode* curr = stak.top();
        stack.pop();
        if(curr->data==data)
            return true;
        if(curr->right)
            stack.push(curr->right);
        if(curr->left)
            stack.push(curr->left);
    }
    return false;

}
void LCATestData(){
    struct treeNode * root = createTreeNode(1);
    root->left = createTreeNode(2);
    root->right = createTreeNode(3);
    root->left->left = createTreeNode(4);
    root->left->right = createTreeNode(5);
    root->right->left = createTreeNode(6);
    root->right->right = createTreeNode(7);
    cout << "LCA(4, 5) = " << findLCA(root, 4, 5);
    cout << "\nLCA(4, 6) = " << findLCA(root, 4, 6);
    cout << "\nLCA(3, 4) = " << findLCA(root, 3, 4);
    cout << "\nLCA(2, 4) = " << findLCA(root, 2, 4);
}
void convertBinaryTreeToThreadedTreeTestData(){
		/*       1
            / \
           2   3
          / \ / \
         4  5 6  7   */
    struct thbnode *root = createThreadedNode(1);
    root->left = createThreadedNode(2);
    root->right = createThreadedNode(3);
    root->left->left = createThreadedNode(4);
    root->left->right = createThreadedNode(5);
    root->right->left = createThreadedNode(6);
    root->right->right = createThreadedNode(7);
	
	queue<struct thbnode *> q;
	populateInorderNodesInQueue(root,&q);
    convertBinaryTreeToThreadedTree(root,&q);
 
    cout << "Inorder traversal of creeated threaded tree is\n";
    inorderUsingThreadedTree(root);
}

void iterativeSearchTestData() {
    struct treeNode *NewRoot = NULL;
    struct treeNode *root = createTreeNode(2);
    root->left = createTreeNode(7);
    root->right = createTreeNode(5);
    root->left->right = createTreeNode(6);
    root->left->right->left = createTreeNode(1);
    root->left->right->right = createTreeNode(11);
    root->right->right = createTreeNode(9);
    root->right->right->left = createTreeNode(4);

    iterativeSearchLevelOrder(root, 6) ? cout << "Found\n" : cout << "Not Found\n";
    iterativeSearchPreOrder(root, 12) ? cout << "Found\n" : cout << "Not Found\n";
}
// Driver program to test above function
void binaryTreeTestData(){
//	verticalSuMBinaryTreeTestData();
//	LCATestData();
//convertBinaryTreeToThreadedTreeTestData();
    cloneTreeTestData();
}
