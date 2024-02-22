#pragma once
#include <cstddef>

constexpr int getXArea(){
    return 1000;
}

constexpr int getYArea(){
    return 1000;
}

constexpr size_t numberOfSimulations(){
    return 10;
}

constexpr size_t numberOfStartingFirepoints(){
    return 10;
}

/*
Some statistics
1 firepoint
    1000х1000 на 1000 итераций - ~26 секунд c ветром, размер: 24-56 Б на клетку
    10000x10000 на 10 итераций - 24.6~ секунд с ветром, размер: 24-56 Б на клетку
10 firepoint
    1000х1000 на 100 итераций - ~2.75 секунд c ветром, размер: 24-56 Б на клетку
    10000x10000 на 10 итераций - 25.8~ секунд с ветром, размер: 24-56 Б на клетку
100 firepoint
    1000х1000 на 100 итераций - ~3.98 секунд c ветром, размер: 24-56 Б на клетку
    10000x10000 на 100 итераций - 256.9~ секунд с ветром, размер: 24-56 Б на клетку
*/

constexpr int ignitionPercentage(){
    return 150; // in formula there is smth wrong I suppose
}

constexpr int cellSizeInMeters(){
    return 20;
}

constexpr int minutesPerIteration(){
    return 10;
}