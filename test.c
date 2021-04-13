#include <stdio.h>   // printf
#include <stdlib.h>  // strtoll
#include <string.h>  // strcmp

/**
 * Whether to read literals that specify a radix
 *
 * see: https://en.wikipedia.org/wiki/Smalltalk#Literals for more info
 * 
 * Examples:
 * 2r01010 // same as binary (radix 2)
 * 16rABCDEF // same as hexadecimal (radix 16)
 * 36rA_NUMBER // radix (36)
 */
// #define ALLOW_SMALLTALK_RADIX_LITERALS

/**
 * Whether to treat numbers with leading zeros as octals
 * 
 * Examples:
 * 0123 // 83 vs 123
 **/
#define LEADING_ZERO_AS_OCTAL

/**
 * Whether to invalidate numbers less than base 10 if digits are higher than base 10
 * 
 * Examples:
 * 0b012 // (value: 0, remaining: "b012") vs (value: 1, remaining: "2")
 **/
#define INVALIDATE_SUB_BASE10 // TODO: unimplemented yet

#include "strtoll.c"  // c8_strToLL
#define testCount 35
struct {
    char *str;
    int base;
} values[testCount] = {
    /* 0*/ {"ABCDEFjunk", 16},
    /* 1*/ {"0xDEADbeef", 0},
    /* 2*/ {"01234567", 0},
    /* 3*/ {"0xFFFFFFFFF", 0},
    /* 4*/ {"9223372036854775807", 0},
    /* 5*/ {"9223372036854775808", 0},
    /* 6*/ {"-ABCDEFjunk", 16},
    /* 7*/ {"-0xDEADbeef", 0},
    /* 8*/ {"-01234567", 0},
    /* 9*/ {"-", 0},
    /*10*/ {"0x", 0},
    /*11*/ {"-0x", 0},
    /*12*/ {"-x", 0},
    /*13*/ {"0b012", 0},
    /*14*/ {"0o012345678", 0},
    /*15*/ {"012345678", 0},
    /*16*/ {"0xEFG", 0},
    /*17*/ {"123456789abcdef", 0},
    /*18*/ {"0xABCDEFG", 0},
    /*19*/ {"12r123456789abcdef", 0},
    /*20*/ {"-12r123456789abcdef", 0},
    /*21*/ {"36rA_NUMBER", 0},
    /*22*/ {"012345678", 8},
    /*23*/ {"-0b012", 0},
    /*24*/ {"00x12345", 16},
    /*25*/ {"00b12345", 16},
    /*26*/ {"00o12345", 16},
    /*27*/ {"0X12345", 0},
    /*28*/ {"0B10101", 0},
    /*29*/ {"0O12345", 0},
    /*30*/ {"-0X12345", 0},
    /*31*/ {"-0B10101", 0},
    /*32*/ {"-0O12345", 0},
    /*33*/ {"37rABC", 0},
    /*34*/ {"0junk", 0},
};

void consistencyCheck() {
    bool success = true;
    for (int i = 0; i < testCount; ++i) {
        char *str1, *str2;
        int err1, err2;
        errno = 0;
        long long d1 = strtoll(values[i].str, &str1, values[i].base);
        err1 = errno;
        errno = 0;
        long long d2 = c8_strToLL(values[i].str, &str2, values[i].base);
        err2 = errno;
        if (d1 != d2 || strcmp(str1, str2) || err1 != err2) {
            printf(
                "%2d: %lld (remaining: \"%s\", errcode: %d)\t%lld (remaining: "
                "\"%s\", errcode: %d)\n",
                i, d1, str1, err1, d2, str2, err2);

            success = false;
        }
    }
    if (success) {
        printf("All tests match!\n");
    } else {
        printf("values printed above don't match\n");
    }
}

int main(int argc, char const *argv[]) {
    consistencyCheck();
    // printf("%lld\n", c8_strToLL(values[19].str, NULL, 16));
    return 0;
}
