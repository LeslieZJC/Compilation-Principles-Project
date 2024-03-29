#include <bits/stdc++.h>
#define maxn 1001
using namespace std;
map<char,map<char,string>> anly; 
char st[maxn],input[maxn];
int top=0,pos=0;
string t;
void init(){
	anly['E']['(']="TR",anly['E']['a']="TR",anly['E']['b']="TR",anly['E']['^']="TR",anly['R']['+']="+E",anly['Y']['+']="~";
	anly['R'][')']="~",anly['R']['#']="~",anly['T']['(']="FY",anly['T']['a']="FY",anly['T']['b']="FY",anly['T']['^']="FY";
	anly['Y']['(']="T",anly['Y']['a']="T",anly['Y']['b']="T",anly['Y']['^']="T",anly['Y'][')']="~",anly['Y']['#']="~";
	anly['F']['(']="PG",anly['F']['a']="PG",anly['F']['b']="PG",anly['F']['^']="PG",anly['G']['+']="~",anly['G']['(']="~";
	anly['G']['a']="~",anly['G']['b']="~",anly['G']['^']="~",anly['G'][')']="~",anly['G']['#']="~",anly['G']['*']="*F";
	anly['P']['(']="(E)",anly['P']['a']="a",anly['P']['b']="b",anly['P']['^']="^";
}//E'--R,T'--Y,F'--G,~´ú±í¿Õ 
void judge(){
	while(top!=0){
		if(anly[st[top-1]][input[pos]]==""){
			cout<<"syntax error"<<endl;
			return; 
		}
		else{
			string g=anly[st[top-1]][input[pos]];
			top--;
			for(int i=g.length()-1;i>=0;i--){
				if(g[i]=='~') break;
				else st[top++]=g[i];
			}
			if(st[top-1]==input[pos]){
				top--;
				pos++;
			}
		}
	}
	cout<<"Right!"<<endl;
}
int main(){
	cin>>t;
	for(int i=0;i<t.length();i++)
		input[i]=t[i];
	input[t.length()]='#';
	st[top++]='#';
	st[top++]='E';
	init();
	judge();
	return 0;
}
