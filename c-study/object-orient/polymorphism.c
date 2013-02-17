// 定义Clonable接口
#include <stdio.h>
#include <stdlib.h>
typedef void *(* Clone)(void);
typedef struct _Clonable {
    Clone copy;
} Clonable;

// Person类扩展Clonable接口
typedef struct Person {
    Clonable base;
    unsigned int age;
} Person_t;

Clonable *ClonePerson()
{
    return (Clonable *) malloc(sizeof(Person_t));
};

// dog类扩展Clonable接口
typedef struct Dog {
    Clonable base;
    unsigned int bb;
} Dog_t;

Clonable *CloneDog()
{
    return (Clonable *) malloc(sizeof(Dog_t));
};

// 创建Clonable对象的Factory
Clonable *ClonableFactory(Clonable * cl)
{
    if (NULL == cl)
        return 0;
    return cl->copy();
}

// 用法
int main()
{
    Person_t p;
    p.base.copy = ClonePerson;
    Dog_t d;
    d.base.copy = CloneDog;

    Person_t *anotherPerson = 0;
    anotherPerson = (Person_t *) ClonableFactory(&p);

    Dog_t *anotherDog = 0;
    anotherDog = (Dog_t *) ClonableFactory(&d);
    return 0;
}
