#include <bits/stdc++.h>
#define maxn 1001
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
	if((n>1&&n<len-1)&&t[n]==1){
		if(t[n+1]!=1){
			flag=0;
			return;
		}
	}	
	if((n>1&&n<len-1)&&t[n]==0){
		if(t[n+1]!=1){
			flag=0;
			return;
		}
	}
	if(n==len-1){
		if(t[n]!=0||t[n+1]!=1)
			flag=0;
		return;
	}
	if(n==1)
		dfs(n+1);
	else
		dfs(n+2);
}
int main(){
	cin>>s;
	len=s.length();
	for(int i=0;i<len;i++)
		t[i+1]=s[i]-'a';
	dfs(1);
	if(flag==1)
		cout<<"YES";
	else 
		cout<<"NO";
	return 0;
}
