/* Given a number X, find next number with same number of 1 bits in its binary representation.
 * How do I find the nth number that has same 1's in its binary representation?
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint32_t snoob(uint32_t x)
{

    uint32_t rightOne;
    uint32_t nextHigherOneBit;
    uint32_t rightOnesPattern;
    uint32_t next = 0;

    if (x) {
        // right most set bit
        rightOne = x & -(signed)x;

        // reset the pattern and set next higher bit
        // left part of x will be here
        nextHigherOneBit = x + rightOne;

        // nextHigherOneBit is now part [D] of the above explanation.
        // isolate the pattern
        rightOnesPattern = x ^ nextHigherOneBit;

        // right adjust pattern
        rightOnesPattern = (rightOnesPattern) / rightOne;

        // correction factor
        rightOnesPattern >>= 2;

        // rightOnesPattern is now part [A] of the above explanation.

        // integrate new pattern (Add [D] and [A])
        next = nextHigherOneBit | rightOnesPattern;
    }

    return next;
}

/*
    Rough algorithm :
    Binary Representation of 12 = (00001100)
    start scanning from right side.
    00001100 <--
    stop at the first 0 bit preceded by a set bit.
    000 0 1100
    swap this 0 bit with the preceded set bit
    000 0 1 100 ---swap---> 000 1 0 100
    starting from the indicated position 00010 ->100 , push all 1's to as right as possible.
    Ans -> 00010001

    Take for example : 00101110 which is 46
    Steps:
    00101110 <---start----
    001 0 1110
    001 0 1 110 --swap--> 001 1 0 110
    00110 push all one ->110
    Ans -> 00110011
*/
uint32_t next_greater(uint32_t n) 
{ 
    uint32_t a=(~n)+1; 
    uint32_t t=a&n; 
    printf("0x%02x, 0x%02x\n", a, t);
    uint32_t x=n+t; 

    uint32_t y=x^n; 
    y=y/t; 
    y=y>>2; 
    printf("0x%02x, 0x%02x\n", x,y);
    return x+y; 
} 

int main(void)
{
    printf("snosnoob: %d.\n", snoob(156));
    printf("snosnoob: %d.\n", next_greater(12));
    return 0;
}
