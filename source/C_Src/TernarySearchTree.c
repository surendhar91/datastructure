#include<TernarySearchTree.h>
#include<stdio.h>
//a ternary node must have five fields.
struct ternaryNode{
	char data; // used to store char
	unsigned isEndOfString : 1;
	struct ternaryNode* left;//left pointer
	struct ternaryNode* equal;//equal
	struct ternaryNode* right;//right pointer
};
//Data in ternary search tree, distributes over the node, unlike binary search tree.
/* Intro:
 * each node in a ternary search tree contains only 3 pointers:
1. The left pointer points to the node whose value is less than the value in the current node.
2. The equal pointer points to the node whose value is equal to the value in the current node.
3. The right pointer points to the node whose value is greater than the value in the current node.

	Apart from above three pointers, each node has a field to indicate data(character in case of dictionary) 
 * and another field to mark end of a string.
 * 
 * */
 
struct ternaryNode* createTernaryNode(char data){
  struct ternaryNode* node = (struct ternaryNode *)malloc(sizeof(struct ternaryNode));
  node->data=data;
  node->isEndOfString=0;
  node->left=node->right=node->equal=NULL;
  return node;
}
/* 
 * Applications of ternary search tree.
 * 1) Browser search suggestion.
 * 2) Finding the nearby neighbour words /find next word in dictionary, or the phone number starting with 9566
 *  Further, ternary search trees can be used any time a hashtable would be used to store strings.
 * 3) Used in spell checks, after having the user typed in a word in editor, the same will be validated 
 * 		for spell check using ternary search tree.
 * 4) Ternary search trees are efficient to use, when the strings to be stored share a common prefix.
 * */
void insertTernaryData(struct ternaryNode** root, char *word){
	//refer the image downloaded in the project url.
	if(!(*root)){//only once
		*root = createTernaryNode(*word);
	}
	if(*(word)<(*root)->data){
		insertTernaryData(&((*root)->left),word); //& is really important
		//traverse lefts
	}else if(*(word)>(*root)->data){
		insertTernaryData(&((*root)->right),word);
	}
	else{//if the created node reaches here, then it will be traversed to equal.
	//for example, CAT word
	/*	C will be created as root
	 *  check if first char of passed word is less, it's not else if it is greater, it's not
	 *  so goes to else, where if the next char exists, go to the equal part to insert data.
	 * 	
	 *  BUGS
	 *  
	 *  C is the root
	 *  B is less than root->data, so goes to left, thereafter creation of word proceeds.
	 * 
	 * */
		if(*(word+1)){
		//if the still more char is left to add, then add it to equal.
			insertTernaryData(&((*root)->equal),word+1);
		}else{//if the end of string is reached, then set end of string as 1. 
			(*root)->isEndOfString = 1;
		}
	}
}
int searchTernaryTree(struct ternaryNode* root,char *word){
	
	//to find BU in tree.
	if(!(root)){//if end of the tree is reached, but couldn't find return 0.
		return 0;
	}
	if(*(word)<root->data){//when b less than c
		searchTernaryTree(root->left,word);
	}else if(*(word)>root->data){
		searchTernaryTree(root->right,word);
	}else{//B equal to B
		if(*(word+1)){ //if the next char exists.
			searchTernaryTree(root->equal,word+1);
		}else{//when no more character exists, if end of string reached or not.
			return root->isEndOfString;
		}
	}
	
}
void traverseTernaryTree(struct ternaryNode* root,char *buffer,int depth){
	if(root){
		traverseTernaryTree(root->left,buffer,depth);//Left tree fully printed
		
		buffer[depth] = root->data;
		if(root->isEndOfString){
			buffer[depth+1]='\0';
			printf("%s",buffer);
		}
		traverseTernaryTree(root->equal,buffer,depth+1);//when equal means go forward to printing the word, advance the pointer
		//to print the data.
		
		traverseTernaryTree(root->right,buffer,depth);//right tree fully printed.
	}
}
void ternarySearchTreeTestData(){
	struct ternaryNode *root = NULL;
 
    insertTernaryData(&root, "cat");
    insertTernaryData(&root, "cats");
    insertTernaryData(&root, "up");
    insertTernaryData(&root, "bug");
 
    printf("Following is traversal of ternary search tree\n");
	char buffer[100];
    traverseTernaryTree(root,buffer,0);
 
    printf("\nFollowing are search results for cats, bu and cat respectively\n");
    searchTernaryTree(root, "cats")? printf("Found\n"): printf("Not Found\n");
    searchTernaryTree(root, "bu")?   printf("Found\n"): printf("Not Found\n");
    searchTernaryTree(root, "cat")?  printf("Found\n"): printf("Not Found\n");	
}