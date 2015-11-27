#include <queue.h>
#include <SinglyLinkedList.h>
#include <stdio.h>
//Start of Circular array Queue Creation process
struct Queue{
	int* arr;
	int front;
	int rear;
	int size;
	unsigned capacity;
};
struct Queue* createQueue(unsigned capacity){
	struct Queue* queue=(struct Queue*)malloc(sizeof(struct Queue));
	queue->arr=(int *)malloc(sizeof(int)*capacity);
	queue->capacity=capacity;
	queue->front=queue->size=0;
	queue->rear=queue->capacity-1;//so that the enqueue and dequeue operation can be circular
	return queue;
}
int isFullQueue(struct Queue* queue){
	return queue->size==queue->capacity;
}
int isEmptyQueue(struct Queue* queue){
	return queue->size==0;
}
void enqueue(struct Queue* queue,int data){
	if(isFullQueue(queue)){
		printf("Queue overflow..");
		return ;
	}
	queue->rear=(queue->rear+1)%queue->capacity; // Start with 0, circular implementation.
	queue->size+=1;
	queue->arr[queue->rear]=data;
}
int dequeue(struct Queue* queue){
	if(isEmptyQueue(queue)){
		printf("Queue underflow..");
		return ;
	}
	int item=queue->arr[queue->front];//get the data to dequeue.
	queue->size-=1;
	queue->front=(queue->front+1)%queue->capacity;//move the front up.
	return item;
	
}
int getFrontOfQueue(struct Queue* queue){
	if(isEmptyQueue(queue)){
		return ;
	}
	return queue->arr[queue->front];
}
int getRearOfQueue(struct Queue* queue){
	if(isEmptyQueue(queue)){
		return ;
	}
	return queue->arr[queue->rear];
}
void queueCreationTestData(){
	struct Queue* queue = createQueue(1000);
 
    enqueue(queue, 10);
    enqueue(queue, 20);
    enqueue(queue, 30);
    enqueue(queue, 40);
 
    printf("%d dequeued from queue\n", dequeue(queue));
 
    printf("Front item is %d\n", getFrontOfQueue(queue));
    printf("Rear item is %d\n", getRearOfQueue(queue));	
}
//End of Circular Array queue creation process.

//Start of linked list queue creation process
struct liQueue{
	struct node *front,*rear;
};
struct liQueue* createLiQueue()
{
	struct liQueue* queue=malloc(sizeof(struct liQueue));
	queue->front=NULL;
	queue->rear=NULL;
	return queue;
}
void enQueueInLi(struct liQueue* queue,int data){
	struct node* temp=createNewNode(data);
	if(queue->rear==NULL){
		queue->front=queue->rear=temp;
		return ;
	}
	queue->rear->next=temp;
	queue->rear=queue->rear->next;
}
int deQueueInLi(struct liQueue* queue){
	if(queue->front==NULL){
		printf("No elements to dequeue..");
	}
	struct node* temp=queue->front;
	queue->front=queue->front->next;
	
	if(queue->front==NULL)
		queue->rear=NULL; //Very very important, if the queue's front is null, it's rear also should be null.
	
	int data=temp->data;
	free(temp);
	return data;
}
void liqueueCreationTestData(){
	struct liQueue *q = createLiQueue();
    enQueueInLi(q, 10);
    enQueueInLi(q, 20);
    deQueueInLi(q);
    deQueueInLi(q);
    enQueueInLi(q, 30);
    enQueueInLi(q, 40);
    enQueueInLi(q, 50);
	printf("Dequeued item is %d", deQueueInLi(q));
}
//End of linked list queue creation process
/*
 * Applications of queue data structure - CPU Scheduling, disk scheduling on accessing a shared resource
 * 
 * 
 * */
 /*
  * Priority Queue is an extension of queue with following properties.
  * 
  * 1) Every item has a priority associated with it.
  * 2) An element with high priority is dequeued before an element with low priority.
  * 3) If two elements have the same priority, they are served according to their order in the queue.
  * 
  * Using Heaps:
  * 
  * Heap is generally preferred for priority queue implementation because heaps provide better performance compared arrays or linked list.
	
	 * In a Binary Heap, getHighestPriority() can be implemented in O(1) time,
							insert() can be implemented in O(Logn) time 
							and deleteHighestPriority() can also be implemented in O(Logn) time.

	 * With Fibonacci heap, insert() and getHighestPriority() can be implemented in O(1) amortized time 
	 * 						and deleteHighestPriority() can be implemented in O(Logn) amortized time.
	 
  * */
/*
 * Operations on Deque:
Mainly the following four basic operations are performed on queue:

insetFront(): Adds an item at the front of Deque.
insertLast(): Adds an item at the rear of Deque.
deleteFront(): Deletes an item from front of Deque.
deleteLast(): Deletes an item from rear of Deque.

In addition to above operations, following operations are also supported
getFront(): Gets the front item from queue.
getRear(): Gets the last item from queue.
isEmpty(): Checks whether Deque is empty or not.
isFull(): Checks whether Deque is full or not.

 * 
 * Implementation of Deque: A Deque can be implemented either using a doubly linked list or circular array.
   In both implementation, we can implement all operations in O(1) time. 

Applications of Deque:
Since Deque supports both stack and queue operations, it can be used as both. 

 * The Deque data structure supports clockwise and anticlockwise rotations in O(1) time 
   which can be useful in certain applications.

 * Also, the problems where elements need to be removed and or added both ends can be efficiently solved 
   using Deque. 
 
 * For example see Maximum of all subarrays of size k problem..

 * */
//Start of queue creation using two stacks.
struct queueInStack{
	struct node* stack1;
	struct node* stack2;
};

void pushInLiStack(struct node **head, int data){
//	struct node *top=*head;
	if(*head==NULL){
		*head=createNewNode(data);
		return ;
	}
	struct node *temp=createNewNode(data);
	temp->next=*head;
	(*head)=temp;
	
}
int popFromLiStack(struct node **head){
	if(*head==NULL){
		printf(" No elements fround to pop ");
		return ;
	}
	struct node *temp=(*head);
	int res=temp->data;
	(*head)=(*head)->next;
	free(temp);
	return res;
}

void enqueueUsingStack(struct queueInStack* queue,int data){
	pushInLiStack(&queue->stack1,data);//pushing the element to stack 1.
	//stack 1 used only for enqueuing the data.
}
int dequeueUsingStack(struct queueInStack* queue){
	if(queue->stack1==NULL&&queue->stack2==NULL){
		printf("No element to deque..");
		return ;
	}
	
	if(queue->stack2==NULL){//if the stack 2 is empty means that we can't dequeu any more elements
		while(queue->stack1!=NULL){//If the stack 1 is not empty, move all the elements from stack1 to stack2
			pushInLiStack(&queue->stack2,popFromLiStack(&queue->stack1));
		}
	}
	return popFromLiStack(&queue->stack2);//directly dequeue from stack 2.
}
void queueCreationUsingStackTestData(){
   struct queueInStack *q = (struct queueInStack*)malloc(sizeof(struct queueInStack));
   q->stack1 = NULL;
   q->stack2 = NULL;
   enqueueUsingStack(q, 1);
   printf("%d  ", dequeueUsingStack(q));
   enqueueUsingStack(q,5);
   enqueueUsingStack(q,2);
   enqueueUsingStack(q,3);    
  
   /* Dequeue items */
   printf("%d  ", dequeueUsingStack(q));
   
   printf("%d  ", dequeueUsingStack(q));  	
	
}

//End of queue creation using two stacks.
//Start of queue creation using one stack + Function stack
void enqueueUsingFunctionStack(struct node **stack, int data){
	pushInLiStack(stack,data);//Make sure, we use double pointer
}
int dequeueUsingFunctionStack(struct node **stack){
	
	int res,temp; //Element to return
	if(*stack==NULL){
			printf("No elements to dequeue from..");
			return ;
	}else if((*stack)->next==NULL){
			return popFromLiStack(stack);
	}else{
			temp = popFromLiStack(stack);
			res = dequeueUsingFunctionStack(stack);
			pushInLiStack(stack,temp);
	}
	return res;
}

void queueCreationUsingFunctionStack(){
  /* Create a queue with items 1 2 3*/
  //struct queue *q = (struct queue*)malloc(sizeof(struct queue));
  struct node *stack1 = NULL;
 
  enqueueUsingFunctionStack(&stack1, 1);
  enqueueUsingFunctionStack(&stack1, 2);
  enqueueUsingFunctionStack(&stack1, 3);    
 
  /* Dequeue items */
  printf("%d  ", dequeueUsingFunctionStack(&stack1));
  printf("%d  ", dequeueUsingFunctionStack(&stack1));
  printf("%d  ", dequeueUsingFunctionStack(&stack1));
		
}
//End of queue creation using one stack + function stack

//Start of find multiple of 3 from a given array of elements
int compareASC(const void *a,const void *b){
	return *(int *)a > *(int *)b; 
}
int compareDESC(const void *a,const void *b){
	return *(int *)a < *(int *)b; 
}
void printAuxResult(int *arr,struct Queue* queue1,struct Queue* queue2,struct Queue* queue3,int *top){

	while(!isEmptyQueue(queue1)){
		arr[(*top)++]=dequeue(queue1);//*top++, first gets the data and increments the element
	}
	while(!isEmptyQueue(queue2)){
		arr[(*top)++]=dequeue(queue2);
	}
	while(!isEmptyQueue(queue3)){
		arr[(*top)++]=dequeue(queue3);
	}
}
int findMaxMultipleOf3(int *arr,int n){
	//To find multiple of 3 from an array, we have a clue that 
	//Sum of digits is always divisble by 3.
	//Create 3 queues
	/*
	 * 8 7 2 0 -> sum is 17
	 * [%3==0]   q1 -> 0  
	 * [%3 == 1] q2 -> 7
	 * [%3==2 ]  q3 -> 8 2

	sum % 3 == 2 means we need to remove one element from queue 3
	or remove two elements from queue 2

	
	 * 8 6 2 0 -> sum is 16
	 * [%3==0]   q1 -> 0 6 
	 * [%3 == 1] q2 -> 
	 * [%3==2 ]  q3 -> 8 2

	sum % 3 == 1 means we need to remove one element from queue 2 if not empty,
	else delete two elements from queue 3.

	
	 * 0 2 5
	 * q1 -> 0
	 * q2 -> 
	 * q3 -> 5 2 

	sum is 7
	sum % 3 == 1 -> deleted two elements from queue 3 
	 * 
	 * */
	//Step 1: sort the element in non decreasing order.
	//so that when queueing / dequeing the elements, only the appropriate elements added or deleted.
	qsort(arr,n,sizeof(int),compareASC);
	struct Queue* queue1=createQueue(n);//To hold elements % 3 == 0
	struct Queue* queue2=createQueue(n);//To hold elements % 3 == 1
	struct Queue* queue3=createQueue(n);//To hold elements % 3 == 2
	
	int i,sum=0;
	for(i=0;i<n;i++)
	{
		sum+=arr[i];
		if(arr[i]%3==0){
			enqueue(queue1,arr[i]);
		}else if(arr[i]%3==1){
			enqueue(queue2,arr[i]);
		}else{
			enqueue(queue3,arr[i]);
		}
	}
	
	if(sum%3==2){
			if(!isEmptyQueue(queue3)){
				//Dequeue an element from queue 3
				dequeue(queue3);
			}else{
				//Dequeue two elements from queue 2
				if(!isEmptyQueue(queue2)){
				 dequeue(queue2);
				}else{
					return 0; // Not possible
				}
				
				if(!isEmptyQueue(queue2)){
				 dequeue(queue2);
				}else{
					return 0; // Not possible
				}
			}
	}
	if(sum%3==1){
			if(!isEmptyQueue(queue2)){ //That hold %3==1 elements
				dequeue(queue2);
			}else{
				//Dequeue two elements from queue 3 [ i.e each of which is 4 ]
				if(!isEmptyQueue(queue3)){
				 dequeue(queue3);
				}else{
					return 0; // Not possible
				}
				
				if(!isEmptyQueue(queue3)){
				 dequeue(queue3);
				}else{
					return 0; // Not possible
				}
			}
	}
	int res[n],top=0;
	printAuxResult(res,queue1,queue2,queue3,&top);
	//if at all the top is not incremented, means that we couldn't find the multiple of 3 from given array
	if(top==0){
		return 0;
	}
	//top provides the size of aux array
	qsort(res,top,sizeof(int),compareDESC);//sort the element in increasing order
	
	for(i=0;i<top;i++){
		printf("%d ",res[i]);
	}
	return top;
}
void findMaxMultipleOf3TestData(){
	int arr[] = {8, 1, 7, 6, 0};
    int size = sizeof(arr)/sizeof(arr[0]);
 
    if (findMaxMultipleOf3( arr, size ) == 0)
        printf( "Not Possible" );
 
    return 0;	
}
//End of find multiple of 3 
struct petrolPump{
	int litre;
	int distance;
};
int findPetrolPumpCircularTour(struct petrolPump arr[],int n){
	/*
	 * Problem description:
	 * Suppose there is a circle. There are n petrol pumps on that circle. You are given two sets of data.
	 * 1. The amount of petrol that petrol pump will give.
	 * 2. Distance from that petrol pump to the next petrol pump.
	 * 
	 * Calculate the first point from where a truck will be able to complete the circle 
	 * (The truck will stop at each petrol pump and it has infinite capacity). 
	 * 
	 * Expected time complexity is O(n). Assume for 1 litre petrol, the truck can go 1 unit of distance.
	 * 
	 * For example, let there be 4 petrol pumps with amount of petrol and distance to next petrol pump 
	 * value pairs as {4, 6}, {6, 5}, {7, 3} and {4, 5}. 
	 * 
	 * The first point from where truck can make a circular tour is 2nd petrol pump. 
	 * 
	 * Output should be “start = 1″ (index of 2nd petrol pump).
	 * 
	 * Solution:-
	 * --------------------
	 * We can use a Queue to store the current tour. We first enqueue first petrol pump to the queue, 
	 * 
	 * we keep enqueueing petrol pumps till we either complete the tour, or current amount of petrol becomes negative. 
	 * 
	 * If the amount becomes negative, then we keep dequeueing petrol pumps till the current amount becomes positive or queue becomes empty.
	 * 
	 * Instead of creating a separate queue, we use the given array itself as queue. We maintain two index variables start and end that represent rear and front of queue. 
	 * 
	 * */
	//alternative way of queueing is to use circular array using the same struct
	int currentPetrolAmt=0;
	int start = 0;
	int end =1; //Starting from 0 index, next index is 1
	currentPetrolAmt = arr[start].litre-arr[start].distance;
	while(start!=end || currentPetrolAmt < 0){
		
		while(currentPetrolAmt<0 && start!=end){
				currentPetrolAmt -= arr[start].litre-arr[end].distance; // deduct the start 
				start = (start + 1) % n; //Now setting this as start element.
				
				if(start==0)//If we are returning to 0, then no path found.
					return -1;
		}
		currentPetrolAmt += arr[end].litre-arr[end].distance;
		end = (end+1)%n;// to make it as circular
	}
	return start;
	//Explaining the iteration as below:
	/*
	 *  Start 0 End 1 [ 6, 4 ]
	 *  currentPetrolAmt = 2
	 *  
	 *  add the End petrol amt which is [3,6]
	 *  currentPetrolAmt = 2 + (3-6) = -1
	 *  increment end index which is End 2
	 *  
	 *  Start 0 End 2
	 *  currentPetrolAmt < 0 and start !=End
	 *  then remove current petrol amt of start  
	 *  currentPetrolAmt = -1 - (6-4) = -3
	 *  Save the Start index as 1
	 *  
	 *  Start 1 End 2
	 *  
	 * currentPetrolAmt = -3 - (3-6) = 0
	 * increment start index -> Start 2 End 0
	 *  
	 * Thereafter it proceeds rightly.
	 * */
}
void circularPetrolPumpTestData(){
	struct petrolPump arr[] = {{6, 4}, {3, 6}, {7, 3}};
 
    int n = sizeof(arr)/sizeof(arr[0]);
    int start = findPetrolPumpCircularTour(arr, n);
 
    (start == -1)? printf("No solution"): printf("Start = %d", start);
	return 0;
}
struct deque{
	int front;
	int rear;
	int *arr;
	int capacity;
//	int max;
};
struct deque* initializeDeque(int capacity){
	struct deque* dq=malloc(sizeof(struct deque));
	dq->front=dq->rear=-1;
	dq->capacity=capacity;
	dq->arr=(int *)malloc(sizeof(int)*capacity);
	return dq;
}
int emptyDQ(struct deque* dq){
	return (dq->rear==-1);
//	return (dq->front==dq->rear);
}
int fullDQ(struct deque* dq){
	return (((dq->rear+1)%dq->capacity)==dq->front);//we can't add element on the rear.
	//(3+1)%3==1
}
void pushBack(struct deque* dq,int data){
	if(fullDQ(dq)){
		printf("Full Deque, can't add any more data in the rear.");
	}else if(emptyDQ(dq)){
		//push the element
		dq->front=dq->rear=0;
		dq->arr[0]=data;
	}else{
		dq->rear=(dq->rear+1)%dq->capacity;//circular array.
		dq->arr[dq->rear]=data;
	}
}
int popBack(struct deque* dq){
	if(emptyDQ(dq)){
		printf("Empty deque, can't delete the element from rear end");
		return ;
	}
	int x=dq->arr[dq->rear];
	if(dq->front==dq->rear){//if there is only one last element.
		dq->front=dq->rear=-1;//initialize to default value.
	}else{
		dq->rear=(dq->rear-1+dq->capacity)%dq->capacity;
	}
	return x;
}
void pushFront(struct deque* dq,int data){
	if(fullDQ(dq)){
		printf("Full deque, can't add any more elements in the front.");
	}else if(emptyDQ(dq)){
		dq->front=dq->rear=0;//initialize to 0, when empty deque [i.e when front and rear are equal]
		dq->arr[0]=data;
	}else{
		dq->front=(dq->front-1+dq->capacity)%dq->capacity; //Circular array
		//0 - 1 + 30 -> 29
		dq->arr[dq->front]=data;
	}
}
int popFront(struct deque* dq){
	if(emptyDQ(dq)){
		printf("Empty deque, can't delete the element from front end");
		return ;
	}
	int x=dq->arr[dq->front];
	if(dq->front==dq->rear){
		//Same holds for pop back
		// 1 2 3
		// fr - re
		// 2 3
		// fr re
		// 3
		// fr,re
		//set the index to -1
		dq->front=dq->rear=-1;
	}
	else{
		dq->front=(dq->front+1)%dq->capacity;
	}
	return x;
}
void displayDeque(struct deque* dq){
//	int i;
	int i=dq->front;
	while(i!=dq->rear){
		printf("%d ",dq->arr[i]);	//iterating i only, not front.
		i=(i+1)%dq->capacity;
	}
	printf("%d ",dq->arr[dq->rear]);
}

void printKMax(int arr[],int n,int k){
	/*Method 3 (A O(n) method: use Dequeue)
			We create a Dequeue, Qi of capacity k, that stores only useful elements of current window of k elements.
			An element is useful if it is in current window and is greater than all other elements on left side of it in current window. 
		 * 
		 * We process all array elements one by one and maintain Qi to contain useful elements of current window and these useful elements are maintained in sorted order.

			The element at front of the Qi is the largest and element at rear of Qi is the smallest of current window. 
			 * 
	 * */
	struct deque* dq=initializeDeque(n);
	int i;
	for(i=0;i<k;++i){
		while(!emptyDQ(dq)&&arr[i]>=arr[dq->rear]){//if the incoming element is greater than, 
			//pop back the previous elements.
			popBack(dq);
		}//Maintains the sorted order.
		pushBack(dq,i);
		//dq->arr stores the index of elements in descending order of the current window
	}
//	displayDeque(dq);
	for(;i<n;++i){
		printf("%d ",arr[dq->arr[dq->front]]);//front always contains the largest element.
		while(!emptyDQ(dq)&&dq->arr[dq->front]<=(i-k)){
				//if not in the current window.
				popFront(dq);
		}
		while(!emptyDQ(dq)&&arr[i]>=arr[dq->arr[dq->rear]]){
			//if the incoming element is greater than, 
			//pop back the previous elements.
			popBack(dq);
		}//maintains the sorted order.
		pushBack(dq,i);
	}
	printf("%d ",arr[dq->arr[dq->front]]);//print the largest element of last window.
	
}
struct kQueue{
	int *arr;
	int *next;
	int *front,*rear;
	int n;
	int k;
	int free; //Next free slot in the array.
};
//Using of Kstack in the most time efficient and space efficient implementation

struct kQueue* createKQueue(int k,int n){
	struct kQueue* kqueue=(struct kQueue*)malloc(sizeof(struct kQueue));
	kqueue->n=n;
	kqueue->k=k;
	kqueue->arr=malloc(sizeof(int)*kqueue->n);
	kqueue->next=malloc(sizeof(int)*kqueue->n);
	kqueue->front=malloc(sizeof(int)*kqueue->k);
	kqueue->rear=malloc(sizeof(int)*kqueue->k);
	int i;
	for(i=0;i<n-1;i++){
			kqueue->next[i]=i+1;
			//if i=1, then next slot 2 is free to occupy
	}
	kqueue->next[n-1]=-1;//n-1 is full
	kqueue->free=0;//0 index is the first free slot to occupy.
	for(i=0;i<k;i++){
		kqueue->front[i]=-1;//Initialize top of k stacks to be -1
	}
	return kqueue;
	
}
int isEmptyKQueue(struct kQueue* kqueue,int qn){
	return kqueue->front[qn]==-1;
}
void enqueueKQueue(struct kQueue* kqueue,int qn,int data){
		//sn -> Stack Number
    
	if(kqueue->free==-1){//if full queue.
			printf("Queue overflow..");
			return;
	}
	int i = kqueue->free;
	
	kqueue->free = kqueue->next[i];
	
	if(isEmptyKQueue(kqueue,qn)){
		kqueue->front[qn]=i; //set the front of the queue to this i.
	}else{//If not an empty queue, next array should point to previous rear element of the queue.
		kqueue->next[kqueue->rear[qn]]=i; //point the rear=rear->next; here kqueue->next is the next pointer, kqueue->rear is the rear element.
					//i is the next rear element.
	}
	kqueue->next[i] = -1; //set the rear element's index in next to -1.
	
	kqueue->rear[qn]=i;//this free slot becomes the rear end of queue
	
	kqueue->arr[i]=data; //store the data to  free slot

}
int dequeueKQueue(struct kQueue* kqueue,int qn){
		//sn -> Stack Number
    
	if(isEmptyKQueue(kqueue,qn)){
			printf("Queue empty");
			return;
	}
	int i = kqueue->front[qn];//get the front index for queue number qn.
	
	kqueue->front[qn]= kqueue->next[i]; //next element of front .
	
	int res = kqueue->arr[i];
	
	kqueue->next[i] = kqueue->free;//next free element to occupy.
	
	kqueue->free = i; //i now is free to occupy.
	
	return res;
	

}

void kqueueTestData(){

		 // Let us create 3 queue in an array of size 10
    int k = 3, n = 10;
    struct kQueue* kqueue=createKQueue(k,n);
 
    // Let us put some items in queue number 2
    enqueueKQueue(kqueue,2,15);
    enqueueKQueue(kqueue,2,45);
 
    // Let us put some items in queue number 1
    enqueueKQueue(kqueue,1, 17);
    enqueueKQueue(kqueue,1, 49);
    enqueueKQueue(kqueue,1, 39);
 
    // Let us put some items in queue number 0
    enqueueKQueue(kqueue,0, 11);
    enqueueKQueue(kqueue,0, 9);
    enqueueKQueue(kqueue,0, 7);
 
	printf("dequeued element from queue 2 is %d",dequeueKQueue(kqueue,2));
	printf("dequeued element from queue 1 is %d",dequeueKQueue(kqueue,1));
	printf("dequeued element from queue 0 is %d",dequeueKQueue(kqueue,0));
}

void dequeTestData(){
	/*struct deque* dq=initializeDeque(10);
	pushBack(dq,12);
	pushBack(dq,11);
	pushFront(dq,5);
	pushBack(dq,10);
	pushFront(dq,7);
	displayDeque(dq);
	popBack(dq);
	popFront(dq);
	displayDeque(dq);
*/
	int arr[] = {12, 1, 78, 90, 57, 89, 56};
    int n = sizeof(arr)/sizeof(arr[0]);
    int k = 3;
    printKMax(arr, n, k);
    return 0;

}

void queueTestData(){
	kqueueTestData();
		//	dequeTestData();
//	circularPetrolPumpTestData();
//	findMaxMultipleOf3TestData();
//	queueCreationUsingFunctionStack();
	//queueCreationTestData();
//	liqueueCreationTestData();
//	queueCreationUsingStackTestData();
}