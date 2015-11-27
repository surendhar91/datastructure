#include<stdio.h>
#include<math.h>
#include<SegmentTree.h>
/*
 * Introduction to Segment Tree:
 * 
 * Find the sum of elements from index l to r where 0 <= l <= r <= n-1 2 Change value of a specified element of the array arr[i] = x where 0 <= i <= n-1.

	A simple solution is to run a loop from l to r and calculate sum of elements in given range. 
	 * To update a value, simply do arr[i] = x. The first operation takes O(n) time and second operation takes O(1) time.

	Another solution is to create another array and store sum from start to i at the ith index in this array. 
	 * Sum of a given range can now be calculated in O(1) time, but update operation takes O(n) time now. 
	 * This works well if the number of query operations are large and very few updates.

	
	 * What if the number of query and updates are equal? 
	 * Can we perform both the operations in O(log n) time once given the array? 
	 * 		We can use a Segment Tree to do both operations in O(Logn) time.
 * 
 * 
 * */
int getMid(int start,int end){
	return (start+(end-start)/2);
}
/*
 * Representation of Segment Tree:
 * 1) Leaf nodes are the elements of the input array
 * 2)Each internal node will be merging of leaf nodes, this merging differs across problem, but for this problem,
 * it is the sum of leaf node values
 * 
 * An array representation of segment tree is for an index i, left child will be in 2*i+1, 2*i-1 location
 * 
 * */	
int* constructSegTree(int arr[],int n){
/*
 * Construction of segment tree:-
 * 
 * 1) We start with a segment, divide it into two halves, we proceed on dividing the segments into two halves
 * and call the same procedure on these two halves.
 * just like heap tree, left subtree stored ini location 2*i+1 and right subtree in location 2*i+2
 * 2) As we move on, we store the sum in corresponding nodes.
 * 
 * All levels of the segment tree will be completely filled except for the last level.
 * A segment tree is also a full binary tree
 * 
 * Height of segment tree is log n. 
 * 
 * size of segment tree is 2*2^(log n)-1, where 2^(log n)-1 is the number of nodes in segment tree, and to 
 * maintain relation, we use 2*
 * 
 * 
 * */
 int height = (int)ceil(log2(n));
 int max_size = 2*(int)(pow(2,height))-1;
 int *segTree = malloc(sizeof(int)*max_size);
 constructSegTreeUtil(arr,0,n-1,segTree,0);
 return segTree;

}
int constructSegTreeUtil(int arr[],int segStart,int segEnd,int *segTree,int segIndex){
	//assume that you have 6 nodes
	//3rd node will be the root, 0-3 will be left subtree always, 4-6 will be right subtree.
	//This subdivision will be used everywhere.
	if(segStart==segEnd)//if the segment start and segment ends are equal
	{//construct the leaf node, just the array element.
		segTree[segIndex]=arr[segStart];
		return segTree[segIndex];
	}
	int mid = getMid(segStart,segEnd);
	//construct the sum of leaves to the internal node, ie merging takes place here
	segTree[segIndex]=constructSegTreeUtil(arr,segStart,mid,segTree,segIndex*2+1)+ // Left subtree, 2*i+1
						constructSegTreeUtil(arr,mid+1,segEnd,segTree,segIndex*2+2); //2*i+2 right subtree
						
	return segTree[segIndex];
	
	
} 
void updateValueOfArr(int arr[],int *segTree,int n,int i,int newVal){
	/*Like tree construction and query operations, update can also be done recursively. 
	 * We are given an index which needs to updated. Let diff be the value to be added. 
	 * We start from root of the segment tree, and add diff to all nodes which have given index in their range.
	If a node doesn’t have given index in its range, we don’t make any changes to that node.* 
	 * */
	if(i==0 || i>n-1){
		return ;//Base case for validating
	}
	int diff = newVal -arr[i];//new value will be greater
	arr[i] = newVal;
	//update the diff to subtrees, all along the path, takes log n time
	updateValueOfArrByAdjustSegTree(segTree,0,n-1,i,diff,0);
	
}
void updateValueOfArrByAdjustSegTree(int *segTree,int segStart,int segEnd,int i,int diffValue,int index){
	//updating segtree only if the node to be updated falls within the range of segstart, segend
	
	if(i<segStart||i>segEnd){//only if the i value is within range.
		return ;
	}
	
	segTree[index]=segTree[index]+diffValue;
	if(segStart!=segEnd){
		//if both are not equal
		int mid = getMid(segStart,segEnd);
		updateValueOfArrByAdjustSegTree(segTree,segStart,mid,i,diffValue,2*index+1);
		updateValueOfArrByAdjustSegTree(segTree,mid+1,segEnd,i,diffValue,2*index+2);
	}
	
	
}
int getSuMOfValuesFromQuery(int *segTree,int n,int queryStart,int queryEnd){
	if(queryStart<0||queryEnd>n-1||queryStart>queryEnd){
		return 0; //validating base case.
	}
	return getSuMOfValuesFromSegTree(segTree,0,n-1,queryStart,queryEnd,0);
}
int getSuMOfValuesFromSegTree(int *segTree,int segStart,int segEnd,int queryStart,int queryEnd,int index){
	
	//if the query start and query end is within segStart and segEnd of node, then return node's data
	// If segment of this node is a part of given range, then return the 
    // sum of the segment
	if(queryStart<=segStart&&queryEnd>=segEnd)
	{//segStart, segEnd [2,2] queryStart,queryEnd [1,3]
	//segStart ,segEnd [3,3] queryStart,queryEnd [1,3]
		return segTree[index];
	}
	if(segEnd<queryStart||segStart>queryEnd){//seg end greater than query
		//segStart, segEnd [0,0] queryStart, queryEnd [1,3] outside the range
		//segStart, segEnd [4,5], [4,4] queryStart, queryEnd [1,3] outside the range
		return 0;//outside the range.
	}
	int mid = getMid(segStart,segEnd);
	return getSuMOfValuesFromSegTree(segTree,segStart,mid,queryStart,queryEnd,2*index+1)+
			getSuMOfValuesFromSegTree(segTree,mid+1,segEnd,queryStart,queryEnd,2*index+2);
	
}
void printArrValues(int *st,int max_size){
		int i;
		for(i=0;i<max_size;i++){
			printf("%d\t",st[i]);
		}
}
void segTreeTestData(){
	int arr[] = {1, 3, 5, 7, 9, 11};
    int n = sizeof(arr)/sizeof(arr[0]);
 
    // Build segment tree from given array
    int *st = constructSegTree(arr, n);
	 printArrValues(st,15);
    // Print sum of values in array from index 1 to 3
    printf("Sum of values in given range = %d\n", getSuMOfValuesFromQuery(st, n, 1, 3));
 
    // Update: set arr[1] = 10 and update corresponding segment
    // tree nodes
    updateValueOfArr(arr, st, n, 1, 10);
	 printArrValues(st,15);

    // Find sum after the value is updated
    printf("Updated sum of values in given range = %d\n",
                                                  getSuMOfValuesFromQuery(st, n, 1, 3));
 
    return 0;
	
}