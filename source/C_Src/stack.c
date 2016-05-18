#include"DoublyLinkedList.h"
#include"stack.h"
#include<stdio.h>
struct Stack{
		int *arr;
		unsigned capacity;
		int top;
};
struct twoStack{
		int *arr;
		int top1,top2;
		unsigned capacity;
};
struct twoStack* createTwoStack(unsigned capacity){
		struct twoStack* twostack=(struct twoStack*)malloc(sizeof(struct twoStack));
		twostack->top2=capacity;
		twostack->top1=-1;
		twostack->capacity=capacity;
		twostack->arr=(int *)malloc(sizeof(int)*capacity);
		return twostack;
}
struct Stack* createStack(unsigned capacity){
		struct Stack* stack=(struct Stack*)malloc(sizeof(struct Stack));
		stack->capacity=capacity;
		stack->top=-1;
		stack->arr=(int *)malloc(sizeof(int)*stack->capacity);
		return stack;
}
int isFull(struct Stack* stack){
		return stack->top==(stack->capacity-1);
}
int isEmpty(struct Stack* stack){
		return stack->top==-1;
}
void pushStack(struct Stack* stack,int data){
		if(isFull(stack))
			return ;
		stack->arr[++stack->top]=data;
		//printf("%d pushed to Stack\n",stack->arr[stack->top]);
}
int pop(struct Stack* stack){
		if(isEmpty(stack)){
				printf(" Empty stack ");
				return 0;
		}
		return stack->arr[stack->top--];
		//printf("%d element popped", );
}
int peek(struct Stack* stack){
		if(!isEmpty(stack)){
			return stack->arr[stack->top];
		}
}
int prec(char op){
		switch(op){
			case '+':
			case '-':
				return 1;
			case '*':
			case '/':
				return 2;
			case '^':
				return 3;
		}
		return -1;
}
int isOperand(char op){
		return (op>='a'&&op<='z')||(op>='A'&&op<='Z');
}
int infixToPostfix(char *exp){
	int i,k;
	struct Stack* stack=createStack(strlen(exp));
	//char output[]=malloc(sizeof(char)*exp)
	for(i=0,k=0;exp[i];i++){
		if(isOperand(exp[i]))
			exp[k++]=exp[i];//If operand output as it is.
		else{
			if(exp[i]=='('){
				pushStack(stack,exp[i]);
			}else if(exp[i]==')'){
				
				while(peek(stack)!='('&&!isEmpty(stack)){
					exp[k++]=pop(stack);
				}
				if(!isEmpty(stack)&&peek(stack)!='('){
					return -1;//This check ensures, the top element is '(', if it is then pop the stack.
				}else{
					pop(stack);//remove the '(' element.
				}
				
			}else {
					while(!isEmpty(stack)&&prec(exp[i])<=prec(peek(stack))){
					//if the precedence of incoming character is lower than the top element, then pop
						exp[k++]=pop(stack);
					//only higher can be placed.
					//imagine pile of plates, only large sized plates can be placed over the smaller ones.
					}
					pushStack(stack,exp[i]);//then place the lower element..
			}
					
			
			
		}
	}
	while(!isEmpty(stack)){
		exp[k++]=pop(stack);
	}
	exp[k]='\0';
	printf("%s",exp);
	
}
int evaluatePostFix(char *exp){
	int i;
	struct Stack* stack=createStack(strlen(exp));
	for(i=0;exp[i];i++){
		if(isdigit(exp[i])){
			pushStack(stack,exp[i]-'0');
		}else{
			int a=pop(stack);
			int b=pop(stack);
			int result;
			switch(exp[i]){
				case '+':result=a+b;
						  break;
				case '-':result=a-b;
						  break;
				case '*':result=a*b;
						  break;
				case '/':result=a/b;
						  break;
				default:break;
			}
			pushStack(stack,result);
		}
		
	}
	return pop(stack);
}
void reverseStringByStack(char str[]){
	int i;
	struct Stack *stack=createStack(strlen(str));
	for(i=0;str[i];i++){
		pushStack(stack,str[i]);
	}
	for(i=0;!isEmpty(stack);i++){
	    str[i]=pop(stack);
	}
	
}
void swapStr(char *a,char *b){
	char temp=*a;
	*a=*b;
	*b=temp;
}
void reverseNormally(char str[]){
	int i,n=strlen(str);
	//printf("%d -> n",n);
	for(i=0;i<n/2;i++){
		swapStr(&str[i],&str[n-i-1]);
	}
}
void pushTwoStack1(struct twoStack* twostack,int data){
		if(twostack->top1<twostack->top2-1){
			//when atleast one data is available
			twostack->arr[++twostack->top1]=data;
		}
}
void pushTwoStack2(struct twoStack* twostack,int data){
		if(twostack->top1<twostack->top2-1){
			//when atleast one data is available
			twostack->arr[--twostack->top2]=data;
		}
}
int popTwoStack1(struct twoStack* twostack){
		if(twostack->top1>=0){
			return twostack->arr[twostack->top1--];	
		}
		return -1;
}
int popTwoStack2(struct twoStack* twostack){
		if(twostack->top2<twostack->capacity){
			return twostack->arr[twostack->top2++];	
		}
		return -1;
}
int isMatchingPair(char character1,char character2){
		if(character2=='{'&&character1=='}'){
				return 1;
		}else if(character2=='('&&character1==')'){
				return 1;
		}else if(character2=='['&&character1==']'){
				return 1;
		}else{
				return 0;
		}
}
int areParenthesisBalanced(char *exp){
		
	int i;
	struct Stack* stack=createStack(strlen(exp));
	for(i=0;exp[i];i++){
		if(exp[i]=='{'||exp[i]=='['||exp[i]=='('){
			pushStack(stack,exp[i]);
		}else if(exp[i]=='}'||exp[i]==']'||exp[i]==')'){
			
			if(!isMatchingPair(exp[i],pop(stack))){
				return 0;
			}
			
		}
	}
	if(isEmpty(stack)){
			return 1;
	}else{
			return 0;
	}
}
void findNextGreaterElement(int arr[],int  n){
    /*
     Given an array, print the Next Greater Element (NGE) for every element. The Next greater Element for an element x is the first greater element on the right side of x in array. Elements for which no greater element exist, consider next greater element as -1.

    Examples:
    a) For any array, rightmost element always has next greater element as -1.
    b) For an array which is sorted in decreasing order, all elements have next greater element as -1.
    c) For the input array [4, 5, 2, 25}, the next greater elements for each element are as follows.

Element       NGE
   4      -->   5
   5      -->   25
   2      -->   25
   25     -->   -1

d) For the input array [13, 7, 6, 12}, the next greater elements for each element are as follows.

  Element        NGE
   13      -->    -1
   7       -->     12
   6       -->     12
   12     -->     -1

     * Algorithm:-
 1) Push the first element to stack.
2) Pick rest of the elements one by one and follow following steps in loop.
….a) Mark the current element as next.
….b) If stack is not empty, then pop an element from stack and compare it with next.
….c) If next is greater than the popped element, then next is the next greater element for the popped element.
….d) Keep popping from the stack while the popped element is smaller than next. next becomes the next greater element for all such popped elements
….g) If next is smaller than the popped element, then push the popped element back.
3) After the loop in step 2 is over, pop all the elements from stack and print -1 as next element for them.
     
     */
	//Finds the next greater element using stack
	int i,next,element;
	struct Stack* stack=createStack(n);
	for(i=0;i<n;i++){
		next=arr[i];//next is the element in the current loop
		if(!isEmpty(stack)){
			//Before pushing the current element
			element=pop(stack);
			//check if the element in the stack is less than the element yet to push
			while(element<next){
				printf("%d-- %d",element,next);
				if(isEmpty(stack)){
					break;
				}
				element=pop(stack);
			}
			if(element>next){
				//if supposedly, the element popped is greater than the yet to push element.
				//push it back to the stack.
				pushStack(stack,element);
			}
		}
		
		//push the element so that we can find greater for it as well.
		pushStack(stack,next);
		
		
	}
	while(!isEmpty(stack)){//for the rest, there is no next greater element.
			printf("%d--%d",pop(stack),-1);
	}
	
}
void printStack(struct Stack stack){
		while(!isEmpty(&stack)){
			printf("%d\n",pop(&stack));
		}
}
void insertAtBottom(struct Stack **stack,int data){
	if(isEmpty(*stack)){
		//first 4 will be added to stack.
		pushStack(*stack,data);
	}else{
		int temp=pop(*stack);//4
		insertAtBottom(stack,data);//3 will be inserted to the stack.
		pushStack(*stack,temp);//4 will be pushed to the stack.
	}
}
void reverseStack(struct Stack **stack){
	// Assume a stack of 1 2 3 4
	if(!isEmpty(*stack)){
		int temp=pop(*stack);
		reverseStack(stack);
		//on end of iteration, data passed is 4
		//data passed will be 3.
		insertAtBottom(stack,temp);
	}
}
void calculateSpan(int arr[],int n,int s[]){
	
	//Find the previous greater element - only smaller should be placed over the larger.
	//http://www.geeksforgeeks.org/the-stock-span-problem/
		struct Stack* stack=createStack(n);
		pushStack(stack,0);//push the index 0 first.
		s[0]=1; // First element's span is always 1
		int i;
		for(i=1;i<n;i++){//Start from 1, because arr[0] is already pushed.
			while(!isEmpty(stack)&&arr[peek(stack)]<arr[i]){//if top of the stack's element is less than
			//the current element, we are iterating
				pop(stack);			
			}
			s[i]=isEmpty(stack)?(i+1):(i-peek(stack));
			pushStack(stack,i);
		}
}
void printArray(int arr[],int n){
		int i;
		for(i=0;i<n;i++){
			printf("%d ",arr[i]);
		}
}
//To make use of special stack, that contains min and main elements
struct specialStack{
	struct Stack *mainStack;
	struct Stack *minStack;
};
void specialStackPush(struct specialStack *splStack,int data){
	if(isEmpty(splStack->mainStack)){
	   pushStack(splStack->mainStack,data);
	   pushStack(splStack->minStack,data);
	}
	else{
		pushStack(splStack->mainStack,data);
		if(data<peek(splStack->minStack)){
			pushStack(splStack->minStack,data);
		}
	}
}
void specialStackPop(struct specialStack *splStack){
	if(!isEmpty(splStack->mainStack)){
		if(peek(splStack->mainStack)==peek(splStack->minStack)){
//			pop(splStack->mainStack);
			pop(splStack->minStack);
		}
		pop(splStack->mainStack);
	}
}
//to find the middle element in a stack.

struct DllStack{
	struct DllNode *head; //top
	struct DllNode *mid;
	int count;
};
void DllStackPush(struct DllStack* stack,int data){
	struct DllNode* newDllNode=(struct DllNode *)malloc(sizeof(struct DllNode));
	newDllNode->data=data;
//	newDllNode->prev=stack->head;
	newDllNode->prev=NULL;	
	newDllNode->next=stack->head;
	if(stack->head){
	  stack->head->prev=newDllNode;	
	}
	stack->head=newDllNode;
	stack->count+=1;
	if(stack->count==1){
			//	set the mid pointer of dll stack
			stack->mid=newDllNode;
	}else{
		//only for the odd nodes.
		if(stack->count & 1){
			stack->mid=stack->mid->prev;//goes back
		}
	}
}
int DllStackPop(struct DllStack* stack){
struct DllNode* node=stack->head;
	if(stack->head){//head is not null, update the prev of head
			stack->head=stack->head->next;
			stack->head->prev=NULL;
	}
	stack->count-=1;
	//for the even nodes.
	if(!((stack->count)&1)){
			stack->mid=stack->mid->next;
	}
	int poppedData=node->data;
	free(node);
	return poppedData;
}
int findMiddleDllStack(struct DllStack* stack){
		if(stack->mid)
			return stack->mid->data;
}
//push and pop operations of k stacks
/*
 * The idea is to use two extra arrays for efficient implementation of k stacks in an array. 
 * This may not make much sense for integer stacks, but stack items can be large for example stacks of employees, students, etc where every item is of hundreds of bytes. For such large stacks, the extra space used is comparatively very less as we use two integer arrays as extra space.

Following are the two extra arrays are used:
 * 
1) top[]: This is of size k and stores indexes of top elements in all stacks.
 * 
2) next[]: This is of size n and stores indexes of next item that is free for the other item to push in array arr[].
in pop 
Here arr[] is actual array that stores k stacks.
Together with k stacks, a stack of free slots in arr[] is also maintained. 
 * The top of this stack is stored in a variable ‘free’.

All entries in top[] are initialized as -1 to indicate that all stacks are empty.
 All entries next[i] are initialized as i+1 because all slots are free initially and pointing to next slot. 
  * Top of free stack, ‘free’ is initialized as 0.
 * 
 * */
struct kStack{
	int *arr;
	int *next;
	int *top;
	int n;
	int k;
	int free; //Next free slot in the array.
};
//Using of Kstack in the most time efficient and space efficient implementation

struct kStack* createKStack(int k,int n){
	struct kStack* kstack=(struct kStack*)malloc(sizeof(struct kStack));
	kstack->n=n;
	kstack->k=k;
	kstack->arr=malloc(sizeof(int)*kstack->n);
	kstack->next=malloc(sizeof(int)*kstack->n);
	kstack->top=malloc(sizeof(int)*kstack->k);
	int i;
	for(i=0;i<n-1;i++){
			kstack->next[i]=i+1;
			//if i=1, then next slot 2 is free to occupy
	}
	kstack->next[n-1]=-1;//n-1 is full
	kstack->free=0;//0 index is the first free slot to occupy.
	for(i=0;i<k;i++){
		kstack->top[i]=-1;//Initialize top of k stacks to be -1
	}
	return kstack;
	
}
void pushKStack(struct kStack* kstack,int sn,int data){
		//sn -> Stack Number
    
	if(kstack->free==-1){
			printf("Stack overflow..");
			return;
	}
	int i=kstack->free;//free slot
	
	kstack->free=kstack->next[i]; //Set the free to next index.
	
	kstack->next[i]=kstack->top[sn];//Setting the next element [ of free slot ] to previous top
	//I.e next of i would point to previous top
	
	kstack->top[sn]=i;// Set the top of current stack number to this free slot.
	
	kstack->arr[i]=data;
//	for example, consider the first free slot is 0,
// next[0] is 1, i.e slot no 1 is free to occupy next, hence setting the free to slot no 1
// now next[0] should be set to the previous top, so that on pop same can be used.
// top of this stack will be i [free slot -> 0]
// put the data to arr[0]
	
	

}

int popKStack(struct kStack* kstack, int sn){
	// sn -> stack number
	int i;
	
	if(kstack->top[sn]==-1){
		printf("Empty stack..");
		return ;
	}
	i=kstack->top[sn]; // get the top of the current stack
	
	kstack->top[sn]=kstack->next[i];//set to the previous top element. [as used in push operation]
	
	kstack->next[i]=kstack->free;//the next free element is the current free element.
	
	kstack->free=i; // the top of the stack is now free to hold.
	
	
	return kstack->arr[i];
}

void kStackTestData(){
	int k = 3, n = 10;
    struct kStack* kstack=createKStack(k,n); 
    // Let us put some items in stack number 2
    pushKStack(kstack,2, 15);
    pushKStack(kstack,2, 45);
 
    // Let us put some items in stack number 1
	pushKStack(kstack,1,17);
	pushKStack(kstack,1,49);
	pushKStack(kstack,1,39);
    // Let us put some items in stack number 0
     pushKStack(kstack,0, 11);
	 pushKStack(kstack,0,9);
	 pushKStack(kstack,0,7);
	 
printf("popped element from stack 1 is %d",popKStack(kstack,1));
	 printf("\npopped element from stack 2 is %d ",popKStack(kstack,2));
	 printf("\npopped element from stack 0 is %d",popKStack(kstack,0));
	
}
/*
 * 
 * 
 * Start of Test Data..
 * 
 * */
void mergeableStackTestData(){

/*
 * 
 * if we use array implementation of stack, then merge is not possible to do in O(1) time as we have to do following steps.
a) 
 * Delete old arrays
b) 
 * Create a new array for s1 with size equal to size of old array for s1 plus size of s2.
c) 
 * Copy old contents of s1 and s2 to new array for s1

 * The above operations take O(n) time.


 * We can use a linked list with two pointers, one pointer to first node (also used as top when elements are added and removed from beginning). The other pointer is needed for last node so that we can quickly link the linked list of s2 at the end of s1. Following are all operations.
a) 
 * push(): Adds the new item at the beginning of linked list using first pointer.
b) 
 * pop(): Removes an item from beginning using first pointer.
c) 
 * merge(): Links the first pointer second stack as next of last pointer of first list.


 * Can we do it if we are not allowed to use extra pointer?

 * We can do it with circular linked list. The idea is to keep track of last node in linked list. The next of last node indicates top of stack.
a) 
 * push(): Adds the new item as next of last node.
b) 
 * pop(): Removes next of last node.
c) 
 * merge(): Links the top (next of last) of second list to the top (next of last) of first list. And makes last of second list as last of whole list.
*/
}
void createStackUsingQueues() {
    /*
     push(s, x) 
     * // x is the element to be pushed and s is stack
      1) Enqueue x to q2
      2) One by one dequeue everything from q1 and enqueue to q2.
      3) Swap the names of q1 and q2 
     * // Swapping of names is done to avoid one more movement of all elements 
     * // from q2 to q1. 

    pop(s)
      1) Dequeue an item from q1 and return it.
 */

}
void dllStackTestData(){
	struct DllStack *ms = (struct DllStack*)malloc(sizeof(struct DllStack));
	ms->count=0;
    DllStackPush(ms, 11);
    DllStackPush(ms, 22);
    DllStackPush(ms, 33);
    DllStackPush(ms, 44);
    DllStackPush(ms, 55);
    DllStackPush(ms, 66);
    DllStackPush(ms, 77);
 
    printf("Item popped is %d\n", DllStackPop(ms));
    printf("Item popped is %d\n", DllStackPop(ms));
    printf("Middle Element is %d\n", findMiddleDllStack(ms));
}
void specialStackTestData(){
	struct specialStack *splStack=malloc(sizeof(struct specialStack));
	splStack->mainStack=createStack(5);
	splStack->minStack=createStack(5);
	specialStackPush(splStack,10);
	specialStackPush(splStack,20);
	specialStackPush(splStack,30);
    printf("Min element is %d",peek(splStack->minStack));
//	printStack(*(splStack->minStack));
    specialStackPush(splStack,5);
//	printStack(*(splStack->minStack));
	printf("Min element is %d",peek(splStack->minStack));
}
void reverseStackTestData(){
  struct Stack *stack = createStack(4);
  pushStack(stack,4);
  pushStack(stack,3);
  pushStack(stack,2);
  pushStack(stack,1);
 
  printf("\n Original Stack ");
  printStack(*stack);
  reverseStack(&stack);
  printf("\n Reversed Stack ");  
  printStack(*stack);
}
void calculateSpanTestData(){
	 int price[] = {10, 4, 5, 90, 120, 80};
    int n = sizeof(price)/sizeof(price[0]);
    int S[n];
 
    // Fill the span values in array S[]
    calculateSpan(price, n, S);
 
    // print the calculated span values
    printArray(S, n);
	return 0;
}
void printNextGreaterTestData(){
	int arr[]= {11, 13, 21, 3};
    int n = sizeof(arr)/sizeof(arr[0]);
    findNextGreaterElement(arr, n);
	return 0;
}
void paranthesisBalancedTestData(){
  char exp[100] = "{()}[]";
  if(areParenthesisBalanced(exp))
    printf("\n Balanced ");
  else
    printf("\n Not Balanced "); 
}
void twoStackTestData(){
	struct twoStack* twostack=createTwoStack(5);
    pushTwoStack1(twostack,5);
    pushTwoStack2(twostack,10);
    pushTwoStack2(twostack,7);
    pushTwoStack2(twostack,15);
    pushTwoStack1(twostack,11);
    printf("Popped element from stack 1 is %d",popTwoStack1(twostack));
    pushTwoStack2(twostack,40);
    printf("Popped element from stack 2 is %d",popTwoStack2(twostack));
}
void reverseStringTestData(){
	 char str[] = "abc";
 
    reverseStringByStack(str);
    printf("Reversed string is %s", str);
	reverseNormally(str);
	printf("Reversed string is %s", str);
}
void stackCreationTestData(){
	struct Stack* stack = createStack(100);
 
    pushStack(stack, 10);
    pushStack(stack, 20);
    pushStack(stack, 30);
 
    printf("%d popped from stack\n", pop(stack));
 
    printf("Top item is %d\n", peek(stack));	
}
void infixToPostfixTestData(){
	 char exp[] = "a+b*(c^d-e)^(f+g*h)-i";
    infixToPostfix(exp);
    return 0;
}
void evaluatePostFixTestData(){
	char exp[] = "231*+9-";
    printf ("Value of %s is %d", exp, evaluatePostFix(exp));
    return 0;
}
void stackTestData(){
//	kStackTestData();
//	mergeableStackTestData();
//	dllStackTestData();
//	specialStackTestData();
//	calculateSpanTestData();
	//reverseStackTestData();
	printNextGreaterTestData();
	//paranthesisBalancedTestData();
	//stackCreationTestData();
	//infixToPostfixTestData();
	//evaluatePostFixTestData();
	//reverseStringTestData();
	//twoStackTestData();
}