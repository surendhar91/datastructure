#include<map>
#include<vector>
#include<stdlib.h>
#include<binaryTreeImp.h>
#include<queue>
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
		printf("%d ",cur->data);//Left most
		if(cur->isThreaded)//threaded means, no right pointer, hence pointing to inorder successor
			cur=cur->right;// Traverse to inorder successor,means N
		else{
			cur=leftMost(cur->right);//find the left most in the right subtree.
		}
		}
		}
	
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
// Driver program to test above function
void binaryTreeTestData(){
//	verticalSuMBinaryTreeTestData();
//	LCATestData();
convertBinaryTreeToThreadedTreeTestData();
}
