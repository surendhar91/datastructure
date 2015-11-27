#include <dynamicPrgm.h>
#include <stdio.h>
#include <BinaryTree.h>
int findMaxLIS(int arr[],int n){
	
	int *lis=(int *)malloc(sizeof(int)*n);
	int i,j,max=0;
	for(i=0;i<n;i++)
		lis[i]=1; //initialise the lis array set to 1
	for(i=1;i<n;i++){
		//Starting with the second element of the array
		for(j=0;j<i;j++){
			//for every i, j looping through from 0 to i.
			//L(i)= 1+max(L(j)) j looping from 0 to i such that arr[j]<arr[i], if there is no such j then return L(i) -> 1
			//for the longest subsequence to hold
			if(arr[j]<arr[i]&&lis[i]<lis[j]+1){
				lis[i]=lis[j]+1;
				}
			
			}
		}
	for(i=0;i<n;i++){
			if(max<lis[i])
				max=lis[i];
	}
	free(lis);
	return max;

}
void maxLisTestData(){
		int arr[] = { 3, 4, -1, 0, 6, 2, 3 };
		int n = sizeof(arr)/sizeof(arr[0]);
		printf("Length of LIS is %d\n", findMaxLIS( arr, n ) );
 
		getchar();
		return 0;
	
}
struct extBNode{
	int data;
	struct extBNode *left,*right;
	int temp;
};
struct extBNode* newExtBNode(int data)
{
    struct extBNode* temp = (struct extBNode *) malloc( sizeof(struct extBNode) );
    temp->data = data;
    temp->left = temp->right = NULL;
    temp->temp = 0;
    return temp;
}
int LargestIndepSetOfBinaryTree(struct bnode* root){
	//Largest independent set of binary tree - a subset of tree nodes is an independent set if there is no
	//edge between any two nodes of the subset.
	//example,consider the following tree
	/**
						10
				
				 20				30
				
			40		50				60
			 
			  
	 * 				70 80
	 * 
	 * The largest independent set is {10,40,60,70,80}, size of it is 5
	 * 
 * */
 
 /*	 1) optimial substructure
  * 	Let LISS(X) indicates size of largest independent set of a tree with root X.

     LISS(X) = MAX { (1 + sum of LISS for all grandchildren of X),
                     (sum of LISS for all children of X) }

	The idea is simple, there are two possibilities for every node X, 
	 * either X is a member of the set or not a member. 
 
	If X is a member, then the value of LISS(X) is 1 plus LISS of all grandchildren. 
	If X is not a member, then the value is sum of LISS of all children.
  * 
  * 2) Overlapping subproblem: recursive apporach, largest indep set of binary tree
	
	 * 	It should be noted that this function computes the same subproblems again and again. 
	 * For example, LISS of node with value 50 is evaluated for node with values 10 and 20 as 50 
	 * is grandchild of 10 and child of 20.
  * */
	if(root==NULL)
		return 0;
	int nodeExcl = LargestIndepSetOfBinaryTree(root->left)+LargestIndepSetOfBinaryTree(root->right);
	int nodeIncl = 1;
	if(root->left){
		nodeIncl += LargestIndepSetOfBinaryTree(root->left->left)+LargestIndepSetOfBinaryTree(root->left->right);
	}
	if(root->right){
		nodeIncl += LargestIndepSetOfBinaryTree(root->right->left)+LargestIndepSetOfBinaryTree(root->right->right);
	}
	return max(nodeIncl,nodeExcl);
	
}
int largestIndepSetOfBinaryTreeUsingDyn(struct extBNode* root){
	/* 
	 * here we make use of another variable temp to mark that this has been accounted for largest indep set of binary tree
	 * 
	 * We mark this node, only if it's already not set, removing the overlapping subproblem.
	 * 
	 * */
	if(root==NULL)
		return 0; 
	if(root->temp)
		return root->temp;
		
	if(root->left==NULL&&root->right==NULL)
		return (root->temp=1);//Count the leave node as 1
	
	int nodeExcl = largestIndepSetOfBinaryTreeUsingDyn(root->left)+largestIndepSetOfBinaryTreeUsingDyn(root->right);
	int nodeIncl = 1;
	if(root->left){
		nodeIncl += largestIndepSetOfBinaryTreeUsingDyn(root->left->left)+largestIndepSetOfBinaryTreeUsingDyn(root->left->right);
	}
	if(root->right){
		nodeIncl += largestIndepSetOfBinaryTreeUsingDyn(root->right->left)+largestIndepSetOfBinaryTreeUsingDyn(root->right->right);
	}
	return (root->temp=max(nodeExcl,nodeIncl));
}

void LissTestData(){
	 // Let us construct the tree given in the above diagram
    struct bnode *root         = newBNode(20);
    root->left                = newBNode(8);
    root->left->left          = newBNode(4);
    root->left->right         = newBNode(12);
    root->left->right->left   = newBNode(10);
    root->left->right->right  = newBNode(14);
    root->right               = newBNode(22);
    root->right->right        = newBNode(25);
	//using recursive apporach
    printf ("Size of the Largest Independent Set is %d ", LargestIndepSetOfBinaryTree(root));
	//using dynamic program approach
	 // Let us construct the tree given in the above diagram
    struct extBNode *root1         = newExtBNode(20);
    root1->left                = newExtBNode(8);
    root1->left->left          = newExtBNode(4);
    root1->left->right         = newExtBNode(12);
    root1->left->right->left   = newExtBNode(10);
    root1->left->right->right  = newExtBNode(14);
    root1->right               = newExtBNode(22);
    root1->right->right        = newExtBNode(25);
 
    printf ("Size of the Largest Independent Set is %d ", largestIndepSetOfBinaryTreeUsingDyn(root1));
}
void dynamicProgrammingTestData(){
	LissTestData();
	//maxLisTestData();
	
}
