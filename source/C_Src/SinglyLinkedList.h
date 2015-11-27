#ifndef SINGLE_H_
#define	SINGLE_H_
struct node{
	int data;
	struct node *next;
};
void insertAfter(struct node *prevNode,int data);
struct node* createNewNode(int data);
struct node* getNodeForData(struct node *head,int data);
void push(struct node **head_ref,int data);
void append(struct node **head_ref,int data);
void printList(struct node *head);
int getSizeOfTheList(struct node *head);
void printListInReverse(struct node *head);
void deleteNode(struct node **head,int data);
struct node* getNthNode(struct node *head,int n);
void deleteNodeGivenPointer(struct node *nodeToDelete);
struct node* findMiddleOfLinkedList(struct node *head);
struct node* findNthNodeFromTheEndOfList(struct node* head,int n);
void deleteLinkedList(struct node **head);
int findNumberOfTimesADataOccurred(struct node *head,int data);
void recursiveReverseList(struct node **head_ref);
struct node* detectLoop(struct node *head);
int verifyPalindromeByRecursion(struct node **left,struct node *right);
int compareLists(struct node *first,struct node *second);
int verifyPalindromeByReversingTheList(struct node *head);
void insertAnElementInSortedList(struct node **head_ref,struct node *element);
struct node* getIntesectionNode(struct node *list1,struct node *list2);
void removeDuplicatesFromSortedList(struct node *head);
void removeDuplicatesFromUnSortedList(struct node *head);
void moveLastElementToFront(struct node **head_ref);
void pairWiseSwap(struct node *head);
void swap(int *a,int *b);
struct node* getIntersectionOfSortedListsUsingReference(struct node *lilst1,struct node *list2);
struct node* getIntersectionOfSortedListsUsingDummy(struct node *list1,struct node *list2);
struct node * getIntersectionOfSortedListsByRecursion(struct node *list1,struct node *list2);
void deleteAlternateNodesOfLinkedList(struct node *head_ref);
struct node* deleteAlternativeLinkedListUsingRecursion(struct node *head);
void moveNodeAtTheFrontFromSourceToDest(struct node **dest_ref,struct node **source_ref);
void alternateNodeSplitsOfLinkedList(struct node *head,struct node **list1,struct node **list2);
void alternatingSplitsUsingDummy(struct node *head,struct node **list1,struct node **list2);
void alternatingSplitsUsingReference(struct node *head,struct node **list1,struct node **list2);
struct node* mergeSortedListsUsingDummyNode(struct node *a,struct node *b);
struct node* mergeSortedListsUsingReference(struct node *a,struct node *b);
struct node* mergeSortedListsUsingRecursion(struct node *a,struct node *b);
int listIdentical(struct node *list1,struct node *list2);
int listIdenticalUsingRecursive(struct node *list1,struct node *list2);
void mergeSortInLinkedLists(struct node **head_ref);
void splitListsInTwoHalves(struct node *head,struct node **list1,struct node **list2);
struct node* reverse(struct node *head,int k);
struct node* kAltReverse(struct node *head,int k);
void detectAndRemoveLoop(struct node* head);
void deleteNodesHavingGreaterValueOnRight(struct node **head_ref);
struct node* addTwoLinkedLists(struct node *list1,struct node *list2);
void deleteNodeWoHeadChange(struct node *head,struct node *data);
struct node * getUnion(struct node *list1,struct node *list2);
struct node * getIntersection(struct node *list1,struct node *list2);
int isPresent(struct node *head,int data);
int isTripletSumSorted(struct node *headA, struct node *headB, 
                 struct node *headC, int givenNumber);
#endif
/*
 * Best practice to link a node at the end using double pointer
 * head_ref=&((*head_ref)->next); FMI refer: getIntersectionOfSortedListsUsingReference
*/