#include "9cc.h"

void gen(Node *node)
{
    if(node->kind==ND_NUM)
    {
        printf("  push %d\n",node->val);
        return ;
    }

    gen(node->lhs);
    gen(node->rhs);


    //printf("  pop rdi\n");
    //printf("  pop rax\n");


    switch(node->kind)
    {
        case ND_ADD:
            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  add rax, rdi\n");
            break;
        case ND_SUB:
            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  sub rax, rdi\n");
            break;
        case ND_MUL:
            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  imul rax, rdi\n");
            break;
        case ND_DIV:
            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  cqo\n");
            printf("  idiv rdi\n");
            break;
        case ND_EQ:
            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  cmp rax, rdi\n");
            printf("  sete al\n");
            printf("  movzb rax ,al\n");
            break;
        case ND_NOTEQ:
            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  cmp rax, rdi\n");
            printf("  setne al\n");
            printf("  movzb rax ,al\n");
            break;
        case ND_LESS:
            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  cmp rax, rdi\n");
            printf("  setl al\n");
            printf("  movzb rax ,al\n");
            break;
        case ND_LESS_EQ:
            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  cmp rax, rdi\n");
            printf("  setle al\n");
            printf("  movzb rax ,al\n");
            break;
        case ND_MORE:
            printf("  pop rax\n");
            printf("  pop rdi\n");
            printf("  cmp rax, rdi\n");
            printf("  setl al\n");
            printf("  movzb rax ,al\n");
            break;
        case ND_MORE_EQ:
            printf("  pop rax\n");
            printf("  pop rdi\n");
            printf("  cmp rax, rdi\n");
            printf("  setle al\n");
            printf("  movzb rax ,al\n");
            break;
        
    }
    printf("  push rax\n");

}
