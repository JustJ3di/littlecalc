#include <iostream>
#include <string>
#include <vector>
#include <stack>

//OPERATION TYPE
using type = enum  OP{NUMBER,SUM,SUB,MUL,DIV,LPAR,RPAR,END};
using token = struct token{
        type op;
        union {
                int i;
                char fun;
        };
        //Constructor to use emplace_back()
        token(type op,char fun):
                op(op), fun(fun){}
        token(type op,int i):
                op(op), i(i){}


};

//parse funcion
std::vector <token> parse(std::string_view &&line) {

        std::cout<<line<<std::endl;
        std::vector <token> tokens;
        for (auto i = 0; i<line.size(); ++i) {

                if (std::isspace(line[i])) {
                        continue;
                }

                char chunk = line[i];

                //Parse number
                if (std::isdigit(chunk) || (chunk == '-' && i + 1 < line.size() && std::isdigit(line[i+1]))) {
                        std::string number_str;

                        if (chunk == '-') {
                                number_str += chunk;
                                i++;
                        }


                        while (i < line.size() && std::isdigit(line[i])) {
                                number_str += line[i];
                                i++;
                        }
                        //decrement index
                        i--;

                        if (number_str.size() > (number_str[0] == '-' ? 1 : 0)) {
                                tokens.emplace_back(NUMBER, std::stoi(number_str));
                        }

                        continue;
                }
                //PARSE MATH OPERATION
                switch (chunk) {
                        case '+':
                                tokens.emplace_back(OP::SUM,'+');
                                break;
                        case '-':
                                tokens.emplace_back(OP::SUB,'-');
                                break;
                        case '*':
                                tokens.emplace_back(OP::MUL,'*');
                                break;
                        case '/':
                                tokens.emplace_back(OP::DIV,'/');
                                break;
                        case '(':
                                tokens.emplace_back(OP::LPAR,'(');
                                break;
                        case ')':
                                tokens.emplace_back(OP::RPAR,')');
                                break;
                        case '\n':
                                tokens.emplace_back(OP::END,'\n');
                                break;
                        default:
                                throw std::invalid_argument("Invalid chunk");

                }
        }

        return tokens;
}


//utilities
void print_tokens(const std::vector<token>& tokens) {

        for (const auto& token : tokens) {
                // --- Operators and Parentheses ---
                if (token.op == SUM || token.op == SUB || token.op == MUL ||
                    token.op == DIV || token.op == LPAR || token.op == RPAR) {

                        // Assuming 'token.fun' holds the actual symbol like '+', '-', '(', etc.
                        std::cout << "Operator/Paren: ";
                        std::cout << token.fun << '\n';

                    }
                // --- Numbers ---
                else if (token.op == NUMBER) {

                        // Assuming 'token.i' holds the integer value of the number
                        std::cout << "Number: ";
                        std::cout << token.i << '\n';

                }
                // --- Default/Unknown ---
                else {
                        std::cout << "Unknown Token Type (" << (int)token.op << ")\n";
                }
        }
}

//SHUNTING yard

unsigned precedence(const token& it) { // Use const reference for efficiency
        switch (it.op) {
                case MUL: case DIV: return 2;
                case SUM: case SUB: return 1;
                case LPAR: return 0;
                default: return 0;
        }
}

std::vector<token> to_rpn(std::vector<token> &&tokens) {
        std::vector<token> output;
        std::stack<token> stack;
       for (const auto& token : tokens) {
        if (token.op == NUMBER) {
            output.emplace_back(token.op, token.i);
        }


        else if (token.op == LPAR) {
            stack.push(token);
        }


        else if (token.op == SUM || token.op == SUB || token.op == MUL || token.op == DIV) {

            while (!stack.empty() && stack.top().op != LPAR &&
                   precedence(stack.top()) >= precedence(token)) {

                output.push_back(stack.top());
                stack.pop();
            }
            stack.push(token);
        }


        else if (token.op == RPAR) {

            while (!stack.empty() && stack.top().op != LPAR) {
                output.push_back(stack.top());
                stack.pop();
            }


            if (stack.empty()) {

                return {};
            }

            // Discard the LPAR from the stack
            stack.pop();
        }
    }

    // 5. After consuming all tokens, move any remaining operators to the output
    while (!stack.empty()) {
        // If an LPAR remains, it means mismatched parentheses (error)
        if (stack.top().op == LPAR) {
            // Error handling
            return {};
        }
        output.push_back(stack.top());
        stack.pop();
    }

    return output;

}

int eval(std::vector <token> &tokens) {
        std::vector<int>result;
        for (auto &&token : tokens) {
                if (token.op == NUMBER) {
                        result.emplace_back(token.i);
                }else if (token.op == SUM ) {
                        int r = result[result.size()-2] + result[result.size()-1];
                        result.pop_back();
                        result.pop_back();
                        result.emplace_back(r);
                }else if (token.op == SUB ) {
                        int r = result[result.size()-2] - result[result.size()-1];
                        result.pop_back();
                        result.pop_back();
                        result.emplace_back(r);
                }else if (token.op == MUL ) {
                        int r = result[result.size()-2] * result[result.size()-1];
                        result.pop_back();
                        result.pop_back();
                        result.emplace_back(r);
                }else if (token.op == DIV ) {
                        int r = result[result.size()-2] / result[result.size()-1];
                        result.pop_back();
                        result.pop_back();
                        result.emplace_back(r);
                }
        }

        return result.back();
}

int main() {
        std::cout<<"Hello CALC!"<<std::endl;
        while (true) {
                std::cout<<">>";
                std::string line;
                std::getline(std::cin,line);

                if (line == "end") {
                        return 0;
                }
                        std::vector<token> tokens = parse(std::move(line));

                        std::vector<token> rpn= to_rpn(std::move(tokens));
                        //print_tokens(rpn); for debug
                        std::cout<<"result = "<<eval(rpn)<<'\n';


        }
        return 0;
}



