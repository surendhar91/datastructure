#ifndef STACK_H_
#define	STACK_H_
struct Stack* createStack(unsigned capacity);
int isFull(struct Stack* stack);
int isEmpty(struct Stack* stack);
void pushStack(struct Stack* stack,int data);
int pop(struct Stack* stack);
int peek(struct Stack* stack);
//infix to postfix
int prec(char op);
int isOperand(char op);
int infixToPostfix(char *exp);
//postfix evaluate
int evaluatePostFix(char *exp);
//Reverse a string using stack
void reverseStringByStack(char str[]);
void swapStr(char *a,char *b);
void reverseNormally(char str[]);
//Using an array for two stacks
struct twoStack* createTwoStack(unsigned capacity);
void pushTwoStack1(struct twoStack* twostack,int data);
void pushTwoStack2(struct twoStack* twostack,int data);
int popTwoStack1(struct twoStack* twostack);
int popTwoStack2(struct twoStack* twostack);
#endif