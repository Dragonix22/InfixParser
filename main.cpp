#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
int main(){
    std::cout << "Enter a math expression: ";
    std::string expr;
    std::cin >> expr;

    std::stack<char> operatorStack;//only should have operators and left parenthesis
    std::string postfix="";
    //stores all operators with value equal to level of precedence(eg - and + have precedence 1, * and / have precedence 2, etc)
    std::unordered_map<char,int> operators={
        {'+',1},{'-',1},{'*',2},{'/',2}
    };
    //Shunting Yard Algorithm(infix to postfix conversion)
    for(int i=0;i<expr.size();i++){
        char c =expr[i];
        if(c>='0'&&c<='9'){
            //single digits only for now
            postfix+=c;
            //std::cout<<i<<" digit parsed \n";
        }
        else if(c=='('){
            operatorStack.push(c);
        }
        else if(c==')'){
            while(operatorStack.top()!='('){
                postfix+=operatorStack.top();
                operatorStack.pop();
            }
            if(operatorStack.empty()){
                throw std::runtime_error("Mismatched parentheses");
            }
            operatorStack.pop();
        }
        else if(operators[c]>=1){
            //if its + or - all operators of stack are popped, if * or / only pop those ones            
            while(!operatorStack.empty()&&c!='('&&operators[operatorStack.top()]>=operators[c]){
                char top = operatorStack.top();
                postfix+=top;                        
                operatorStack.pop();
            }
            operatorStack.push(c);
            //std::cout<<i<<" operator parsed \n";
        }
        else{
            throw std::runtime_error("Invalid character");
        }
    } 
    while(!operatorStack.empty()){
        postfix+=operatorStack.top();
        operatorStack.pop();
    }
    std::cout<<postfix<<std::endl;
    return(0);
}