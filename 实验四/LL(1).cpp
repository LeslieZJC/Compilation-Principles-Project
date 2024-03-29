#include<iostream>
#include<vector>
#include<map>
#include<cstring>
#include<stack>
using namespace std;
/*定义产生式的语法集结构*/
typedef struct {
	char formula[200];//产生式
	int length;
} grammarElement;
grammarElement  gramOldSet[200];//原始文法的产生式集
typedef struct {
	char set[100];
	int length;
	bool is;
} Set;
Set firstSET[100];//first集合
Set followSET[100];//follow集合
string M[100][100];//预测分析表
vector <char> non_ter;//非终结符
vector <char> terSymbol;//终结符
vector <char> Symbol;//产生式右侧所有符号
map<char,int> symbol;
int count;//产生式个数
void get_first(char E);
bool is_non_ter(char E) { //判断E是否为非终结符
	for(int i=0; i<non_ter.size(); i++)
		if(non_ter[i]==E)
			return true;
	return false;
}
bool is_terSymbol(char E) { //判断E是否为终结符
	for(int i=0; i<terSymbol.size(); i++)
		if(terSymbol[i]==E)
			return true;
	return false;
}
bool is_Symbol(char E) { //判断E是否已经存在
	for(int i=0; i<Symbol.size(); i++)
		if(Symbol[i]==E)
			return true;
	return false;
}
bool is_first(char E) { //判断E的first是否已经求出
	int i=symbol.at(E);
	if(firstSET[i].is) {
		return true;
	} else return false;
}
bool is_follow(char E) { //判断E的follow是否已经求出
	int i=symbol.at(E);
	if(followSET[i].is) {
		return true;
	} else return false;
}
bool is_null(char E) {//非终结符E能否推出@
	if(!firstSET[symbol.at(E)].is)
		get_first(E);
	for(int i=0; i<firstSET[symbol.at(E)].length; i++) {
		if(firstSET[symbol.at(E)].set[i]=='@')
			return true;
	}
	return false;
}
int is_have(grammarElement g,char E) { //检测产生式g的右部是否含有非终结符E
	for(int i=3; i<g.length; i++) { //遍历产生式的右部,不足：无法检测产生式右部含有多个E
		if(g.formula[i]==E)
			return i;
	}
	return 0;
}
Set add(Set S1,Set S2) { //将F1和F2中除@外的元素的并集添到F1中
	if(S1.length==0) {
		for(int i=0; i<S2.length; i++) {
			if(S2.set[i]=='@') {
				S2.set[i]=S2.set[S2.length-1];
				S2.length--;
			}
		}
		return S2;
	}

	for(int i=0; i<S2.length; i++)
		for(int j=0; j<S1.length; j++) {
			if(S2.set[i]==S1.set[j])
				break;
			if(j==S1.length-1&&S2.set[i]!='@') {
				S1.set[S1.length]=S2.set[i];
				S1.length++;
			}
		}
	return S1;
}
Set add_null(Set S1,Set S2) { //将F1和F2中的元素的并集添到F1中
	if(S1.length==0) {
		return S2;
	}
	for(int i=0; i<S2.length; i++)
		for(int j=0; j<S1.length; j++) {
			if(S2.set[i]==S1.set[j])
				break;
			if(j==S1.length-1) {
				S1.set[S1.length]=S2.set[i];
				S1.length++;
			}
		}
	return S1;
}
void get_first(char E) {
	int i=symbol.at(E);
	if(firstSET[i].is)
		return;
	firstSET[i].length=0;
	for(int j=0; j<count; j++) { //遍历所有产生式
		if(gramOldSet[j].formula[0]==E) { //查找该非终结符所在的产生式
			int x=3; //指向产生式右部字符的指针
			if(is_terSymbol(gramOldSet[j].formula[x])) { //产生式右部第一个字符为终结符
				Set n;
				n.is=true;
				n.length=1;
				n.set[0]=gramOldSet[j].formula[x];
				firstSET[i]=add_null(firstSET[i],n);//将该终结符加入first集
				//	printf("2\n");
			}
			if(is_non_ter(gramOldSet[j].formula[x])) { //产生式右部第一个字符为非终结符
				if(gramOldSet[j].formula[x]==E) { //产生式右部第一个字符等于当前字符，跳到下一条产生式
					//	printf("3\n");
					continue;
				}
				if(is_first(gramOldSet[j].formula[x])) { //当前非终结符其FIRST集是否已求出
					//	printf("5\n");
					firstSET[i]=add(firstSET[i],firstSET[symbol.at(gramOldSet[j].formula[x])]);//交集并入
				} else {
					//	printf("6\n");
					get_first(gramOldSet[j].formula[x]);//求first集
					firstSET[i]=add(firstSET[i],firstSET[symbol.at(gramOldSet[j].formula[x])]);//交集并入
				}
				while(is_null(gramOldSet[j].formula[x])) { //当前非终结符能推出@
					x++;//指针右移 
					if(x>=gramOldSet[j].length) { //当前字符是右部最后一个字符
						//	printf("7\n");
						Set n;
						n.is=true;
						n.length=1;
						n.set[0]='@';
						firstSET[i]=add_null(firstSET[i],n);//将@加入first集
						break;
					}
					if(is_terSymbol(gramOldSet[j].formula[x])){//若是终结符，加入该终结符后结束循环 
						Set n;
						n.is=true;
						n.length=1;
						n.set[0]=gramOldSet[j].formula[x];
						firstSET[i]=add_null(firstSET[i],n);//将@加入first集
						break; 						
						} 
					if(is_first(gramOldSet[j].formula[x])) { //当前非终结符其FIRST集是否已求出
						//	printf("8\n");
						firstSET[i]=add(firstSET[i],firstSET[symbol.at(gramOldSet[j].formula[x])]);//交集并入
					} else {
						//	printf("9\n");
						get_first(gramOldSet[j].formula[x]);//求first集
						firstSET[i]=add(firstSET[i],firstSET[symbol.at(gramOldSet[j].formula[x])]);//交集并入
					}
				}
			}
		}
	}
	firstSET[i].is=true;
	//printf("%c的first已求出\n",E);
}
void get_follow() {
	bool flag=true;
	for(int i=0; i<non_ter.size(); i++)//遍历所有非终结符
		followSET[i].length=0;
	while(flag) {
		flag=false;
		for(int i=0; i<non_ter.size(); i++) {//遍历所有非终结符
			char E=non_ter[i];
			if(E==gramOldSet[0].formula[0]) { //E为开始符号
				Set n;
				n.is=true;
				n.length=1;
				n.set[0]='#';
				followSET[i]=add_null(followSET[i],n);//将@加入first集
			}
			for(int j=0; j<count; j++) { //遍历所有产生式
				if(is_have(gramOldSet[j],E)!=0) {//找出右部含有E的产生式
					int x=is_have(gramOldSet[j],E);//x为产生式E的位置
					if(x>=gramOldSet[j].length-1) { //E在产生式最后
						if(is_follow(gramOldSet[j].formula[0])) { //已求出follow集合,则follow并入
							//printf("0\n");
							int before=followSET[i].length;
							followSET[i]=add(followSET[i],followSET[symbol.at(gramOldSet[j].formula[0])]);
							if(before!=followSET[i].length) {
								flag=true;//继续循环
							}
						} else {
							//printf("1\n");
							flag=true;//继续循环
						}
					} else {//E不在产生式的最后
						//printf("2\n");
						//求E后边串的first集合
						Set str;//E后的串的first集合
						str.length=0;
						while(x<gramOldSet[j].length-1) {
							x++;//产生式的指针往后移一个
							if(is_non_ter(gramOldSet[j].formula[x])) { //该字符是非终结符
								//该终结符的非空字符并入first
								if(is_first(gramOldSet[j].formula[x])) { //当前非终结符其FIRST集已求出
									str=add(str,firstSET[symbol.at(gramOldSet[j].formula[x])]);//其first集合的非@元素交集并入
								} else {
									get_first(gramOldSet[j].formula[x]);//求first集
									str=add(str,firstSET[symbol.at(gramOldSet[j].formula[x])]);//交集并入
								}
								if(is_null(gramOldSet[j].formula[x])) {//该非终结符能推出@
									if(x==gramOldSet[j].length-1) {//所有字符都能推出@
										Set n;
										n.is=true;
										n.length=1;
										n.set[0]='@';
										str=add_null(str,n);//将@加入first集
									}
									continue;//继续循环
								} else {	//该非终结符不能推出@
									break;//停止循环
								}
							} else { //E后边的字符是终结符
								Set n;
								n.is=true;
								n.length=1;
								n.set[0]=gramOldSet[j].formula[x];
								str=add_null(str,n);//将该终结符加入first集
								break;
							}
						}
//						for(int x=0; x<str.length; x++)
//							cout<<" "<<str.set[x]<<endl;
						int before=followSET[i].length;
						//把str的first集合中的非@元素加到E的follow中
						followSET[i]=add(followSET[i],str);
						for(int z=0; z<str.length; z++) {
							if(str.set[z]=='@') { //str中含有@
								followSET[i]=add(followSET[i],followSET[symbol.at(gramOldSet[j].formula[0])]);
							}
						}
						if(before!=followSET[i].length) {
							flag=true;
						}
					}
				}
			}
			followSET[i].is=true;
		}

	}
}
void table() {
	for(int i=0; i<count; i++) { //遍历所有的产生式
		int x=3;
		//cout<<gramOldSet[i].formula<<endl;
		//产生式右边串的first集合
		Set st;
		st.length=0;
		while(x<gramOldSet[i].length) {
			if(is_non_ter(gramOldSet[i].formula[x])) { //该字符是非终结符
				//该终结符的非空字符并入first
				if(is_first(gramOldSet[i].formula[x])) { //当前非终结符其FIRST集已求出
					st=add(st,firstSET[symbol.at(gramOldSet[i].formula[x])]);//其first集合的非@元素交集并入
				} else {
					printf("error!\n");
				}
				if(is_null(gramOldSet[i].formula[x])) {//该非终结符能推出@
					if(x==gramOldSet[i].length-1) {//所有字符都能推出@
						Set n;
						n.is=true;
						n.length=1;
						n.set[0]='@';
						st=add_null(st,n);//将@加入first集
					}
					x++;
					continue;//继续循环
				} else {	//该非终结符不能推出@
					break;//停止循环
				}
			} else { //字符是终结符
				Set n;
				n.is=true;
				n.length=1;
				n.set[0]=gramOldSet[i].formula[x];
				st=add_null(st,n);//将该终结符加入first集
				break;
			}
			x++;
		}

		for(int j=0; j<st.length; j++) {
			if(st.set[j]=='@') {
				for(int z=0; z<followSET[symbol.at(gramOldSet[i].formula[0])].length; z++) {
					int x=symbol.at(gramOldSet[i].formula[0]);
					int y=symbol.at(followSET[symbol.at(gramOldSet[i].formula[0])].set[z]);
					M[x][y]=gramOldSet[i].formula;
				}
			} else {
				int x=symbol.at(gramOldSet[i].formula[0]);
				int y=symbol.at(st.set[j]);
//				printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
//				for(int c=0; c<terSymbol.size(); c++)
//					cout<<terSymbol[c]<<": "<<M[x][symbol.at(terSymbol[c])]<<"{"<<x<<" "<<symbol.at(terSymbol[c])<<endl;
				M[x][y]=gramOldSet[i].formula;
			}
		}

	}

}
void init() {
	non_ter.clear();
	terSymbol.clear();
	Symbol.clear();
	symbol.clear();
	for(int z=0; z<100; z++) {
		memset(firstSET[z].set, 0, sizeof firstSET[z].set);
		memset(followSET[z].set, 0, sizeof followSET[z].set);
		firstSET[z].is=false;
		followSET[z].is=false;
		for(int y=0; y<100; y++)
			M[z][y]="error";
	}
}
int main() {
	init();
	printf("输入产生式格式为 X->YYY.. ，@表示空，以end结束：\n");
	string temp_pro="";
	cin>>temp_pro;
	int i=0;
	count=0;//产生式的个数
	while(temp_pro!="end") {
		int j;
		for(j=0; j<temp_pro.length(); j++) {
			gramOldSet[i].formula[j] = temp_pro[j];
			if(j>=3&&!is_Symbol(gramOldSet[i].formula[j]))
				Symbol.push_back(gramOldSet[i].formula[j]);
		}
		//gramOldSet[i].formula[j] = '\0';
		gramOldSet[i].length=temp_pro.length();
		if(!is_non_ter(gramOldSet[i].formula[0])) {
			non_ter.push_back(gramOldSet[i].formula[0]);//非终结符
			symbol.insert(pair<char,int>(gramOldSet[i].formula[0],non_ter.size()-1));
		}
		count++;
		i++;
		temp_pro="";
		cin>>temp_pro;
	}
	for(int j=0; j<Symbol.size(); j++) {
		if(!is_non_ter(Symbol[j])) {
			terSymbol.push_back(Symbol[j]);//终结符
			symbol.insert(pair<char,int>(Symbol[j],99-(terSymbol.size()-1)));
		}
	}
	terSymbol.push_back('#');//把#写入终结符
	symbol.insert(pair<char,int>('#',99-(terSymbol.size()-1)));
	cout<<"产生式的个数："<<count<<endl;
	cout<<"非终结符："<<endl;
	for(int j=0; j<non_ter.size(); j++)
		cout<<non_ter[j]<<" ";
	cout<<"\n终结符："<<endl;
	for(int j=0; j<terSymbol.size(); j++)
		cout<<terSymbol[j]<<" ";
	//求first集合
	cout<<"\nfirst集:";
	for(i=0; i<non_ter.size(); i++) { //遍历所有的非终结符
		get_first(non_ter[i]);
		int j=0;
		cout<<"\n"<<non_ter[i]<<":";
		while(j<firstSET[i].length) {
			printf("%c ",firstSET[i].set[j]);
			j++;
		}
	}
	//求follow集
	cout<<"\nfollow集:";
	get_follow();
	for(i=0; i<non_ter.size(); i++) {
		int j=0;
		cout<<"\n"<<non_ter[i]<<":";
		while(j<followSET[i].length) {
			printf("%c ",followSET[i].set[j]);
			j++;
		}
	}
	//构造分析表
	table();
	printf("\n~~~~~~~预测分析表~~~~~~~~~\n");
	for(int j=0; j<terSymbol.size(); j++)
		cout<<"      "<<terSymbol[j]; 
	for(i=0; i<non_ter.size(); i++) {
		printf("\n%c   ",non_ter[i]);		
		//	cout<<terSymbol[j]<<": "<<M[symbol.at(non_ter[i])][symbol.at(terSymbol[j])]<<endl;
		for(int j=0; j<terSymbol.size(); j++)
			cout<<" "<<M[symbol.at(non_ter[i])][symbol.at(terSymbol[j])];
	}
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	//程序检测
	cout<<"请输入一段语句进行判断："<<endl;
	char in[100];
	int p=0;//指向输入串的指针
	scanf("%s",in);
	int c=0;//输入串的长度
	while(in[c]!='\0')
		c++;
	//cout<<"输入的字符串长度为 "<<c<<endl;
	int FLAG=true;
	stack <char> S;
	char TOP;//分析栈栈顶元素 
	S.push('#');
	S.push(gramOldSet[0].formula[0]);//把开始符压入栈 
	while(FLAG) {
		TOP=S.top();//栈顶出栈 
		S.pop();
		if(!is_non_ter(*(in+p))) {//违法字符报错 
			if(!is_terSymbol(*(in+p))) {
				printf("error!\n");
				cout<<"不是已知的非终结符或终结符"<<endl;
				break;
			}
		}
	//	cout<<TOP<<" "<<*(in+p)<<endl;
		if(TOP=='#'){
			if(*(in+p)=='#'){
				printf("\n成功规约\n");
				break;
			}
			else{
				printf("error!\n");
				break;
			}
		} 
		else if(TOP==*(in+p)){//如果终结符匹配，则读进下一个输入串 
			p++;
			TOP=S.top();
		}
		else if(M[symbol.at(TOP)][symbol.at(*(in+p))]!="error") {//存在归约式 
			string temp=M[symbol.at(TOP)][symbol.at(*(in+p))];
			cout<<"产生式逆序入栈:                                "<<temp<<endl;
			for(int z=temp.length()-1; z>2; z--) {
				if(temp[z]!='@'){ //不为空，推入分析栈 			
					S.push(temp[z]);
				}
				else{
					break;//否则不推入 
				}
			}
		} else {//不存在归约式，报错 
			printf("error!!\n");
			printf("不存在归约式\n");
			break;
		}				
		cout<<"剩余输入串：                      "<<in+p<<endl;
		stack <char> SS=S;
		printf("栈内剩余元素：        ");
		while(!SS.empty()){
			printf("%c",SS.top());
			SS.pop();
		}
		printf("\n");
	}
}

 

