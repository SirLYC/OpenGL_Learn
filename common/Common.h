//
// Created by Liu Yuchuan on 2020/1/7.
//

#ifndef OPENGL_LEARN_COMMON_H
#define OPENGL_LEARN_COMMON_H

#include <string>
#include <iostream>
#include <chrono>

#define eprintf(fmt, ...) fprintf(stderr, fmt, #__VA_ARGS__);

int readFileAsText(const char *filename, char *buff, int maxRead);

template<typename Func>
inline long long int measureTime(Func func);

template<typename Func>
inline long long int measureTimeNano(Func func);

template<typename Func>
long long int measureTime(Func func) {
    auto start = std::chrono::system_clock::now();
    func();
    auto end = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

template<typename Func>
inline long long int measureTimeNano(Func func) {
    auto start = std::chrono::system_clock::now();
    func();
    auto end = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}


#endif //OPENGL_LEARN_COMMON_H
