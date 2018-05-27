#include "StereoRectification.h"

int main(int argc, char **argv) {
    if(!strcmp(argv[1], "-rect"))
        StereoRectification();
    return 0;
}