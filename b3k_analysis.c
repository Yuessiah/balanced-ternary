#include <string.h>
#include <stdio.h>

/* Pictures of zero, largest positive and largest negative numbers. */
static const char *pat = "┌───┐\n"
                         "│   │\n"
                         "└───┘\n"
                         "├─┴─┤\n"
                         "┤   ├\n"
                         "├─┬─┤\n"
                         "┌┬┬┬┐\n"
                         "├   ┤\n"
                         "└┴┴┴┘\n";

/* Precomputed number of digits in a picture and byte size of it. */
#define SZD 8
#define SZP 42

/* Trits description in order from low to high values. */
struct ds {
    int pos, len, exp, val;
} digit[SZD] = {
    { 32, 3 },
    { 26, 6 },
    { 16, 8 },
    { 0, 6 },
    { 6, 3 },
    { 9, 7 },
    { 21, 5 },
    { 35, 7 }
};

/* A number to convert and signed one to keep if it was negative. */
static int src = 0, inv = 1;

/* A buffer for the result. */
static char res[SZP+1];

/* Use it like 'echo -42 | ./b3k' */
int main (int q, char * argv[])
{
    int i;
    for(i = 0; pat[i];) {
        if(i%42 == 0) puts("---");
        printf("i = %2d, ", i%42);
        if(pat[i] == ' ' || pat[i] == '\n') {
            printf("{%d}\n", pat[i]);
            i++;
            continue;
        }
        printf("{%d, %d, %d}\n", pat[i], pat[i+1], pat[i+2]);
        i+=3;
    }

    scanf("%d", &src);

    /* Split the value into its modulus and signed multiplier. */
    if (src < 0)
        src *= inv = -1;

    int r;
    struct ds *p;

    for (r = 0; r < SZD; ++r) {
        p = &digit[r];
        p->exp = r ? (3 * digit[r-1].exp) : 1;
        p->val = 0;
    }

    /* Truncate to the largest allowed positive. */
    p = &digit[SZD - 1];
    r = 3 * p->exp / 2;
    if (src > r)
        src = r;
    else
        r = src;

    /* Like 'echo "obase=3; $src" | bc'. */
    do {
        while (r < p->exp)
            --p;
        r -= p->exp;
        ++p->val;
    } while (r);

    /* Convert to the balanced form. */
    while (p < &digit[SZD]) {
        if (r)
            ++p->val;
        r = p->val > 1;
        if (r)
            p->val -= 3;
        p->val *= inv;
        ++p;
    }

    /* Restore the sign. */
    src *= inv;

    /* Show the result. */
    for (p = &digit[0]; p < &digit[SZD]; ++p) {
        printf("exp = %d\n", p->exp);
        printf("pos = %d\n", p->pos);
        printf("len = %d\n", p->len);
        printf("val = %d\n", p->val);

        r = p->pos;
        if (p->val)
            r += SZP;
        if (p->val < 0)
            r += SZP;
        strncpy(res + p->pos,
                pat + r,
                p->len);

        for(i = 0; i < SZP; i++) res[i]? putchar(res[i]) : putchar('.');
        puts("\n---");
    }

    printf("%s", res);

    return 0;
}
