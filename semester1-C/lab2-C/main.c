#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int base = 1000000000;

typedef struct {
    int value[35];
} uint1024_t;

uint1024_t from_uint(unsigned int x) {
    uint1024_t a;
    for (int i = 34; i >= 0; i--) {
        a.value[i] = -1 * base;
    }
    int digit = 34;
    if (!x) {
        a.value[34] = 0;
    }
    while (x) {
        a.value[digit--] = x % base;
        x /= base;
    }
    return a;
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {
    uint1024_t sum;
    int carry = 0;
    for (int i = 34; i >= 0; i--) {
        if (x.value[i] == -1 * base) {
            sum.value[i] = y.value[i];
        } else {
            if (y.value[i] == -1 * base) {
                sum.value[i] = x.value[i];
            } else {
                sum.value[i] = (x.value[i] + y.value[i]) % base + carry;
                carry = (x.value[i] + y.value[i]) / base;
            }
        }
    }
    if (carry) {
        printf("ERROR Undefined behavior: Overflow\n");
    }
    return sum;
}

int if_bigger(uint1024_t x, uint1024_t y) {
    int flag = 0;
    for (int i = 0; i < 35; i++) {
        if (x.value[i] > y.value[i]) {
            flag = 1;
        } else if (x.value[i] < y.value[i]) {
            if (!flag) {
                return 0;
            }
        }
    }
    return 1;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
    uint1024_t diff = x;
    if (if_bigger(x, y)) {
        for (int i = 34; i >= 0; i--) {
            if (y.value[i] != -1 * base) {
                diff.value[i] -= y.value[i];
            }
            //printf("i: %d   X: %d    Y: %d  Value: %d\n", i, x.value[i], y.value[i], diff.value[i]);
        }
        for (int i = 34; i > 0; i--) {
            if (diff.value[i] < 0 && diff.value[i] != -1 * base) {
                diff.value[i] += base;
                diff.value[i - 1]--;
                //printf("i: %d      Value: %d\n", i, diff.value[i]);
            }
        }
    } else {
        printf("ERROR: Minuend is smaller than subtrahend\n");
    }
    return diff;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
    uint1024_t product;
    for (int i = 0; i < 35; i++) {
        product.value[i] = -1 * base;
    }
    for (int i = 34; i >= 0; i--) {
        if (y.value[i] == -1 * base) {
            break;
        }
        for (int j = 34; j >= 0; j--) {
            if (x.value[j] == -1 * base) {
                break;
            }
            long long tmp = 0;
            if (!(y.value[i] == -1 * base && x.value[j] == -1 * base)) {
                tmp = (y.value[i] * 1ll) * (x.value[j] * 1ll);
                //printf("i: %d     j: %d    TMP: %lld\n", i, j, tmp);
            }
            if (tmp && i + j - 34 < 0) {
                printf("%lld   ERROR Undefined behavior: Overflow\n", tmp);
                return product;
            } else {
                if (product.value[i + j - 34] == -1 * base) {
                    product.value[i + j - 34] = tmp % base;
                } else {
                    product.value[i + j - 34] += tmp % base;
                }
                //printf("Value [(%d) %d + %d - 34]:  %d\n\n", (i + j - 34), i, j, product.value[i + j - 34]);
                if (product.value[i + j - 35] == -1 * base) {
                    product.value[i + j - 35] = tmp / base + product.value[i + j - 34] / base;
                } else {
                    product.value[i + j - 35] += tmp / base + product.value[i + j - 34] / base;
                }
                product.value[i + j - 34] -= product.value[i + j - 34] / base * base;
            }
        }
    }
    return product;
}

void printf_value(uint1024_t x) {
    int flag = 0;
    for (int i = 0; i < 35; i++) {
        if (x.value[i] == -1 * base) {
            continue;
        } else if (x.value[i] == 0) {
            if (i > 0) {
                if (flag) {
                    printf("000000000");
                } else {
                    if (i == 34) {
                        printf("0");
                    } else {
                        continue;
                    }
                }
            } else {
                continue;
            }
        } else {
            printf("%d", x.value[i]);
            flag = 1;
        }
    }
}

void put_digit(uint1024_t *x, char val[], int numbers, int index, int valInd) {
    char digit[numbers];
    for (int i = 0; i < numbers; i++) {
        digit[i] = val[valInd++];
    }
    x->value[index] = atoi(digit);
    //printf("Digit: %d\n", atoi(digit));
}

void scanf_value(uint1024_t *x, char val[]) {
    int length = strlen(val);
    //printf("Length: %d\n", length);
    int needed = length / 9 + (length % 9 ? 1 : 0);
    //printf("Needed: %d\n", needed);
    for (int i = 0; i < 35 - needed; i++) {
        x->value[i] = -1 * base;
    }
    int numbers = length % 9 + (!(length % 9) ? 9 : 0);
    //printf("Numbers: %d\n", numbers);
    put_digit(x, val, numbers, 35 - needed, 0);
    int counter = 0;
    for (int i = 36 - needed; i < 35; i++) {
        put_digit(x, val, 9, i, numbers + (counter * 9));
        counter++;
    }
}

int main(int argc, char *argv[]) {
    uint1024_t a, b, c;
    unsigned int ui = 123456789;
    scanf_value(&a, argv[1]);
    scanf_value(&b, argv[2]);

    printf("First number is: ");
    printf_value(a);
    printf(".\n");

    printf("Second number is: ");
    printf_value(b);
    printf(".\n");

    c = add_op(a, b);
    printf("The sum (first + second) equals: ");
    printf_value(c);
    printf(".\n");

    if (if_bigger(a, b)) {
        c = subtr_op(a, b);
        printf("The subtract (first - second) equals: ");
    } else {
        c = subtr_op(b, a);
        printf("The subtract (second - first) equals: ");
    }
    printf_value(c);
    printf(".\n");

    c = mult_op(a, b);
    printf("The product (first * second) equals: ");
    printf_value(c);
    printf(".\n");

    printf("We can get uint1024_t from unsigned int: %d. Here it is: ", ui);
    printf_value(from_uint(ui));
    printf(".\n");
}
