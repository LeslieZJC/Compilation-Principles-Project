#include <bits/stdc++.h>
#define maxn 100
using namespace std;
int T[maxn][maxn],g[maxn],n,m,len;
string s;
void init(){
	T[0][0]=1;T[0][1]=2;T[1][0]=1;T[1][1]=3;T[2][0]=1;T[2][1]=2;
	T[3][0]=1;T[3][1]=4;T[4][0]=1;T[4][1]=2;
}
int dfa(int cs,int js){
	int state=cs;
	for(int i=1;i<=len;i++){
		if(T[state][g[i]]!=0){
			state=T[state][g[i]];
		}
		else break;
	}
	if(state==js) return 1;
	else return 0;
}
int main(){
	cin>>s>>n>>m;
	init();
	len=s.length();
	for(int i=0;i<len;i++)
		g[i+1]=s[i]-'a';
	if(dfa(n,m)==1) cout<<"YES";
	else cout<<"NO";
	return 0;
}
