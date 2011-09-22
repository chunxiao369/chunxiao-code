/*
 * 1).演示如何使用参数个数可变的函数，采用ANSI标准形式 
 * va_start使argp指向第一个可选参数
 * va_arg返回参数列表中的当前参数并使argp指向参数列表中的下一个参数
 * va_end把argp指针清为NULL
 * 函数体内可以多次遍历这些参数，但是都必须以va_start开始，并以va_end结尾
 */
#if 0
#include <stdio.h> 
#include <string.h>
#include <stdarg.h> 
/*函数原型声明，至少需要一个确定的参数，注意括号内的省略号*/ 
int demo( char*, ... );
int main( void )
{
   demo("DEMO", "This", "is", "a", "demo!", ""); 
   return 0;
}
/*ANSI标准形式的声明方式，括号内的省略号表示可选参数*/ 
int demo( char *msg, ... ) 
{ 
   /*定义保存函数参数的结构*/
   va_list argp; 
   int argno = 0; 
   char *para;

/*argp指向传入的第一个可选参数，msg是最后一个确定的参数*/ 
   va_start( argp, msg ); 
   while (1) 
   { 
     para = va_arg( argp, char *); 
     if ( strcmp( para, "") == 0 ) 
       break; 
     printf("Parameter #%d is: %s\n", argno, para); 
     argno++; 
   } 
   va_end( argp ); 
   /*将argp置为NULL*/
   return 0; 
}

#endif 

#if 0
/*
 * 2)//示例代码1：可变参数函数的使用
 */
#include "stdio.h"
#include "stdarg.h"
void simple_va_fun(int start, ...) 
{ 
    va_list arg_ptr; 
    int nArgValue =start;
    int nArgCout=0;     //可变参数的数目
    va_start(arg_ptr,start); //以固定参数的地址为起点确定变参的内存起始地址。
    do
    {
        ++nArgCout;
        printf("the %d th arg: %d\n",nArgCout,nArgValue);     //输出各参数的值
        nArgValue = va_arg(arg_ptr,int);                      //得到下一个可变参数的值
    } while(nArgValue != -1);                
    return; 
}

int main(int argc, char* argv[])
{
    simple_va_fun(100,-1); 
    simple_va_fun(100,200,-1); 
    return 0;
}
#endif 

#if 1
/*
 * 3)示例代码2:扩展——自己实现简单的可变参数的函数
 * 下面是一个简单的printf函数的实现，参考了<The C Programming Language>中的例子
 */
#include "stdio.h"
#include "stdlib.h"
void myprintf(char* fmt, ...)        //一个简单的类似于printf的实现，//参数必须都是int 类型
{ 
    char* pArg=NULL;               //等价于原来的va_list 
    char c;
    
    pArg = (char*) &fmt;          //注意不要写成p = fmt !!因为这里要对//参数取址，而不是取值
    pArg += sizeof(fmt);         //等价于原来的va_start          

    do
    {
        c =*fmt;
        if (c != '%')
        {
            putchar(c);            //照原样输出字符
        }
        else
        {
           //按格式字符输出数据
           switch(*++fmt) 
           {
            case'd':
                printf("%d",*((int*)pArg));           
                break;
            case'x':
                printf("%#x",*((int*)pArg));
                break;
            default:
                break;
            } 
            pArg += sizeof(int);               //等价于原来的va_arg
        }
        ++fmt;
    }while (*fmt != '\0'); 
    pArg = NULL;                               //等价于va_end
    return; 
}

int main(int argc, char* argv[])
{
    int i = 1234;
    int j = 5678;
    
    myprintf("the first test:i=%d\n",i,j); 
    myprintf("the secend test:i=%d; %x;j=%d;\n",i,0xabcd,j); 
    return 0;
}
#endif

#if 0
int max(int n, ...) {                // 定参 n 表示后面变参数量，定界用，输入时切勿搞错
　va_list ap;                         // 定义一个 va_list 指针来访问参数表
     va_start(ap, n);                       // 初始化 ap，让它指向第一个变参，n之后的参数
    int maximum = -0x7FFFFFFF;          // 这是一个最小的整数
    int temp;
     for(int i = 0; i < n; i++) {
    temp = va_arg(ap, int);          // 获取一个 int 型参数，并且 ap 指向下一个参数
    if(maximum < temp) maximum = temp;
     }
    va_end(ap);                         // 善后工作，关闭 ap
    return max;
}
// 在主函数中测试 max 函数的行为(C++ 格式)
int main() {
   cout << max(3, 10, 20, 30) << endl;
   cout << max(6, 20, 40, 10, 50, 30, 40) << endl;
}

/*
 * 基本用法阐述至此，可以看到，这个方法存在两处极严重的漏洞：
 * 其一，输入参数的类型随意性，
 * 使得参数很容易以一个不正确的类型获取一个值
 * (譬如输入一个float，却以int型去获取他)，
 * 这样做会出现莫名其妙的运行结果；
 * 其二，变参表的大小并不能在运行时获取，
 * 这样就存在一个访问越界的可能性，导致后果严重的 RUNTIME ERROR.
 */

#endif 
