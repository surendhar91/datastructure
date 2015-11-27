#ifndef SEG_H_
#define SEG_H_
int getMid(int start,int end);
int getSuMOfValuesFromSegTree(int *segTree,int segStart,int segEnd,int queryStart,int queryEnd,int index);
void updateValueOfArrByAdjustSegTree(int *segTree,int segStart,int segEnd,int i,int diffValue,int index);
void updateValueOfArr(int arr[],int *segTree,int n,int i,int newVal);
int getSuMOfValuesFromQuery(int *segTree,int n,int queryStart,int queryEnd);
int constructSegTreeUtil(int arr[],int segStart,int segEnd,int *segTree,int segIndex);
int* constructSegTree(int arr[],int n);
#endif
