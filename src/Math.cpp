#include "../include/Math.h"
#include "../include/Properties.h"
#include <cmath>
#include "Math.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <bits/stdc++.h> 

Math::Math(/* args */)
{
}

Math::~Math()
{
}

double Math1::calculateKoef(float windSpeed, double slopeAngleRad) const
{
    double windKoeff = pow(196, 0.0133 * windSpeed);
    double slopeKoeff = 5.275 * tan(slopeAngleRad);
    return windKoeff * slopeKoeff;
}

double Math1::calculateWindKoef(const cell *c, directions InvestigatedDirection) const
{
    clock_t start = clock();
    auto wind = c->getWind().get();
    if (wind == nullptr)
    {
        return double(1);
    }
    WindMetaData meta = {abs(static_cast<int>(wind->getWindDirection()) - static_cast<int>(InvestigatedDirection)), wind->getWindSpeed()};
    auto a = wind_result_[meta];
    if (a != 0){
        clock_t per_iteration = clock() - start;
        wind_timer_ = per_iteration+wind_timer_;
        wind_counter_++;
        return wind_result_[meta];
    };
    float angleRadians = wind->angleBetweenDirections(InvestigatedDirection);
    double result = std::exp(wind->getWindSpeed() * (cos(angleRadians) - 1));
    result_wind_[result]++;

    wind_result_[meta] = result;
    clock_t per_iteration = clock() - start;
    wind_timer_ = per_iteration+wind_timer_;
    wind_counter_++;

    return result;
}

double Math1::calculateGroundSlopeKoef(directions InvestigatedDirection, int altitudeDifference) const
{
    clock_t start = clock();

    SlopeMetaData meta = {bool(static_cast<int>(InvestigatedDirection) / int(2)), altitudeDifference};
    
    // auto a = slope_result_[meta];
    // if (a != 0)
    // {
    //     clock_t per_iteration = clock() - start;
    //     slope_timer = per_iteration+slope_timer;
    //     slope_counter++;
    //     return a;
    // }

    double coef = 0;
    if (InvestigatedDirection == directions::North || InvestigatedDirection == directions::West ||
        InvestigatedDirection == directions::East || InvestigatedDirection == directions::South)
    {
        coef = atan2(static_cast<double>(altitudeDifference), static_cast<double>(cellSizeInMeters()));
    }
    else
    {
        coef = atan2(static_cast<double>(altitudeDifference), static_cast<double>(cellSizeInMeters() * sqrt(2.0)));
    }
    double result = std::exp(0.5 * coef);
    // SlopeMetaData meta = {bool(static_cast<int>(InvestigatedDirection) / int(2)), altitudeDifference};
    slope_counter_[meta]++;
    // slope_result_[meta] = result;

    clock_t per_iteration = clock() - start;
    slope_timer = per_iteration+slope_timer;
    slope_counter++;
    return result;
}

bool Math1::willSpread(const cell *c, directions InvestigatedDirection, int altitudeDifference) const
{
    double fireKoeff = calculateWindKoef(c, InvestigatedDirection);
    double slopeKoeff = calculateGroundSlopeKoef(InvestigatedDirection, altitudeDifference);
    int result = fireKoeff * slopeKoeff * 100 - 40;
    result_overall[result]++;
    return result + (rand() % 100) > ignitionPercentage();
}

bool Math1::willSpreadThroughOne(const cell *c, directions InvestigatedDirection, int altitudeDifference) const
{
    double fireKoeff = calculateWindKoef(c, InvestigatedDirection);
    double slopeKoeff = calculateGroundSlopeKoef(InvestigatedDirection, altitudeDifference);
    return int(fireKoeff * slopeKoeff * 100) + (rand() % 30) > throughPercentage();
}

bool to_bool(std::string const &s)
{
    return s != "0";
}

Math1::Math1()
{
    std::ifstream csvFile{"data/slope.csv"};
    std::string row;
    while (std::getline(csvFile, row))
    {
        std::stringstream rowStream(row);
        std::string col;
        std::vector<std::string> cols;
        while (std::getline(rowStream, col, ','))
        {
            cols.push_back(col);
        }
        auto a = atoi(cols[1].c_str());
        slope_result_[SlopeMetaData{to_bool(cols[0].c_str()), a}] = atof(cols[2].c_str());
    }
    csvFile.close();
    std::ifstream csvFile2{"data/wind.csv"};
    while (std::getline(csvFile2, row))
    {
        std::stringstream rowStream(row);
        std::string col;
        std::vector<std::string> cols;
        while (std::getline(rowStream, col, ','))
        {
            cols.push_back(col);
        }
        float a = atof(cols[1].c_str());

        wind_result_[WindMetaData{atoi(cols[0].c_str()), a}] = atof(cols[2].c_str());
    }
    csvFile2.close();
    slope_counter = 0;
    slope_timer = 0;
    wind_counter_ = 0;
    wind_timer_ = 0;
}

Math1::~Math1()
{
    printf("total time %f, overall counts %d\n", (double)(slope_timer), slope_counter);
    printf("total time %f, overall counts %d\n", (double)(wind_timer_), wind_counter_);
    // printf("slope profiling\n");
    // std::map<int, int> slope_distribution;

    // for (auto &&i : slope_result_)
    // {
    //     printf("%d,%d,%f\n", i.first.altitude_difference_, i.first.diagonal_direction_, i.second);
    // }

    // for (auto &&i : slope_counter_)
    //  {
    //      printf("%d,%d;%d\n", i.first.altitude_difference_, i.first.diagonal_direction_, i.second);
    //      slope_distribution[i.second]++;
    //  }
    //  printf("slope distribution\n");
    //  for (auto &&i : slope_distribution)
    //  {
    //      printf("%d;%d\n", i.first, i.second);
    //  }
    //  printf("wind profiling\n");
    //  for (auto &&i : result_wind_)
    //  {
    //      printf("%2.2f;%d\n", i.first, i.second);
    //  }
    //  printf("overall profiling\n");
    //  for (auto &&i : result_overall)
    //  {
    //      printf("%d;%d\n", i.first, i.second);
    //  }
}