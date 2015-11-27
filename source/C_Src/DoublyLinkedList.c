#include <DoublyLinkedList.h>
#include <stdio.h>
//Circular linked list
struct DllNode* appendTwoDll(struct DllNode* a,struct DllNode *b){
		
	struct DllNode* aLast;
	struct DllNode* bLast;
	
	if(a==NULL)return b;//If a is null, then return b
	if(b==NULL) return a;//if b is null, then return a.
	
	aLast=a->prev; //Last node in a List
	bLast=b->prev; //Last node in b list
	
	joinDllNodes(aLast, b); //a last joining to b
	joinDllNodes(bLast, a);//b last joining to a.
	
	return a;
	
}
void joinDllNodes(struct DllNode* nodeA,struct DllNode *nodeB){
	nodeA -> next = nodeB;
	nodeB -> prev = nodeA;
}
void printDllList(struct DllNode* head){
	struct DllNode* top = head;
//	head=head->next;
	while(head!=NULL){//circular dll list.
		printf("%d  ",head->data);
		head=head->next;
		if(head==top)
			break;
	}
}