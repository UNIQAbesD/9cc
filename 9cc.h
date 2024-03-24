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
extern Token *token;
extern char *user_input;
Token *new_token(TokenKind kind,Token *cur,char *str,int len);

typedef enum
{
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
    ND_EQ,
    ND_NOTEQ,
    ND_LESS,
    ND_LESS_EQ,
    ND_MORE,
    ND_MORE_EQ,
}NodeKind;

typedef struct Node Node;
struct Node
{
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
};
Node *new_node(NodeKind kind,Node *lhs,Node *rhs);
Node *new_node_num(int val);


void error(char *fmt, ...);
void error_at(char *loc,char *fmt,...);


Token *tokenize(char *p);
bool at_eof();
bool consume(char *op);
void expect(char *op);
int expect_number();

Node *expr();
Node* equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

void gen(Node *node);



