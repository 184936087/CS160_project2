#pragma once

#include "helpers.hpp"

#include <climits>
#include <vector>
#include <iostream>

// SCANNER DEFINITION

// You can freely add member fields and functions to this class.
class Scanner {
    int line;
    //int semicolonCount = 0;
    int value;
    int current_token_length;
    bool very_end_newline;
    std::vector<Token> tokens;
    

public:
    // You really need to implement these four methods for the scanner to work.
    std::string total_input;
    Token nextToken();
    void eatToken(Token);
    int lineNumber();
    int getNumberValue();
    
    Scanner();
};

// PARSER DEFINITION

// You can freely add member fields and functions to this class.
class Parser {
    Scanner scanner;
    std::vector<long> return_value;
    int result;
    bool evaluate;
    bool need_to_go_back;


    void start();
    // You will need to define the recursive descent functions you're going to use here.
    // void expression();
    // void term();
    // void factor();
    long E();
    long E_prime(long value);
    long F();



public:
    void parse();
    
    Parser(bool);
};
