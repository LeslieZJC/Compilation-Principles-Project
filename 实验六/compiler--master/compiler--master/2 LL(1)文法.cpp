#include<iostream>
#include<set>
#include<algorithm>
#include<vector>
#include <iomanip>
#include<fstream>
#define maxn 100

using namespace std;
struct G{
	char left;//����ʽ�� 
	set<char> first;//ÿ����ѡʽ��first���� 
	string right;//��ѡʽ
};
int total;//����ʽ���� 
G g[maxn];//����ʽ 
set<char> First[maxn];
set<char> Follow[maxn];
string Terminal="";//�ս�� 
string NotTerminal="";//���ս�� 
vector<char> stack; 
vector<char> input;
int haveNone[maxn]={0};//�п� 
int hadFollow[maxn]={0};//�Ѿ����follow 
int hadFirst[maxn]={0};//�Ѿ����first 
int table[maxn][maxn]={0};//������ 
void getFirst(char X)
{
	int flag=0;
	int index=NotTerminal.find(X);
	if(hadFirst[index]>total)return;
	else hadFirst[index]++;
	for(int i=0;i<total;i++)
	{
		if(g[i].left==X)
		{
			if(haveNone[index]) {
				
				First[index].insert('@');
				if(g[i].right[0]=='@'){
					g[i].first.insert('@');
					continue;
				}	
			}
			//��һ��Ϊ�ս�� 
			if(Terminal.find(g[i].right[0])!=string::npos)
				{
					g[i].first.insert(g[i].right[0]);
					First[index].insert(g[i].right[0]);
				}
			else{
			//��һ��Ϊ���ս�� 
				for(int j=0;j<g[i].right.length();j++)
				 {
				 	//ֱ���ҵ��ս�����߲����Ƴ��մ��ķ��ս����ֹͣ
					if(Terminal.find(g[i].right[j])!=string::npos)
					{
						g[i].first.insert(g[i].right[j]);
						First[index].insert(g[i].right[j]);	
						break;
					}else{
						int index_temp = NotTerminal.find(g[i].right[j]);
						getFirst(g[i].right[j]);
						set<char>::iterator iter;
						for(iter=First[index_temp].begin();iter!=First[index_temp].end();iter++)
						{
							g[i].first.insert(*iter);
								First[index].insert(*iter);		
						} 
				 		if(!haveNone[index_temp])break;
				 }
				 
			}
		}
		}
	}
}

void getFollow(char X)
{
	int index = NotTerminal.find(X);
	if(hadFollow[index]>total)return;
	else hadFollow[index]++;
	for(int i=0;i<total;i++)
	{
		if(g[i].right.find(X)!=string::npos){
			int index_temp = g[i].right.find(X);
			for(int j=index_temp;j<g[i].right.length();j++)
			{
				if(j+1<g[i].right.length()){
					if(Terminal.find(g[i].right[j+1])!=string::npos){
						Follow[index].insert(g[i].right[j+1]);
						break;
					}else{
						set<char>::iterator iter;
						int temp = NotTerminal.find(g[i].right[j+1]);
						for(iter=First[temp].begin();iter!=First[temp].end();iter++)
						{
							if(*iter!='@')
							Follow[index].insert(*iter);
						}
						if(haveNone[temp])continue;
						else break;	
					}
				}else{
					getFollow(g[i].left);
					int temp=NotTerminal.find(g[i].left);
					set<char>::iterator iter;
					for(iter=Follow[temp].begin();iter!=Follow[temp].end();iter++)
					{
						Follow[index].insert(*iter);
					}
					
				}
			}
		}
	}
} 

void getTable()
{
	for(int i=0;i<total;i++)
	{
		int p = NotTerminal.find(g[i].left);
		if(Terminal.find(g[i].right[0])!=string::npos){
			int q=Terminal.find(g[i].right[0]);
			table[p][q]=i+1;//��һ����� 
		}else if(g[i].right[0]=='@'){
			//��������� 
			set<char>::iterator iter;
			for(iter=Follow[p].begin();iter!=Follow[p].end();iter++)
			{
				int temp = Terminal.find(*iter);
				table[p][temp]=i+1; 
			}
		}else{//�����ǵ�һ�ֻ��ߵڶ��� 
				set<char>::iterator iter;
				for(iter=g[i].first.begin();iter!=g[i].first.end();iter++)
				{
					if(*iter=='@')continue;
					int temp = Terminal.find(*iter);
					table[p][temp]=i+1; 
					cout<<p<<" "<<temp<<" "<<i+1<<endl;
				}
				if(g[i].first.find('@')==g[i].first.end()) continue;//�����Ƴ��մ�	
				//��һ�� 
				
				// ִ�е�������ǵڶ��� 
						for(iter=Follow[p].begin();iter!=Follow[p].end();iter++)
						{
							int temp = Terminal.find(*iter);
							table[p][temp]=i+1; 
						}
		}
	}
}

void getAnalysis(string str)
{
	stack.push_back('#');
    stack.push_back(NotTerminal[0]);
    input.push_back('#');
    for(int i=str.length()-1;i>=0;i--){input.push_back(str[i]);}
	cout<<setw(20)<<"����ջ"<<setw(20)<<"���봮"<<setw(20)<<"���ò���ʽ"<<endl;
    //���ʣ�����봮���Ȳ�Ϊ0����һֱѭ��
    while(input.size()>0){
        string outputs = "";
        for(int i=0;i<stack.size();i++){
            outputs+=stack[i];
        }
        cout<<setw(20)<<outputs;
        outputs = "";
        for(int i=input.size()-1;i>=0;i--){
            outputs+=input[i];
        }
        cout<<setw(20)<<outputs;
        char a = stack[stack.size()-1];
        char b = input[input.size()-1];
        //�������ƥ�䣬���Ҷ�Ϊ# 
        if(a==b&&a=='#'){
            cout<<setw(20)<<"�����ɹ�!"<<endl;
            return;
        }
        //����ƥ��
        if(a==b){
            stack.pop_back();
            input.pop_back();
            cout<<setw(20)<<a<<" ƥ��"<<endl;
        }else if(table[NotTerminal.find(a)][Terminal.find(b)]>0){
            //�����ֵ
            int index = table[NotTerminal.find(a)][Terminal.find(b)]-1;
            stack.pop_back();
            if(g[index].right!="@"){
                for(int i=g[index].right.length()-1;i>=0;i--){
                    stack.push_back(g[index].right[i]);
                }
            }
            cout<<setw(20)<<g[index].left<<"->"<<g[index].right<<endl;
        }else{
            cout<<setw(20)<<"����������LL(1)���ķ���"<<endl;
            return;
        }
    }
}

void getOutput()
{
	cout<<"�ս����"<<Terminal<<endl;
	cout<<"���ս����"<<NotTerminal<<endl;
	set<char>::iterator iter;
	cout<<"First���ϣ�"<<endl;
	for(int i=0;i<NotTerminal.length();i++)
	{
		getFirst(NotTerminal[i]);
		cout<<"First("<<NotTerminal[i]<<")={";
		for(iter=First[i].begin();iter!=First[i].end();iter++)
		{
			cout<<(*iter)<<",";
		}
		cout<<"}"<<endl;
	}
	cout<<"������ѡʽFirst����"<<endl; 
	for(int i=0;i<total;i++)
	{
		cout<<"First("<<g[i].left<<"->"<<g[i].right<<")={";
		for(iter=g[i].first.begin();iter!=g[i].first.end();iter++)
		{
			cout<<(*iter)<<",";
		}
		cout<<"}"<<endl;
	}
	Follow[0].insert('#');
	cout<<"Follow����"<<endl;
	for(int i=0;i<NotTerminal.length();i++)
	{
		getFollow(NotTerminal[i]);
		cout<<"Follow("<<NotTerminal[i]<<")={";
		for(iter=Follow[i].begin();iter!=Follow[i].end();iter++)
		{
			cout<<(*iter)<<",";
		}
		cout<<"}"<<endl;
	}
} 


void getOutput2()
{
		cout<<setw(10)<<"������:"<<endl; 
	cout<<setw(10);
	for(int i=0;i<Terminal.length();i++)
	cout<<Terminal[i]<<setw(5);
	cout<<endl; 
	for(int i=0;i<NotTerminal.length();i++)
	{
		cout<<NotTerminal[i]<<setw(5);
		for(int j=0;j<Terminal.length();j++)
		{
			cout<<table[i][j]<<setw(5);
		}
		cout<<endl;
	}
}

int main()
{
	ifstream read;
	read.open("LL(1)input.txt",ios::app);
	read>>total;
	for(int i=0;i<total;i++)
	{
		//getchar();
		read>>g[i].left>>g[i].right;
		cout<<g[i].left<<"->"<<g[i].right<<endl;
		if(NotTerminal.find(g[i].left)==string::npos)
		{
			NotTerminal+=g[i].left;
		}
		for(int j=0;j<g[i].right.length();j++)
		{
			if(g[i].right[j]=='@'){
				int temp=NotTerminal.find(g[i].left);
				haveNone[temp]=1;
			}
			else if(g[i].right[j]>='A'&&g[i].right[j]<='Z')
				{
					if(NotTerminal.find(g[i].right[j])==string::npos)
					{
					NotTerminal+=g[i].right[j];
					}
				}
			else if(Terminal.find(g[i].right[j])==string::npos)
				{
					Terminal+=g[i].right[j];
				}
			
		}
	}
	getOutput();
	Terminal+='#';
	getTable();
	getOutput2();
	getAnalysis("i+i*i");
	//getAnalysis("vi,i:r") ;
	return 0;
} 
