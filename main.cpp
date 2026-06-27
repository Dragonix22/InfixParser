#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>

//stores all operators with value equal to level of precedence(eg - and + have precedence 1, * and / have precedence 2, etc)
const std::unordered_map<char,int> precedence={
    {'+',1},{'-',1},{'*',2},{'/',2}
};

bool isDigit(char c){
    return (c>='0'&&c<='9');
}

bool isOperator(char c){
    return precedence.count(c);
}

std::string infixToPostfix(const std::string& expr){
    std::stack<char> operator_stack;//only should have operators and left parenthesis
    std::string postfix="";    
    //Shunting Yard Algorithm(infix to postfix conversion)
    for(int i=0;i<expr.size();i++){
        char c =expr[i];
        if(isDigit(c)){
            //single digits only for now
            postfix+=c;
        }
        else if(c=='('){
            operator_stack.push(c);
        }
        else if(c==')'){
            while(!operator_stack.empty()&&operator_stack.top()!='('){
                postfix+=operator_stack.top();
                operator_stack.pop();
            }
            if(operator_stack.empty()){
                throw std::runtime_error("Mismatched parentheses");
            }
            operator_stack.pop();
        }
        else if(isOperator(c)){
            //if its + or - all operators of stack are popped, if * or / only pop those ones            
            while(!operator_stack.empty()&&operator_stack.top()!='('&&precedence.at(operator_stack.top())>=precedence.at(c)){
                char top = operator_stack.top();
                postfix+=top;                        
                operator_stack.pop();
            }
            operator_stack.push(c);
        }
        else{
            throw std::runtime_error("Invalid character");
        }
    } 
    while(!operator_stack.empty()){
        if(operator_stack.top()=='('){
            throw std::runtime_error("Mismatched parentheses");
        }
        postfix+=operator_stack.top();
        operator_stack.pop();
    }
    return postfix;
}

int applyOperator(int lhs, int rhs, char op)
{
    switch(op)
    {
        case '+': return lhs + rhs;
        case '-': return lhs - rhs;
        case '*': return lhs * rhs;
        case '/': return lhs / rhs;
        default:
            throw std::runtime_error("Unknown operator");
    }
}

int evaluatePostfix(const std::string& postfix){
    std::stack<int> values;
    for(size_t i=0;i<postfix.size();i++){
        char c = postfix[i];
        if(isDigit(c)){
            values.push(c-'0');
        }
        else{
            if(values.size()<2){
                throw std::runtime_error("Incorrect Postfix Expression");
            }
            int rhs=values.top();
            values.pop();
            int lhs = values.top();
            values.pop();
            
            values.push(applyOperator(lhs,rhs,c));
            
        }
    }
    if (values.size() != 1) {
        throw std::runtime_error("Invalid postfix expression");
    }
    return values.top();
}

int main(){
    std::cout << "Enter a math expression: ";
    std::string expr;
    std::cin >> expr;
    
    std::string postfix=infixToPostfix(expr);   
    std::cout<<"Postfix: "<< postfix << std::endl;

    int answer = evaluatePostfix(postfix);
    std::cout<<"Answer: "<<answer<<std::endl;

    return(0);
}