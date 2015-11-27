#include <SinglyLinkedList.h>
#include <stdio.h>
void push(struct node **head_ref,int data){		
	struct node *newNode=createNewNode(data);
	newNode->next=(*head_ref);
	(*head_ref)=newNode;
}
void insertAfter(struct node *prevNode,int data){
	struct node *newNode=createNewNode(data);
	newNode->next=prevNode->next;
	prevNode->next=newNode;
}
void append(struct node **head_ref,int data){
	struct node *last=(*head_ref);
	if(last==NULL){
			*head_ref=createNewNode(data);
			return ;
	}
	while(last->next!=NULL){
			last=last->next;
	}
	last->next=createNewNode(data);
	return ;
}
struct node* createNewNode(int data){
	struct node *newNode=malloc(sizeof(struct node));
	newNode->data=data;
	newNode->next=NULL;
	return newNode;
}
void printList(struct node *head){
	while(head!=NULL){
			printf("%d\t",head->data);
			head=head->next;
	}
}
struct node* getNodeForData(struct node *head,int data){
	while(head!=NULL){
			if(head->data==data){
				return head;	
			}
			head=head->next;
	}
	return NULL;
}
void deleteNode(struct node **head,int data){

	struct node *temp=*head;
	struct node *prev;
	
	if(temp!=NULL&&temp->data==data){
			(*head)=temp->next;
			free(temp);
			return ;
	}
	while(temp!=NULL&&temp->data!=data){
		    prev=temp;
			temp=temp->next;
	}
	if(temp==NULL){
	//Not found
		return; 	
	}
	prev->next=temp->next;
	free(temp);
	return ;
	
}
//Auxillary functions

struct node* getNthNode(struct node* head,int n){
		int count=0;
		while(head!=NULL){
				count++;
				if(count==n)
					return head;
				head=head->next;
		}
		return NULL;
}
//Given only a pointer to a node to be deleted in a singly linked list, how do you delete it?
void deleteNodeGivenPointer(struct node *nodeToDelete){
	//Simply coopy the next element's data
	struct node *temp=nodeToDelete->next;
	nodeToDelete->data=temp->data;
	nodeToDelete->next=temp->next;
	free(temp);
	return ;
		
}
struct node* findMiddleOfLinkedList(struct node *head){

		struct node *fastPtr,*slowPtr;
		fastPtr=slowPtr=head;
		while(fastPtr!=NULL&&fastPtr->next!=NULL){
			slowPtr=slowPtr->next;
			fastPtr=fastPtr->next->next;
		}
		return slowPtr;
}
struct node* findNthNodeFromTheEndOfList(struct node* head,int n){
		struct node *fastPtr,*slowPtr;
		fastPtr=slowPtr=head;
		int count=0;
		while(count<n&&fastPtr!=NULL){
				count++;
				fastPtr=fastPtr->next;
		}
		if(count!=n){
			printf("Nth node not found in the list");
			return NULL;
		}
		while(fastPtr!=NULL&&slowPtr!=NULL){
				fastPtr=fastPtr->next;
				slowPtr=slowPtr->next;
		}
		if(slowPtr)printf("%d is the %dth node from the end of list\n",slowPtr->data,n);
		return slowPtr;
}

void deleteLinkedList(struct node **head){
		struct node* currPtr=(*head);
		struct node* nextPtr=(*head);
		while(currPtr!=NULL){
				nextPtr=currPtr->next;
				free(currPtr);
				currPtr=nextPtr;
		}
		(*head)=NULL;
}
int findNumberOfTimesADataOccurred(struct node *head,int data){
		int count=0;
		while(head!=NULL){
				if(head->data==data){
					count++;
				}
				head=head->next;
		}
		return count;
}
void reverseLinkedList(struct node **head){
		struct node *prev=NULL;
		struct node *current=(*head);
		struct node *next;
		while(current!=NULL){
				next=current->next;
				current->next=prev;
				prev=current;
				current=next;
		}
		(*head)=prev;
}
void recursiveReverseList(struct node **head_ref){

	struct node *first,*rest;
	//empty list check
	if((*head_ref)==NULL)
		return;
	
	//divide the list
	first=(*head_ref);
	rest=first->next;
	
	recursiveReverseList(&rest);//note the rest 
	
	if(rest==NULL)//if has only one node
		return ;
	// 1->2->3->4  
	// 3->4 will be pointed as 4->3->NULL
	// 2->3->4 will be pointed as 4->3->2->NULL
	first->next->next=first;//two nodes
	first->next=NULL;
	
	//fixing the head
	//rest will always be the last node
	(*head_ref)=rest;
	
}
//To detect loop in the linked list
struct node* detectLoop(struct node *head){
	struct node *fastPtr,*slowPtr;
	fastPtr=slowPtr=head;
	while(fastPtr!=NULL&&fastPtr->next!=NULL){
			fastPtr=fastPtr->next->next;
			slowPtr=slowPtr->next;
			if(fastPtr==slowPtr){
					printf("loop detected");
					break;
			}
	}
	slowPtr=head;
	while(fastPtr!=NULL&&slowPtr!=NULL){
			if(fastPtr==slowPtr){
			  printf("Start of the loop %d",fastPtr->data);
			  break;
			}
			fastPtr=fastPtr->next;
			slowPtr=slowPtr->next;
	}
	return fastPtr;
}

int verifyPalindromeByRecursion(struct node **left,struct node *right){
		if(right==NULL)return 1;
		int isPal=verifyPalindromeByRecursion(left,right->next);
		if(isPal==0)
			return 0;
		int temp=0;
		if((*left)->data==right->data)
			 temp=1;
	    *left=(*left)->next;
		return temp;
}
int compareLists(struct node *first,struct node *second){
		while(first!=NULL&&second!=NULL){
				if(first->data!=second->data)
					return 0;
				first=first->next;second=second->next;
		}
		return 1;
}
int verifyPalindromeByReversingTheList(struct node *head){
	struct node *fastPtr,*slowPtr;
	struct node *prevOfSlowPtr,*midNode;
	midNode=NULL;
	fastPtr=slowPtr=(head);
	if((head)==NULL)
		return;
	while(fastPtr!=NULL&&fastPtr->next!=NULL){
			fastPtr=fastPtr->next->next;
			prevOfSlowPtr=slowPtr;
			slowPtr=slowPtr->next;
	}
	if(fastPtr!=NULL)//Only for odd number of  linked lists
	{
			midNode=slowPtr;
			slowPtr=slowPtr->next;
	}
	prevOfSlowPtr->next=NULL; //Ending the first half of the linked list
	struct node *secondHalf=slowPtr;
	reverseLinkedList(&secondHalf);
	int comparisonResult=compareLists(head,secondHalf);
	reverseLinkedList(&secondHalf);
	if(midNode!=NULL){
			prevOfSlowPtr->next=midNode;
			midNode->next=secondHalf;
	}else{
			prevOfSlowPtr->next=secondHalf;
	}
	
	return comparisonResult;
	
}
void sortedInsert(struct node **head_ref,struct node *element){

		if((*head_ref)==NULL||(*head_ref)->data>=element->data){
				element->next=(*head_ref);
				*head_ref=element;
				return ;
		}
		struct node *current=(*head_ref);
		while(current->next!=NULL&&current->next->data<=element->data){
				current=current->next;
		}
		element->next=current->next;
		current->next=element;
		
}
struct node* getIntesectionNode(struct node *list1,struct node *list2){
	//Get size of the two lists
		int list1Size=getSizeOfTheList(list1);
		int list2Size=getSizeOfTheList(list2);
		struct node *currentList=list1;
		struct node *secondList=list2;
		int diff=0;
		//Iterate through the diffs
		if(list1Size<list2Size){
			currentList=list2;
			secondList=list1;
			diff=list2Size-list1Size;
		}else{
			diff=list1Size-list2Size;
		}
		int i=0;
		while(i<diff){
				currentList=currentList->next;
				i++;
		}
		while(currentList!=NULL&&secondList!=NULL){
				if(currentList==secondList){
					break;
				}currentList=currentList->next;secondList=secondList->next;
		}
		return currentList;
}
int getSizeOfTheList(struct node *head){
		int count=0;
		while(head!=NULL){
				head=head->next;
				count++;
		}
		return count;
		
}
void printListInReverse(struct node *head){
		if(head==NULL)
			return ;
		printListInReverse(head->next);
		printf("%d\t",head->data);
}
void removeDuplicatesFromSortedList(struct node *head){
	while(head!=NULL){
		struct node* nextNode=head->next;
		 if(nextNode!=NULL&&head->data==nextNode->data){
		     	 head->next=nextNode->next;
				 free(nextNode);
		 }else{//Very very important
			head=head->next;
		 }
	}
}
void removeDuplicatesFromUnSortedList(struct node *head){
	
	struct node *firstList,*secondList,*temp,*prevNode;
	firstList=secondList=head;
	while(firstList!=NULL){
			secondList=firstList;
			while(secondList->next!=NULL){
					if(firstList->data==secondList->next->data){
							
							temp=secondList->next;
							secondList->next=secondList->next->next;
							free(temp);
							
					}else{
							secondList=secondList->next;
					}
				
			}
			firstList=firstList->next;
	}
}
void pairWiseSwap(struct node *head){
		
	if(head!=NULL&&head->next!=NULL){
			swap(&head->data,&head->next->data);
			pairWiseSwap(head->next->next);
	}
	
	
}
void swap(int *a,int *b){
		int temp=*a;
		*a=*b;
		*b=temp;
}
void moveLastElementToFront(struct node **head_ref){
		struct node *list=(*head_ref);
		struct node *prevNode;
		prevNode=NULL;
		while(list!=NULL&&list->next!=NULL){
				prevNode=list;
				list=list->next;
		}
		if(prevNode!=NULL){
			    //list=prevNode->next;
				prevNode->next=NULL;
				list->next=(*head_ref);
				(*head_ref)=list;
		}
	
}

struct node * getIntersectionOfSortedListsByRecursion(struct node *list1,struct node *list2){
	if(list1==NULL||list2==NULL)
		return NULL;
	if(list1->data<list2->data)
		return getIntersectionOfSortedListsByRecursion(list1->next,list2);
	if(list2->data<list1->data)
		return getIntersectionOfSortedListsByRecursion(list1,list2->next);
	struct node *temp=(struct node *)malloc(sizeof(struct node));
	temp->data=list1->data;
	temp->next=getIntersectionOfSortedListsByRecursion(list1->next,list2->next);
	return temp;
}
//To create a list at the tail of the node, we can use dummy node or reference strategy
struct node* getIntersectionOfSortedListsUsingReference(struct node *list1,struct node *list2){
	struct node **head_ref;
	struct node *result=NULL;
	head_ref=&result;
	while(list1!=NULL&&list2!=NULL){
			if(list1->data==list2->data){
					push(head_ref,list1->data);
					head_ref=&((*head_ref)->next);//Best practice to create a node at the end.
					list1=list1->next;
					list2=list2->next;
			}else if(list1->data<list2->data){
					list1=list1->next;
			}else{
					list2=list2->next;
			}
	}
	return result;
}
struct node* getIntersectionOfSortedListsUsingDummy(struct node *list1,struct node *list2){
	struct node dummy;//declare a dummy variable, this is head of the resulting list
	struct node *tail=&dummy;
	tail->next=NULL;
	while(list1!=NULL&&list2!=NULL){
			if(list1->data==list2->data){
				push((&tail->next),list1->data);//Important
				tail=tail->next;//Still pointing at dummy, hence move ahead.
				list1=list1->next;
				list2=list2->next;
			}else if(list1->data<list2->data){
					list1=list1->next;
			}else{
					list2=list2->next;
			}
	}//Because dummy has some node.
	return dummy.next;//Returning the list that starts.
}
void deleteAlternateNodesOfLinkedList(struct node *head_ref){
		if(head_ref==NULL||head_ref->next==NULL)
			return ;
		struct node *prev=head_ref;
		struct node *currentNode=(head_ref)->next;
		while(prev!=NULL&&currentNode!=NULL){
				prev->next=currentNode->next;
				free(currentNode);
				prev=prev->next;
				if(prev!=NULL){
					currentNode=prev->next;
				}
		}
}
struct node* deleteAlternativeLinkedListUsingRecursion(struct node *head){
		if(head==NULL)return NULL;
		struct node* currentNode;
		currentNode=head->next;
		if(currentNode==NULL)
			return NULL;
		head->next=currentNode->next;
		free(currentNode);
		deleteAlternativeLinkedListUsingRecursion(head->next);
}
void moveNodeAtTheFrontFromSourceToDest(struct node **dest_ref,struct node **source_ref){
		struct node *newNode=*source_ref;
		*source_ref=newNode->next;
		newNode->next=*dest_ref;
		*dest_ref=newNode;
}
void alternateNodeSplitsOfLinkedList(struct node *head,struct node **list1,struct node **list2){
		struct node *a,*b;
		a=NULL;b=NULL;
		while(head!=NULL){
				moveNodeAtTheFrontFromSourceToDest(&a,&head);
				if(head!=NULL){
						moveNodeAtTheFrontFromSourceToDest(&b,&head);
				}
		}
		*list1=a;
		*list2=b;
}
void alternatingSplitsUsingDummy(struct node *head,struct node **list1,struct node **list2){
		struct node adummy;
		struct node bdummy;
		struct node *atail=&adummy;
		atail->next=NULL;
		struct node *btail=&bdummy;
		btail->next=NULL;
		while(head!=NULL){
				moveNodeAtTheFrontFromSourceToDest(&(atail->next),&head);
				atail=atail->next;//Point to the tail always.
				if(head!=NULL){
				   moveNodeAtTheFrontFromSourceToDest(&(btail->next),&head);
				   btail=btail->next;
				}
		}
		*list1=adummy.next;
		*list2=bdummy.next;
	
}
void alternatingSplitsUsingReference(struct node *head,struct node **list1,struct node **list2){
		struct node **a,**b;
		struct node *resulta=NULL,*resultb=NULL;
		a=&resulta;
		b=&resultb;
		while(head!=NULL){
				moveNodeAtTheFrontFromSourceToDest(a,&head);
				a=&((*a)->next);
				if(head!=NULL){
				   moveNodeAtTheFrontFromSourceToDest(b,&head);
				   b=&((*b)->next);
				}
		}
		*list1=resulta;
		*list2=resultb;
	
}
struct node* mergeSortedListsUsingDummyNode(struct node *a,struct node *b){
		struct node dummy;
		struct node *tail=&dummy;
		tail->next=NULL;
		
				while(a!=NULL&&b!=NULL){
						if(a->data<=b->data){
							moveNodeAtTheFrontFromSourceToDest(&(tail->next),&a);
							tail=tail->next;
						}else{
							moveNodeAtTheFrontFromSourceToDest(&(tail->next),&b);
							tail=tail->next;
						}
				}
				while(a!=NULL){
					moveNodeAtTheFrontFromSourceToDest(&(tail->next),&a);
					tail=tail->next;
				}
				while(b!=NULL){
					moveNodeAtTheFrontFromSourceToDest(&(tail->next),&b);
					tail=tail->next;
				}
				
		return dummy.next;
}
struct node* mergeSortedListsUsingReference(struct node *a,struct node *b){
		struct node *result=NULL;
		struct node **listPtr=&result;
		while(a!=NULL&&b!=NULL){
			if(a->data<=b->data){
				moveNodeAtTheFrontFromSourceToDest(listPtr,&a);
			}else{
				moveNodeAtTheFrontFromSourceToDest(listPtr,&b);
			}
		   listPtr=&((*listPtr)->next);
		}
		while(a!=NULL){
		   moveNodeAtTheFrontFromSourceToDest(listPtr,&a);
		   listPtr=&((*listPtr)->next);
		}
		while(b!=NULL){
		   moveNodeAtTheFrontFromSourceToDest(listPtr,&b);
		   listPtr=&((*listPtr)->next);
		}
		return result;
}
struct node* mergeSortedListsUsingRecursion(struct node *a,struct node *b){
		struct node* result=NULL;
		if(a==NULL)
			return b;
		else if(b==NULL)return a;
		result=malloc(sizeof(struct node));
		if(a->data<b->data){
			result->data=a->data;
			result->next=mergeSortedListsUsingRecursion(a->next,b);
		}else{
			result->data=b->data;
			result->next=mergeSortedListsUsingRecursion(a,b->next);
		}
		return result;
}
int listIdentical(struct node *list1,struct node *list2){
	while(1){
		if(list1==NULL&&list2==NULL)
			return 1;
		if(list1==NULL&&list2!=NULL)
			return 0;
		if(list1!=NULL&&list2==NULL)
			return 0;
		if(list1->data!=list2->data)
			return 0;
			
		list1=list1->next;
		list2=list2->next;
	}
}
int listIdenticalUsingRecursive(struct node *list1,struct node *list2){
	if(list1==NULL&&list2==NULL)
			return 1;
		if(list1==NULL&&list2!=NULL)
			return 0;
		if(list1!=NULL&&list2==NULL)
			return 0;
		if(list1->data!=list2->data)
			return 0;
	return listIdenticalUsingRecursive(list1->next,list2->next);
	
}
void mergeSortInLinkedLists(struct node **head_ref){

	struct node *a,*b;
	if(head_ref==NULL||(*head_ref)->next==NULL){
			return ;
	}
	splitListsInTwoHalves(*head_ref,&a,&b);
	//printList(a);
	//printList(b);
	mergeSortInLinkedLists(&a);
	mergeSortInLinkedLists(&b);
	*head_ref=mergeSortedListsUsingRecursion(a,b);
	
}
void splitListsInTwoHalves(struct node *head,struct node **list1,struct node **list2){
	struct node *fastptr,*slowptr;
	struct node *prevOfSlowPtr;
	while(head==NULL||head->next==NULL){
		//if the lengh is less than 2
			*list1=head;
			*list2=NULL;
			return ;
	}
	//fastptr=head->next;
	fastptr=slowptr=head;
	while(fastptr!=NULL&&fastptr->next!=NULL){
			fastptr=fastptr->next->next;
			prevOfSlowPtr=slowptr;
			slowptr=slowptr->next;
	}
	*list1=head;
	*list2=slowptr;
	prevOfSlowPtr->next=NULL;
}
struct node* reverse(struct node *head,int k){
	struct node *prev,*current,*next;
	prev=next=NULL;
	current=head;
	int count=0;
	while(current!=NULL&&count<k){
		next=current->next;
		current->next=prev;
		prev=current;
		current=next;count++;
	}
	if(next!=NULL){
			head->next=reverse(next,k);
	}
	return prev;
	
}
struct node* kAltReverse(struct node *head,int k){

	struct node *prev,*current,*next;
	prev=next=NULL;
	current=head;
	int count=0;
	while(current!=NULL&&count<k){
		next=current->next;
		current->next=prev;
		prev=current;
		current=next;count++;
	}
	if(head!=NULL){
		head->next=current;//Point to k+1th node
	}
	count=0;//skip the next k node
	while(count<k-1&&current!=NULL){
			current=current->next;
			count++;
	}
	if(current!=NULL){
			current->next=kAltReverse(current->next,k);
	}
	return prev;
}
void deleteNodesHavingGreaterValueOnRight(struct node **head_ref){
	//reverse the linked list first and get the max value,
	//elements that are less than the max value shall be deleted
	/*
	 * 
	 * 1. Reverse the list.
2. Traverse the reversed list. Keep max till now. If next node < max, then delete the next node, otherwise max = next node.
3. Reverse the list again to retain the original order. 
	 * */
		reverseLinkedList(head_ref);
		struct node *head=*head_ref;
		struct node *prev,*next;
		if(head==NULL||head->next==NULL)
			return;
		prev=head;
		int max=head->data;
		while(head!=NULL){
				
				if(head->data<max){
					prev->next=head->next;
					free(head);
				}else{
					max=head->data;
					prev=head;
					head=head->next;
				}
				head=prev->next;
		}
		reverseLinkedList(head_ref);
		
}


void detectAndRemoveLoop(struct node* head){
	struct node* startNode=detectLoop(head);
	struct node* loopNode=startNode;
	while(loopNode->next!=startNode){
			loopNode=loopNode->next;
	}
	loopNode->next=NULL;
}
void segregateEvenAndOdd(struct node **head_ref){
	/*
	 * 
…1) Get pointer to the last node.
…2) Move all the odd nodes to the end.
……..a) Consider all odd nodes before the first even node and move them to end.
……..b) Change the head pointer to point to the first even node.
……..b) Consider all odd nodes after the first even node and move them to the end.
	 * */
	
	struct node *end=*head_ref;
	while(end->next!=NULL){
			end=end->next;
	}
	struct node *new_end=end;
	struct node *curr=*head_ref;
	while(curr!=end&&curr->data%2!=0){
			new_end->next=curr;
			curr=curr->next;
			new_end->next->next=NULL;
			new_end=new_end->next;
	}
	struct node *prev=NULL;
	if(curr->data%2==0){
		(*head_ref)=curr;
		
		while(curr!=end){
			if(curr->data%2==0){
				prev=curr;
				curr=curr->next;
			}else{
				prev->next=curr->next;//The odd node
				curr->next=NULL;//Set the next of odd node to null
				new_end->next=curr;//point new end next node to this odd node
				new_end=curr;//mark this new odd node as end of the list
				curr=prev->next;//Next move the current pointer to next
				//new_end->next->next=NULL;
			}
		}
	}else{
			prev=curr;
	}
	/* If there are more than 1 odd nodes and end of original list is
      odd then move this node to end to maintain same order of odd
      numbers in modified list */
	if(new_end!=end&&end->data%2!=0){//the odd node will be put towards the last.
		 prev->next=end->next;
		 end->next=NULL;
		 new_end->next=end;
		}
	}




struct node* addTwoLinkedLists(struct node *list1,struct node *list2){

		struct node *result=NULL,*prev;
		int carry=0,data,temp;
		while(list1!=NULL||list2!=NULL){//when or the other is true
			data=((list1)?list1->data:0)+((list2)?list2->data:0);
			temp=carry+data;
			data=temp%10;
			carry=temp/10;
			if(result==NULL)
			{
			 result=createNewNode(data);
			 prev=result;
			}else{
			  prev->next=createNewNode(data);
			  prev=prev->next;
			
			}
			if(list1)list1=list1->next;
			if(list2)list2=list2->next;
		}
		if(carry!=0)
			prev->next=createNewNode(carry);
		return result;
}

void deleteNodeWoHeadChange(struct node *head,struct node *data){
	
	struct node *n;
	if(head==data){
		head->data=head->next->data;
		n=head->next;
		head->next=head->next->next;
		free(n);
	    return ;
	}
	n=head->next;
	head=head->next;
	while(head!=NULL&&head!=data){
		n=head;
		head=head->next;
	}
	if(head!=NULL){
			//Element found
			printf("Element found.");
			n->next=head->next;
			free(head);
	}else{
		printf("Element not found");
	}
	
}
int isPresent(struct node *head,int data){
	while(head!=NULL){
			if(head->data==data){
					return 1;
			}
			head=head->next;
	}
	return 0;
	
}
struct node * getUnion(struct node *list1,struct node *list2){

		struct node* result=NULL;
		struct node *templ1,*templ2;
		templ1=list1;templ2=list2;
		while(list1!=NULL){
				push(&result,list1->data);
				list1=list1->next;
		}
		
		while(list2!=NULL){
			if(!isPresent(templ1,list2->data)){
				push(&result,list2->data);
			}
			list2=list2->next;
		}
		return result;
	
}

struct node * getIntersection(struct node *list1,struct node *list2){
		struct node* result=NULL;
		/*while(list1!=NULL){
				push(&result,list1->data);
				list1=list1->next;
		}*/
		
		while(list2!=NULL){
			if(isPresent(list1,list2->data)){
				push(&result,list2->data);
			}
			list2=list2->next;
		}
		return result;
	
}

int isTripletSumSorted(struct node *headA, struct node *headB, 
                 struct node *headC, int givenNumber){
			//Assuming head b and head c are sorted
			int currentData;
			while(headA!=NULL){
					struct node *b=headB;
					struct node *c=headC;
					while(b!=NULL&&c!=NULL){
						currentData=(b->data+c->data+headA->data);
						if(currentData==givenNumber){
							//Element found
							printf("Triplet found %d %d %d",headA->data,b->data,c->data);
							//once found return true.
							return 1;
						}else if(currentData<givenNumber){
							b=b->next;
						}else{
							c=c->next;
						}
						}
						headA=headA->next;
				}
					
				
}

void singlyLinkedListTestData(){
	
	struct node *head=NULL;
	int n;

	append(&head,9);
	
	//printList(head);
	push(&head,4);
	
	//printList(head);
	
	append(&head,7);
	append(&head,10);
	insertAfter(getNodeForData(head,4),3);//4 3 9 7 10
	//printList(head);
	//To delete a node
	//deleteNode(&head,3);
	
	//To get Nth Node
	//printf("%d",getNthNode(head,2)->data);
	
	//To delete a node given a pointer
	//deleteNodeGivenPointer(getNodeForData(head,3));
	
	//To find the middle node of the given linked list
	//printf("Middle of the linked list is %d\t",findMiddleOfLinkedList(head)->data);
	
	//To find Nth node from the end of the linked list
	
	//scanf("%d",&n);
	//findNthNodeFromTheEndOfList(head,n);
	
	//To delete the whole linked list
	//deleteLinkedList(&head);
	//if(head==NULL)printf("linked list deleted..");
	
	//To identify the number of times a int occurred
	//scanf("%d",&n);
	//printf("Number of times data %d occurred is %d",n,findNumberOfTimesADataOccurred(head,n));
	
	
	//printList(head);
	//printListInReverse(head);
	
	//To reverse a linked list using iterative approach.
	//reverseLinkedList(&head);
	//printList(head);
	
	//moveLastElementToFront(&head);
	//printList(head);
	
	//Recursively reverse the linked list by dividing
	//recursiveReverseList(&head);
	//printList(head);
	
	
	//To detect the loop in a linked list
	/*
	push(&head, 20);
	push(&head, 4);
	push(&head, 15);
	push(&head, 17);
 
	// Create a loop for testing 
	head->next->next->next->next = head->next;
	detectLoop(head);
	 * */
	//palindromeTestData();
	//sortedInsertionTestData();
	//detectIntersectionOfListsTestData();
	//removeDuplicatesFromSortedListTestData();
	//removeDuplicatesFromUnSortedListTestData();
	//pairWiseSwappingTestData();
	//getIntersectionOfSortedListsTestData();
	//deleteAlternateNodesTestData();
	//alternateSplitNodesTestData();
	//mergeSortedListsTestData();
	//listIdenticalTestData();
	//mergeSortTestData();
	//reverseListInGroupsTestData();
	//deleteLesserOnRightTestData();
	//segregateEvenAndOddTEstData();
	//detectAndRemoveLoopTestData();
	//addTwoLinkedListTestData();
	//deleteNodeTestData();
	//UnionAndIntersectionTestData();
	//isTripletEqualToGivenNumberTestData();
	return 0;
}
void palindromeTestData(){
	//To detect if the given linked list is a palindrome or not
	/*  
	 * A simple solution is to use a stack of list nodes. This mainly involves three steps.

 * 1) Traverse the given list from head to tail and push every visited node to stack.

 * 2) Traverse the list again. For every visited node, pop a node from stack and compare data of popped node with currently visited node.

 * 3) If all nodes matched, then return true, else false.
	
 * struct node *temp=NULL;
	char string[]="abacaba";
	int i;
	for(i=0;string[i]!='\0';i++){
			push(&temp,string[i]);
	}
	printList(temp);
	if(verifyPalindromeByRecursion(&temp,temp))
		printf("Given linked list is a palindrome..");
		 
 
	struct node *temp=NULL;
	char string[]="abacaba";
	int i;
	for(i=0;string[i]!='\0';i++){
			push(&temp,string[i]);
	}
	printList(temp);
	if(verifyPalindromeByReversingTheList(temp))
		printf("Given linked list is a palindrome..");
	
	 */
	
}
void sortedInsertionTestData(){
	/*	
	struct node *new_node = createNewNode(5);
    sortedInsert(&head, new_node);
    new_node = createNewNode(10);
    sortedInsert(&head, new_node);
    new_node = createNewNode(7);
    sortedInsert(&head, new_node);
    new_node = createNewNode(3);
    sortedInsert(&head, new_node);
    new_node = createNewNode(1);
    sortedInsert(&head, new_node);
    new_node = createNewNode(9);
    sortedInsert(&head, new_node);
    printf("\n Created Linked List\n");
    printList(head);*/
}
void detectIntersectionOfListsTestData(){
/*  struct node* newNode;
  struct node* head1 =
            (struct node*) malloc(sizeof(struct node));
  head1->data  = 10;
 
  struct node* head2 =
            (struct node*) malloc(sizeof(struct node));
  head2->data  = 3;
 
  newNode = (struct node*) malloc (sizeof(struct node));
  newNode->data = 6;
  head2->next = newNode;
 
  newNode = (struct node*) malloc (sizeof(struct node));
  newNode->data = 9;
  head2->next->next = newNode;
 
  newNode = (struct node*) malloc (sizeof(struct node));
  newNode->data = 15;
  head1->next = newNode;
  head2->next->next->next  = newNode;
 
  newNode = (struct node*) malloc (sizeof(struct node));
  newNode->data = 30;
  head1->next->next= newNode;
 
  head1->next->next->next = NULL;
 
  printf("\n The node of intersection is %d \n",
          getIntesectionNode(head1, head2)->data);
	
	 * */
	}
void removeDuplicatesFromSortedListTestData(){
	//struct node* head = NULL;
   
  /* Let us create a sorted linked list to test the functions
   Created linked list will be 11->11->11->13->13->20 */
  /*push(&head, 20);
  push(&head, 13);
  push(&head, 13); 
  push(&head, 11);
  push(&head, 11);
  push(&head, 11);                                   
 
  printf("\n Linked list before duplicate removal  ");
  printList(head);
 
 // Remove duplicates from linked list 
  removeDuplicatesFromSortedList(head);
 
  printf("\n Linked list after duplicate removal ");        
  printList(head); */         
	
	
}
void removeDuplicatesFromUnSortedListTestData(){
	//struct node *start = NULL;
 
  /* The constructed linked list is:
   10->12->11->11->12->11->10*/
 /* push(&start, 10);
  push(&start, 11);
  push(&start, 12);
  push(&start, 11);
  push(&start, 11);
  push(&start, 12);
  push(&start, 10);
 
  printf("\n Linked list before removing duplicates ");
  printList(start);
 
  removeDuplicatesFromUnSortedList(start);
 
  printf("\n Linked list after removing duplicates ");
  printList(start);*/
	
}
void pairWiseSwappingTestData(){
	
	//struct node *start = NULL;
 
  /* The constructed linked list is:
   1->2->3->4->5 */
  /*push(&start, 5);
  push(&start, 4);
  push(&start, 3);
  push(&start, 2);
  push(&start, 1);
 
  printf("\n Linked list before calling  pairWiseSwap() ");
  printList(start);
 
  pairWiseSwap(start);
 
  printf("\n Linked list after calling  pairWiseSwap() ");
  printList(start);*/
}
void getIntersectionOfSortedListsTestData(){
	/*
		 struct node* a = NULL;
  struct node* b = NULL;
  struct node *intersect = NULL;
  
  // Let us create the first sorted linked list to test the functions
   //Created linked list will be 1->2->3->4->5->6 
  push(&a, 6);
  push(&a, 5);
  push(&a, 4);
  push(&a, 3);
  push(&a, 2);
  push(&a, 1);                                  
  
  // Let us create the second sorted linked list
  // Created linked list will be 2->4->6->8 
  push(&b, 8);
  push(&b, 6);
  push(&b, 4);
  push(&b, 2);                                   
  
  // Find the intersection two linked lists 
  intersect = getIntersectionOfSortedListsUsingDummy(a, b);
  
  printf("\n Linked list containing common items of a & b \n ");
  printList(intersect);          
  */
}
void deleteAlternateNodesTestData(){
	/*int arr[100]; 
 
  // Start with the empty list 
  struct node* head = NULL;
 
  // Using push() to construct below list
    //1->2->3->4->5  
  push(&head, 5);
  push(&head, 4);
  push(&head, 3);
  push(&head, 2);
  push(&head, 1);   
 
  printf("\n List before calling deleteAlt() ");
  printList(head);
 
  deleteAlternativeLinkedListUsingRecursion(head);
 
  printf("\n List after calling deleteAlt() ");
  printList(head);	
*/
}
void alternateSplitNodesTestData(){
	/*
		// Start with the empty list 
  struct node* head = NULL;
  struct node* a = NULL;
  struct node* b = NULL; 
 
  //Let us create a sorted linked list to test the functions
   //Created linked list will be 0->1->2->3->4->5 
  push(&head, 5);
  push(&head, 4);
  push(&head, 3);
  push(&head, 2);
  push(&head, 1);                                   
  push(&head, 0); 
 
  printf("\n Original linked List:  ");
  printList(head);
 
  // Remove duplicates from linked list 
  alternatingSplitsUsingReference(head, &a, &b);
 
  printf("\n Resultant Linked List 'a' ");
  printList(a);           
 
  printf("\n Resultant Linked List 'b' ");
  printList(b);  */
}
void mergeSortedListsTestData(){
	
		/* Start with the empty list */
  struct node* res = NULL;
  struct node* a = NULL;
  struct node* b = NULL; 
 
  /* Let us create two sorted linked lists to test the functions
   Created lists shall be a: 5->10->15,  b: 2->3->20 */
  push(&a, 15);
  push(&a, 10);
  push(&a, 5);
   
  push(&b, 20);
  push(&b, 3);
  push(&b, 2); 
 
  /* Remove duplicates from linked list */
  res = mergeSortedListsUsingReference(a, b);
 
  printf("\n Merged Linked List is: \n");
  printList(res);
}
void listIdenticalTestData(){
  /*struct node *a = NULL;  
  struct node *b = NULL;    
 
  // The constructed linked lists are :
   //a: 3->2->1
   //b: 3->2->1 
  push(&a, 1);
  push(&a, 2);
  push(&a, 3);
   
  push(&b, 1);
  push(&b, 2);
  push(&b, 3); 
 
  if(listIdenticalUsingRecursive(a, b) == 1)
    printf(" Linked Lists are identical ");
  else
    printf(" Linked Lists are not identical ");*/
}
void mergeSortTestData(){
		  // Start with the empty list 
/*  struct node* res = NULL;
  struct node* a = NULL;
  
  // Let us create a unsorted linked lists to test the functions
  // Created lists shall be a: 2->3->20->5->10->15 
  push(&a, 15);
  push(&a, 10);
  push(&a, 5);
  push(&a, 20);
  push(&a, 3);
  push(&a, 2);
  
  // Sort the above created Linked List 
  mergeSortInLinkedLists(&a);
  
  printf("\n Sorted Linked List is: \n");
  printList(a);          
  */
}
void reverseListInGroupsTestData(){
/*
		 // Start with the empty list 
    struct node* head = NULL;
  
     // Created Linked list is 1->2->3->4->5->6->7->8 
     push(&head, 8);
     push(&head, 7);
     push(&head, 6);
     push(&head, 5);
     push(&head, 4);
     push(&head, 3);
     push(&head, 2);
     push(&head, 1);          
 
     printf("\n Given linked list \n");
     printList(head);
     head = kAltReverse(head, 2);
 
     printf("\n Reversed Linked list \n");
     printList(head);
 */
	
}
void deleteLesserOnRightTestData(){
/*
 struct node *head = NULL;
 
    // Create following linked list
     // 12->15->10->11->5->6->2->3 
    push(&head,60);
    push(&head,50);
    push(&head,40);
    push(&head,30);
    push(&head,20);
    push(&head,10);
    //push(&head,15);
    //push(&head,12);
 
    printf("Given Linked List: ");
    printList(head);
 
    deleteNodesHavingGreaterValueOnRight(&head);
 
    printf("\nModified Linked List: ");
    printList(head);
 */
 }
 void detectAndRemoveLoopTestData(){
 /*
	
    struct node* head = NULL;
 
    push(&head, 10);
    push(&head, 4);
    push(&head, 15);
    push(&head, 20);
    push(&head, 50);
 
    head->next->next->next->next->next = head->next->next;
 
    detectAndRemoveLoop(head);
 
    printf("Linked List after removing loop \n");
    printList(head);
 */
	 
}
void segregateEvenAndOddTEstData(){
		/* Start with the empty list 
    struct node* head = NULL;
 
     Let us create a sample linked list as following
      0->2->4->6->8->10->11 
 
    push(&head, 6);
    push(&head, 1);
    push(&head, 4);
    push(&head, 5);
    push(&head, 10);
    push(&head, 12);
    push(&head, 8);
 
    printf("\n Original Linked list ");
    printList(head);
 
	segregateEvenAndOdd(&head);
    printf("\n Modified Linked list ");
    printList(head);*/
}
void addTwoLinkedListTestData(){
  /*  struct node* res = NULL;
    struct node* first = NULL;
    struct node* second = NULL;
 
    // create first list 7->5->9->4->6
    push(&first, 6);
    push(&first, 4);
    push(&first, 9);
    push(&first, 5);
    push(&first, 7);
    printf("First List is ");
    printList(first);
	
    // create second list 8->4
    push(&second, 4);
    push(&second, 8);
	
    printf("Second List is ");
    printList(second);
 
    // Add the two lists and see result
    res = addTwoLinkedLists(first, second);
	
    printf("Resultant list is ");
    printList(res);
 */
	
}
void deleteNodeTestData(){
	
//	struct node *head = NULL;
 
    /* Create following linked list
      12->15->10->11->5->6->2->3 
    push(&head,3);
    push(&head,2);
    push(&head,6);
    push(&head,5);
    push(&head,11);
    push(&head,10);
    push(&head,15);
    push(&head,12);
 
    printf("Given Linked List: ");
    printList(head);
	*/
    /* Let us delete the node with value 10 
    printf("\nDeleting node %d: ", head->next->next->data);
    deleteNodeWoHeadChange(head, head->next->next);
 
    printf("\nModified Linked List: ");
    printList(head);*/
 
    /* Let us delete the the first node 
    printf("\nDeleting first node ");
    deleteNodeWoHeadChange(head, head);
 
    printf("\nModified Linked List: ");
    printList(head);
 
    getchar();
    return 0;
	*/
}
void UnionAndIntersectionTestData(){
		 /* Start with the empty list
    struct node* head1 = NULL;
    struct node* head2 = NULL;
    struct node* intersecn = NULL;
    struct node* unin = NULL;
  */
    /*create a linked lits 10->15->5->20 
    push (&head1, 20);
    push (&head1, 4);
    push (&head1, 15);
    push (&head1, 10);*/
 
    /*create a linked lits 8->4->2->10 
    push (&head2, 10);
    push (&head2, 2);
    push (&head2, 4);
    push (&head2, 8);
 
    intersecn = getIntersection (head1, head2);
    unin = getUnion (head1, head2);
 
    printf ("\n First list is \n");
    printList (head1);
 
    printf ("\n Second list is \n");
    printList (head2);
 
    printf ("\n Intersection list is \n");
    printList (intersecn);
 
    printf ("\n Union list is \n");
    printList (unin);
 
    return 0;*/
}
void isTripletEqualToGivenNumberTestData(){
	
  /* Start with the empty list 
    struct node* headA = NULL;
    struct node* headB = NULL;
    struct node* headC = NULL;
 */
    /*create a linked list 'a' 10->15->5->20 
    push (&headA, 20);
    push (&headA, 4);
    push (&headA, 15);
    push (&headA, 10);
 */
    /*create a sorted linked list 'b' 2->4->9->10 
    push (&headB, 10);
    push (&headB, 9);
    push (&headB, 4);
    push (&headB, 2);*/
 
    /*create another sorted linked list 'c' 8->4->2->1 
    push (&headC, 1);
    push (&headC, 2);
    push (&headC, 4);
    push (&headC, 8);
 
    int givenNumber = 25;
 
    isTripletSumSorted(headA, headB, headC, givenNumber);
	
	return 0;*/
 	
}