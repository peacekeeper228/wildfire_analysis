#pragma once
#include <cstddef>
#include <cstdint>
#include <cmath>
#include <string>



constexpr int getXArea(){
    return 110;
}

constexpr int getYArea(){
    return 110;
}

constexpr size_t numberOfSimulations(){
    return 100;
}

constexpr size_t numberOfStartingFirepoints(){
    return 1;
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
    10000x10000 на 100 итераций (новый комп) - 90 секунд с ветром, размер 32-64 на клетку
with safe pointers
10000 firepoint
    1000х1000 на 100 итераций (новый комп) - ~4.9 секунд c ветром
    10000x10000 на 100 итераций (новый комп) - 440~ секунд с ветром
    10000x10000 на 100 итераций (новый комп) - 320~ секунд с ветром на 2 потока
    10000x10000 на 100 итераций (новый комп) - 270~ секунд с ветром на 4 потока
1000000 firepoint
    1000х1000 на 100 итераций (новый комп) - секунд c ветром
    10000x10000 на 100 итераций (новый комп) -  секунд с ветром
    10000x10000 на 100 итераций (новый комп) -  секунд с ветром на 2 потока
    10000x10000 на 100 итераций (новый комп) - 520~ секунд с ветром на 4 потока
*/

constexpr int cellSizeInMeters(){
    return 20;
}

constexpr int minutesPerIteration(){
    return 10;
}

constexpr std::string analyzedPolygon(){
    return "ST_GeomFromText('POLYGON((36.7 55.3, 36.75 55.3, 36.75 55.35, 36.7 55.35, 36.7 55.3))', 4326)";
}

constexpr double pi() { return std::atan(1) * 4; }