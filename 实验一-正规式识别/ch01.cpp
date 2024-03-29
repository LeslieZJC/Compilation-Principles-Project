#include <bits/stdc++.h>
#define maxn 10010
using namespace std;
string s;
int len,flag=1,t[maxn];
void dfs(int n){
	if(n==1&&t[n]!=1){
		flag=0;
		return;
	}
	if(t[n]!=1&&t[n]!=0){
		flag=0;
		return;
	}	
	if(n==len-2){
		if(t[n]!=1||t[n+1]!=0||t[n+2]!=1)
			flag=0;
		return;
	}
	dfs(n+1);
}
int main(){
	cin>>s;
	len=s.length();
	for(int i=0;i<len;i++)
		t[i+1]=s[i]-'0';
	dfs(1);
	if(flag==1)
		cout<<"YES";
	else 
		cout<<"NO";
	return 0;
}
