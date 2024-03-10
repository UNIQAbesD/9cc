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
}typedef TokenKind;\

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
char *user_input;


Token *new_token(TokenKind kind,Token *cur,char *str)
{
    Token *tok=calloc(1,sizeof(Token));
    tok->kind=kind;
    tok->next=NULL;
    tok->str=str;
    cur->next=tok;
    return tok;
}




typedef enum
{
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
}NodeKind;

typedef struct Node Node;

struct Node
{
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
};

Node *new_node(NodeKind kind,Node *lhs,Node *rhs)
{
    Node *node=caloc(1,sizeof(Node));
    node->kind=kind;
    node->lhs=lhs;
    node->rhs=rhs;
    return node;
}
Node *new_node_num(int val)
{
    Node *node=calloc(1,sizeof(Node));
    node->kind=ND_NUM;
    node->val=val;
    return node;
}

void error(char *fmt, ...)
{
    va_list ap;
    va_start(ap,fmt);
    vfprintf(stderr,fmt,ap);
    fprintf(stderr,"\n");
    exit(1);
}


void error_at(char *loc,char *fmt,...)
{
    va_list ap;
    va_start(ap,fmt);

    int pos =loc-user_input;
    fprintf(stderr,"%s\n",user_input);
    fprintf(stderr,"%*s",pos," ");
    fprintf(stderr,"^");
    fprintf(stderr,fmt,ap);
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
        error_at(token->str,"'%c'ではありません\n",op);
    }
    token=token->next;
}

int expect_number()
{
    if(token->kind!=TOKEN_NUM)
    {
        error_at(token->str,"数ではありません\n");
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
    user_input=argv[1];
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