
#include "calculator.hpp"
#include <string>
#include <cmath>
#include <iostream>
#include <vector>
#define epsilon INT_MIN

using namespace std;
// Scanner implementation

// You may have to modify this constructor, although it might not be neccessary.
Scanner::Scanner(){
    line = 1;
    value = 0;
    current_token_length = 1;
    very_end_newline = 1;
    string s_line;
    total_input = "";
    char single_char;
    while (cin.get(single_char)) {
        total_input += single_char;
        
    }
    // total_input = total_input.substr(0,total_input.size()-1);
    // if (total_input[0] == '-' && total_input[1] == 'n'){
    //     total_input = total_input.substr(2,total_input.size());
    // }

    //cout << "total_input after constructor: " << total_input << " ";

    //cout << " total_input: "<< total_input << endl;
}

// You need to fill this method with the appropriate code for it to work as described in the project description.
Token Scanner::nextToken() {
    // I am a placeholder. Change me into something that can actually decypher the next token without consuming it.
    int i = 0;
    current_token_length = 1;
    Token result;
    while (total_input[i] == ' '){
       i++;
       current_token_length++;
    }

    //string newln = total_input.substr(0,2);
    //cout << " euqal to \n? " << (newln == '\\n') << endl;

    if (total_input.size() == 0)
        tokens.push_back(T_EOF);
    // else if (newln == "\\n"){
    //     tokens.push_back(T_NEWLN);
    //     current_token_length++;
    // }
    else if (total_input[i] == '+')
        tokens.push_back(T_PLUS);
    else if (total_input[i] == '-')
        tokens.push_back(T_MINUS);
    else if (total_input[i] == '*')
        tokens.push_back(T_MULTIPLY);
    else if (total_input[i] == '/')
        tokens.push_back(T_DIVIDE);
    else if (total_input[i] == 'm'){
        i++;
        if (total_input[i] == 'o'){
            i++;
            if (total_input[i] == 'd'){
                tokens.push_back(T_MODULO);
                current_token_length += 2;
            }
            else{
                scanError(line, total_input[i]);
            }
        }
        else{
            scanError(line, total_input[i]);
        }

    }
    else if (total_input[i] == '(')
        tokens.push_back(T_OPENPAREN);
    else if (total_input[i] == ')')
        tokens.push_back(T_CLOSEPAREN);
    else if (total_input[i] == ';'){
        tokens.push_back(T_SEMICOLON);
        //very_end_newline = 0;
    }
    else if (total_input[i] - '0' >= 0 && total_input[i] - '0' <= 9){
        string s = "";
        while (total_input[i] - '0' >= 0 && total_input[i] - '0' <= 9){
            s += total_input[i];
            current_token_length++;
            i++;
        }
        current_token_length--;
        tokens.push_back(T_NUMBER);
        value = stoi(s);
    }
    else if (total_input[i] == '\n'){
        // if (very_end_newline){
        //     tokens.push_back(T_EOF);
        // }
        // else
        tokens.push_back(T_NEWLN);
    }
        
    else
        scanError(line, total_input[i]);
    //cout << "total_input: " << total_input << " ";
    // for (int i = 0; i < tokens.size(); i++){
    //     cout << tokenToString(tokens[i]) << " ";
    // }
    result = tokens[0];
    tokens.clear();
    return result;
}

// You need to fill this method with the appropriate code for it to work as described in the project description.
void Scanner::eatToken(Token toConsume) {
    total_input = total_input.substr(current_token_length,total_input.size());
    if (toConsume == T_NEWLN)
        line++;
    current_token_length = 1;
    // I am a placeholder. I'm not even fun. Change me into something that can actually consume tokens!
    // WRITEME
}

int Scanner::lineNumber() {
    return this->line;
}

int Scanner::getNumberValue() {
    return this->value;
}

// Parser implementation

// You may need to modify this constructor and make it do stuff, although it might not be neccessary.
Parser::Parser(bool eval){
    evaluate = eval;
    result = 0;
    need_to_go_back = 0;
}

void Parser::parse() {
    start();
}

void Parser::start() {
    return_value.push_back(E());
    while (scanner.nextToken() == T_SEMICOLON){
        scanner.eatToken(T_SEMICOLON);
        return_value.push_back(E());
    }
    Token t;
    while((t = scanner.nextToken()) == T_NEWLN){
        scanner.eatToken(T_NEWLN);
    }
    if (t != T_EOF)
        parseError(scanner.lineNumber(), t);


    if (evaluate){
        for (vector<long>::iterator it = return_value.begin() ; it != return_value.end(); ++it)
            cout << *it << endl;
    }
    
    //cout << "result:" << endl;
    

}

long Parser::E() {
    long f = F();
    //cout << "F: " << f << endl;
    long e_p = E_prime(f);
    //cout << "e_p: " << e_p << endl;
    return e_p;
    

}
long Parser::E_prime(long value) {
    if (value >= INT_MAX)
        outOfBoundsError(scanner.lineNumber(), value);
    //cout << "enter E_prime with value: " << value << endl;
    Token t = scanner.nextToken();
    //int localValue = 0;
    // else{
        if (t == T_PLUS){
            //cout << "enter PLUS case\n";
            scanner.eatToken(T_PLUS);
            long f = F();
            //cout << "enter with f = " << f << " and value = " << value << endl;
            if (scanner.nextToken() == T_MULTIPLY || scanner.nextToken() == T_DIVIDE || scanner.nextToken() == T_MODULO){
                need_to_go_back = 1;
                long e_p = E_prime(f);
                value += e_p;
                need_to_go_back = 0;
                return E_prime(value);
            }
            else{
                int e_p = E_prime(value + f);
                return e_p;
            }
        }
        else if (t == T_MINUS){
            scanner.eatToken(T_MINUS);
            long f = F();
            if (scanner.nextToken() == T_MULTIPLY || scanner.nextToken() == T_DIVIDE || scanner.nextToken() == T_MODULO){
                need_to_go_back = 1;
                long e_p = E_prime(f);
                value -= e_p;
                need_to_go_back = 0;
                return E_prime(value);
            }
            else{
                long e_p = E_prime(value - f);
                return e_p;
            }
        }

        else if (t == T_MULTIPLY){
            scanner.eatToken(T_MULTIPLY);
            long f = F();
            if (scanner.nextToken() == T_MULTIPLY || scanner.nextToken() == T_DIVIDE || scanner.nextToken() == T_MODULO){
                long e_p = E_prime(value * f);
                return e_p;
            }
            else{
                if (need_to_go_back){
                    return value * f;
                }
                else
                    return E_prime(value * f);
            }
        }

        else if (t == T_DIVIDE){
            scanner.eatToken(T_DIVIDE);
            long f = F();
            if (scanner.nextToken() == T_MULTIPLY || scanner.nextToken() == T_DIVIDE || scanner.nextToken() == T_MODULO){
                if (f == 0)
                    divideByZeroError(scanner.lineNumber(), value);
                long e_p = E_prime(value / f);
                return e_p;
            }
            else{
                if (need_to_go_back){
                    if (f == 0)
                        divideByZeroError(scanner.lineNumber(), value);
                    return value / f;
                }
                else{
                    if (f == 0)
                        divideByZeroError(scanner.lineNumber(), value);
                    return E_prime(value / f);
                }
            }
        }
        else if (t == T_MODULO){
            scanner.eatToken(T_MODULO);
            long f = F();
            if (scanner.nextToken() == T_MULTIPLY || scanner.nextToken() == T_DIVIDE || scanner.nextToken() == T_MODULO){
                long e_p = E_prime(value % f);
                return e_p;
            }
            else{
                if (need_to_go_back){
                    return value % f;
                }
                else
                    return E_prime(value % f);
            }
        }
        else if (t == T_NEWLN){
            scanner.eatToken(T_NEWLN);
            return E_prime(value);
        }

        else
            return value;

    //}
    //return e_p;
}

long Parser::F() {
    Token t = scanner.nextToken();
    //cout << "F got token: " << tokenToString(t) << endl;
    long i = 0;
    if (t == T_OPENPAREN){
        //cout << "find open paren\n";
        scanner.eatToken(T_OPENPAREN);
        i = E();
        t = scanner.nextToken();
        if (t == T_CLOSEPAREN)
            scanner.eatToken(T_CLOSEPAREN);
        else{
            mismatchError(scanner.lineNumber(), T_CLOSEPAREN, t);
        }
    }
    else if (t == T_NUMBER){
        i = scanner.getNumberValue();
        scanner.eatToken(T_NUMBER);
    }
    else if (t == T_NEWLN){
        scanner.eatToken(T_NEWLN);
        i = F();
    }
    else{
        parseError(scanner.lineNumber(), t);
    }

    // cout << tokenToString(t) << endl;

    return i;
}



// You will need to add more methods for this to work. Don't forget to also define them in calculator.hpp!
// WRITEME

