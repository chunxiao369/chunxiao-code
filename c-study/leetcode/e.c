#include <stdio.h>
#include <stdint.h>

int main(int argc, char**argv)
{
        uint64_t f = 600851475143llu;
        uint64_t t;
        uint64_t s;


        t = 2;
        while(t<f){
                s = f%t;
                if(!s){
                        printf("%d ", t); 
                        f = f/t;
                        t = 2;
                        continue;
                }   
                ++t;
        }   

        printf("%d\n", f); 
        return 0;
}
