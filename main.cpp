#include <iostream>
#include <string.h>
#include <algorithm>
#include <cmath>
#include <stack>
#include "BigFloat.h"
using namespace std;

bool is_operator(char op);
int priority(char op);
double calculate(double a,double b,char op);
double calculateline(char line[]);
void Delspace(char *source);
bool is_variable(char line[]);
int is_in_variable(char ch);
bool ContainFunction(string line,string func);
string SolveFunction(string line);
string BigFloat_calculate(string strline);
char findop(string line);

char variable[26];
int variablesize=0;
double variablevalue[26];

int main(){
    try
    {
    char line[100];
    cout<<"Please enter the expression you want to calculate: "<<endl;
    char *p;
    while(1){
        cin.getline(line,100);
//        cout<<strlen(line)<<endl;//
        if(strlen(line)<1)
        {
            cout<<"input null"<<endl;
            return 0;
        }
        p=line;
        Delspace(p);
        string strline=line;
//检测注释
        if(strline.find('#')!=string::npos){
            string str1=strline.substr(0,strline.find('#'));
            strline=str1;
            strcpy(line,strline.c_str());
            if(strlen(line)<1)
        {
            main();
        }
        }
        if(strline.find("/*")!=string::npos&&strline.find("*/")!=string::npos){
            string str1=strline.substr(0,strline.find("/*"));
            string str2=strline.substr(strline.find("*/")+2,strline.length()-strline.find("*/")-2);
            strline=str1+str2;
            strcpy(line,strline.c_str());
            if(strlen(line)<1)
        {
            main();
        }
        }
        if(strline.find("/*")!=string::npos){
                string str1=strline.substr(0,strline.find('/'));
                strline=str1;
                strcpy(line,strline.c_str());
                if(strlen(line)<1)
        {
            cout<<"input null"<<endl;
            return 0;
        }
            while(1){
                char line2[100];
                cin.getline(line2,100);
                if(strlen(line)<1)
            {
                cout<<"input null"<<endl;
                return 0;
            }
                char *p2;
                p2=line2;
                Delspace(p2);
                string strline2=line2;
                if(strline2.find("*/")!=string::npos)
                    break;
            }
        }

        if(is_variable(line)){
            int length=strlen(line);
            char copy[100];
//            cout<<strlen(copy)<<endl<<length-2;
            for(int i=2;i<length;i++){
                copy[i-2]=line[i];
            }
            copy[length-2]=0;
            int index=is_in_variable(line[0]);
            if(index!=-1){
                variablevalue[index]=calculateline(copy);
            }
            else{
            variable[variablesize]=line[0];
            // variable[variablesize+1]=0;
            variablevalue[variablesize]=calculateline(copy);//存储变量的值
            // variablevalue[variablesize+1]=0;
            variablesize++;
            }
            continue;
        }
        if(strline.find("mode2")!=string::npos){
            cout<<"this is mode2"<<endl;
            while (strline.find("mode1")==string::npos){
                cin.getline(line,100);
                p=line;
                Delspace(p);
                strline=line;
                cout<<BigFloat_calculate(strline)<<endl;
            }
        }
        double ans=calculateline(line);
        cout<<ans<<endl;
    }
    throw 1;}catch(const std::exception& e)
    {
        std::cout << 'a';
    }
    return 0;
}

bool is_operator(char op){
    switch (op)
    {
    case '+':
    case '-':
    case '*':
    case '/':
    case '(':
    case ')':
    case '^':
    case '%':
        return 1;
    default:
        return 0;
    }
}

void addzeros(char line[]){
    if(strlen(line)==1)
        return;
    if(line[0]=='-'){
        for(int i=strlen(line)-1;i>=0;i--){
            line[i+1]=line[i];
        }
        line[0]='0';
    }
    for(int i=0;i<strlen(line)-1;i++){
        if(line[i]=='('&&line[i+1]=='-'){
            for(int j=strlen(line)-1;j>i;j--){
                line[j+1]=line[j];
            }
            line[i+1]=='0';
        }
    }
}

int priority(char op){
    switch (op)
    {
    case '#':
        return -1;
    case '+':
    case '-':
        return 1;
    case ')':
        return 5;
    case '(':
        return 0;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    case '%':
        return 4;
    default:
        return 0;
    }
}

double calculate(double a,double b,char op){
    switch (op)
    {
    case '+':return a+b;
    case '-':return a-b;
    case '*':return a*b;
    case '/':return a/b;
    case '^':return pow(a,b);
    case '%':return (int)a%(int)b;
    default:
        break;
    }
    return 0;
}

double calculateline(char line[]){
    string source=line;
    string strline=SolveFunction(source);
    strcpy(line,strline.c_str());
    stack<char> op;
    stack<double> num;
    addzeros(line);
    op.push('#');
    num.push('#');
    int i=0;
    int index=is_in_variable(line[0]);
    if(strlen(line)==1&&line[0]>='0'&&line[0]<='9'){
        num.push(line[0]-'0');
        return num.top();
    }
    else if(strlen(line)==1&&index!=-1){
        num.push(variablevalue[index]);
        return num.top();
    }
    
    while(i<=strlen(line)-1){
        if(!(is_operator(line[i]))&&line[i]!='\0'){//一个数字的读取存栈
            double sum=0;
            int po=-1;
            index=is_in_variable(line[i]);
                if(index!=-1){//是否是变量
                    num.push(variablevalue[index]);
                    i++;
                    continue;
                }
                if((line[i]<'0'||line[i]>'9')&&line[i]!='.'&&line[i]!='\0'){
                    cout<<"Erorr, you have entered the variable that undefined"<<endl;
                    main();
                }
                while(!(is_operator(line[i]))&&line[i]!='.'&&line[i]!='\0')//当字符是数字时则循环计算
                {
                    sum=sum*10+line[i]-'0';
                    i++;
                }
                if(line[i]=='.'){
                    i++;
                while (!(is_operator(line[i]))&&line[i]!='.'&&line[i]!='\0')
                {
                    sum=pow(10,po)*(line[i]-'0')+sum;
                    po--;
                    i++;
                }
            }
            num.push(sum);
        }
        else if(line[i]=='('){
            op.push(line[i]);
            i++;
        }
        else if(line[i]==')'){
            while(op.top()!='('){
                double t1=num.top();
                num.pop();
                double t2=num.top();
                num.pop();
                double temp=calculate(t2,t1,op.top());
                op.pop();
                num.push(temp);
            }
            op.pop();
            i++;
        }
        //处理括号内数字

        else if(is_operator(line[i])&&op.top()=='#'){
            op.push(line[i]);
            i++;
        }
        else{
            if(priority(line[i])<=priority(op.top())){
                double t1=num.top();
                num.pop();
                double t2=num.top();
                num.pop();
                double temp=calculate(t2,t1,op.top());
                op.pop();
                num.push(temp);

            }
            else{
                op.push(line[i]);
                i++;   
            }
        }
    }
    while (op.top()!='#')
    {
        double t1=num.top();
                num.pop();
                double t2=num.top();
                num.pop();
                double temp=calculate(t2,t1,op.top());
                op.pop();
                num.push(temp);
    }
    return num.top();
}

void Delspace(char *source){
char *dest=source;
while(*source!=0){
if (*source!=' '){
*dest++=*source++;
}
else{
source++;
}
} 
*dest++=0;//字符串结束符
}

bool is_variable(char line[]){
    if(strlen(line)<3){
        return false;
    }
    if(!(line[0]>='a'&&line[0]<='z')){//字符若不是小字母
        return false;
    }
    if(line[1]!='='){
        return false;
    }
    return true;
}

int is_in_variable(char ch){
    for(int i=0;i<variablesize;i++){
        if(variable[i]==ch)
            return i;
    }
    return -1;
}

bool ContainFunction(string line,string func){
    if(line.find(func)!=string::npos)
        return true;
    
    else
        return false;
}

string SolveFunction(string line){
    while(ContainFunction(line,"sqrt(")){
        int start=line.find("sqrt(");
        int end;
        for(int i=start;i<line.length();i++){
            if(line[i]==')'){
                end=i;
                break;
            }
        }
        string func=line.substr(start+5,end-start-5);
        string before=line.substr(0,start);
        string after=line.substr(end+1,line.length()-end-1);
        char num[end-start-5];
        strcpy(num,func.c_str());
        if(calculateline(num)<0){
            cout<<"NaN"<<endl;
            main();
        }
        string addnum=to_string(sqrt(calculateline(num)));
        line=before+addnum+after;
 //       cout<<addnum<<endl;
    }
    while(ContainFunction(line,"pow(")){
        int start=line.find("pow(");
        int end;
        for(int i=start;i<line.length();i++){
            if(line[i]==')'){
                end=i;
                break;
            }
        }
        string func=line.substr(start+4,end-start-4);
        string before=line.substr(0,start);
        string after=line.substr(end+1,line.length()-end-1);
        int Douhao=func.find(',');
        string num1=func.substr(0,Douhao);
        string num2=func.substr(Douhao+1,end-Douhao-1);
        char cnum1[Douhao],cnum2[end-Douhao-1];
        strcpy(cnum1,num1.c_str());
        strcpy(cnum2,num2.c_str());
        string addnum=to_string(pow(calculateline(cnum1),calculateline(cnum2)));
        line=before+addnum+after;    
    }
    while(ContainFunction(line,"max(")){
        int start=line.find("max(");
        int end;
        for(int i=start;i<line.length();i++){
            if(line[i]==')'){
                end=i;
                break;
            }
        }
        string func=line.substr(start+4,end-start-4);
        string before=line.substr(0,start);
        string after=line.substr(end+1,line.length()-end-1);
        int Douhao=func.find(',');
        string num1=func.substr(0,Douhao);
        string num2=func.substr(Douhao+1,end-Douhao-1);
        char cnum1[Douhao],cnum2[end-Douhao-1];
        strcpy(cnum1,num1.c_str());
        strcpy(cnum2,num2.c_str());
        string addnum=to_string(max(calculateline(cnum1),calculateline(cnum2)));
        line=before+addnum+after;    
    }
    while(ContainFunction(line,"min(")){
        int start=line.find("min(");
        int end;
        for(int i=start;i<line.length();i++){
            if(line[i]==')'){
                end=i;
                break;
            }
        }
        string func=line.substr(start+4,end-start-4);
        string before=line.substr(0,start);
        string after=line.substr(end+1,line.length()-end-1);
        int Douhao=func.find(',');
        string num1=func.substr(0,Douhao);
        string num2=func.substr(Douhao+1,end-Douhao-1);
        char cnum1[Douhao],cnum2[end-Douhao-1];
        strcpy(cnum1,num1.c_str());
        strcpy(cnum2,num2.c_str());
        string addnum=to_string(min(calculateline(cnum1),calculateline(cnum2)));
        line=before+addnum+after;    
    }
    while(ContainFunction(line,"abs(")){
        int start=line.find("abs(");
        int end;
        for(int i=start;i<line.length();i++){
            if(line[i]==')'){
                end=i;
                break;
            }
        }
        string func=line.substr(start+4,end-start-4);
        string before=line.substr(0,start);
        string after=line.substr(end+1,line.length()-end-1);
        char num[end-start-4];
        strcpy(num,func.c_str());
        string addnum=to_string(abs(calculateline(num)));
        line=before+addnum+after;
 //       cout<<addnum<<endl;
    }
    while(ContainFunction(line,"sin(")){
        int start=line.find("sin(");
        int end;
        for(int i=start;i<line.length();i++){
            if(line[i]==')'){
                end=i;
                break;
            }
        }
        string func=line.substr(start+4,end-start-4);
        string before=line.substr(0,start);
        string after=line.substr(end+1,line.length()-end-1);
        char num[end-start-4];
        strcpy(num,func.c_str());
        string addnum=to_string(sin(calculateline(num)));
        line=before+addnum+after;
 //       cout<<addnum<<endl;
    }
    while(ContainFunction(line,"cos(")){
        int start=line.find("cos(");
        int end;
        for(int i=start;i<line.length();i++){
            if(line[i]==')'){
                end=i;
                break;
            }
        }
        string func=line.substr(start+4,end-start-4);
        string before=line.substr(0,start);
        string after=line.substr(end+1,line.length()-end-1);
        char num[end-start-4];
        strcpy(num,func.c_str());
        string addnum=to_string(cos(calculateline(num)));
        line=before+addnum+after;
 //       cout<<addnum<<endl;
    }
    while(ContainFunction(line,"tan(")){
        int start=line.find("tan(");
        int end;
        for(int i=start;i<line.length();i++){
            if(line[i]==')'){
                end=i;
                break;
            }
        }
        string func=line.substr(start+4,end-start-4);
        string before=line.substr(0,start);
        string after=line.substr(end+1,line.length()-end-1);
        char num[end-start-4];
        strcpy(num,func.c_str());
        string addnum=to_string(tan(calculateline(num)));
        line=before+addnum+after;
 //       cout<<addnum<<endl;
    }
    while(ContainFunction(line,"exp(")){
        int start=line.find("exp(");
        int end;
        for(int i=start;i<line.length();i++){
            if(line[i]==')'){
                end=i;
                break;
            }
        }
        string func=line.substr(start+4,end-start-4);
        string before=line.substr(0,start);
        string after=line.substr(end+1,line.length()-end-1);
        char num[end-start-4];
        strcpy(num,func.c_str());
        string addnum=to_string(exp(calculateline(num)));
        line=before+addnum+after;
 //       cout<<addnum<<endl;
    }
    while(ContainFunction(line,"log(")){
        int start=line.find("log(");
        int end;
        for(int i=start;i<line.length();i++){
            if(line[i]==')'){
                end=i;
                break;
            }
        }
        string func=line.substr(start+4,end-start-4);
        string before=line.substr(0,start);
        string after=line.substr(end+1,line.length()-end-1);
        char num[end-start-4];
        strcpy(num,func.c_str());
        string addnum=to_string(log(calculateline(num)));
        line=before+addnum+after;
 //       cout<<addnum<<endl;
    }
    while(ContainFunction(line,"PI")){
        int start=line.find("PI");
        int end=start+1;
        
        string func=line.substr(start,2);
        string before=line.substr(0,start);
        string after=line.substr(end+1,line.length()-end-1);
        string addnum=to_string(M_PI);
        line=before+addnum+after;
 //       cout<<addnum<<endl;
    }
    while(ContainFunction(line,"E")){
        int start=line.find("E");
        int end=start;
        
        string func=line.substr(start,1);
        string before=line.substr(0,start);
        string after=line.substr(end+1,line.length()-end-1);
        string addnum=to_string(M_E);
        line=before+addnum+after;
 //       cout<<addnum<<endl;
    }
    return line;
}

string BigFloat_calculate(string strline)
{
    BigFloat strnum1,strnum2,result;
    char op=findop(strline);
    int splitx;
    switch (op)
    {
    case 0:
        return strline;
    case '+':
        splitx=strline.find('+');
        strnum1=BigFloat_calculate(strline.substr(0,splitx));
        strnum2=BigFloat_calculate(strline.substr(splitx+1,strline.length()-splitx-1));
        result=strnum1+strnum2;
        return result.ToString();
    case '-':
        splitx=strline.find('-');
        strnum1=BigFloat_calculate(strline.substr(0,splitx));
        strnum2=BigFloat_calculate(strline.substr(splitx+1,strline.length()-splitx-1));
        result=strnum1-strnum2;
        return result.ToString();
    case '*':
        splitx=strline.find('*');
        strnum1=BigFloat_calculate(strline.substr(0,splitx));
        strnum2=BigFloat_calculate(strline.substr(splitx+1,strline.length()-splitx-1));
        result=strnum1*strnum2;
        return result.ToString();
    case '/':
        splitx=strline.find('/');
        strnum1=BigFloat_calculate(strline.substr(0,splitx));
        strnum2=BigFloat_calculate(strline.substr(splitx+1,strline.length()-splitx-1));
        result=strnum1/strnum2;
        return result.ToString();
           
    default:
        break;
    }
    return 0;
}

char findop(string line)
{
    size_t op[4];
    op[0]=line.find('+');
    op[1]=line.find('-');
    op[2]=line.find('*');
    op[3]=line.find('/');
    size_t min=op[0];
    for(int i=0;i<4;i++){
        if(i<=1&&op[i]!=string::npos){
            min=op[i];
            break;
        }
        if(op[i]<min)
            min=op[i];
    }
    if(min==string::npos)return 0;
    if(min==op[0])return '+';
    if(min==op[1])return '-';
    if(min==op[2])return '*';
    if(min==op[3])return '/';
    return 0;
}