#include <bits/stdc++.h>
#define maxn 10001
using namespace std;
map<string,int> h;
map<string,int> flag;
set<string> First[maxn];
set<string> Follow[maxn];
string grammar[maxn]; 
int ne[maxn],idx=1,len=1; 
void ccin(){//输入文法
	ifstream infile;
	string path="test.txt";
	infile.open(path.data()); 
	assert(infile.is_open());
	string s;
	vector<string> data;
	while(getline(infile,s)){
		s=s.c_str();
		data.push_back(s);
		int pos_1=s.find('-');
		h[s.substr(0,pos_1)]=-1;
	}
	for(int i=0;i<data.size();i++){
		s=data[i];
		int pos_1=s.find('-'),pos_2=s.find('>');
		grammar[idx]=s.substr(pos_2+1);
		ne[idx]=h[s.substr(0,pos_1)];
		h[s.substr(0,pos_1)]=idx++;
	}
	infile.close();
}
int get_num(string s){
	int num;
	for(int i=0;i<s.length();i++){
		num+=int(s[i]-'\0');
	}
	return num;
}
void get_first(string l){
	for(int i=h[l];i!=-1;i=ne[i]){
		if(i==0) break;
		int lens=grammar[i].length(),pos=0;
		while(pos<lens){
			string t=grammar[i].substr(pos,1);
			if(t=="~"){
				First[get_num(l)].insert("~");
				break;
			}
			else if(h[t]==0){//是终结符
				h.erase(t);
				First[get_num(l)].insert(t);
				break;
			}
			else{
				get_first(t);
				set<string>::iterator it=First[get_num(t)].find("~");
				if(it!=First[get_num(t)].end()){//找到空集
					for(set<string>::iterator it=First[get_num(t)].begin();it!=First[get_num(t)].end();it++){
						string in=*it;
						First[get_num(l)].insert(in);
					}
					if(pos!=lens-1) First[get_num(l)].erase("~");
					pos++;
				}
				else{
					for(set<string>::iterator it=First[get_num(t)].begin();it!=First[get_num(t)].end();it++){
						string in=*it;
						First[get_num(l)].insert(in);
					}
					break;
				}
			}
		}
	}
}
void init(){
	map<string,int>::iterator iter;
    for(iter=h.begin();iter!=h.end();iter++){
		get_first(iter->first);
    }
} 
void print_first(){
	map<string,int>::iterator iter;
    for(iter=h.begin();iter!=h.end();iter++){
    	cout<<iter->first<<":{ ";
     	for(set<string>::iterator i=First[get_num(iter->first)].begin();i!=First[get_num(iter->first)].end();i++)
        	cout<<*i<<" ";
        cout<<"}"<<endl;
    }
}
void get_follow(){
	//循环十遍 
}
int main(){
	ccin();
	init();
	print_first();
	return 0;
}
