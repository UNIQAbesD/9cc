#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

enum {
    TOKEN_RESERVED,
    TOKEN_NUM,
    TOKEN_EOF
}typedef TokenKind;

typedef struct Token Token;

struct Token
{
    TokenKind kind;
    Token *next;
    int val;
    char *str;
};


//現在着目しているトークン
Token *token;

Token *new_token(TokenKind kind,Token *cur,char *str)
{
    Token *tok=calloc(1,sizeof(Token));
    tok->kind=kind;
    tok->next=NULL;
    tok->str=str;
    cur->next=tok;
    return tok;
}

void error(char *fmt, ...)
{
    va_list ap;
    va_start(ap,fmt);
    vfprintf(stderr,fmt,ap);
    fprintf(stderr,"\n");
    exit(1);
}

Token *tokenize(char *p)
{
    Token head;
    head.next=NULL;
    Token *cur=&head;
    while(*p)
    {
        if(isspace(*p))
        {
            p++;
            continue;
        }
        if(*p=='+'||*p=='-')
        {
            cur=new_token(TOKEN_RESERVED,cur,p++);
            continue;
        }
        if(isdigit(*p))
        {
            cur=new_token(TOKEN_NUM,cur,p);
            cur->val=strtol(p,&p,10);
            continue;
        }
        error("トークナイズできません");
    }
    new_token(TOKEN_EOF,cur,p);
    return head.next;
}

bool at_eof()
{
    return token->kind==TOKEN_EOF;
}

bool consume(char op)
{
    if(token->kind!=TOKEN_RESERVED||token->str[0]!=op)
    {
        return false;
    }
    token=token->next;
    return true;
    
}

void expect(char op)
{
    if(token->kind!=TOKEN_RESERVED||token->str[0]!=op)
    {
        error("'%c'ではありません\n",op);
    }
    token=token->next;
}

int expect_number()
{
    if(token->kind!=TOKEN_NUM)
    {
        error("数ではありません\n");
    }
    int val=token->val;
    token=token->next;
    return val;
}




int main(int argc,char **argv)
{
    if (argc!=2)
    {
        fprintf(stderr,"引数の個数が正しくありません\n");
        return 1;
    }
    token=tokenize(argv[1]);

    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");
    printf("   mov rax, %d\n",expect_number());
    while(!at_eof())
    {
        if(consume('+'))
        {
            printf("   add rax, %d\n",expect_number());
            continue;
        }

        expect('-');
        printf("   sub rax, %d\n",expect_number());

    }
    printf("   ret\n");
    return 0;
       
}