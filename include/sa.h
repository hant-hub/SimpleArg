#ifndef SA_H
#define SA_H

#include "cutils.h"
#include <stdlib.h>
#ifndef ARGS
#define ARGS
#endif

#ifndef OPTS
#define OPTS
#endif

typedef char* string;

typedef struct SAvals {
#define X(name, type) type name;
    ARGS
#undef X
#define X(name, type) struct { char set; type val; } name;
    OPTS
#undef X
} SAvals;

typedef struct SA {
    int err;
    SAvals vals;
} SA;

static int optcmp(char* a1, char* a2, int l1, int l2) {
    for (int i = 0; i < l1; i++) {
        if (a1[0] != a2[0]) return 0;
        a1++;
        a2++;
    }
    return l1 == l2;
}

static int parseint(SA* s, char* arg) {
    int val = 0;
    while (*arg) {
        char c = *arg;

        val *= 10;
        if (c < '0' || c > '9') {
            s->err = 1;
            return 0;
        }
        c -= '0';
        val += c;

        arg++;
    }

    return val;
}

static float parsefloat(SA* s, char* arg) {
    int val = 0;
    while (*arg && *arg != '.') {
        char c = *arg;

        val *= 10;
        if (c < '0' || c > '9') { 
            s->err = 1;
            return 0;
        }
        c -= '0';
        val += c;

        arg++;
    }

    if (!(*arg)) return (float)val;
    arg++;

    float frac = 0.0;
    float mult = 1.0;
    while (*arg) {
        char c = *arg;

        mult *= 0.1;
        if (c < '0' || c > '9') 
            exit(EXIT_FAILURE); 
        c -= '0';
        frac += c * mult;

        arg++;
    }

    return val + frac;
}

static string parsestring(SA* s, char* arg) {
    return arg;
}

//parser
static SA Parse(int argc, char** argv) {
    SA args = {};

    int i = 0;
    //force all args first
    //Check if we have args, if not
    //fail
    #define X(name, type)  \
        if (i >= argc) { \
            args.err = 1; \
            return args; \
        } \
        args.vals.name = parse##type (&args, argv[i++]);

    ARGS
    #undef X

    //all options must happen after args

    #define X(name, type) \
        if (optcmp(arg, #name, len, sizeof(#name) - 1)) { \
            args.vals.name.set = 1; \
            args.vals.name.val = parse##type (&args, val); \
        }
          
    for (; i < argc; i++) {
        char* arg = argv[i];
        if (*arg != '-') continue; 
        arg++;
        if (*arg == '-') return args; //TODO(ELI): Decide how to handle
        
        char* val = arg;
        while (*val && *val != '=') val++;
        if (!val) val = argv[++i];
        else val++;
        
        int len = val - arg - 1;

        OPTS
    }
    #undef X


    return args;
}

#undef ARGS
#endif
