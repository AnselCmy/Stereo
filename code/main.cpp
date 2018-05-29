#include "StereoRectification.h"
#include "SGBM.h"

int main(int argc, char **argv) {
    if(!strcmp(argv[1], "-rect"))
        StereoRectification();
    if(!strcmp(argv[1], "-sgbm"))
        SGBM();

    return 0;
}