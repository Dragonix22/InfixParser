#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>

//Tokens
enum class TokenType{
    Number,
    Operator,
    LeftParen,
    RightParen
};
struct Token {
    TokenType type;
    double value;
    char op;
};

bool isOperatorChar(char c){
    return (c=='+'||c=='-'||c=='*'||c=='/');
}


std::vector<Token> tokenize(const std::string& expr){
    std::vector<Token> tokens;
    for(size_t i=0;i<expr.size();i++){
        char c=expr[i];
        if(isspace((unsigned char)c)) continue;
        //multi digit detection
        if(isdigit((unsigned char)c)){
            std::string num;
            int dots=0;
            while(i<expr.size()&&(isdigit(expr[i])||expr[i]=='.')){
                if(expr[i]=='.')dots++;
                if(dots>1) throw std::runtime_error("Invalid number");
                num+=expr[i];
                i++;
            }
            i--;//corrects loop overshoot
            tokens.push_back({TokenType::Number,std::stod(num),'\0'});
        }
        //operators
        else if(isOperatorChar(c)){
            tokens.push_back({TokenType::Operator,0,c});
        }
        else if(c=='('){
            tokens.push_back({TokenType::LeftParen,0,'\0'});
        }
        else if(c==')'){
            tokens.push_back({TokenType::RightParen,0,'\0'});
        }
        else {
            throw std::runtime_error("Invalid character");
        }
    }

    return tokens;
}

int precedence(char op){
    switch(op){
        //equivalent so bleeding between cases is not a problem(omission of break)
        case '+':
        case '-': return 1;

        case '*':
        case '/': return 2;

        default: return 0;
    }
}


std::vector<Token> infixToPostfix(std::vector<Token> tokens){
    std::stack<Token> operator_stack;//only should have operators and left parenthesis
    std::vector<Token> postfixTokens;
    
    //Shunting Yard Algorithm(infix to postfix conversion)
    for(Token t:tokens){
        if(t.type==TokenType::Number){
            postfixTokens.push_back(t);
        }   
        else if(t.type==TokenType::LeftParen){
            operator_stack.push(t);
        }
        else if(t.type==TokenType::RightParen){
            while(!operator_stack.empty()&&operator_stack.top().type!=TokenType::LeftParen){
                postfixTokens.push_back(operator_stack.top());
                operator_stack.pop();
            }
            if(operator_stack.empty()){
                throw std::runtime_error("Mismatched Parenthesis");
            }
            operator_stack.pop();
        }
        else if(t.type==TokenType::Operator){
            while(!operator_stack.empty()&&operator_stack.top().type!=TokenType::LeftParen&&(precedence(operator_stack.top().op)>=precedence(t.op))){
                postfixTokens.push_back(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.push(t);
        }
    }

    while(!operator_stack.empty()){
        if(operator_stack.top().type==TokenType::LeftParen){
            throw std::runtime_error("Mistmatched Parenthesis");
        }
        postfixTokens.push_back(operator_stack.top());
        operator_stack.pop();
    }
    return postfixTokens;
}

double applyOperator(double lhs, double rhs, char op)
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

double evaluatePostfix(std::vector<Token> postfixTokens){
    std::stack<double> values;

    for(const Token& t:postfixTokens){
        if(t.type==TokenType::Number){
            values.push(t.value);
        }
        if(t.type==TokenType::Operator){
            if(values.size()<2){
                throw std::runtime_error("Invalid Expression");
            }
            double rhs=values.top();
            values.pop();
            double lhs=values.top();
            values.pop();
            double result = applyOperator(lhs,rhs,t.op);
            values.push(result);
        }
    }
    if(values.size()!=1)
        throw std::runtime_error("Invalid postfix expression");
    return values.top();
}

void printTokenVector(std::vector<Token> tokens){
    std::cout<<"{";
    for(size_t i=0;i<tokens.size();i++){
        Token t = tokens[i];
        switch (t.type)
        {
        case TokenType::Operator:
            std::cout<<"OPERATOR("<<t.op<<")";
            break;
        case TokenType::Number:
            std::cout<<"NUMBER("<<t.value<<")";
            break;
        case TokenType::LeftParen:
            std::cout<<"LeftParen()";
            break;
        case TokenType::RightParen:
            std::cout<<"RightParen()";
            break;
        default:
            throw std::runtime_error("Invalid token");
            break;
        }
        if(i<tokens.size()-1){
            std::cout<<", ";
        }
    }
    std::cout<<"}";
}

int main(){
    std::cout << "Enter a math expression: ";
    std::string expr;
    std::getline(std::cin,expr);
    
    std::vector<Token> tokens = tokenize(expr);
    printTokenVector(tokens);
    std::cout<<std::endl;
    std::vector<Token> postfixTokens = infixToPostfix(tokens);
    printTokenVector(postfixTokens);
    std::cout<<std::endl;
    double result = evaluatePostfix(postfixTokens);
    std::cout<<"Result: "<<result;
    return(0);
}