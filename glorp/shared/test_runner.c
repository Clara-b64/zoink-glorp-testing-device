
#include "runner.h"
#include <stdio.h>

int main(void){
    /* Line-buffer stdout even when piped: if the exercise crashes or the
    ** sanitizer aborts at exit, the per-test results already printed must
    ** survive (block buffering would lose them on _exit). */
    setvbuf(stdout, NULL, _IOLBF, 0);

    UNITY_BEGIN();

    glorpRun();

    return UNITY_END();

}
