/*输出float变量的指数(exponent)和尾数(mantissa)*/
#include <math.h>
#include <stdio.h>

void print_float_components(const char* name, float value) {
    union {
        float f;
        unsigned int i;
    } u;
    u.f = value;

    unsigned int sign = (u.i >> 31) & 0x1;
    unsigned int exponent = (u.i >> 23) & 0xFF;
    unsigned int mantissa = u.i & 0x7FFFFF;

    printf("%s vlaue: %f: sign = %d, Exponent = %u (biased), Mantissa = 0x%06X\n", name, value, sign, exponent, mantissa);
    printf("%s vlaue: %f: sign = %d, Exponent = %u (biased), Mantissa = %u\n", name, value, sign, exponent, mantissa);
    printf("value by calc: %f\n", powf(-1, sign) * (1 + (float)mantissa / (1 << 23)) * powf(2, (int)exponent - 127));
}

int main(void) 
{
    float x = 1;
    float y = 3.2;
    float z = -0.5;
    float m = 0.15;
    float n = -0.25;

    printf("float size: %ld.\n", sizeof(float));

    print_float_components("x", x);
    print_float_components("y", y);
    print_float_components("z", z);
    print_float_components("m", m);
    print_float_components("n", n);

    // 更详细的解释
    printf("\n=== 详细解释 ===\n");
    printf("IEEE 754单精度浮点数格式：1位符号 + 8位指数 + 23位尾数\n");
    printf("指数偏移量：127\n");
    printf("实际值 = (-1)^符号位 × 1.(尾数/2^23) × 2^(指数-127)\n");
    
    return 0;
}
