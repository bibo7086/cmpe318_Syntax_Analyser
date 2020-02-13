//
//  main.cpp
//  Lab3_cmpe318_Syntax_Analysis
//
//  Created by Saeed Ali on 5/4/17.
//  Copyright © 2017 Saeed Ali. All rights reserved.
//  ANSI-STYLE

#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <string>
#include <string>
#include <vector>
#include <ctype.h>
#include <iostream>
#include <stdbool.h> 
using namespace std;
#define LETTER 0
#define DIGIT 1
#define FRACTION 2
#define UNKNOWN 99

int charClass;
string input = "";
char lexeme [100];
int nextToken;
int lexLen;
int token;
FILE *in_fp, *fopen();
void addChar();
void getChar();
void getNonBlank();
int lookup(char );
void unconsumed_input();
int lex();
void G();
void E();
void R();
void T();
void S();
void F();
void D();


bool error = false;
char next_token = '%';
int main()
{
    char* inputfile;
    int do_again = 0;
    
    printf("press 1 to start Syntax analyser or press 0 to exit: ");
    while(scanf("%d", &do_again) && do_again == 1){
        printf("Please enter the filepath or the name of the input file\n");
        scanf("%s", inputfile);
        
        if((in_fp = fopen(inputfile, "r")) == NULL)
        {
            perror("fopen()");
            _exit(1);
        }
        else
        {
            do{
                G();
                lex();
            }while (nextToken != EOF);
        }
        printf("\npress 1 to call lexcial analyzer again or 0 to exit: ");
        
        
    }
    printf("\n");
    return 0;
}

int lex() {
    getChar();
    while (isspace(next_token))
        getChar();

   switch (charClass) {
            case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0; break;
    }
    
    return nextToken;
}



void getChar() {
    if ((next_token = getc(in_fp)) != EOF) {
    if (isalpha(next_token))
        charClass = LETTER;
    else if (isdigit(next_token))
        charClass = DIGIT;
    else charClass = UNKNOWN;
    }
    else
        charClass = EOF;
}


void unconsumed_input()
{
    while(charClass != EOF){
        input += next_token;
        getChar();
    }
}

void G(){
    input = "";
    lex();
    printf("G -> E\n");
    E();
    
    if (next_token == '$'  && !(error))  {
        printf("success\n");
    }
    
    else {
        unconsumed_input();
        printf("failure: unconsumed input %s \n", input.c_str());
        
    }
}

/* E -> T R */
void E(){
    if (error)
        return;
    printf("E -> T R\n");
    T();
    R();
}

/* R -> + T R | - T R | e */
void R(){
    if (error )
        return;
    
    if (next_token == '+')  {
        printf("R -> + T R\n");
        lex();
        T();
        R();
    }
    else if (next_token == '-')  {
        printf("R -> - T R\n");
        lex();
        T();
        R();
    }
    else  {
        printf("R -> e\n");
    }
}

/* T -> F S */
void T(){
    if (error)
        return;
    printf("T -> F S\n");
    F();
    S();
}

/* S -> * F S | / F S | e          */
void S(){
    if (error)
        return;
    if (next_token == '*') {
        printf("S -> * F S\n");
        lex();
        F();
        S();
    }
    else if(next_token == '/') {
        printf("S -> / F S\n");
        lex();
        F();
        S();
    }
    else {
        printf("S -> e\n");
    }
}

/*         F -> ( E ) | D    */
void F(){
    if (error == true ){
        return;
    }
    if (next_token == '(' )  {
        printf("F ->( E ) \n");
        lex();
        E();
        if (error == true ){
            return;
        }
        if (next_token == ')')
        {
            lex();
        }
        
        
      
        else {
            error = true ;
            printf("error: unexptected token %c \n", next_token);
            unconsumed_input();
            printf("unconsumed_input %s \n", input.c_str());
            return;
        }
    }
    else if (isdigit(next_token)) {
        printf("F -> D \n");
        D();
        if (error == true ){
            return;
        }
    }
    
    
    else {
        error = true;
        printf("error: unexptected token %c \n", next_token);
        unconsumed_input();
        printf("unconsumed_input %s \n", input.c_str());
        
    }
}

void D(){
    if (error )
        return;
    if  (isdigit(next_token)) {
        printf("D -> %c\n", next_token);
        lex();
    }
    else {
        error= true;
        printf("error: unexptected token %c\n", next_token);
        unconsumed_input();
        printf("unconsumed_input %s \n", input.c_str());
        
    }
}
