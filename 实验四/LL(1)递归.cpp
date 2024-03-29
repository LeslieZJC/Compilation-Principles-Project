#include <bits/stdc++.h>
#define maxn 10010
using namespace std;
string t;
char input[maxn];
int l=0,len=0;
void parseE();
void parseE_();
void parseT();
void parseT_();
void parseP();
void parseF();
void parseF_();
void match(char x);
int main(){
	cin>>t;
	for(int i=0;i<t.length();i++)
		input[i]=t[i];
	parseE();
	cout<<"Right!";
	return 0;
}
void parseE(){
	switch(input[l]){
		case '(':case 'a':case 'b':case '^':
			parseT();
			parseE_();
			break;
		default:
			cout<<"syntax error"<<endl;
			exit(0);
	}
}
void parseE_(){
	switch(input[l]){
		case '+':
			match('+');
			parseE();
			break;
		case ')':case '#':
			break;
		default:
			cout<<"syntax error"<<endl;
			exit(0);
	}	
}
void parseT(){
	switch(input[l]){
		case '(':case 'a':case 'b':case '^':
			parseF();
			parseT_();
			break;
		default:
			cout<<"syntax error"<<endl;
			exit(0);
	}
}
void parseT_(){
	switch(input[l]){
		case '(':case 'a':case 'b':case '^':
			parseT();
			break;
		case'+':case '#':case ')':
			break;
		default:
			cout<<"syntax error"<<endl;
			exit(0);
	}
}
void parseF(){
	switch(input[l]){
		case '(':case 'a':case 'b':case '^':
			parseP();
			parseF_();
			break;
		default:
			cout<<"syntax error"<<endl;
			exit(0);	
	}
}
void parseF_(){
	switch(input[l]){
		case '*':
			match('*');
			parseF();
			break;
		case '(':case 'a':case 'b':case '^':case '+':case '#':case ')':
			break;
		default:
			cout<<"syntax error"<<endl;
			exit(0);	
	}
}
void parseP(){
	switch(input[l]){
		case '(':
			match('(');
			parseE();
			match(')');
			break;
		case 'a':match('a'); break;
		case 'b':match('b'); break;
		case '^':match('^'); break;
		default:
			cout<<"syntax error"<<endl;
			exit(0);
	}
}
void match(char x){
	if(x==input[l]){
		l++;
	} 
	else{
		cout<<"syntax error"<<endl;
		exit(0);
	}	
}
