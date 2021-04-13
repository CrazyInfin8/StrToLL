#include <errno.h>    // errno, ERANGE
#include <limits.h>   // ULLONG_MAX
#include <stdbool.h>  // bool, true, false

static long long c8_strToLL(char* str, char** v, unsigned int base) {
    long long num = 0;
    long long cutoff = LLONG_MAX;
    bool tooBig = false, neg = false, hasDigits = false;
#ifdef ALLOW_SMALLTALK_RADIX_LITERALS
    bool radixSet = base;
#endif
    char* start = str, prefix = NULL;
    char c = *str;
    if (c == '-') {
        neg = true;
        c = *++str;
    } else if (c == '+') {
        c = *++str;
    }
    if (c == '0' && base == 0) {
        c = *++str;
        switch (c) {
            case 'x':
                base = 16;
#ifdef ALLOW_SMALLTALK_RADIX_LITERALS
                radixSet = true;
#endif
                ++str;
                break;
            case 'o':
                base = 8;
#ifdef ALLOW_SMALLTALK_RADIX_LITERALS
                radixSet = true;
#endif
                ++str;
                break;
            case 'b':
                base = 2;
#ifdef ALLOW_SMALLTALK_RADIX_LITERALS
                radixSet = true;
#endif
                ++str;
                break;
            default:
#ifdef LEADING_ZERO_AS_OCTAL
                base = 8;
#else
                base = 10;
#endif
                start = str;
                break;
        }
    } else if (base == 0)
        base = 10;
    else {
        switch (base) {
            case 2:
                if (*(str + 1) == 'b') str += 2;
                break;
            case 8:
                if (*(str + 1) == 'o') str += 2;
                break;
            case 16:
                if (*(str + 1) == 'x') str += 2;
                break;
        }
    }
    long long cutlimit = cutoff % base;
    cutoff /= base;
    for (;;) {
        c = *str++;
#ifdef ALLOW_SMALLTALK_RADIX_LITERALS
        if (c == 'r' && !radixSet) {
            radixSet = true;
            if (num <= 36 && num >= 2) {
                base = num;
                num = 0;
                hasDigits = false;
                continue;
            } else {
                --str;
                break;
            }
        } else
#endif
            if (c >= '0' && c <= '9')
            c -= '0';
        else if (c >= 'a' && c <= 'z')
            c -= 'a' - 10;
        else if (c >= 'A' && c <= 'Z')
            c -= 'A' - 10;
        else if (c == '_')
            continue;
        else {
            --str;
            break;
        }
        if (c >= base) {
            --str;
            break;
        }
        hasDigits = true;
        if (num > cutoff || (num == cutoff && c > cutlimit)) tooBig = true;
        if (!tooBig) {
            num *= base;
            num += c;
        }
    }
    if (v) {
        if (hasDigits)
            *v = str;
        else
            *v = start;
    }
    if (tooBig) {
        errno = ERANGE;
        return LLONG_MAX;
    }
    if (neg) num *= -1;
    return num;
}