#include<queueImp.h>
using namespace std;
void generatePrintBinary(int n){
		//using BFS.
	queue<string> q;
	q.push("1");
//	cout<<q.front();
//	string str="1";
	while(n--){
		
		cout<<q.front()<<"\n";
		//prints 1
		string s1 = q.front();// get the front element
		q.pop();//pop the element from front.
		//s1 -> store 1
		string s2=s1;
		//s2 -> store 1
		s1.append("0");
		q.push(s1);
		s2.append("1");
		q.push(s2);
	}
	
}
// Driver program to test above function
void queueTestData(){
	int n = 10;
    generatePrintBinary(n);
//    return 0;
//    return 0;
}