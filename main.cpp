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

int main(){
    char line[100];
    cout<<"Please enter the expression you want to calculate: "<<endl;
    while(1){
        cin.getline(line,100);
        char *p;
        p=line;
        Delspace(p);
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
    if(strlen(line)==1){
        num.push(line[0]-'0');
        return num.top();
    }
    while(i<=strlen(line)-1){
        if(!(is_operator(line[i]))&&line[i]!='\0'){//一个数字的读取存栈
            double sum=0;
            int po=-1;
            if(line[i]!='.'){
                
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

}