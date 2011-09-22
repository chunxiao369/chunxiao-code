#include "stdio.h" 
#include "string.h" 
//#define NULL 0 

typedef struct BiTNode{ 
char data; 
struct BiTNode *lchild,*rchild; 
}BiTNode,*BiTree; 

BiTree Create(BiTree T)
{ 
    char ch; 
    ch=getchar(); 
    if(ch=='#') 
        T=NULL; 
    else { 
        if(!(T=(BiTNode *)malloc(sizeof(BiTNode)))) 
            printf("Error!"); 
        T->data=ch; 
        T->lchild=Create(T->lchild); 
        T->rchild=Create(T->rchild); 
    } 
    return T; 
}

void Preorder(BiTree T)
{ 
    if(T) { 
        printf("%c",T->data); 
        Preorder(T->lchild); 
        Preorder(T->rchild); 
    } 
}

int Sumleaf(BiTree T)
{ 
    int sum=0,m,n; 
    if(T) { 
        if((!T->lchild)&&(!T->rchild)) 
            sum++; 
        m=Sumleaf(T->lchild); 
        sum+=m; 
        n=Sumleaf(T->rchild); 
        sum+=n; 
    } 
    return sum; 
}

void zhongxu(BiTree T)
{ 
    if(T){ 
        zhongxu(T->lchild); 
        printf("%c",T->data); 
        zhongxu(T->rchild); 
    } 
}

void houxu(BiTree T)
{ 
    if (T) { 
        houxu(T->lchild); 
        houxu(T->rchild); 
        printf("%c",T->data); 
    } 
}

int Depth(BiTree T)
{ 
    int dep=0,depl,depr; 
    if(!T)
        dep=0; 
    else { 
        depl = Depth(T->lchild); 
        depr = Depth(T->rchild); 
        dep  = 1 + (depl > depr ? depl : depr); 
    } 
    return dep; 
}

int main()
{ 
    BiTree T = NULL; 
    int sum,dep;
    T = Create(T); 
    Preorder(T); 
    printf("\n"); 
    zhongxu(T); 
    printf("\n"); 
    houxu(T); 
    printf("\n"); 
    sum=Sumleaf(T); 
    printf("%d",sum); 
    dep=Depth(T); 
    printf("\n%d",dep); 
    return 0;
} 
