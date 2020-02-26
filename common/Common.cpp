//
// Created by Liu Yuchuan on 2020/1/7.
//
#include "Common.h"
#include <iostream>
#include <algorithm>


int readFileAsText(const char *filename, char *buff, int maxRead) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        eprintf("Cannot open file %s", filename);
        return -1;
    }

    int read = std::min(512, maxRead - 1);

    int readSize;
    int totalRead = 0;

    while (totalRead < maxRead &&
           (readSize = fread(buff + totalRead, sizeof(char), std::min(maxRead - totalRead, read), file)) > 0) {
        totalRead += readSize;
    }
    buff[totalRead] = 0;

    return totalRead;
}
