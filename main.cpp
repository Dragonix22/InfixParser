#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
int main(){
    std::cout << "Enter a math expression: ";
    std::string expr;
    std::cin >> expr;

    std::stack<char> operator_stack;//only should have operators and left parenthesis
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
            operator_stack.push(c);
        }
        else if(c==')'){
            while(operator_stack.top()!='('){
                postfix+=operator_stack.top();
                operator_stack.pop();
            }
            if(operator_stack.empty()){
                throw std::runtime_error("Mismatched parentheses");
            }
            operator_stack.pop();
        }
        else if(operators[c]>=1){
            //if its + or - all operators of stack are popped, if * or / only pop those ones            
            while(!operator_stack.empty()&&c!='('&&operators[operator_stack.top()]>=operators[c]){
                char top = operator_stack.top();
                postfix+=top;                        
                operator_stack.pop();
            }
            operator_stack.push(c);
            //std::cout<<i<<" operator parsed \n";
        }
        else{
            throw std::runtime_error("Invalid character");
        }
    } 
    while(!operator_stack.empty()){
        postfix+=operator_stack.top();
        operator_stack.pop();
    }
    std::cout<<"Postfix: "<< postfix << std::endl;
    //postfix evaluation
    

    return(0);
}