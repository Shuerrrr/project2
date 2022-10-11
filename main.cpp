#include <iostream>
#include <string.h>
#include <cmath>
#include <stack>
using namespace std;

bool is_operator(char op);
int priority(char op);
double calculate(double a,double b,char op);
double calculateline(char line[]);
void Delspace(char *source);
bool is_variable(char line[]);
int is_in_variable(char ch);

char variable[26];
int variablesize=0;
double variablevalue[26];

int main(){
    char line[100];
    cout<<"Please enter the expression you want to calculate: "<<endl;
    char *p;
    while(1){
        cin.getline(line,100);
//        cout<<strlen(line)<<endl;//

        p=line;
        Delspace(p);
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
        double ans=calculateline(line);
        cout<<ans<<endl;
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
}

double calculateline(char line[]){
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
                    return NULL;
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
