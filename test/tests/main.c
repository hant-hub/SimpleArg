#define CU_IMPL
#include <cutils.h>


#define ARGS \
   X(a, int) \
   X(b, float) \
   X(c, string)

#define OPTS \
    X(out, int) \
    X(t, bool)

#include <sa.h>


int main(int argc, char* argv[]) {
    for (u32 i = 0; i < argc; i++) {
        debuglog("%n", argv[i]);
    }

    char* testargs[] = {
        "1",
        "2.5",
        "test",
        "rand",
        "-o=10",
        "-out=10",
        "-otest=bam",
    };

    SA vals = Parse(ARRAY_SIZE(testargs), testargs);

    debuglog("err: %d", vals.err);
    SAvals results = vals.vals;

    debuglog("a: %d", results.a);
    debuglog("b: %f", results.b);
    debuglog("c: %n", results.c);

    debuglog("out: %d %d", results.out.set, results.out.val);
    debuglog("t: %d %d", results.t.set, results.t.val);
    return 0;
}
