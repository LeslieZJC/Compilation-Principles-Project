#include<iostream>
#include<vector>
#include<map>
#include<cstring>
#include<stack>
using namespace std;
/*�������ʽ���﷨���ṹ*/
typedef struct {
	char formula[200];//����ʽ
	int length;
} grammarElement;
grammarElement  gramOldSet[200];//ԭʼ�ķ��Ĳ���ʽ��
typedef struct {
	char set[100];
	int length;
	bool is;
} Set;
Set firstSET[100];//first����
Set followSET[100];//follow����
string M[100][100];//Ԥ�������
vector <char> non_ter;//���ս��
vector <char> terSymbol;//�ս��
vector <char> Symbol;//����ʽ�Ҳ����з���
map<char,int> symbol;
int count;//����ʽ����
void get_first(char E);
bool is_non_ter(char E) { //�ж�E�Ƿ�Ϊ���ս��
	for(int i=0; i<non_ter.size(); i++)
		if(non_ter[i]==E)
			return true;
	return false;
}
bool is_terSymbol(char E) { //�ж�E�Ƿ�Ϊ�ս��
	for(int i=0; i<terSymbol.size(); i++)
		if(terSymbol[i]==E)
			return true;
	return false;
}
bool is_Symbol(char E) { //�ж�E�Ƿ��Ѿ�����
	for(int i=0; i<Symbol.size(); i++)
		if(Symbol[i]==E)
			return true;
	return false;
}
bool is_first(char E) { //�ж�E��first�Ƿ��Ѿ����
	int i=symbol.at(E);
	if(firstSET[i].is) {
		return true;
	} else return false;
}
bool is_follow(char E) { //�ж�E��follow�Ƿ��Ѿ����
	int i=symbol.at(E);
	if(followSET[i].is) {
		return true;
	} else return false;
}
bool is_null(char E) {//���ս��E�ܷ��Ƴ�@
	if(!firstSET[symbol.at(E)].is)
		get_first(E);
	for(int i=0; i<firstSET[symbol.at(E)].length; i++) {
		if(firstSET[symbol.at(E)].set[i]=='@')
			return true;
	}
	return false;
}
int is_have(grammarElement g,char E) { //������ʽg���Ҳ��Ƿ��з��ս��E
	for(int i=3; i<g.length; i++) { //��������ʽ���Ҳ�,���㣺�޷�������ʽ�Ҳ����ж��E
		if(g.formula[i]==E)
			return i;
	}
	return 0;
}
Set add(Set S1,Set S2) { //��F1��F2�г�@���Ԫ�صĲ�����F1��
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
Set add_null(Set S1,Set S2) { //��F1��F2�е�Ԫ�صĲ�����F1��
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
	for(int j=0; j<count; j++) { //�������в���ʽ
		if(gramOldSet[j].formula[0]==E) { //���Ҹ÷��ս�����ڵĲ���ʽ
			int x=3; //ָ�����ʽ�Ҳ��ַ���ָ��
			if(is_terSymbol(gramOldSet[j].formula[x])) { //����ʽ�Ҳ���һ���ַ�Ϊ�ս��
				Set n;
				n.is=true;
				n.length=1;
				n.set[0]=gramOldSet[j].formula[x];
				firstSET[i]=add_null(firstSET[i],n);//�����ս������first��
				//	printf("2\n");
			}
			if(is_non_ter(gramOldSet[j].formula[x])) { //����ʽ�Ҳ���һ���ַ�Ϊ���ս��
				if(gramOldSet[j].formula[x]==E) { //����ʽ�Ҳ���һ���ַ����ڵ�ǰ�ַ���������һ������ʽ
					//	printf("3\n");
					continue;
				}
				if(is_first(gramOldSet[j].formula[x])) { //��ǰ���ս����FIRST���Ƿ������
					//	printf("5\n");
					firstSET[i]=add(firstSET[i],firstSET[symbol.at(gramOldSet[j].formula[x])]);//��������
				} else {
					//	printf("6\n");
					get_first(gramOldSet[j].formula[x]);//��first��
					firstSET[i]=add(firstSET[i],firstSET[symbol.at(gramOldSet[j].formula[x])]);//��������
				}
				while(is_null(gramOldSet[j].formula[x])) { //��ǰ���ս�����Ƴ�@
					x++;//ָ������ 
					if(x>=gramOldSet[j].length) { //��ǰ�ַ����Ҳ����һ���ַ�
						//	printf("7\n");
						Set n;
						n.is=true;
						n.length=1;
						n.set[0]='@';
						firstSET[i]=add_null(firstSET[i],n);//��@����first��
						break;
					}
					if(is_terSymbol(gramOldSet[j].formula[x])){//�����ս����������ս�������ѭ�� 
						Set n;
						n.is=true;
						n.length=1;
						n.set[0]=gramOldSet[j].formula[x];
						firstSET[i]=add_null(firstSET[i],n);//��@����first��
						break; 						
						} 
					if(is_first(gramOldSet[j].formula[x])) { //��ǰ���ս����FIRST���Ƿ������
						//	printf("8\n");
						firstSET[i]=add(firstSET[i],firstSET[symbol.at(gramOldSet[j].formula[x])]);//��������
					} else {
						//	printf("9\n");
						get_first(gramOldSet[j].formula[x]);//��first��
						firstSET[i]=add(firstSET[i],firstSET[symbol.at(gramOldSet[j].formula[x])]);//��������
					}
				}
			}
		}
	}
	firstSET[i].is=true;
	//printf("%c��first�����\n",E);
}
void get_follow() {
	bool flag=true;
	for(int i=0; i<non_ter.size(); i++)//�������з��ս��
		followSET[i].length=0;
	while(flag) {
		flag=false;
		for(int i=0; i<non_ter.size(); i++) {//�������з��ս��
			char E=non_ter[i];
			if(E==gramOldSet[0].formula[0]) { //EΪ��ʼ����
				Set n;
				n.is=true;
				n.length=1;
				n.set[0]='#';
				followSET[i]=add_null(followSET[i],n);//��@����first��
			}
			for(int j=0; j<count; j++) { //�������в���ʽ
				if(is_have(gramOldSet[j],E)!=0) {//�ҳ��Ҳ�����E�Ĳ���ʽ
					int x=is_have(gramOldSet[j],E);//xΪ����ʽE��λ��
					if(x>=gramOldSet[j].length-1) { //E�ڲ���ʽ���
						if(is_follow(gramOldSet[j].formula[0])) { //�����follow����,��follow����
							//printf("0\n");
							int before=followSET[i].length;
							followSET[i]=add(followSET[i],followSET[symbol.at(gramOldSet[j].formula[0])]);
							if(before!=followSET[i].length) {
								flag=true;//����ѭ��
							}
						} else {
							//printf("1\n");
							flag=true;//����ѭ��
						}
					} else {//E���ڲ���ʽ�����
						//printf("2\n");
						//��E��ߴ���first����
						Set str;//E��Ĵ���first����
						str.length=0;
						while(x<gramOldSet[j].length-1) {
							x++;//����ʽ��ָ��������һ��
							if(is_non_ter(gramOldSet[j].formula[x])) { //���ַ��Ƿ��ս��
								//���ս���ķǿ��ַ�����first
								if(is_first(gramOldSet[j].formula[x])) { //��ǰ���ս����FIRST�������
									str=add(str,firstSET[symbol.at(gramOldSet[j].formula[x])]);//��first���ϵķ�@Ԫ�ؽ�������
								} else {
									get_first(gramOldSet[j].formula[x]);//��first��
									str=add(str,firstSET[symbol.at(gramOldSet[j].formula[x])]);//��������
								}
								if(is_null(gramOldSet[j].formula[x])) {//�÷��ս�����Ƴ�@
									if(x==gramOldSet[j].length-1) {//�����ַ������Ƴ�@
										Set n;
										n.is=true;
										n.length=1;
										n.set[0]='@';
										str=add_null(str,n);//��@����first��
									}
									continue;//����ѭ��
								} else {	//�÷��ս�������Ƴ�@
									break;//ֹͣѭ��
								}
							} else { //E��ߵ��ַ����ս��
								Set n;
								n.is=true;
								n.length=1;
								n.set[0]=gramOldSet[j].formula[x];
								str=add_null(str,n);//�����ս������first��
								break;
							}
						}
//						for(int x=0; x<str.length; x++)
//							cout<<" "<<str.set[x]<<endl;
						int before=followSET[i].length;
						//��str��first�����еķ�@Ԫ�ؼӵ�E��follow��
						followSET[i]=add(followSET[i],str);
						for(int z=0; z<str.length; z++) {
							if(str.set[z]=='@') { //str�к���@
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
	for(int i=0; i<count; i++) { //�������еĲ���ʽ
		int x=3;
		//cout<<gramOldSet[i].formula<<endl;
		//����ʽ�ұߴ���first����
		Set st;
		st.length=0;
		while(x<gramOldSet[i].length) {
			if(is_non_ter(gramOldSet[i].formula[x])) { //���ַ��Ƿ��ս��
				//���ս���ķǿ��ַ�����first
				if(is_first(gramOldSet[i].formula[x])) { //��ǰ���ս����FIRST�������
					st=add(st,firstSET[symbol.at(gramOldSet[i].formula[x])]);//��first���ϵķ�@Ԫ�ؽ�������
				} else {
					printf("error!\n");
				}
				if(is_null(gramOldSet[i].formula[x])) {//�÷��ս�����Ƴ�@
					if(x==gramOldSet[i].length-1) {//�����ַ������Ƴ�@
						Set n;
						n.is=true;
						n.length=1;
						n.set[0]='@';
						st=add_null(st,n);//��@����first��
					}
					x++;
					continue;//����ѭ��
				} else {	//�÷��ս�������Ƴ�@
					break;//ֹͣѭ��
				}
			} else { //�ַ����ս��
				Set n;
				n.is=true;
				n.length=1;
				n.set[0]=gramOldSet[i].formula[x];
				st=add_null(st,n);//�����ս������first��
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
	printf("�������ʽ��ʽΪ X->YYY.. ��@��ʾ�գ���end������\n");
	string temp_pro="";
	cin>>temp_pro;
	int i=0;
	count=0;//����ʽ�ĸ���
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
			non_ter.push_back(gramOldSet[i].formula[0]);//���ս��
			symbol.insert(pair<char,int>(gramOldSet[i].formula[0],non_ter.size()-1));
		}
		count++;
		i++;
		temp_pro="";
		cin>>temp_pro;
	}
	for(int j=0; j<Symbol.size(); j++) {
		if(!is_non_ter(Symbol[j])) {
			terSymbol.push_back(Symbol[j]);//�ս��
			symbol.insert(pair<char,int>(Symbol[j],99-(terSymbol.size()-1)));
		}
	}
	terSymbol.push_back('#');//��#д���ս��
	symbol.insert(pair<char,int>('#',99-(terSymbol.size()-1)));
	cout<<"����ʽ�ĸ�����"<<count<<endl;
	cout<<"���ս����"<<endl;
	for(int j=0; j<non_ter.size(); j++)
		cout<<non_ter[j]<<" ";
	cout<<"\n�ս����"<<endl;
	for(int j=0; j<terSymbol.size(); j++)
		cout<<terSymbol[j]<<" ";
	//��first����
	cout<<"\nfirst��:";
	for(i=0; i<non_ter.size(); i++) { //�������еķ��ս��
		get_first(non_ter[i]);
		int j=0;
		cout<<"\n"<<non_ter[i]<<":";
		while(j<firstSET[i].length) {
			printf("%c ",firstSET[i].set[j]);
			j++;
		}
	}
	//��follow��
	cout<<"\nfollow��:";
	get_follow();
	for(i=0; i<non_ter.size(); i++) {
		int j=0;
		cout<<"\n"<<non_ter[i]<<":";
		while(j<followSET[i].length) {
			printf("%c ",followSET[i].set[j]);
			j++;
		}
	}
	//���������
	table();
	printf("\n~~~~~~~Ԥ�������~~~~~~~~~\n");
	for(int j=0; j<terSymbol.size(); j++)
		cout<<"      "<<terSymbol[j]; 
	for(i=0; i<non_ter.size(); i++) {
		printf("\n%c   ",non_ter[i]);		
		//	cout<<terSymbol[j]<<": "<<M[symbol.at(non_ter[i])][symbol.at(terSymbol[j])]<<endl;
		for(int j=0; j<terSymbol.size(); j++)
			cout<<" "<<M[symbol.at(non_ter[i])][symbol.at(terSymbol[j])];
	}
	printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	//������
	cout<<"������һ���������жϣ�"<<endl;
	char in[100];
	int p=0;//ָ�����봮��ָ��
	scanf("%s",in);
	int c=0;//���봮�ĳ���
	while(in[c]!='\0')
		c++;
	//cout<<"������ַ�������Ϊ "<<c<<endl;
	int FLAG=true;
	stack <char> S;
	char TOP;//����ջջ��Ԫ�� 
	S.push('#');
	S.push(gramOldSet[0].formula[0]);//�ѿ�ʼ��ѹ��ջ 
	while(FLAG) {
		TOP=S.top();//ջ����ջ 
		S.pop();
		if(!is_non_ter(*(in+p))) {//Υ���ַ����� 
			if(!is_terSymbol(*(in+p))) {
				printf("error!\n");
				cout<<"������֪�ķ��ս�����ս��"<<endl;
				break;
			}
		}
	//	cout<<TOP<<" "<<*(in+p)<<endl;
		if(TOP=='#'){
			if(*(in+p)=='#'){
				printf("\n�ɹ���Լ\n");
				break;
			}
			else{
				printf("error!\n");
				break;
			}
		} 
		else if(TOP==*(in+p)){//����ս��ƥ�䣬�������һ�����봮 
			p++;
			TOP=S.top();
		}
		else if(M[symbol.at(TOP)][symbol.at(*(in+p))]!="error") {//���ڹ�Լʽ 
			string temp=M[symbol.at(TOP)][symbol.at(*(in+p))];
			cout<<"����ʽ������ջ:                                "<<temp<<endl;
			for(int z=temp.length()-1; z>2; z--) {
				if(temp[z]!='@'){ //��Ϊ�գ��������ջ 			
					S.push(temp[z]);
				}
				else{
					break;//�������� 
				}
			}
		} else {//�����ڹ�Լʽ������ 
			printf("error!!\n");
			printf("�����ڹ�Լʽ\n");
			break;
		}				
		cout<<"ʣ�����봮��                      "<<in+p<<endl;
		stack <char> SS=S;
		printf("ջ��ʣ��Ԫ�أ�        ");
		while(!SS.empty()){
			printf("%c",SS.top());
			SS.pop();
		}
		printf("\n");
	}
}

 

