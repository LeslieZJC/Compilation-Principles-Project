/*
0���ո�  1����ĸ/�»���  3������ĸ/����/�»���  4��0-9
5��.  6����0-9  7��=  8����=  9��<  10��>  11��!  12��+/-/%  13��/  14����/��*
15��*  16��&  17����&  18��|  19����|  20��,  21��[  22��]  23��(  24��)  25��{
26��}  27��;  28��/n  29��#  30���޷�ʶ��ķ��� 
״̬Ϊ0ʱΪ�� 
classֵ�� 
if	1	�գ�^��	>=	19	�գ�^��
else	2	�գ�^��	<	20	�գ�^��
for	3	�գ�^��	<=	21	�գ�^��
while	4	�գ�^��	!=	22	�գ�^��
break	5	�գ�^��	==	23	�գ�^��
return	6	�գ�^��	!	24	�գ�^��
continue	7	�գ�^��	&&	25	�գ�^��
float	8	�գ�^��	||	26	�գ�^��
int	9	�գ�^��	,	27	�գ�^��
char	10	�գ�^��	=	28	�գ�^��
��ʶ��	11	���ֱ��ַ	[	29	�գ�^��
������	12	�������ַ	]	30	�գ�^��
��ʵ��	12	�������ַ	(	31	�գ�^��
��	12	�������ַ	)	32	�գ�^��
+	13	�գ�^��	{	33	�գ�^��
-	14	�գ�^��	}	34	�գ�^��
*	15	�գ�^��	;	35	�գ�^��
/	16	�գ�^��	.	36	�գ�^��
%	17	�գ�^��	���з�	37	�գ�^��
>	18	�գ�^��	�ļ�����	38	�գ�^��
|  39  �գ�^��  &  40  �գ�^�� 
!  41  �գ�^��  //  42  ע��  43 ���� 44 
*/
#include <bits/stdc++.h>
#define maxn 1001
using namespace std;
int mov[maxn][maxn],len,len_token=1,len_notes=1,len_namel=1,len_constl=1,len_mistake=1;//״̬ת�Ʊ��ı����� 
int e[maxn],sign[maxn];//classֵ 
char in[maxn];//����� 
string nameL[maxn],constL[maxn],notes[maxn],mistake[maxn],interr[maxn];//���ֱ�������ע�ͱ������ 
struct node{
	int encode;//���� 
	string seman;//���ԣ���ַ�� 
}token[maxn];//token��������еĵ��� 
void init();//��ʼ��״̬ 
int judge(char x,char f,int p,int zt);//�ж�������ַ�����ʲô״̬ 
int is_letter();//�ж��ǲ��Ǳ�ʶ�� 
void scan();//�����ļ�
void ccout();//����ļ� 
int is_keyword(string x);//�ж��ǲ��ǹؼ��� 
void get_token(int l,string t,int class_);//��token��
void dfa();//�Զ��� 
void print_token();//��ӡtoken�� 
string sub_string(int l,int r);//�����ʻ��ַ���ȡ���� 
void find_wrong();//���﷨�Ĵ��� 
int main(){ 
	scan();
	init();
	dfa();
	ccout();
	print_token();
	return 0;
}
void init(){
	memset(mov,0,sizeof mov);
	memset(e,0,sizeof e);
	memset(sign,0,sizeof sign);
	mov[1][0]=1,mov[1][1]=2,mov[2][1]=2,mov[2][3]=3,mov[1][4]=4,mov[4][4]=4,mov[4][5]=5,mov[5][32]=45;
	mov[5][4]=5,mov[5][6]=6, mov[4][31]=7, mov[1][5]=8,mov[1][7]=9,mov[9][7]=10,mov[9][8]=11,mov[2][4]=2,mov[4][3]=7;
	mov[1][9]=12,mov[12][7]=13,mov[12][8]=14,mov[1][10]=15,mov[15][7]=16,mov[15][8]=17,mov[1][11]=18,mov[4][1]=44;
	mov[18][7]=19,mov[18][8]=20,mov[1][12]=21,mov[1][13]=22,mov[22][14]=23,mov[22][13]=24,mov[22][15]=25;
	mov[1][15]=41,mov[1][16]=26,mov[26][16]=27,mov[26][17]=28,mov[1][18]=29,mov[29][18]=30,mov[29][19]=31;
	mov[1][20]=32,mov[1][21]=33,mov[1][22]=34,mov[1][23]=35,mov[1][24]=36,mov[1][25]=37,mov[1][26]=38,mov[1][30]=43;
	mov[1][27]=39,mov[1][28]=40,mov[1][29]=42,e[3]=1,e[6]=2,e[7]=3,e[8]=36,e[10]=23,e[11]=28,e[13]=21,e[14]=20;
	e[16]=19,e[17]=18,e[19]=22,e[20]=41,e[21]=13,e[23]=16,e[24]=42,e[25]=43,e[27]=25,e[28]=40,e[30]=26,e[31]=39,e[45]=46;
	e[32]=27,e[33]=29,e[34]=30,e[35]=31,e[36]=32,e[37]=33,e[38]=34,e[39]=35,e[40]=37,e[41]=15,e[42]=38,e[43]=44,e[44]=45;
}
void scan(){
	cout<<"--------���ڵ�������--------"<<endl;
	ifstream f{"test.txt",ios::in};
  	stringstream ss;
  	ss<<f.rdbuf();
  	string data=ss.str();
  	for(int i=0;i<data.length();i++)
  		in[i+1]=data[i];
	in[data.length()+1]=in[0]='#';
	len=data.length()+1;
	cout<<data<<endl;
	f.close();
	cout<<"---------�������------------"<<endl;
}
void dfa(){
	int state=1,ne,cs=1;
	int pos;
	for(int i=1;i<=len;i++){
		int t=judge(in[i],in[i-1],i,state);
		sign[i]=1;
		if(mov[state][t]!=0){
			ne=mov[state][t];
			if(state==cs&&ne!=cs) pos=i;//��¼λ�� 
			if(e[ne]==0){
				state=ne;
			}//��0����Ϊ������	
			else{
				string temp;
				if(ne==24){
					for(int j=i+1;j<=len;j++){
						if(in[j]!='\n') temp+=in[j];
						else{
							i=j-1;
							break;
						}
					}
					notes[len_notes++]=temp;
				}else if(ne==25){
					for(int j=i+1;j<=len;j++){
						if(in[j]=='*'&&in[j+1]=='/'){
							i=j;
							break;
						}
						else temp+=in[j];
					}
					notes[len_notes++]=temp;
				}//����ע��
				else if(ne==43){
					if(in[i]!='\t'){
						mistake[len_mistake]=in[i];
						interr[len_mistake++]="�Ƿ�����";
					}	
				}
				else if(ne==44){
					int j=pos;
					while(true){
						if((in[j]<='9'&&in[j]>='0')||in[j]=='_'||in[j]=='.'||(in[j]<='Z'&&in[j]>='A')||(in[j]<='z'&&in[j]>='a')){
							temp+=in[j];
							j++;
						}
						else{
							i=j-1;
							break;
						}
					}
					mistake[len_mistake]=temp;
					interr[len_mistake++]="�Ƿ���ʶ��";
				}
				else if(ne==45){
					int j=pos;
					while(true){
						if((in[j]<='9'&&in[j]>='0')||in[j]=='_'||in[j]=='.'||(in[j]<='Z'&&in[j]>='A')||(in[j]<='z'&&in[j]>='a')){
							temp+=in[j];
							j++;
						}
						else{
							i=j-1;
							break;
						}
					}
					mistake[len_mistake]=temp;
					interr[len_mistake++]="�Ƿ�����";
				}
				if(ne==3||ne==6||ne==7||ne==11||ne==14||ne==17||ne==20||ne==23||ne==28||ne==31) i=i-1;  
				temp=sub_string(pos,i);
				get_token(len_token,temp,e[ne]);
				len_token++;
				state=cs;
			}	
		}
		else continue;
	}
}
int judge(char x,char f,int p,int zt){
	int flag=-1;
	if((x<='Z'&&x>='A')||(x<='z'&&x>='a')||x=='_') flag=1;
	if(x<='9'&&x>='0') flag=4;
	if((f<='Z'&&f>='A')||(f<='z'&&f>='a')||f=='_'){
		if((x>'z'||(x<'a'&&x>'Z')||(x<'A'&&x>'9')||x<'0')&&x!='_'&&sign[p]!=1)
			return 3; 
	}
	if(f<='9'&&f>='0'){
		if(zt==4){
			if((x>'z'||(x<'a'&&x>'Z')||(x<'A'&&x>'9')||x<'0')&&x!='_'&&sign[p]!=1&&x!='.')
				return 31;
		}
		if(zt==5){
			if((x>'z'||(x<'a'&&x>'Z')||(x<'A'&&x>'9')||x<'0')&&x!='_'&&sign[p]!=1&&x!='.')
				return 6;
			else if(flag!=4)
				return 32;
		}
	}
	if(f=='='||f=='<'||f=='>'||f=='!'){
		if(x!='='&&(zt==9||zt==12||zt==15||zt==18)) return 8;
	}
	if(x=='+'||x=='-'||x=='%') flag=12;
	if(f=='/'){
		if(x!='/'&&x!='*'&&zt==22) return 14;
	}
	if(f=='&'){
		if(x!='&'&&zt==26) return 17;
	}
	if(f=='|'){
		if(x!='|'&&zt==29) return 19;
	}
	switch(x){
		case ' ':flag=0;break;
		case '.':flag=5;break;
		case '=':flag=7;break;
		case '<':flag=9;break;
		case '>':flag=10;break;
		case '!':flag=11;break;
		case '/':flag=13;break;
		case '*':flag=15;break;
		case '&':flag=16;break;
		case '|':flag=18;break;
		case ',':flag=20;break;
		case '[':flag=21;break;
		case ']':flag=22;break;
		case '(':flag=23;break;
		case ')':flag=24;break;
		case '{':flag=25;break;
		case '}':flag=26;break;
		case ';':flag=27;break;
		case '\n':flag=28;break;
		case '#':flag=29;break;
	}
	if(flag==-1) flag=30; 
	return flag;
}
string sub_string(int l,int r){
	string cnt;
	for(int i=l;i<=r;i++)
		cnt+=in[i];
	return cnt;
}
void get_token(int l,string t,int class_){
	int flag;
	 switch(class_){
	 	case 0:break; 
	 	case 1:
	 		if(is_keyword(t)!=0){
	 			token[l].encode=is_keyword(t);
	 			token[l].seman=t;
	 		}
	 		else{
	 			flag=0; 
	 			token[l].encode=11;
	 			for(int i=1;i<len_namel;i++){
	 				if(nameL[i].compare(t)==0){
	 					flag=i;//����ͬ�ı�ʶ�� 
						break;
					}
				}
				if(flag==0){
					nameL[len_namel]=t;
		 			token[l].seman=to_string(len_namel);//������תΪ�ַ����ٴ��ȥ 
		 			len_namel++;
				}
	 			else{
	 				token[l].seman=to_string(flag);	
				}
			}
			break;
		case 2:
		case 3:
			flag=0; 
	 		token[l].encode=12;
	 		for(int i=1;i<len_constl;i++){
	 			if(constL[i].compare(t)==0){
	 				flag=i;//����ͬ�ĳ��� 
					break;
				}
			}
			if(flag==0){
				constL[len_constl]=t;
				token[l].seman=to_string(len_constl);
				len_constl++;
			}
			else{
				token[l].seman=to_string(flag);
			}
			break;
		case 13:
			if(t=="+"){
				token[l].encode=13;
				token[l].seman=t;
			}
			else if(t=="-"){
				token[l].encode=14;
				token[l].seman=t;
			}
			else if(t=="%"){
				token[l].encode=17;
				token[l].seman=t;
			}
			break;
		case 42:case 43:case 44:case 45:case 46:len_token--;break;
		default:
			token[l].encode=class_;
			token[l].seman=t;
	 }
}
int is_keyword(string x){
	if(x=="if") return 1;
	if(x=="else") return 2;
	if(x=="for") return 3;
	if(x=="while") return 4;
	if(x=="break") return 5;
	if(x=="return") return 6;
	if(x=="continue") return 7;
	if(x=="float") return 8;
	if(x=="int") return 9;
	if(x=="char") return 10;
	return 0;
}
void ccout(){
	ofstream outfile;
	outfile.open("nameL.txt");
	outfile<<"�±�\t����\n";
	for(int i=1;i<=len_namel-1;i++){
		outfile<<i<<":\t"<<nameL[i]<<endl;
	}
	outfile.close();
	outfile.open("constL.txt");
	outfile<<"�±�\t����\n";
	for(int i=1;i<=len_constl-1;i++){
		outfile<<i<<":\t"<<constL[i]<<endl;
	}
	outfile.close();
	outfile.open("token.txt");
	for(int i=1;i<=len_token-1;i++){
		if(token[i].encode==11){
			int pos=stoi(token[i].seman);
			string t="[\t"+to_string(token[i].encode)+"\t"+nameL[pos]+"\t]\n";
			outfile<<t;
		}
		else if(token[i].encode==12){
			int pos=stoi(token[i].seman);
			string t="[\t"+to_string(token[i].encode)+"\t"+constL[pos]+"\t]\n";
			outfile<<t;
		}
		else{
			if(token[i].encode==37){
				string t="[\t"+to_string(token[i].encode)+"\t\\n\t]\n";
				outfile<<t;
			}
			else{
				string t="[\t"+to_string(token[i].encode)+"\t"+token[i].seman+"\t]\n";
				outfile<<t;
			}
		}
		
	}
	outfile.close();
	outfile.open("ע�ͱ�.txt");
	for(int i=1;i<=len_notes-1;i++){
		string t="[ע��"+to_string(i)+":"+notes[i]+"]\n";
		outfile<<t;		
	}
	outfile.close();
	outfile.open("error��.txt");
	for(int i=1;i<=len_mistake-1;i++){
		string t="[����"+to_string(i)+":"+mistake[i]+"]:"+interr[i]+"\n";
		outfile<<t;		
	}
	outfile.close();
}
void print_token(){
	cout<<"---------���ڴ�ӡtoken��------------"<<endl;
	for(int i=1;i<=len_token-1;i++){
		if(token[i].encode==11){
			int pos=stoi(token[i].seman);
			cout<<"[\t"<<token[i].encode<<"\t"<<nameL[pos]<<"\t]"<<endl;
		}
		else if(token[i].encode==12){
			int pos=stoi(token[i].seman);
			cout<<"[\t"<<token[i].encode<<"\t"<<constL[pos]<<"\t]"<<endl;
		}
		else{
			if(token[i].encode==37)
				cout<<"[\t"<<token[i].encode<<"\t\\n\t]"<<endl;
			else
				cout<<"[\t"<<token[i].encode<<"\t"<<token[i].seman<<"\t]"<<endl;
		}
	}
	cout<<"------------���ڴ�ӡ������--------------"<<endl;
	for(int i=1;i<len_constl;i++){
		cout<<"constL["<<i<<"]="<<constL[i]<<endl;
	}
	cout<<"------------���ڴ�ӡ��ʶ����--------------"<<endl;
	for(int i=1;i<len_namel;i++){
		cout<<"nameL["<<i<<"]="<<nameL[i]<<endl;
	}
	cout<<"------------���ڴ�ӡע�ͱ�--------------"<<endl;
	for(int i=1;i<len_notes;i++){
		cout<<"notes["<<i<<"]="<<notes[i]<<endl;
	}
	cout<<"------------���ڴ�ӡ�����--------------"<<endl;
	for(int i=1;i<len_mistake;i++){
		cout<<"mistake["<<i<<"]="<<mistake[i]<<":"<<interr[i]<<endl;
	}
}
