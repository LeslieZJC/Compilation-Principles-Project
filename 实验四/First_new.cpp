#include <bits/stdc++.h>
#define maxn 10001
using namespace std;
map<string,int> h;
map<string,int> flag;
set<string> First[maxn];
set<string> Follow[maxn];
set<string> Select[maxn];
map<string,map<string,string>> anly; 
string grammar[maxn],state; 
char st[maxn],input[maxn];
int top=0,pos=0;
string temp;
int ne[maxn],idx=1,len=1; 
void ccin(){//输入文法
	ifstream infile;
	string path="test4.txt";
	infile.open(path.data()); 
	assert(infile.is_open());
	string s;
	vector<string> data;
	while(getline(infile,s)){
		s=s.c_str();
		data.push_back(s);
		int pos_1=s.find('-');
		if(state=="") state=s.substr(0,pos_1);
		h[s.substr(0,pos_1)]=-1;
	}
	for(int i=0;i<data.size();i++){
		s=data[i];
		int pos_1=s.find('-'),pos_2=s.find('>');
		grammar[idx]=s.substr(pos_2+1);
		ne[idx]=h[s.substr(0,pos_1)];
		h[s.substr(0,pos_1)]=idx++;
		if(i==0) state=s.substr(0,pos_1);//初始符 
	}
	infile.close();
}
void print_wf(){
	cout<<"---------------该文法如下----------------"<<endl;
	map<string,int>::iterator iter;
	for(iter=h.begin();iter!=h.end();iter++){
		for(int i=h[iter->first];i!=-1;i=ne[i]){
			cout<<iter->first<<"->"<<grammar[i]<<endl;
		}
	}
}
int get_num(string s){
	int num;
	for(int i=0;i<s.length();i++){
		num+=int(s[i]-'\0');
	}
	return num;
}
void get_first(){
	map<string,int>::iterator iter;
	for(int i=0;i<=h.size();i++){
		for(iter=h.begin();iter!=h.end();iter++){//遍历 
			for(int i=h[iter->first];i!=-1;i=ne[i]){
				int lens=grammar[i].length(),pos=0;
				string l=iter->first;
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
						if(iter->first==t) break;//左递归 就break 
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
    for(iter=h.begin();iter!=h.end();iter++){
    	for(int i=h[iter->first];i!=-1;i=ne[i]){
    		cout<<grammar[i]<<":{ ";
    		string t=grammar[i]+iter->first;
    		for(set<string>::iterator it=First[get_num(t)].begin();it!=First[get_num(t)].end();it++)
        		cout<<*it<<" ";
        	cout<<"}"<<endl;
		}
    }
}
void get_follow(){
	Follow[get_num(state)].insert("#");
	map<string,int>::iterator iter;
	for(int k=0;k<=h.size();k++){
		for(iter=h.begin();iter!=h.end();iter++){
			for(int i=h[iter->first];i!=-1;i=ne[i]){
				int lens=grammar[i].length(),pos=0;
				string l=iter->first;
				while(pos<lens){
					string r1=grammar[i].substr(pos,1),r2;
					if(pos+1>=lens) r2="\0";//右部已经扫描到最后一个字符 
					else r2=grammar[i].substr(pos+1,1);
					if(r1=="~"){
						break;
					}
					else if(h[r1]==0){//目前字符为终结符 
						h.erase(r1);
						pos++;
					}
					else if(h[r2]==0&&r2!="\0"){//下一个字符是终结符 
						h.erase(r2);
						Follow[get_num(r1)].insert(r2);
						pos++;
					}
					else if(r2=="\0"){//下一个字符为空
						h.erase(r2);
						for(set<string>::iterator it=Follow[get_num(l)].begin();it!=Follow[get_num(l)].end();it++){
							string in=*it;
							Follow[get_num(r1)].insert(in);
						}
						break;
					}
					else{//下一个字符为非终结符 
						set<string>::iterator it=First[get_num(r2)].find("~");
						if(it!=First[get_num(r2)].end()){//在未扫描的产生式中可以推导出空串
							for(set<string>::iterator it=Follow[get_num(l)].begin();it!=Follow[get_num(l)].end();it++){
								string in=*it;
								Follow[get_num(r1)].insert(in);
							}	
						}
						for(set<string>::iterator it=First[get_num(r2)].begin();it!=First[get_num(r2)].end();it++){
							string in=*it;
							Follow[get_num(r1)].insert(in);
						}
						Follow[get_num(r1)].erase("~");
						pos++;
					}
				}
			}
    	}
	} 
}
void print_follow(){
	map<string,int>::iterator iter;
	for(iter=h.begin();iter!=h.end();iter++){
    	cout<<iter->first<<":{ ";
     	for(set<string>::iterator i=Follow[get_num(iter->first)].begin();i!=Follow[get_num(iter->first)].end();i++)
        	cout<<*i<<" ";
        cout<<"}"<<endl;
    }
}
void get_first_right(){//获得右部first集 
	map<string,int>::iterator iter;
	for(iter=h.begin();iter!=h.end();iter++){
		for(int i=h[iter->first];i!=-1;i=ne[i]){
			string t=grammar[i]+iter->first;
			int lens=grammar[i].length(),pos=0;
			while(pos<lens){
				string r1=grammar[i].substr(pos,1);
				if(h[r1]==0){
					h.erase(r1);
					First[get_num(t)].insert(r1);
					break;
				}//为终结符 
				else if(h[r1]!=0){
					set<string>::iterator it=First[get_num(r1)].find("~");
					if(it!=First[get_num(r1)].end()){//找到空集
						for(set<string>::iterator it=First[get_num(r1)].begin();it!=First[get_num(r1)].end();it++){
							string in=*it;
							First[get_num(t)].insert(in);
						}
						pos++;
					}
					else{
						for(set<string>::iterator it=First[get_num(r1)].begin();it!=First[get_num(r1)].end();it++){
							string in=*it;
							First[get_num(t)].insert(in);
						}
						break;
					}
				}//为非终结符 
			}
		}
	}
}
int get_select(){
	map<string,int>::iterator iter;
	for(iter=h.begin();iter!=h.end();iter++){
		for(int i=h[iter->first];i!=-1;i=ne[i]){
			//同一个左部
			string t=grammar[i]+iter->first;
			set<string>::iterator it=First[get_num(t)].find("~");
			if(it!=First[get_num(t)].end()){//找到空集
				for(set<string>::iterator it=First[get_num(t)].begin();it!=First[get_num(t)].end();it++){
					string in=*it;
					Select[get_num(t)].insert(in);
				}
				for(set<string>::iterator it=Follow[get_num(iter->first)].begin();it!=Follow[get_num(iter->first)].end();it++){
					string in=*it;
					Select[get_num(t)].insert(in);
				}//将follow集加入 
				Select[get_num(t)].erase("~");
			}
			else{
				for(set<string>::iterator it=First[get_num(t)].begin();it!=First[get_num(t)].end();it++){
					string in=*it;
					Select[get_num(t)].insert(in);
				}
			}
		}
	}
}
void print_select(){
	map<string,int>::iterator iter;
	for(iter=h.begin();iter!=h.end();iter++){
    	for(int i=h[iter->first];i!=-1;i=ne[i]){
    		string t=grammar[i]+iter->first;
    		cout<<iter->first<<"->"<<grammar[i]<<":{ ";
    		for(set<string>::iterator it=Select[get_num(t)].begin();it!=Select[get_num(t)].end();it++)
        		cout<<*it<<" ";
        	cout<<"}"<<endl;
		}
    }
}
int judge(){//判断是否为LL(1)文法 
	map<string,int>::iterator iter;
	for(iter=h.begin();iter!=h.end();iter++){
		set<string> union_;
    	for(int i=h[iter->first];i!=-1;i=ne[i]){
    		string t=grammar[i]+iter->first;
    		for(set<string>::iterator it=Select[get_num(t)].begin();it!=Select[get_num(t)].end();it++){
				string in=*it;
				set<string>::iterator ite=union_.find(in);
				if(ite!=union_.end()) return 0;
			}
			//交集 
			for(set<string>::iterator it=Select[get_num(t)].begin();it!=Select[get_num(t)].end();it++){
				string in=*it;
				union_.insert(in);
			}
    	}
    }
    return 1;
}
//void get_table(){//获得select表 
//	map<string,int>::iterator iter;
//	for(iter=h.begin();iter!=h.end();iter++){
//		for(int i=h[iter->first];i!=-1;i=ne[i]){
//			string t=iter->first+grammar[i];//字符串转为字符 
//			set<string>::iterator it;
//			for(set<string>::iterator it=Select[get_num(t)].begin();it!=Select[get_num(t)].end();it++){
//				string in=*it;
//				anly[iter->first][in]=grammar[i];
//			}
//		}
//	}
//}
//void judge_LL1(){
//	while(top!=0){
//		if(anly[st[top-1]][input[pos]]==""){
//			cout<<"syntax error"<<endl;
//			return; 
//		}
//		else{
//			string g=anly[st[top-1]][input[pos]];
//			top--;
//			for(int i=g.length()-1;i>=0;i--){
//				if(g[i]=='~') break;
//				else st[top++]=g[i];
//			}
//			if(st[top-1]==input[pos]){
//				top--;
//				pos++;
//			}
//		}
//	}
//	cout<<"Right!"<<endl;
//}
int main(){
	ccin();
	print_wf();
	get_first();
	get_first_right();
	cout<<"---------------First集-----------------"<<endl;
	print_first();
	cout<<"---------------Follow集----------------"<<endl;
	get_follow();
	print_follow();
	cout<<"---------------Select集----------------"<<endl;
	get_select();
	print_select();
	if(judge()==1){
		cout<<"-------------该文法为LL(1)文法-------------"<<endl;
//		cout<<"-------------请输入待判断的字符串：";
//		cin>>temp;
//		for(int i=0;i<t.length();i++)
//			input[i]=temp[i];
//		input[t.length()]='#';
//		st[top++]='#';
//		st[top++]='E';
//		get_table();
//		judge_LL1();
	}
	else cout<<"---------------该文法不为LL(1)文法--------------"<<endl;
	return 0;
}
