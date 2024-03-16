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
    int len;
};


//現在着目しているトークン
Token *token;
char *user_input;


Token *new_token(TokenKind kind,Token *cur,char *str,int len)
{
    Token *tok=calloc(1,sizeof(Token));
    tok->kind=kind;
    tok->next=NULL;
    tok->str=str;
    tok->len=len;
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
    Node *node=calloc(1,sizeof(Node));
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
        if(*p=='+'||*p=='-'||*p=='*'||*p=='/'||*p=='('||*p==')')
        {
            cur=new_token(TOKEN_RESERVED,cur,p++,1);
            continue;
        }
        if(isdigit(*p))
        {
            cur=new_token(TOKEN_NUM,cur,p,1);
            cur->val=strtol(p,&p,10);
            continue;
        }
        error("トークナイズできません:%s",p);
    }
    new_token(TOKEN_EOF,cur,p,1);
    return head.next;
}

bool at_eof()
{
    return token->kind==TOKEN_EOF;
}

bool consume(char *op)
{
    if(token->kind!=TOKEN_RESERVED||strlen(op)!=token->len||memcmp(token->str,op,token->len))
    {
        return false;
    }
    token=token->next;
    return true;
    
}

void expect(char *op)
{
    if(token->kind!=TOKEN_RESERVED||strlen(op)!=token->len||memcmp(token->str,op,token->len))
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


Node *expr();




Node *primary()
{
    if(consume("("))
    {
        Node *node=expr();
        expect(")");
        return node;
    }

    return new_node_num(expect_number());
}

Node *unary()
{
    if(consume("+"))
    {
        return primary();
    }
    if(consume("-"))
    {
        return new_node(ND_SUB,new_node_num(0),primary());
    }
    return primary();
}


Node *mul()
{
    Node *node=primary();
    for(;;)
    {
        if(consume("*"))
        {
            node = new_node(ND_MUL,node,primary());
        }
        else if(consume("/"))
        {
            node=new_node(ND_DIV,node,primary());
        }
        else
        {
            return node;
        }
    }
}


Node *expr()
{
    Node *node=mul();
    for(;;)
    {
        if(consume("+"))
        {
            node = new_node(ND_ADD,node,mul());
        }
        else if(consume("-"))
        {
            node=new_node(ND_SUB,node,mul());
        }
        else
        {
            return node;
        }
    }
}


void gen(Node *node)
{
    if(node->kind==ND_NUM)
    {
        printf("  push %d\n",node->val);
        return ;
    }

    gen(node->lhs);
    gen(node->rhs);


    printf("  pop rdi\n");
    printf("  pop rax\n");


    switch(node->kind)
    {
        case ND_ADD:
            printf("  add rax, rdi\n");
            break;
        case ND_SUB:
            printf("  sub rax, rdi\n");
            break;
        case ND_MUL:
            printf("  imul rax, rdi\n");
            break;
        case ND_DIV:
            printf("  cqo\n");
            printf("  idiv rdi\n");

            break;
    }
    printf("  push rax\n");

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
    Node *node=expr();


    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    gen(node);
    printf("  pop rax\n");
    printf("  ret\n");
    return 0;
       
}