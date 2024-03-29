#include <iostream>
#include <iomanip>
#include <fstream> // �ļ���
#include <string> // �ַ���
#include <algorithm> // �ַ�������
#include <unordered_map>  // ��ϣ��
#include <map>   // ͼ
#include <stack> // ջ
#include <set>   // ��
#include <vector> //����
#include "Lexical.h"
using namespace std;

// ��Ҫ��ȫ�ֱ�������
unordered_map<string,string> grammar;// �ķ�����,��ϣ��洢��
string S; // ��ʼ��
set<string> Vn; // ���ս����
set<string> Vt; // �ս����
set<string> formulas; // ����ʽ�������selectʱ�������
unordered_map<string, set<char>> FIRST;// FIRST����
unordered_map<string, set<char>> FOLLOW;// FOLLOW����
unordered_map<string, set<char>> SELECT;// Select����
map<pair<char,char>, string> TABLE;// Ԥ�������
vector <int> tokenClass;  //token����-����
string  tokenStr;   //token�����Ӧ���ַ���
//=====================����Ԥ����=======================
void readFile();	 // ��ȡ�ļ�
void pretreatment(); // Ԥ�����򻯡����ŷ����

set<char> findOneFirst(string vn); // ĳ�����ս����First����
void findFirst();				  // ʹ�ù�ϣ��洢

set<char> findOneFollow(string vn); // ĳ�����ս����Follow����
void findFollow();                 // ʹ�ù�ϣ��洢

set<char> findOneSelect(string formula);  // ĳ������ʽ��Select����
void findSelect();                       // ʹ�ù�ϣ��洢

void isLL1();      // �ж��Ƿ�ΪLL(1)����

void makeTable();  // ����Ԥ�������

void LL_1Analyse(); // �����ַ���

// �����󲿻�ȡ����ʽ���Ҳ������ϣ�
vector<string> getRights(string left);
// �ж����ս�����Ƿ��ս��
bool isVn(char v);
bool isVt(char v);
// �ж�ĳ�����ս���ܷ��Ƴ���
bool canToEmpty(char v);
//�ж������ַ�set�Ľ����Ƿ�Ϊ��
bool isIntersect(set<char> a, set<char> b);
// ���������
void printTable();
// �õ������ַ���
string getStackRemain(stack<char> stack_remain);
// ��ʾ���һ��char��
void printSet(set<char> sets);
// ��FOLLOW�����е�Ԫ�ظ����������жϣ�ֱ��follow���ϲ�������
int getFCount();
// ��ȡToken����-����
void getToken();
// ��ȡToken���б����Ӧ���ַ���
void tokenToStr();
// =====================================������===================================
int main() {
    // =====================================���ôʷ�������������Token��================================
    LexicalAnalyzer();
    cout <<endl<<setw(75)<<right<<setfill('=')<< "LL(1)������===================================" << endl << endl;
    // =====================================������Ĵ��룺LL(1)����==================================
    // =====================================1����ȡ�ķ����Ҽ򵥴���==================================
    readFile();
    // =====================================2����First��==========================================
    findFirst();
    // =====================================3����Follow��==========================================
    findFollow();
    // =====================================4����Select��=========================================
    findSelect();
    // =====================================5���ж��Ƿ���LL1�ķ�=====================================
    isLL1();
    // =====================================6������������==========================================
    makeTable();
    // =====================================7����ȡToken����-����====================================
    getToken();
    // =====================================8��ת��Token���б���ɶ�Ӧ���ַ���==========================
    tokenToStr();
    // =====================================9�������ַ���===========================================
    LL_1Analyse();
    return 0;
}
// =====================================���ܺ���===================================
// ��ȡ�ļ�
void readFile() {
    // �����ļ���
//    cout << endl << "�������ļ�����";
//    char file[100];
//    cin >> file;
    char file[100]="rule.txt";
    // ����չʾ�ļ���������
    cout << endl << setw(75) << right << setfill('=') << "�ķ���ȡ======================================" << endl;
    // ifstream�ļ������ļ�
    ifstream fin(file);
    if (!fin.is_open())
    {
        cout << "���ļ�ʧ��";
        exit(-1); // ֱ���˳�
    }
    string line;
    bool isGet = false;
    while (getline(fin, line)) // ���ж�ȡ
    {
        if (!isGet)
        {
            // �õ���ʼ��
            S = line[0];
            isGet = true;
        }
        formulas.insert(line); // �õ����б��ʽ
        cout << line << endl;  // ���

        // �����ϣ�����Ѿ����ڸü�,���ں���
        for (auto iter = grammar.begin(); iter != grammar.end(); ++iter) {
            if (iter->first == string(1, line[0]))
            {
                iter->second = iter->second + "|" + line.substr(3);
                break;
            }
        }
        // ���洢�ķ��Ĺ�ϣ���в����ֵ��
        grammar.insert(pair<string, string>(string(1, line[0]), line.substr(3)));
    }
    fin.close(); // �ر��ļ���

    pretreatment();
}
// Ԥ�������ŷ���
void pretreatment() {
    cout << endl << setw(75) << right << setfill('=') << "���ŷ���======================================" << endl;
    // �����ķ���ϣ��
    for (auto iter = grammar.begin(); iter != grammar.end(); ++iter) {
        Vn.insert(iter->first); // ���ս����
        // �ս������
        string str = iter->second;
        for (size_t i = 0; i < str.length(); i++)
        {
            if (str[i] != '|' && (str[i] < 'A' || str[i] > 'Z'))
            {
                Vt.insert(string(1, str[i]));
            }
        }
    }
    // ����ս���ͷ��ս������
    cout << "��ʼ���ţ�" << S << endl;
    cout << "���ս����Vn = " << "{";
    for (auto iter = Vn.begin(); iter != Vn.end(); ) {
        cout << *iter;
        if ((++iter) != Vn.end())
        {
            cout << ",";
        }
    }
    cout << "}" << endl;

    cout << "�ս����Vt = " << "{";
    for (auto iter = Vt.begin(); iter != Vt.end(); ) {
        cout << *iter;
        if ((++iter) != Vt.end())
        {
            cout << ",";
        }
    }
    cout << "}" << endl;
}
//  ��ĳһ�����ս����First����
set<char> findOneFirst(string vn) {
    // ���˼·
    //1����ȡvn���Ҳ������з���
    //2�������Ҳ���һ�����Ҳ��������������first�����뵽results
    //2.1 ������ǰ�Ҳ���һ�����ַ�������
    //�����һ���ַ����ս��������first���ϲ������������������Ӷ���Ŀշ���
    //����Ƿ��ս������ݹ鴦��
    //������ս�������ƿջ���Ҫѭ��������Ҳ�����һ�ַ�(�����)
    //3���������������������ַ������ƿգ���Ҫɾ��results�еĿշ������ؽ��

    set<char> results; // first���洢
    vector<string> rights = getRights(vn); // ��ȡ�Ҳ�
    if (!rights.empty()) // ����Ҳ���Ϊ��
    {
        // �����Ҳ����ϣ�ÿһ���Ҳ��ֱ����first�����뵽�÷��ս����first�����У�
        for (auto iter = rights.begin(); iter != rights.end();++iter) {
            string right = *iter;
            // ������ǰ�Ҳ��� //�����һ���ַ����ս��������first���ϲ�������ѭ����
            //����Ƿ��ս������ݹ鴦��
            //������ս�������ƿջ���Ҫѭ��������Ҳ�����һ�ַ�(�����)
            for (auto ch = right.begin(); ch != right.end(); ++ch) {
                if (isVn(*ch)) // ����Ƿ��ս������Ҫ�ݹ鴦��
                {
                    //�Ȳ�first���ϡ�����Ѿ����˾Ͳ���Ҫ�ظ����
                    if (FIRST.find(string(1, *ch)) == FIRST.end()) // fisrt�����в�����
                    {
                        // �ݹ������������
                        set<char> chars = findOneFirst(string(1, *ch));
                        // �����������
                        results.insert(chars.begin(),chars.end());
                        FIRST.insert(pair<string,set<char>>(string(1,*ch),chars));
                    }
                    else { // ���ھͰѸü���ȫ���ӵ�firsts�����Ч�ʣ�
                        set<char> chars = FIRST[string(1, *ch)];
                        results.insert(chars.begin(), chars.end());
                    }

                    // �������ַ������ƿգ��Һ��滹���ַ�����ô����Ҫ������һ���ַ�
                    if (canToEmpty(*ch) && (iter + 1) != rights.end())
                    {
                        continue;
                    }
                    else
                        break; // ����ֱ���˳�������ǰ�Ҳ���ѭ��

                }
                else { // ������Ƿ��ս����ֱ�Ӱ�����ַ�����first���ϣ���������
                    // ��һ�����ǰ��Ŀ�Ҳ�ӽ�ȥ�������ɾ����
                    results.insert(*ch);
                    break;
                }
            }
        }
    }
    // ���������ս�������ƿգ���ɾ����
    if (!canToEmpty(vn[0]))
    {
        results.erase('~');
    }
    return results;
}
// ���First����ʹ�ù�ϣ��洢
void findFirst() {
    // �������ս�����ϣ�������ϣ�����ں�����ѯ
    for (auto iter = Vn.begin(); iter != Vn.end(); ++iter) {
        string vn = *iter; // ��ȡ���ս��
        set<char> firsts = findOneFirst(vn); // ���һ��Vn��first��
        FIRST.insert(pair<string, set<char>>(vn, firsts));
    }
    // ��ʾ���
    cout << endl << setw(75) << right << setfill('=') << "FISRT������====================================" << endl;
    for (auto iter = FIRST.begin(); iter != FIRST.end();++iter) {
        cout <<"FIRST("<< iter->first<<")" << "= ";
        set<char> sets = iter->second;
        printSet(sets);
    }
}
// �������ս���������Follow����
set<char> findOneFollow(string vn) {
    //���˼·��
    //1�����ڿ�ʼ���ţ���#�ӵ�results
    //2��������ǰ�ķ����е��Ҳ���ʽ��
    //2.1 ������ǰ�Ҳ����з��������������vn����ɽ�����һ�����Ի�ȡresultsԪ��
    //�����ǰ�ַ�vn�����һ���ַ���˵��λ�ھ�β�����#����
    //�������vn����ַ�
    // ����ٴ�����vn�����˲��˳�ѭ�������ⲿѭ��
    // ��������ս����ֱ�Ӽ��뵽results����break�˳�ѭ��
    // ������Ƿ��ս������ô����first���ϣ�ȥ���պ���뵽results
    // ��ʱ��Ҫ�����Ǽ���ѭ����������ѭ����
    //�����ǰ�ַ������ƿգ����Ҳ������һ���ַ���˵����Ҫ����������һ���ַ�
    //��������ƿյ��������һ���ַ�����ô��#����results
    //����������ƿգ�ֱ������ѭ�����ɣ������ƿգ������ַ���first���ϲ��п�����Ϊvn��follow���ϣ�
    //3��������ɣ�����results������������£�
    set<char> results; // �洢�����
    if (vn == S) // ����ǿ�ʼ����
    {
        results.insert('#'); // ����ֹ���ӽ�ȥ����Ϊ�����#S#
    }

    // �����ķ����е��Ҳ�����
    for (auto iter = formulas.begin(); iter != formulas.end(); ++iter)
    {
        string right = (*iter).substr(3); // ��ȡ��ǰ�Ҳ�
        // ������ǰ�Ҳ������Ƿ��е�ǰ����
        for (auto i_ch = right.begin(); i_ch != right.end();)
        {
            if (*i_ch == vn[0]) { // ���vn�������˵�ǰ�Ҳ�
                if ((i_ch+1)==right.end()) // vn�ǵ�ǰ�Ҳ����һ���ַ�
                {
                    results.insert('#'); // ������ֹ��
                    break;
                }
                else { // vn���滹���ַ�,�������ǣ�����������vn��i_ch����һ�����ҽ�������ѭ����
                    while (i_ch != right.end())
                    {
                        ++i_ch;// ָ�����
                        if (*i_ch == vn[0])
                        {
                            --i_ch;
                            break;
                        }
                        if (isVn(*i_ch)) // ������ַ��Ƿ��ս������first���еķǿ�Ԫ�ؼӽ�ȥ
                        {
                            set<char> tmp_f = FIRST[string(1, *i_ch)];
                            tmp_f.erase('~'); // ��ȥ��
                            results.insert(tmp_f.begin(), tmp_f.end());


                            // ��Ҫ���ַ��ɷ��ƿգ���Ҫ�����Ƿ����ѭ��
                            if (canToEmpty(*i_ch))
                            {
                                if ((i_ch + 1) == right.end()) // ��������һ���ַ�������#
                                {
                                    results.insert('#');
                                    break;// ����ѭ��
                                }
                                // ����ѭ��
                            }
                            else // ��������ѭ��
                                break;
                        }
                        else {  // ������ַ����ս��
                            results.insert(*(i_ch));  // ������ַ�
                            break;  // ����ѭ��
                        }
                    }
                }
            }
            else {
                ++i_ch;
            }
        }
    }
    return results;
}
// ����Follow����
void completeFollow(string vn) {
    // �����ķ����е��Ҳ�����
    for (auto iter = formulas.begin(); iter != formulas.end(); ++iter)
    {

        string right = (*iter).substr(3); // ��ȡ��ǰ�Ҳ�
        // ������ǰ�Ҳ������Ƿ��е�ǰ����
        for (auto i_ch = right.begin(); i_ch != right.end();)
        {
            char vn_tmp = *i_ch;
            if (vn_tmp == vn[0]) { // ���vn�������˵�ǰ�Ҳ�
                if ((i_ch + 1) == right.end()) // vn�ǵ�ǰ�Ҳ����һ���ַ�
                {
                    char left = (*iter)[0];
                    set<char> tmp_fo = FOLLOW[string(1,left)]; // ��ȡ�󲿵�follow����
                    set<char> follows = FOLLOW[string(1,vn_tmp)]; // ��ȡ�Լ���ԭ����follow����
                    follows.insert(tmp_fo.begin(),tmp_fo.end());
                    FOLLOW[vn] = follows; // �޸�
                    break;
                }
                else { // �������һ���ַ�����Ҫ����֮����ַ����Ƿ�����ƿ�
                    while (i_ch != right.end())
                    {
                        ++i_ch; // ע��ָ������ˣ�����
                        if (canToEmpty(*i_ch))
                        {
                            if ((i_ch+1)!=right.end()) // �������һ��Ԫ�أ���Ҫ������������û�п����ƿյ�
                            {
                                continue;
                            }
                            else { // ���һ��Ҳ���ƿ�,����󲿼ӽ�ȥ
                                char left = (*iter)[0];
                                set<char> tmp_fo = FOLLOW[string(1, left)]; // �󲿵�follow����
                                set<char> follows = FOLLOW[string(1, vn_tmp)]; // ��ǰ���ŵ�follow����
                                follows.insert(tmp_fo.begin(), tmp_fo.end());
                                FOLLOW[vn] = follows; // �޸�ԭֵ
                                break;
                            }
                        }
                        else  // ��������ƿգ����˳�ѭ��
                            break;
                    }
                }
            }
            ++i_ch; // ����Ѱ��vn�Ƿ����
        }
    }
}
// ���Follow����ʹ�ù�ϣ��洢
void findFollow() {
    // �������з��ս�����������follow����
    for (auto iter = Vn.begin(); iter != Vn.end(); ++iter) {
        string vn = *iter; // ��ȡ���ս��
        set<char> follows = findOneFollow(vn); // ���һ��Vn��follow��
        FOLLOW.insert(pair<string, set<char>>(vn, follows)); // �洢����ϣ����߲�ѯЧ��
    }
    // ����follow����ֱ��follow��������
    int old_count = getFCount();
    int new_count = -1;
    while (old_count != new_count) // �ս���ڱ仯�������������ֱ��follow���ϲ�������
    {
        old_count = getFCount();
        // �ٴα������ս��������������Ҳ���ĩ�˵ģ����󲿵�follow���ӽ���
        for (auto iter = Vn.begin(); iter != Vn.end(); ++iter) {
            string vn = *iter; // ��ȡ���ս��
            completeFollow(vn);
        }
        new_count = getFCount();
    }
    // ��ʾ���
    cout << endl << setw(75) << right << setfill('=') << "FOLLOW������====================================" << endl;
    for (auto iter = FOLLOW.begin(); iter != FOLLOW.end(); ++iter) {
        cout << "FOLLOW(" << iter->first << ")" << "= ";
        set<char> sets = iter->second;
        printSet(sets);
    }
}
// �������ʽ���Select����
set<char> findOneSelect(string formula) {
    // ���˼·
    // 1���õ�����ʽ��left��right
    // 2�������Ҳ�����ʽ�����ȷ����Ҳ���һ���ַ�:right[0]
    // ������ս���������Ϊ�շ������follow(left)����results������ֱ�ӰѸ÷��ż��뵽results��,Ȼ��break
    // ����Ƿ��ս������first(right[0])-'~'���뵽results������������ƿգ���Ҫ�������󿴣�continue��
    set<char> results; // �洢���
    // 1���õ�����ʽ��left��right
    char left = formula[0]; // ��
    string right = formula.substr(3); // �Ҳ�
    //cout << "Select���Ϸ���" << left << "->" << right << endl;// ������
    // 2�������Ҳ�����ʽ�����ȷ����Ҳ���һ���ַ�:right[0]
    for (auto iter = right.begin(); iter != right.end(); ++iter)
    {
        //cout << "�����Ҳ�" << *iter << endl; // ������
        // ����Ƿ��ս������first(right[0])-'~'���뵽results������������ƿգ���Ҫ�������󿴣�continue��
        if (isVn(*iter))
        {
            set<char> chs = FIRST.find(string(1, *iter))->second; // �õ��÷��ŵ�first��
            chs.erase('~'); // ȥ���շ�
            results.insert(chs.begin(), chs.end()); // ����select
            if (canToEmpty(*iter)) // ��������ƿգ�����������һ���ַ����뵽select����
            {
                if ((iter+1)==right.end()) // ��ǰ�����һ���ַ������follow(left)����results,Ȼ��break
                {
                    set<char> chs = FOLLOW.find(string(1, left))->second; // �õ��󲿵�follow
                    results.insert(chs.begin(), chs.end()); // ����select
                }
                else { // ����������һ�ַ�
                    continue;
                }
            }else
                break; // ���ַ��������ƿգ��˳�ѭ��
        }
        else {// ������ս���������Ϊ�շ������follow(left)����results������ֱ�ӰѸ÷��ż��뵽results��,Ȼ��break
            if (*iter == '~') // ����ǿ�
            {
                set<char> chs = FOLLOW.find(string(1, left))->second; // �õ��󲿵�follow
                results.insert(chs.begin(), chs.end()); // ����select
            }
            else
                results.insert(*iter); // ֱ�Ӽ���select
            break; // �˳�ѭ��
        }
    }

    return results;
}
// ���Select����ʹ�ù�ϣ��洢
void findSelect() {
    // �������ʽ����
    for (auto iter = formulas.begin(); iter != formulas.end(); ++iter) {
        string formula = *iter; // ��ȡ���ʽ
        set<char> selects = findOneSelect(formula); // ���һ��Vn��first��
        SELECT.insert(pair<string, set<char>>(formula, selects));  // ���뵽��ϣ����߲�ѯЧ��
    }

    // ��ʾ���
    cout << endl << setw(75) << right << setfill('=') << "SELECT������====================================" << endl;
    for (auto iter = SELECT.begin(); iter != SELECT.end(); ++iter) {
        cout << "SELECT(" << iter->first << ")" << "= ";
        set<char> sets = iter->second;
        printSet(sets);
    }
}
// �ж��Ƿ�ΪLL(1)����
void isLL1() {
    // ���˼·��ͨ��Ƕ��ѭ��SELECT���ϣ��жϲ�ͬ�ı��ʽ������ͬʱ��SELECT����֮���ཻ�Ƿ��н���
    // ����н���˵������LL1��������LL1����
    for (auto i1 = SELECT.begin(); i1 != SELECT.end(); ++i1)
    {
        for (auto i2 = SELECT.begin(); i2 != SELECT.end(); ++i2)
        {
            char left1 = (i1->first)[0]; // ��ȡ��2
            char left2 = (i2->first)[0]; // ��ȡ��2
            if (left1 == left2) // �����
            {
                if (i1->first != i2->first) //���ʽ��һ��
                {
                    if (isIntersect(i1->second, i2->second)) { // ���select�����н���
                        // ����LL1�ķ�
                        cout << "��������"<<i1->first<<"��"<<i2->first<<"��select�����н���"<<"����������ķ�������LL(1)�ķ������޸ĺ�����" << endl;
                        exit(0); // ֱ���˳�
                    }
                }
            }
        }
    }
    // ��LL��1���ķ�
    cout << setw(75) << right << setfill('=') << "���������======================================" << endl << endl;
    cout << "������������������ķ�����LL(1)�ķ�..." << endl;
}
// ����Ԥ�������
void makeTable() {
    cout << "����Ϊ�����������..." << endl;
    // ���˼·��
    // 1������select���ϣ����ڼ�����Ϊleft��->right;����ֵ�������󵥸��ַ�ch��
    // ��left��ch�����ΪTABLE�ļ�����->right��Ϊֵ
    // map��ֵ�Ե���ʽ���ռ���࣬��ѯЧ�ʸߵ�
    char left_ch;
    string right;
    set<char> chars;
    for (auto iter = SELECT.begin(); iter != SELECT.end(); ++iter) // ����select����
    {
        left_ch = iter->first[0]; // ��ȡ��
        right = iter->first.substr(1); // ��ȡ->�Ҳ�
        chars = iter->second;
        // ����chars.һ��������
        for (char ch : chars) { // �����ս��
            TABLE.insert(pair<pair<char, char>,string>(pair<char, char>(left_ch, ch),right));
        }
    }
    // ���������
    printTable();
}
// ���Ԥ�������
void printTable() {
    // ���������
    cout << setw(75) << right << setfill('=') << "Ԥ�������====================================" << endl;
    cout << setw(8) << left << setfill(' ') << "VN/VT";
    set<string> vts = Vt;
    vts.erase("~");
    vts.insert("#");
    for (string str : vts) // �����ս��
    {
        cout << setw(12) << left << setfill(' ') << str;
    }
    cout << endl << endl;
    for (string vn : Vn)
    {
        cout << setw(7) << left << setfill(' ') << vn;
        for (string vt : vts) // �����ս��
        {
            if (TABLE.find(pair<char, char>(vn[0], vt[0])) == TABLE.end()) //����Ҳ���
            {
                cout << setw(6) << left << "ERR" << " ";
            }
            else {
                cout << setw(6) << left << TABLE.find(pair<char, char>(vn[0], vt[0]))->second << " ";
            }
        }
        cout << endl;
    }
    cout << setw(115) << setfill('=') << " " << endl<< endl<< endl;
}
// ��ȡToken���еı��봮
void getToken(){

    ifstream myfile(".\\token.txt");
    if (!myfile.is_open()){
        cout << "���ܴ�!";
        system("pause");
        exit(1);
    }

    vector<string> vec;
    string temp;

    while (getline(myfile, temp))             //����getline������ȡÿһ�У���������Ϊ��λ���뵽vector
    {
        vec.push_back(temp);
    }

    cout << "����Token.txt����..." << endl;
    for (auto it = vec.begin()+1; it != vec.end(); it++)         //��1��Ϊ��ͷ����ӵ�2�п�ʼ��ȡ����
    {
        istringstream is(*it);                    //��ÿһ�е����ݳ�ʼ��һ���ַ�����������
        string s;
        int pam = 0;

        while (is >> s)                          //�Կո�Ϊ�磬��istringstream������ȡ�����뵽����s��
        {
            if (pam == 2)                       //��ȡ�ڶ��е�����
            {
                int r = atoi(s.c_str());     //����������ת������string����ת����int
                tokenClass.push_back(r);
            }
            pam++;
        }
    }
    cout << "��ȡ�ı�������Ϊ��" << endl;

    for (auto it = tokenClass.begin(); it != tokenClass.end(); it++)
    {
        cout << *it << " ";
    }
    cout<<endl;
}
//��Token���б��봮ת��Ϊ��Ӧ�ս���ַ���
void tokenToStr(){
    for (auto it = tokenClass.begin(); it != tokenClass.end(); it++){
        switch(*it){
            case 1: tokenStr.push_back('$'); break;    //if         ---$
            case 2: tokenStr.push_back('@'); break;    //else       ---@
            case 3: tokenStr.push_back('o'); break;    //for        ---o
            case 4: tokenStr.push_back('w'); break;    //while      ---w
            case 5: tokenStr.push_back('b'); break;    //break      ---b
            case 6: tokenStr.push_back('r'); break;    //return     ---r
            case 7: tokenStr.push_back('t'); break;    //continue   ---t
            case 8: tokenStr.push_back('f'); break;    //float       ---f
            case 9: tokenStr.push_back('i'); break;    //int         ---i
            case 10: tokenStr.push_back('c'); break;   //char        ---c
            case 11: tokenStr.push_back('d');break;    //��ʶ��       ---d
            case 12: tokenStr.push_back('n');break;    //����         ---n
            case 13: tokenStr.push_back('+');break;    //�Ӻ�         ---+
            case 14: tokenStr.push_back('-');break;    //����         ----
            case 15: tokenStr.push_back('*');break;    //�˺�         ---*
            case 16: tokenStr.push_back('/');break;    //����         ---/
            case 17: tokenStr.push_back('%');break;    //����         ---��
            case 18: tokenStr.push_back('>');break;    //����         --->
            case 19: tokenStr.push_back('m');break;    //��С��        ---m  (min)
            case 20: tokenStr.push_back('<');break;    //С��          ---<
            case 21: tokenStr.push_back('a');break;    //������        ---a  (max)
            case 22: tokenStr.push_back('u');break;    //������        ---u  (unequal)
            case 23: tokenStr.push_back('e');break;    //����          ---e  (equal)
            case 24: tokenStr.push_back('!');break;    //��̾��        ---��
            case 25: tokenStr.push_back('&');break;    //��           ---&
            case 26: tokenStr.push_back('|');break;    //��           ---|
            case 27: tokenStr.push_back(',');break;    //����         ---,
            case 28: tokenStr.push_back('=');break;    //��ֵ         ---=
            case 29: tokenStr.push_back('[');break;    //������      ---[
            case 30: tokenStr.push_back(']');break;    //�ҷ�����      ---]
            case 31: tokenStr.push_back('(');break;    //��Բ����      ---(
            case 32: tokenStr.push_back(')');break;    //��Բ����      ---)
            case 33: tokenStr.push_back('{');break;    //������      ---{
            case 34: tokenStr.push_back('}');break;    //�һ�����      ---}
            case 35: tokenStr.push_back(';');break;    //�ֺ�         ---;
            case 36: tokenStr.push_back('.');break;    //�������      ---.
            case 37: tokenStr.push_back('l');break;    //���з�        ---l  (line feed)
            case 38: tokenStr.push_back('#');break;    //�ļ�����      ---#
            default: cout<<"ERROR!δ֪����!";
        }
    }
    cout << "��ȡ��Ӧ���ַ���Ϊ��" << endl;
    for (auto it = tokenStr.begin(); it !=tokenStr.end(); it++){
        cout << *it << " ";
    }
    cout<<endl;
}
// �����ַ���
void LL_1Analyse() {
    // ���˼·��
    //1����������ջ����#����ʼ����ջ
    //2���������봮��һ����������a��ջ����X���������
    // 2.1 ���X���ս��
    // �����a��ȣ�˵��ƥ��ɹ���X��ջ������ȡ��һ���ַ�
    // �������޷�ƥ�䣺ʧ���˳�
    // 2.2 ���X����ֹ��
    // aҲ����ֹ�������ܷ����ַ������ɹ��˳�
    // a������ֹ���������ܷ����ַ�����ʧ���˳�
    // 2.3 ����X���Ƿ��ս��
    // ����Ԥ����������Ƿ��б��ʽ
    // ���û�У���������ʧ���˳�
    // ����У�XԪ�س�ջ�����ʽ�����ջ������ѭ�����봮��Ҫ�ظ�����a
    //3��������ɣ��������
    cout << "������ɣ���������token�����Ӧ���ַ���..." << endl;
    string str;    // ���봮
    str = tokenStr;
    cout << "���ڷ���..." << endl;
    cout << endl << setw(str.length()+40) << right << setfill('=') << "�����봮�ķ�������================================================" << endl;

    cout << setw(8) << left << setfill(' ') << "����";
    cout << setw(23) << left << setfill(' ') << "����ջ";
    cout << setw(str.length()+3) << left << setfill(' ') << "ʣ�����봮";
    cout << setw(16) << left << setfill(' ') << "�������" << endl;

    stack<char> stack_a; // ����ջ
    stack_a.push('#'); // ��ֹ����ջ
    stack_a.push(S[0]); // ��ʼ���Ž�ջ

    // ��ʼ����ʾ����
    int step = 1; // ������
    stack<char> stack_remain = stack_a; // ʣ�����ջ
    string str_remain = str; // ʣ�����봮
    string str_situation = "������"; // �������
    int line=1;        //��ǰ������token����������
    int pos=1;         //��¼������token��λ�����봮�ĵڼ����ַ�
    int sumpos=0;       //��¼ǰ����������а����˶��ٸ��ַ������ں�������������ַ���׼ȷλ�á�
    // ��ʼ������ʾ
    cout << setw(8) << left << setfill(' ') << step;
    cout << setw(20) << left << setfill(' ') << getStackRemain(stack_remain);
    cout << setw(str.length()+3) << left << setfill(' ') << str_remain;

    // ������������ַ��������ս�������з�����ֹ����ɣ���һ�����ַ�����
    for (auto iter = str.begin(); iter != str.end();) {
        char a = *iter; // ���봮�͵�ǰ��ȡ�����ַ�a
        char X = stack_a.top(); // ջ��Ԫ����X
        if (a == 'l')//�������з���line+1,��ȡ��һ���ַ�
        {
            line++;     sumpos=pos;
            for (auto i_r = str_remain.begin(); i_r != str_remain.end(); i_r++)
            {
                if (*i_r == a) {
                    str_remain.erase(i_r);
                    break; // ֻɾ����һ��,
                }
            }
            iter++;pos++;
            str_situation = "�����С�";
        }
        else if (isVt(X)) // ���X��Vt�ս����ջ��Ԫ�س�ջ��Ȼ���ȡ��һ���ַ�
        {
            if (X == a) // �������ַ�ƥ��
            {
                stack_a.pop(); // �Ƴ�ջ��Ԫ��
                // ��ʣ�����봮���Ƴ���Ԫ��
                for (auto i_r = str_remain.begin(); i_r != str_remain.end(); i_r++)
                {
                    if (*i_r == a) {
                        str_remain.erase(i_r);
                        break; // ֻɾ����һ��
                    }
                }
                // ������װ��ʾ�ַ���
                string msg = "��" + string(1, a) + "��ƥ��";
                str_situation = msg;
                // ��ȡ��һ���ַ�
                iter++;pos++;
            }
            else { // �޷�ƥ�䣬��������
                str_situation="�����ִ���!!!��";
                cout << setw(16) << left << setfill(' ') << str_situation << endl;
                cout << "\nλ�ڵ�"<<line<<"�еĵ�"<<pos-sumpos<<"���ַ���������";
                cout << "�� �������� ��"<<X<<"�� ������ ��"<<a<<"�� ��" << endl;
                exit(-1); // �����˳�
            }
        }
        else if (X == '#') // �ķ���������
        {
            if (a == '#') // ��ǰ����Ҳ�����һ������ �� ���ܷ������
            {
                str_situation="�����ܡ�";
                cout << setw(16) << left << setfill(' ') << str_situation << endl;
                exit(0); // �ɹ��˳�
            }
            else {
                str_situation="�����ִ���!!!��";
                cout << setw(16) << left << setfill(' ') << str_situation << endl;
                cout << "���������ķ��Ƶ����������봮δ����!" << endl;
                exit(-1);
            }
        }
        else  // X��Vn���ս��,�鿴Ԥ������Ƿ��ж�Ӧ���ʽ
        {
            // �鿴TABLE��X��a���Ƿ��н��
            if (TABLE.find(pair<char, char>(X, a)) == TABLE.end()) //����Ҳ���
            {
                if (!canToEmpty(X)) // Ҳ�����ƿ�
                {
                    str_situation="�����ִ���!!!��";
                    cout << setw(16) << left << setfill(' ') << str_situation << endl;
                    cout << "\nλ�ڵ�"<<line<<"�еĵ�"<<pos-sumpos<<"���ַ���������";
                    cout <<"����λ���ϲ�Ӧ�ó��� ��"<<a<<"�� ��" << endl;
                    exit(-1); // ʧ���˳�
                }
                else {  // �����ƿգ�
                    stack_a.pop(); // �Ƴ�ջ��Ԫ��	// ������װ�ַ���
                    str_situation.clear();
                    str_situation.push_back(X);
                    str_situation = str_situation + "->";
                    str_situation = str_situation + "~";
                }
            }
            else //����ҵõ���Ӧ�ı��ʽ
            {
                stack_a.pop();// �Ƚ���ǰ���ų�ջ
                string str = TABLE.find(pair<char, char>(X, a))->second.substr(2); // ��ȡ���ʽ���������ջ(��ȥ->)
                // ������װ�ַ���
                str_situation.clear();
                str_situation.push_back(X);
                str_situation = str_situation + "->";
                str_situation = str_situation +str;

                reverse(str.begin(),str.end());
                for (auto iiter = str.begin(); iiter != str.end(); ++iiter)
                {
                    if (*iiter != '~')
                    {
                        stack_a.push(*iiter);
                    }
                }
                // ��Ҫ����ʶ����ַ���
            }
        }
        // ������ʾ����
        step++; // ��������1
        stack_remain = stack_a; // ��ʣ��ջΪ��ǰջ
        // ÿ��ѭ����ʾһ��
        cout << setw(16) << left << setfill(' ') << str_situation << endl;
        cout << setw(8) << left << setfill(' ') << step;
        cout << setw(20) << left << setfill(' ') << getStackRemain(stack_remain);
        cout << setw(str.length()+3) << left << setfill(' ') << str_remain;

    }
}

// =====================================��������===================================
// �����󲿷���ĳһ����ʽ���Ҳ�����
vector<string> getRights(string left){
    vector<string> rights;
    if (grammar.find(left)== grammar.end()) // �﷨��û����һ�ֱ�ӷ��ؿ�
    {
        return rights;
    }
    else {
        string str = grammar.find(left)->second;

        str = str + '|';   // ĩβ�ټ�һ���ָ����Ա��ȡ���һ������
        size_t pos = str.find('|');//find�����ķ���ֵ�����ҵ��ָ������طָ�����һ�γ��ֵ�λ�ã�
        //���򷵻�npos
        //�˴���size_t������Ϊ�˷���λ��
        while (pos != string::npos)
        {
            string x = str.substr(0, pos);//substr������������ַ���
            rights.push_back(x);          // ����right����
            str = str.substr(pos + 1);     // �����ַ���
            pos = str.find('|');         // ���·ָ���λ��
        }
        return rights;
    }
}

// �ж��Ƿ�Ϊ�ս��
bool isVn(char v) {
    if (v >= 'A' && v <= 'Z') { // ��д��ĸ���Ƿ��ս��
        return true;
    }
    else {
        return false;
    }
}
// �ж��Ƿ�Ϊ���ս��
bool isVt(char v) {
    if (isVn(v) || v == '#' || v == '|' || v=='l') // ����Ƿ��ս������ֹ�������з����ָ���,�����ս��
    {
        return false;
    }
    return true;
}

// �ж�ĳ�����ս���ܷ��Ƴ���
bool canToEmpty(char vn) {
    vector<string> rights = getRights(string(1,vn)); // vn�����Ƴ����Ҳ���
    for (auto i = rights.begin();i!=rights.end();++i) // �����Ҳ����ϣ����ǰ����Ҳ������ƿտ���ǰ����,��Ȼ��Ҫ�������
    {
        string right = *i; // ��Ϊһ���Ҳ�
        // ��������Ҳ�
        for (auto ch = right.begin(); ch != right.end(); ++ch) {
            if ((*ch)=='~')// ���chΪ�գ�˵�������ƿգ���Ϊ�����ܴ����Ҳ���"��b"���������������Ҫ���Ƿ������һ���ַ���
            {
                return true;
            }
            else if (isVn(*ch)) { // �����vn����Ҫ�ݹ�
                if (canToEmpty(*ch))// ��������ƿ�
                {
                    // ���������һ���ַ����򷵻�true
                    //������ܴ���"AD"A->��D�����ƿյ������������Ҫ���Ƿ����һ���ַ�
                    if ((ch + 1) == right.end())
                    {
                        return true;
                    }
                    continue; // ��ǰ�ַ������ƿգ����������һ���ַ����޷�ȷ���ܷ��ƿգ�����Ҫ���Ҳ�����һ���ַ�
                }
                else  // ����������ƿգ�˵����ǰ�Ҳ��������ƿգ���Ҫ����һ���Ҳ�
                    break;
            }
            else // ����Ƿǿ�vt˵��Ŀǰ�Ҳ������ƿգ���Ҫ����һ���Ҳ�
                break;
        }
    }
    return false;
}

// �ж������ַ�set�Ľ����Ƿ�Ϊ��
bool isIntersect(set<char> as, set<char> bs) {
    for (char a : as) {
        for (char b : bs) {
            if (a == b)
            {
                return true;
            }
        }
    }
    return false;
}

// �õ������ַ���
string getStackRemain(stack<char> stack_remain) {
    string str;// ʣ�����ջ��
    while (!stack_remain.empty())
    {
        str.push_back(stack_remain.top());
        stack_remain.pop();// ��ջ
    }
    reverse(str.begin(),str.end());
    return str;
}

// ��ʾ���һ��char��
void printSet(set<char> sets) {
    cout << "{ ";
    for (auto i = sets.begin(); i != sets.end();) {
        cout << *i;
        if (++i != sets.end())
        {
            cout << " ,";
        }
    }
    cout << " }" << endl;
}

// ��FOLLOW�����е�Ԫ�ظ���
int getFCount() {
    int count = 0;
    for (auto iter = FOLLOW.begin(); iter != FOLLOW.end(); ++iter) {
        count = count + iter->second.size();
    }
    return count;
}
