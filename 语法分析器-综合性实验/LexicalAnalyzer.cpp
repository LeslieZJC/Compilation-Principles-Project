#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MaxSize 1000
#define MaxLen 100
#define MaxStr 10
#define inDetail 1
typedef struct Token{
    int Class;
    char *Seman;
    int index;
}Token;
typedef struct Annotation{
    int startLine;
    int endLine;
    char content[MaxLen];
}AnnoL;
typedef struct ErrorList{
    int lline;
    char* type;
    int pos;
    char* tip;
}ErrorL;
char NameL[MaxLen][MaxStr];
char ConstL[MaxLen][MaxStr];
char Data[MaxSize];
Token token[MaxLen];
AnnoL annoL[MaxLen];
ErrorL errorL[MaxLen];
char WordL[][MaxStr]={"if","else","for","while","break","return","continue","float","int","char","��ʶ��","����","+","-","*","/","%",
                      ">",">=","<","<=","!=","==","!","&&","||",",","=","[","]","(",")","{","}",";",".","���з�","��ֹ��"};
char KeyWord[][MaxStr]={"if","else","for","while","break","return","continue","float","int","char"};
int i,k,cl,nl,al,el,lline,EndFlag=0;
// i��¼��ǰ��ȡ��Դ�����е��ַ�
// k��¼��ǰtoken���е�Ԫ�ظ���
// cl��¼��ǰConstL���е�Ԫ�ظ���
// nl��¼��ǰNameL���е�Ԫ�ظ���
// al��¼��ǰannoL���е�ע������
// el��¼����errorL���еĴ�����Ϣ����
//���ļ����ȡԴ����
int getData_byFile(char Data[]){
    int cf = 0;
    FILE *fp;
    fp = fopen("Src.txt","r");
    if(fp == NULL){
        printf("\n���ܴ�!");
        exit(1);
    }
    while(fscanf(fp, "%c", &Data[cf]) != EOF) //��ȡ���ݵ����飬ֱ���ļ���β(����EOF)
        cf++;
    fclose(fp);
    Data[cf]='#';   //��Դ�����ļ���β����һ��������#
    return cf;
}
//��Token���浽�ļ���
void saveToken_byFile(){
    FILE *fp;
    fp = fopen("token.txt","wt");
    if(fp == NULL){
        printf("\n���ܴ�!");
        exit(1);
    }
    fprintf(fp,"%-5s  %-10s %-10s %-10s\n","���","����","����","��ַ");
    for(int j=0;j<k;j++){
        if(token[j].Class==11 || token[j].Class==12)fprintf(fp,"%-5d %-10s %-10d %-10d\n",j,token[j].Seman,token[j].Class,token[j].index);
        else  fprintf(fp,"%-5d %-10s %-10d %-10d\n",j,WordL[token[j].Class-1],token[j].Class,-1);
    }
    fclose(fp);
}
void saveConstL_byFile(){
    FILE *fp;
    fp = fopen("ConstL.txt","wt");
    if(fp == NULL){
        printf("\n���ܴ�!");
        exit(1);
    }
    fprintf(fp,"%-5s  %-10s\n","Index","Const");
    for(int j=0;j<cl;j++){
        fprintf(fp,"%-5d  %-10s\n",j,ConstL[j]);
    }
    fclose(fp);
}
void saveNamwL_byFile(){
    FILE *fp;
    fp = fopen("NameL.txt","wt");
    if(fp == NULL){
        printf("\n���ܴ�!");
        exit(1);
    }
    fprintf(fp,"%-5s  %-10s\n","Index","Name");
    for(int j=0;j<nl;j++){
        fprintf(fp,"%-5d  %-10s\n",j,NameL[j]);
    }
    fclose(fp);
}
int isLetter(char ch)  //�ж��Ƿ�Ϊ��ĸ,��->1,��->0
{
    if ((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <= 'Z'))
        return 1;
    else
        return 0;

}
int isDigit(char ch)  //�ж��Ƿ�Ϊ����,��->1,��->0
{
    if (ch >= '0'&&ch <= '9')
        return 1;
    else
        return 0;
}
int isDelimiter(char ch){ //�ж��Ƿ�Ϊ���,��->1,��->0
    if((ch < 'a' || ch > 'z') && (ch < 'A' || ch > 'Z') && (ch < '0' || ch > '9'))return 1;
    else return 0;
}

void showToken(int TokenLen){
    int j;
    printf("\n ---------------------TokenList------------------");
    for(j = 0; j < TokenLen; j++){
        if((token[j].Class)==12 ){
            printf("\n %-3d %-5s\tClass: %-3d  Seman: %-5d Index: %-5d",j+1,token[j].Seman,token[j].Class,token[j].Seman,token[j].index);
        }else if((token[j].Class)==11){printf("\n %-3d %-5s\tClass: %-3d  Seman: %-5d Index: %-5d",j+1,token[j].Seman,token[j].Class,token[j].Seman,token[j].index);}
        else printf("\n %-3d %-5s\tClass: %-3d  Seman: %-5d ",j+1,WordL[token[j].Class-1],token[j].Class,token[j].Seman);
    }
}
void showConstL(){
    printf("\n ----------ConstList-----------");
    for(int c=0;c<cl;c++){
        printf("\n   ConstL[%d] = %s",c,ConstL[c]);
    }
}
void showNameL(){
    printf("\n ----------NameList-----------");
    for(int n=0;n<nl;n++){
        printf("\n   NameL[%d] = %s",n,NameL[n]);
    }
}
void showAnnoL(){
    printf("\n --------------AnnotationList--------------");
    for(int j=0;j<al;j++){
        printf("\n��%d~%d�е�ע�ͣ�%s",annoL[j].startLine,annoL[j].endLine,annoL[j].content);
    }
}
void showErrorL(){
    printf("\n --------------------------ErrorList--------------------------");
    for(int j=0;j<el;j++){
        printf("\n��%d�� ��'%c'��ʼ ����[%s]Error:%s",errorL[j].lline,Data[errorL[j].pos],errorL[j].type,errorL[j].tip);
    }
}
void error(int lline,char *type,char* tip){
    errorL[el].lline=lline;
    errorL[el].type=type;
    errorL[el].pos=i;
    errorL[el].tip=tip;
    el++;
}
int isRepeated(char* name,char List[MaxLen][MaxStr],int listLen){
    int pos=0;
    while (pos<listLen){
        if(strcmp(name,List[pos])==0) return pos;
        pos++;
    }
    return -1;
}
int isKeyWord(char* name){
    int pos=0;
    int length = sizeof(KeyWord)/sizeof (KeyWord[0]);
    while (pos<length){
        if(strcmp(name,KeyWord[pos])==0) return pos;
        pos++;
    }
    return -1;
}
void Next_Token(){
    char ch,name[10];
    int state,len,p;
    ch = Data[i];
    while (ch==' ' || ch=='\t' ||ch=='\n'){
        if(ch=='\n') {lline++;  token[k++].Class=37;}
        ch = Data[++i];
    }
    if(inDetail!=0)printf("\ni=%d,ch='%c'",i,Data[i]);
    state=0;
    while(true){
        if(inDetail!=0)printf("\nstate=%d",state);
        switch(state){
            case 0:
                if(isLetter(ch) || ch=='_'){ len=0; name[len++]=ch; state=1;}
                else if(ch == '0'){ len=0; name[len++]=ch; state=3;}
                else if( isDigit(ch)){ len=0; name[len++]=ch; state=4;}
                else{
                    switch(ch){
                        case '=': state=9; break;
                        case '<': state=12; break;
                        case '>': state=15; break;
                        case '!': state=18; break;
                        case '&': state=21; break;
                        case '|': state=23; break;
                        case ',': state=25; break;
                        case '[': state=26; break;
                        case ']': state=27; break;
                        case '(': state=28; break;
                        case ')': state=29; break;
                        case '{': state=30; break;
                        case '}': state=31; break;
                        case ';': state=32; break;
                        case '.': state=33; break;
                        case '+': state=34; break;
                        case '-': state=35; break;
                        case '*': state=36; break;
                        case '%': state=37; break;
                        case '/': state=38; break;
                        case '#': state=42; break;
                        default: printf("Error��ʶ�𵽷Ƿ�����%c",ch); break;
                    }
                }
                break;
            case 1: /*����ĸ���»��߿�ͷ*/
                ch=Data[++i];
                if(isLetter(ch)|| isDigit(ch)){ name[len++]=ch; state=1;}
                else {state=2;}
                break;
            case 2: /*��ʶ�� or �ؼ���*/
                name[len]='\0';
                if((p=isKeyWord(name))!=-1){ token[k].Class=(p+1); }
                else{
                    token[k].Class=11;
                    if((p=isRepeated(name, NameL,nl))!=-1) {token[k].Seman=NameL[p]; token[k].index=p;} //NameL�����Ѵ���ʶ������ĳ��������֣�
                    else{
                        if(inDetail!=0)printf("\nNameL���в�����%s",name);
                        strcpy(NameL[nl],name);
                        token[k].index=nl;
                        token[k].Seman=NameL[nl++];
                        if(inDetail!=0)printf("\n���뵽NameL[%d]",nl);
                    }
                }
                k++;  i--;
                return;
            case 3: /*����0*/
                ch=Data[++i];
                if(isLetter(ch)){
                    while (ch!=' '&&ch!=';'&&ch!='\n'&&ch!='\t'&&ch!='#'&& !isDelimiter(ch)) ch = Data[++i];
                    error(lline,"Invalid identifier","��ʶ�����������ֿ�ͷ");
                    i--;return;
                }
                if(ch=='.'){ name[len++]=ch; state=5;}
                else if(isDigit(ch)){
                    error(lline,"decimal number","��λʮ�����������ֲ�����0��ͷ");
                    while (ch!=' '&&ch!=';'&&ch!='\n'&&ch!='\t'&&ch!='#'&& !isDelimiter(ch)) ch = Data[++i];
                    i--;
                    return;
                }
                else{state=7;}
                break;
            case 4:/*����1~9*/
                ch=Data[++i];
                if(isLetter(ch)){
                    while (ch!=' '&&ch!=';'&&ch!='\n'&&ch!='\t'&&ch!='#'&& !isDelimiter(ch)) ch = Data[++i];
                    error(lline,"Invalid identifier","��ʶ�����������ֿ�ͷ");
                    i--;return;
                }
                if(isDigit(ch)){ name[len++]=ch; state=4;}
                else if(ch=='.'){name[len++]=ch; state=5;}
                else {state=8;}
                break;
            case 5:/*����+'.'*/
                ch=Data[++i];
                if(isDigit(ch)){name[len++]=ch; state=5;}
                else if(ch=='.'){
                    error(lline,"Invalid suffix","һ�������в��ܳ��ֶ��С����");
                    while (ch!=' '&&ch!=';'&&ch!='\n'&&ch!='\t'&&ch!='#'&&!isDelimiter(ch)) ch = Data[++i];
                    i--;
                    return;
                }
                else{state=6;}
                break;
            case 6:/*��ʵ��*/
                token[k].Class=12;
                name[len]='\0';
                if((p=isRepeated(name, ConstL,cl))!=-1){token[k].Seman=ConstL[p];token[k].index=p;}   //ConstL�����Ѵ���ʶ������ĳ��������֣�
                else{
                    if(inDetail!=0)printf("\nConstL���в�����%s",name);
                    strcpy(ConstL[cl],name);
                    token[k].index=cl;
                    token[k].Seman=ConstL[cl++];
                    if(inDetail!=0)printf("\n���뵽ConstL[%d]",cl);
                }
                k++;  i--;
                return;
            case 7:/*��0����*/
                token[k].Class=12;
                name[len]='\0';
                if((p=isRepeated(name,ConstL,cl))!=-1)  {token[k].Seman=ConstL[p];token[k].index=p;}//ConstL�����Ѵ���ʶ������ĳ��������֣�
                else{
                    if(inDetail!=0)printf("\nConstL���в�����%s",name);
                    strcpy(ConstL[cl],name);
                    token[k].index=cl;
                    token[k].Seman=ConstL[cl++];
                    if(inDetail!=0)printf("\n���뵽ConstL[%d]",cl);
                }
                k++;  i--;
                return;
            case 8:/*������*/
                token[k].Class=12;
                name[len]='\0';
                if((p=isRepeated(name,ConstL,cl))!=-1)   {token[k].Seman=ConstL[p];token[k].index=p;}//ConstL�����Ѵ���ʶ������ĳ��������֣�
                else{
                    if(inDetail!=0)printf("\nConstL���в�����%s",name);
                    strcpy(ConstL[cl],name);
                    token[k].index=cl;
                    token[k].Seman= ConstL[cl++];
                    if(inDetail!=0)printf("\n���뵽ConstL[%d]",cl);
                }
                k++;  i--;
                return;
            case 9:/* '=' */ch=Data[++i]; if(ch=='=') state=10;else state=11;break;
            case 10:/* '=='  */token[k].Class=23; k++; return;
            case 11:/* '=' + ��'=' */token[k].Class=28; k++; i--; return;
            case 12:/* '<' */ch=Data[++i]; if(ch=='=') state=13;else state=14;break;
            case 13:/* '<='  */token[k].Class=21; k++;  return;
            case 14:/* '<' + ��'=' */token[k].Class=20; k++; i--; return;
            case 15:/* '>' */ch=Data[++i]; if(ch=='=') state=16;else state=17;break;
            case 16:/* '>='  */token[k].Class=19; k++;  return;
            case 17:/* '>' + ��'=' */token[k].Class=18; k++; i--; return;
            case 18:/* '!' */ch=Data[++i]; if(ch=='=') state=19;else state=20;break;
            case 19:/* '!='  */token[k].Class=22; k++;  return;
            case 20:/* '!' + ��'=' */token[k].Class=24; k++; i--; return;

            case 21:/* '&' */ch=Data[++i];
                if(ch=='&') state=22;
                else{   /* '&' + ��'&' */
                    error(lline,"match","&Ӧ��'&&'��ʽ����");
                    while (ch!=' '&&ch!='\n'&&ch!='\t'&&ch!='#') ch = Data[++i];
                    i--;  return;
                } break;
            case 22:/* '&&'  */token[k].Class=25; k++;  return;

            case 23:/* '|' */ch=Data[++i];
                if(ch=='&') state=24;
                else{   /* '|' + ��'|' */
                    error(lline,"match","|Ӧ��'||'��ʽ����");
                    while (ch!=' '&&ch!='\n'&&ch!='\t'&&ch!='#') ch = Data[++i];
                    i--;  return;
                } break;
            case 24:/* '||'  */token[k].Class=26; k++;  return;
            case 25:/* ',' */  token[k].Class=27; k++;  return;
            case 26:/* '[' */  token[k].Class=29; k++;  return;
            case 27:/* ']' */  token[k].Class=30; k++;  return;
            case 28:/* '(' */  token[k].Class=31; k++;  return;
            case 29:/* ')' */  token[k].Class=32; k++;  return;
            case 30:/* '{' */  token[k].Class=33; k++;  return;
            case 31:/* '}' */  token[k].Class=34; k++;  return;
            case 32:/* ';' */  token[k].Class=35; k++;  return;
            case 33:/* '.' */  token[k].Class=36; k++;  return;
            case 34:/* '+' */  token[k].Class=13; k++;  return;
            case 35:/* '-' */  token[k].Class=14; k++;  return;
            case 36:/* '*' */  token[k].Class=15; k++;  return;
            case 37:/* '%' */  token[k].Class=17; k++;  return;
            case 38:/* '/'+... */ch=Data[++i];
                if(ch=='/') state=39;
                else if(ch=='*') state=40;
                else state=41;
                break;
            case 39:/* '//' */ch=Data[++i]; len=0;
                while (ch!='\n' && ch!='#'){annoL[al].content[len++]=ch; ch=Data[++i];}
                annoL[al].content[len]='\0';
                annoL[al].startLine=annoL[al].endLine=lline;
                i--; al++; state=0; return;
            case 40:/* '/*' */ch=Data[++i]; len=0;
                annoL[al].startLine=lline;
                while (ch!='*' || Data[i+1]!='/'){
                    if(ch=='#'){ i--;return;}
                    if(ch=='\n'){lline++;ch=Data[++i];}
                    else {annoL[al].content[len++]=ch; ch=Data[++i];}
                }
                annoL[al].content[len]='\0';
                annoL[al].endLine=lline;
                i++; al++; return;
            case 41:/* '/' */token[k].Class=16; k++; i--; return;
            case 42:/* '#' */
                token[k].Class=38; EndFlag=1; k++;return;
        }
    }
}
int main(){
    int count,j;
    /*-----------------------���ļ��л�ȡԴ����---------------------------*/
    count=getData_byFile(Data);
    printf("�ʷ��������������txt�ļ�\n");
    for( j = 0; j <= count; j ++)  printf("%c",Data[j]);
    /*------------------------�ʷ���������---------------------------*/
    i=0;
    while (EndFlag==0){
        Next_Token();
        i++;
        if(inDetail!=0)printf("\ni=%d",i);
    }
    if (EndFlag==1){
        printf("\n����������#\n�ʷ��������̽���");
        showToken(k);
    }
    /*------------------------������Ԫ����---------------------------*/
    showConstL();
    showNameL();
    showAnnoL();
    showErrorL();
    saveToken_byFile();
    saveConstL_byFile();
    saveNamwL_byFile();
}