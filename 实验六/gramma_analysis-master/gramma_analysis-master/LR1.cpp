#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <fstream>
#include <assert.h>
using namespace std;
/* ����ʽ�ṹ�壬�󲿷��ź��Ҳ����Ŵ� */
struct Production {
    char left;
    vector<char> rigths;
    /* ����== */
    bool operator==(Production& rhs) const {
        if (left != rhs.left)
            return false;
        for (int i = 0; i < rigths.size(); i++) {
            if (i >= rhs.rigths.size())
                return false;
            if (rigths[i] != rhs.rigths[i])
                return false;
        }
        return true;
    }
};
/* LR1��Ŀ */
struct LR1Item {
    Production p;
    /* ���λ�� */
    int location;
    /* ��ǰ������ */
    char nex;
};
/* LR1��Ŀ�� */
struct LR1Items {
    vector<LR1Item> items;
};
/* LR1��Ŀ���淶�� */
struct CanonicalCollection {
    /* ��Ŀ������ */
    vector<LR1Items> items;
    /* ����DFA��ͼ��firstΪת�Ƶ���״̬��ţ�second�Ǿ�ʲôת�� */
    vector< pair<int, char> > g[100];
}CC;
/* �ķ��ṹ�� */
struct Grammar {
    int num;  // ����ʽ����
    vector<char> T;   // �ս��
    vector<char> N;   // ���ս��
    vector<Production> prods;  //����ʽ
}grammar;
/* FIRST����FOLLOW�� */
map<char, set<char> > first;
map<char, set<char> > follow;
/* DFA���У� ���ڴ洢��ת�Ƶ���Ч��Ŀ�� */
queue< pair<LR1Items, int> > Q;
/* action���goto�� */
pair<int, int> action[100][100]; // first��ʾ����������0->ACC 1->S 2->R second��ʾת��״̬���߲���ʽ���
int goton[100][100];
/* �������� */
string str;
/* ����ջ */
stack< pair<int, char> > ST; // first��state��second ��symble
/* �ж�ch�Ƿ����ս�� */
int isInT(char ch){
    for (int i = 0; i < grammar.T.size(); i++) {
        if (grammar.T[i] == ch) {
            return i + 1;
        }
    }
    return 0;
}
/* �ж�ch�Ƿ��Ƿ��ս�� */
int isInN(char ch){
    for (int i = 0; i < grammar.N.size(); i++) {
        if (grammar.N[i] == ch) {
            return i + 1;
        }
    }
    return 0;
}
/* ��(T U N)��FIRST�� */
void getFirstSet(){
    /* �ս����FIRST�����䱾�� */
    for (int i = 0; i < grammar.T.size(); i++) {
        char X = grammar.T[i];
        set<char> tmp;
        tmp.insert(X);
        first[X] = tmp;
    }
    /* �����ս����FIRST�������仯ʱѭ�� */
    bool change = true;
    while (change) {
        change = false;
        /* ö��ÿ������ʽ */
        for (int i = 0; i < grammar.prods.size(); i++) {
            Production &P = grammar.prods[i];
            char X = P.left;
            set<char> &FX = first[X];
            /* ����Ҳ���һ�������ǿջ������ս��������뵽�󲿵�FIRST���� */
            if (isInT(P.rigths[0]) || P.rigths[0] == '&') {
                /* �����Ƿ�FIRST���Ƿ��Ѿ����ڸ÷��� */
                auto it = FX.find(P.rigths[0]);
                /* ������ */
                if (it == FX.end()) {
                    change = true; // ��עFIRST�������仯��ѭ������
                    FX.insert(P.rigths[0]);
                }
            } 
			else {
                /* ��ǰ�����Ƿ��ս�����ҵ�ǰ���ſ����Ƴ��գ������ж���һ������ */
                bool nex = true;
                /* ���жϷ��ŵ��±� */
                int idx = 0;
                while (nex && idx < P.rigths.size()) {
                    nex = false;
                    char Y = P.rigths[idx];
                    set<char> &FY = first[Y];
                    for (auto it = FY.begin(); it != FY.end(); it++) {
                        /* �ѵ�ǰ���ŵ�FIRST���зǿ�Ԫ�ؼ��뵽�󲿷��ŵ�FIRST���� */
                        if (*it != '&') {
                            auto itt = FX.find(*it);
                            if (itt == FX.end()) {
                                change = true;
                                FX.insert(*it);
                            }
                        }
                    }
                    /* ��ǰ���ŵ�FIRST�����п�, ���nexΪ�棬idx�±�+1 */
                    auto it = FY.find('&');
                    if (it != FY.end()) {
                    	if (idx+1 == P.rigths.size()) {
                    		FX.insert('&');
						}
                        nex = true;
                        idx = idx + 1;
                    }
                }
            }
        }
        cout<<1;
    }
    printf("FIRST:\n");
    for (int i = 0; i < grammar.N.size(); i++) {
    	cout<<1;
        char X = grammar.N[i];
        printf("%c: ", X);
        for (auto it = first[X].begin(); it != first[X].end(); it++) {
            printf("%c ", *it);
        }
        printf("\n");
    }
}
/* ����alpha����FIRST���� ���浽FS������ */
void getFirstByAlphaSet(vector<char> &alpha, set<char> &FS){
    /* ��ǰ�����Ƿ��ս�����ҵ�ǰ���ſ����Ƴ��գ������ж���һ������ */
    bool nex = true;
    int idx = 0;
    while (idx < alpha.size() && nex) {
        nex = false;
        /* ��ǰ�������ս����գ����뵽FIRST���� */
        if (isInT(alpha[idx]) || alpha[idx] == '&') {
            /* �ж��Ƿ��Ѿ���FIRST���� */
            auto itt = FS.find(alpha[idx]);
            if (itt == FS.end()) {
                FS.insert(alpha[idx]);
            }
        } 
		else {
            char B = alpha[idx];
            set<char> &FB = first[B];
            for (auto it = first[B].begin(); it != first[B].end(); it++) {
                /* ��ǰ����FIRST�������գ����nexΪ�棬��������ǰѭ�� */
                if (*it == '&') {
                    nex = true;
                    continue;
                }
                /* �ѷǿ�Ԫ�ؼ��뵽FIRST���� */
                auto itt = FS.find(*it);
                if (itt == FS.end()) {
                    FS.insert(*it);
                }
            }
        }
        idx = idx + 1;
    }
    /* ����������ʽ�Ҳ�ĩβnex��Ϊ�棬˵��alpha�����ƿգ����ռ��뵽FIRST���� */
    if (nex) {
        FS.insert('&');
    }
}
/* �ж���LR1��Ŀt������Ч��Ŀ��I�� */
bool isInLR1Items(LR1Items &I, LR1Item &t){
    for (auto it = I.items.begin(); it != I.items.end(); it++) {
        LR1Item &item = *it;
        if (item.p == t.p && item.location == t.location && item.nex == t.nex)
            return true;
    }
    return false;
}
/* ��ӡĳ����Ŀ�� */
void printLR1Items(LR1Items &I){
    for (auto it = I.items.begin(); it != I.items.end(); it++) {
        LR1Item &L = *it;
        printf("%c->", L.p.left);
        for (int i = 0; i < L.p.rigths.size(); i++) {
            if (L.location == i)
                printf(".");
            printf("%c", L.p.rigths[i]);
        }
        if (L.location == L.p.rigths.size())
            printf(".");
        printf(",%c   ", L.nex);
    }
    printf("\n");
}
/* ��I�ıհ� */
void closure(LR1Items &I){
    bool change =  true;
    while (change) {
        change = false;
        LR1Items J;
        /* ö��ÿ����Ŀ */
        J.items.assign(I.items.begin(), I.items.end());
        for (auto it = J.items.begin(); it != J.items.end(); it++) {
            LR1Item &L = *it;
            /* �ǹ�Լ��Ŀ */
            if (L.location < L.p.rigths.size()) {
                char B = L.p.rigths[L.location];
                if (isInN(B)) {
                    /* �ѷ���������LR1��Ŀ����հ��� */
                    set<char> FS;
                    vector<char> alpha;
                    alpha.assign(L.p.rigths.begin() + L.location + 1, L.p.rigths.end());
                    alpha.push_back(L.nex);
                    getFirstByAlphaSet(alpha, FS);
                    for (int i = 0; i < grammar.prods.size(); i++) {
                        Production &P = grammar.prods[i];
                        if (P.left == B) {
                            /* ö��ÿ��b in B�����FIRST�� */
                            for (auto it = FS.begin(); it != FS.end(); it++) {
                                char b = *it;
                                LR1Item t;
                                t.location = 0;
                                t.nex = b;
                                t.p.left = P.left;
                                t.p.rigths.assign(P.rigths.begin(), P.rigths.end());
                                if (!isInLR1Items(I, t)) {
                                    /* ��Ǹı� */
                                    change = true;
                                    I.items.push_back(t);
                                } 
                            }
                        } 
                    }
                }
            }
        }
    }
}
/* �ж��Ƿ�����Ŀ���淶���У����ڷ������ */
int isInCanonicalCollection(LR1Items &I){
    for (int i = 0; i < CC.items.size(); i++) {
        LR1Items &J = CC.items[i];
        bool flag = true;
        if (J.items.size() != I.items.size()) {
            flag = false;
            continue;
        }
        /* ÿ����Ŀ���ڸ���Ŀ���У�����Ϊ���������Ŀ����� */
        for (auto it = I.items.begin(); it != I.items.end(); it++) {
            LR1Item &t = *it;
            if (!isInLR1Items(J, t)) {
                flag = false;
                break;
            }
        }
        if (flag) {
            return i + 1;
        }
    }
    return 0;
}
/* ת�ƺ�����IΪ��ǰ����Ŀ����JΪת�ƺ����Ŀ��, ��Xת�� */
void go(LR1Items &I, char X, LR1Items &J){
    for (auto it = I.items.begin(); it != I.items.end(); it++) {
        LR1Item &L = *it;
        /* �ǹ�Լ��Ŀ */
        if (L.location < L.p.rigths.size()) {
            char B = L.p.rigths[L.location];
            /* ���������Ƿ��ս�����ҷ��ս��ΪX����λ�ü�1, ���뵽ת����Ŀ����*/
            if (B == X) {
                LR1Item t;
                t.location = L.location + 1;
                t.nex = L.nex;
                t.p.left = L.p.left;
                t.p.rigths.assign(L.p.rigths.begin(), L.p.rigths.end());
                J.items.push_back(t);
            }
        }
    }
    /* ��J������Ŀ��������հ� */
    if (J.items.size() > 0) {
        closure(J);
    }
}
/* ����DFA����Ŀ���淶�� */
void DFA(){
    /* ������ʼ��Ŀ�� */
    LR1Item t;
    t.location = 0;
    t.nex = '$';
    t.p.left = grammar.prods[0].left;
    t.p.rigths.assign(grammar.prods[0].rigths.begin(), grammar.prods[0].rigths.end());
    LR1Items I;
    I.items.push_back(t);
    closure(I);
    /* �����ʼ��Ч��Ŀ�� */
    CC.items.push_back(I);
    /* ���¼������Ч��Ŀ���������չ������ */
    Q.push(pair<LR1Items, int>(I, 0));
    while (!Q.empty()) {
        LR1Items &S = Q.front().first;
        int sidx = Q.front().second;
        /* ����ÿ���ս�� */
        for (int i = 0; i  < grammar.T.size(); i++) {
            LR1Items D;
            go(S, grammar.T[i], D);
            int idx;
            /* ����Ϊ�� */
            if (D.items.size() > 0) {
                /* �����Ƿ��Ѿ�����Ч��Ŀ������ */
                idx = isInCanonicalCollection(D); 
                if (idx > 0) {
                    idx = idx - 1;
                } else {
                    idx = CC.items.size();
                    CC.items.push_back(D);
                    /* ���¼������Ч��Ŀ���������չ������ */
                    Q.push(pair<LR1Items, int>(D, idx));
                }
                /* ��ԭ״̬��ת��״̬��һ���ߣ����ϵ�ֵΪת�Ʒ��� */
                CC.g[sidx].push_back(pair<char, int>(grammar.T[i], idx));
            }
        }
        /* ����ÿ�����ս�� */
        for (int i = 0; i  < grammar.N.size(); i++) {
            LR1Items D;
            go(S, grammar.N[i], D);
            int idx;
            if (D.items.size() > 0) {
                /* �����Ƿ��Ѿ�����Ч��Ŀ������ */
                idx = isInCanonicalCollection(D); 
                if (idx != 0) {
                    idx = idx - 1;
                } else {
                    idx = CC.items.size();
                    CC.items.push_back(D);
                    /* ���¼������Ч��Ŀ���������չ������ */
                    Q.push(pair<LR1Items, int>(D, idx));
                }
                /* ��ԭ״̬��ת��״̬��һ���ߣ����ϵ�ֵΪת�Ʒ��� */
                CC.g[sidx].push_back(pair<char, int>(grammar.N[i], idx));
            }
        }
    /* ��ǰ״̬��չ��ϣ��Ƴ�����*/
    Q.pop();
    }
    printf("CC size: %d\n", CC.items.size());
    for (int i = 0; i < CC.items.size(); i++) {
        printf("LR1Items %d:\n", i);
        printLR1Items(CC.items[i]);
        for (int j = 0; j < CC.g[i].size(); j++) {
            pair<char, int> p= CC.g[i][j];
            printf("to %d using %c\n", p.second, p.first);
        }
    }
}
/* ����LR1������ */
void productLR1AnalysisTabel(){
    for (int i = 0; i < CC.items.size(); i++) {
        LR1Items &LIt= CC.items[i];
        /* ����action�� */
        for (auto it = LIt.items.begin(); it != LIt.items.end(); it++) {
            LR1Item &L = *it;
            /* �ǹ�Լ��Ŀ */
            if (L.location < L.p.rigths.size()) {
                char a = L.p.rigths[L.location];
                int j = isInT(a);
                /* a���ս�� */
                if (j > 0) {
                    j = j - 1;
                    /* �ҵ���Ӧa�ĳ��ߣ��õ���ת�Ƶ���״̬ */
                    for (int k = 0; k < CC.g[i].size(); k++) {
                        pair<char, int> p = CC.g[i][k];
                        if (p.first == a) {
                            action[i][j].first = 1; // 1->S
                            action[i][j].second = p.second;  //ת��״̬
                            break;
                        }
                    }
                }
            } else { // ��Լ��Ŀ
                /* ������Ŀ */
                if (L.p.left == grammar.prods[0].left) {
                    if (L.nex == '$')
                        action[i][grammar.T.size() - 1].first = 3;
                } else {
                    /* �ս�� */
                    int  j = isInT(L.nex) - 1;
                    /* �ҵ�����ʽ��Ӧ����� */
                    for (int k = 0; k < grammar.prods.size(); k++) {
                        if (L.p == grammar.prods[k]) {
                            action[i][j].first = 2;
                            action[i][j].second = k;
                            break;
                        }
                    }
                }
            }
        }
        /* ����goto�� */
        for (int k = 0; k < CC.g[i].size(); k++) {
            pair<char, int> p = CC.g[i][k];
            char A = p.first;
            int j = isInN(A);
            /* �ս�� */
            if (j > 0) {
                j = j - 1;
                goton[i][j] = p.second; //ת��״̬
            }
        }
    }
    /* ��ӡLR1������ */
    for (int i = 0; i < grammar.T.size() / 2; i++)
        printf("\t");
    printf("action");
    for (int i = 0; i < grammar.N.size() / 2 + grammar.T.size() / 2 + 1; i++)
        printf("\t");
    printf("goto\n");
    printf("\t");
    for (int i = 0; i  < grammar.T.size(); i++) {
        printf("%c\t", grammar.T[i]);
    }
    printf("|\t");
    for (int i = 1; i  < grammar.N.size(); i++) {
        printf("%c\t", grammar.N[i]);
    }
    printf("\n");
    for (int i = 0; i < CC.items.size(); i++) {
        printf("%d\t", i);
        for (int j = 0; j < grammar.T.size(); j++) {
            if (action[i][j].first == 1) {
                printf("%c%d\t", 'S', action[i][j].second);
            } else if (action[i][j].first == 2) {
                printf("%c%d\t", 'R', action[i][j].second);
            } else if (action[i][j].first == 3) {
                printf("ACC\t");
            } else {
                printf("\t");
            }
        }
        printf("|\t");
        for (int j = 1; j < grammar.N.size(); j++) {
            if (goton[i][j]) {
                printf("%d\t", goton[i][j]);
            } else {
                printf("\t");
            }
            
        }
        printf("\n");
    }
}
void input(){
	ifstream infile;
	string path="tt1.txt";
	infile.open(path.data()); 
	assert(infile.is_open());
	string s;int num=0;
	while(getline(infile,s)){
		s=s.c_str();
		Production tmp;
        tmp.left = s[0];
        for (int j = 3; j < s.size(); j++) {
            tmp.rigths.push_back(s[j]);
        }
        grammar.prods.push_back(tmp);
        num++;
	}
	grammar.num=num;
	infile.close();
	path="tt2.txt";
	infile.open(path.data()); 
	assert(infile.is_open());
	while(getline(infile,s)){
		s=s.c_str();
		char temp=s[0];
		grammar.N.push_back(temp);
	}
	infile.close();
	path="tt3.txt";
	infile.open(path.data()); 
	assert(infile.is_open());
	while(getline(infile,s)){
		s=s.c_str();
		char temp=s[0];
		grammar.T.push_back(temp);
	}
	infile.close();
	grammar.T.push_back('$');
} 
void initGrammar(){
    printf("���������ķ�:\n");
    cin >> grammar.num;
    string s;
    printf("Please enter the production:\n");
    for (int i = 0; i < grammar.num; i++) {
        cin >> s;
        Production tmp;
        tmp.left = s[0];
        for (int j = 3; j < s.size(); j++) {
            tmp.rigths.push_back(s[j]);
        }
        grammar.prods.push_back(tmp);
    }
    printf("Please enter the non-terminators(end with #):\n");
    char ch;
    cin >> ch;
    while (ch != '#') {
        grammar.N.push_back(ch);
        cin >> ch;
    }
    printf("Please enter the terminators(end with #):\n");
    cin >> ch;
    while (ch != '#') {
        grammar.T.push_back(ch);
        cin >> ch;
    }
    /* ��$�����ս�� */
    grammar.T.push_back('$');
    /* ��FIRST�� */
    getFirstSet();
    /* ����DFA��SLR1Ԥ������� */
    DFA();
    productLR1AnalysisTabel();
    /* ���������������ʼ������ջ */
    printf("Please enter the String to be analyzed:\n");
    cin >> str;
    str += '$';
    ST.push(pair<int, char>(0, '-'));
}
/* �������� */
void process(){
    int ip = 0;
    printf("The ans:\n");
    do {
        int s = ST.top().first;
        char a = str[ip];
        int j = isInT(a) - 1;
        /* �ƽ� */
        if (action[s][j].first == 1) {
            ST.push(pair<int, char>(action[s][j].second, a));
            ip = ip + 1;
        } else if (action[s][j].first == 2) { // ��Լ
            Production &P = grammar.prods[action[s][j].second];
            /* �������������ʽ */
            printf("%c->", P.left);
            for (int i = 0; i < P.rigths.size(); i++) {
                ST.pop();
                printf("%c", P.rigths[i]);
            }
            printf("\n");
            s = ST.top().first;
            char A = P.left;
            j = isInN(A) - 1;
            ST.push(pair<int, char>(goton[s][j], A));
        } else if (action[s][j].first == 3) {   //����
            printf("ACC\n");
            return;
        } else {
            printf("error\n");
        }
    } while(1);
}
int main(){
    input();
    getFirstSet();
    DFA();
    productLR1AnalysisTabel();
//    process();
    return 0;
}
