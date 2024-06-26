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
/* 产生式结构体，左部符号和右部符号串 */
struct Production {
    char left;
    vector<char> rigths;
    /* 重载== */
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
/* LR1项目 */
struct LR1Item {
    Production p;
    /* 点的位置 */
    int location;
    /* 向前看符号 */
    char nex;
};
/* LR1项目集 */
struct LR1Items {
    vector<LR1Item> items;
};
/* LR1项目集规范族 */
struct CanonicalCollection {
    /* 项目集集合 */
    vector<LR1Items> items;
    /* 保存DFA的图，first为转移到的状态序号，second是经什么转移 */
    vector< pair<int, char> > g[100];
}CC;
/* 文法结构体 */
struct Grammar {
    int num;  // 产生式数量
    vector<char> T;   // 终结符
    vector<char> N;   // 非终结符
    vector<Production> prods;  //产生式
}grammar;
/* FIRST集和FOLLOW集 */
map<char, set<char> > first;
map<char, set<char> > follow;
/* DFA队列， 用于存储待转移的有效项目集 */
queue< pair<LR1Items, int> > Q;
/* action表和goto表 */
pair<int, int> action[100][100]; // first表示分析动作，0->ACC 1->S 2->R second表示转移状态或者产生式序号
int goton[100][100];
/* 待分析串 */
string str;
/* 分析栈 */
stack< pair<int, char> > ST; // first是state，second 是symble
/* 判断ch是否是终结符 */
int isInT(char ch){
    for (int i = 0; i < grammar.T.size(); i++) {
        if (grammar.T[i] == ch) {
            return i + 1;
        }
    }
    return 0;
}
/* 判断ch是否是非终结符 */
int isInN(char ch){
    for (int i = 0; i < grammar.N.size(); i++) {
        if (grammar.N[i] == ch) {
            return i + 1;
        }
    }
    return 0;
}
/* 求(T U N)的FIRST集 */
void getFirstSet(){
    /* 终结符的FIRST集是其本身 */
    for (int i = 0; i < grammar.T.size(); i++) {
        char X = grammar.T[i];
        set<char> tmp;
        tmp.insert(X);
        first[X] = tmp;
    }
    /* 当非终结符的FIRST集发生变化时循环 */
    bool change = true;
    while (change) {
        change = false;
        /* 枚举每个产生式 */
        for (int i = 0; i < grammar.prods.size(); i++) {
            Production &P = grammar.prods[i];
            char X = P.left;
            set<char> &FX = first[X];
            /* 如果右部第一个符号是空或者是终结符，则加入到左部的FIRST集中 */
            if (isInT(P.rigths[0]) || P.rigths[0] == '&') {
                /* 查找是否FIRST集是否已经存在该符号 */
                auto it = FX.find(P.rigths[0]);
                /* 不存在 */
                if (it == FX.end()) {
                    change = true; // 标注FIRST集发生变化，循环继续
                    FX.insert(P.rigths[0]);
                }
            } 
			else {
                /* 当前符号是非终结符，且当前符号可以推出空，则还需判断下一个符号 */
                bool nex = true;
                /* 待判断符号的下标 */
                int idx = 0;
                while (nex && idx < P.rigths.size()) {
                    nex = false;
                    char Y = P.rigths[idx];
                    set<char> &FY = first[Y];
                    for (auto it = FY.begin(); it != FY.end(); it++) {
                        /* 把当前符号的FIRST集中非空元素加入到左部符号的FIRST集中 */
                        if (*it != '&') {
                            auto itt = FX.find(*it);
                            if (itt == FX.end()) {
                                change = true;
                                FX.insert(*it);
                            }
                        }
                    }
                    /* 当前符号的FIRST集中有空, 标记nex为真，idx下标+1 */
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
/* 产找alpha串的FIRST集， 保存到FS集合中 */
void getFirstByAlphaSet(vector<char> &alpha, set<char> &FS){
    /* 当前符号是非终结符，且当前符号可以推出空，则还需判断下一个符号 */
    bool nex = true;
    int idx = 0;
    while (idx < alpha.size() && nex) {
        nex = false;
        /* 当前符号是终结符或空，加入到FIRST集中 */
        if (isInT(alpha[idx]) || alpha[idx] == '&') {
            /* 判断是否已经在FIRST集中 */
            auto itt = FS.find(alpha[idx]);
            if (itt == FS.end()) {
                FS.insert(alpha[idx]);
            }
        } 
		else {
            char B = alpha[idx];
            set<char> &FB = first[B];
            for (auto it = first[B].begin(); it != first[B].end(); it++) {
                /* 当前符号FIRST集包含空，标记nex为真，并跳过当前循环 */
                if (*it == '&') {
                    nex = true;
                    continue;
                }
                /* 把非空元素加入到FIRST集中 */
                auto itt = FS.find(*it);
                if (itt == FS.end()) {
                    FS.insert(*it);
                }
            }
        }
        idx = idx + 1;
    }
    /* 如果到达产生式右部末尾nex还为真，说明alpha可以推空，将空加入到FIRST集中 */
    if (nex) {
        FS.insert('&');
    }
}
/* 判断是LR1项目t否在有效项目集I中 */
bool isInLR1Items(LR1Items &I, LR1Item &t){
    for (auto it = I.items.begin(); it != I.items.end(); it++) {
        LR1Item &item = *it;
        if (item.p == t.p && item.location == t.location && item.nex == t.nex)
            return true;
    }
    return false;
}
/* 打印某个项目集 */
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
/* 求I的闭包 */
void closure(LR1Items &I){
    bool change =  true;
    while (change) {
        change = false;
        LR1Items J;
        /* 枚举每个项目 */
        J.items.assign(I.items.begin(), I.items.end());
        for (auto it = J.items.begin(); it != J.items.end(); it++) {
            LR1Item &L = *it;
            /* 非规约项目 */
            if (L.location < L.p.rigths.size()) {
                char B = L.p.rigths[L.location];
                if (isInN(B)) {
                    /* 把符合条件的LR1项目加入闭包中 */
                    set<char> FS;
                    vector<char> alpha;
                    alpha.assign(L.p.rigths.begin() + L.location + 1, L.p.rigths.end());
                    alpha.push_back(L.nex);
                    getFirstByAlphaSet(alpha, FS);
                    for (int i = 0; i < grammar.prods.size(); i++) {
                        Production &P = grammar.prods[i];
                        if (P.left == B) {
                            /* 枚举每个b in B后面的FIRST集 */
                            for (auto it = FS.begin(); it != FS.end(); it++) {
                                char b = *it;
                                LR1Item t;
                                t.location = 0;
                                t.nex = b;
                                t.p.left = P.left;
                                t.p.rigths.assign(P.rigths.begin(), P.rigths.end());
                                if (!isInLR1Items(I, t)) {
                                    /* 标记改变 */
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
/* 判断是否在项目集规范族中，若在返回序号 */
int isInCanonicalCollection(LR1Items &I){
    for (int i = 0; i < CC.items.size(); i++) {
        LR1Items &J = CC.items[i];
        bool flag = true;
        if (J.items.size() != I.items.size()) {
            flag = false;
            continue;
        }
        /* 每个项目都在该项目集中，则认为这个两个项目集相等 */
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
/* 转移函数，I为当前的项目集，J为转移后的项目集, 经X转移 */
void go(LR1Items &I, char X, LR1Items &J){
    for (auto it = I.items.begin(); it != I.items.end(); it++) {
        LR1Item &L = *it;
        /* 非规约项目 */
        if (L.location < L.p.rigths.size()) {
            char B = L.p.rigths[L.location];
            /* 如果点后面是非终结符，且非终结符为X，点位置加1, 加入到转移项目集中*/
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
    /* 若J中有项目，则求其闭包 */
    if (J.items.size() > 0) {
        closure(J);
    }
}
/* 构建DFA和项目集规范族 */
void DFA(){
    /* 构建初始项目集 */
    LR1Item t;
    t.location = 0;
    t.nex = '$';
    t.p.left = grammar.prods[0].left;
    t.p.rigths.assign(grammar.prods[0].rigths.begin(), grammar.prods[0].rigths.end());
    LR1Items I;
    I.items.push_back(t);
    closure(I);
    /* 加入初始有效项目集 */
    CC.items.push_back(I);
    /* 把新加入的有效项目集加入待扩展队列中 */
    Q.push(pair<LR1Items, int>(I, 0));
    while (!Q.empty()) {
        LR1Items &S = Q.front().first;
        int sidx = Q.front().second;
        /* 遍历每个终结符 */
        for (int i = 0; i  < grammar.T.size(); i++) {
            LR1Items D;
            go(S, grammar.T[i], D);
            int idx;
            /* 若不为空 */
            if (D.items.size() > 0) {
                /* 查找是否已经在有效项目集族里 */
                idx = isInCanonicalCollection(D); 
                if (idx > 0) {
                    idx = idx - 1;
                } else {
                    idx = CC.items.size();
                    CC.items.push_back(D);
                    /* 把新加入的有效项目集加入待扩展队列中 */
                    Q.push(pair<LR1Items, int>(D, idx));
                }
                /* 从原状态到转移状态加一条边，边上的值为转移符号 */
                CC.g[sidx].push_back(pair<char, int>(grammar.T[i], idx));
            }
        }
        /* 遍历每个非终结符 */
        for (int i = 0; i  < grammar.N.size(); i++) {
            LR1Items D;
            go(S, grammar.N[i], D);
            int idx;
            if (D.items.size() > 0) {
                /* 查找是否已经在有效项目集族里 */
                idx = isInCanonicalCollection(D); 
                if (idx != 0) {
                    idx = idx - 1;
                } else {
                    idx = CC.items.size();
                    CC.items.push_back(D);
                    /* 把新加入的有效项目集加入待扩展队列中 */
                    Q.push(pair<LR1Items, int>(D, idx));
                }
                /* 从原状态到转移状态加一条边，边上的值为转移符号 */
                CC.g[sidx].push_back(pair<char, int>(grammar.N[i], idx));
            }
        }
    /* 当前状态扩展完毕，移除队列*/
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
/* 生成LR1分析表 */
void productLR1AnalysisTabel(){
    for (int i = 0; i < CC.items.size(); i++) {
        LR1Items &LIt= CC.items[i];
        /* 构建action表 */
        for (auto it = LIt.items.begin(); it != LIt.items.end(); it++) {
            LR1Item &L = *it;
            /* 非规约项目 */
            if (L.location < L.p.rigths.size()) {
                char a = L.p.rigths[L.location];
                int j = isInT(a);
                /* a是终结符 */
                if (j > 0) {
                    j = j - 1;
                    /* 找到对应a的出边，得到其转移到的状态 */
                    for (int k = 0; k < CC.g[i].size(); k++) {
                        pair<char, int> p = CC.g[i][k];
                        if (p.first == a) {
                            action[i][j].first = 1; // 1->S
                            action[i][j].second = p.second;  //转移状态
                            break;
                        }
                    }
                }
            } else { // 规约项目
                /* 接受项目 */
                if (L.p.left == grammar.prods[0].left) {
                    if (L.nex == '$')
                        action[i][grammar.T.size() - 1].first = 3;
                } else {
                    /* 终结符 */
                    int  j = isInT(L.nex) - 1;
                    /* 找到产生式对应的序号 */
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
        /* 构建goto表 */
        for (int k = 0; k < CC.g[i].size(); k++) {
            pair<char, int> p = CC.g[i][k];
            char A = p.first;
            int j = isInN(A);
            /* 终结符 */
            if (j > 0) {
                j = j - 1;
                goton[i][j] = p.second; //转移状态
            }
        }
    }
    /* 打印LR1分析表 */
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
    printf("正在输入文法:\n");
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
    /* 把$当作终结符 */
    grammar.T.push_back('$');
    /* 求FIRST集 */
    getFirstSet();
    /* 构建DFA和SLR1预测分析表 */
    DFA();
    productLR1AnalysisTabel();
    /* 读入待分析串并初始化分析栈 */
    printf("Please enter the String to be analyzed:\n");
    cin >> str;
    str += '$';
    ST.push(pair<int, char>(0, '-'));
}
/* 分析程序 */
void process(){
    int ip = 0;
    printf("The ans:\n");
    do {
        int s = ST.top().first;
        char a = str[ip];
        int j = isInT(a) - 1;
        /* 移进 */
        if (action[s][j].first == 1) {
            ST.push(pair<int, char>(action[s][j].second, a));
            ip = ip + 1;
        } else if (action[s][j].first == 2) { // 规约
            Production &P = grammar.prods[action[s][j].second];
            /* 弹出并输出产生式 */
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
        } else if (action[s][j].first == 3) {   //接受
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
