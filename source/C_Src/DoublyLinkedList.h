#ifndef DOUBLE_H_
#define	DOUBLE_H_
struct DllNode{
		struct DllNode *prev;
		int data;
		struct DllNode *next;
};
//to be used for circular linked list alone.
struct DllNode* appendTwoDll(struct DllNode* a,struct DllNode *b);//joins the list and returns the joined list head.
void joinDllNodes(struct DllNode *a,struct DllNode *b); //sets the previous and next pointer of a, b
void printDllList(struct DllNode *head);
#endif