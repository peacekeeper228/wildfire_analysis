#include "../include/Math.h"
#include "../include/Properties.h"
#include <cmath>
#include "Math.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <bits/stdc++.h> 

IMath::IMath(/* args */)
{
}

IMath::~IMath()
{
}

double Math::calculateKoef(float windSpeed, double slopeAngleRad) const
{
    double windKoeff = pow(196, 0.0133 * windSpeed);
    double slopeKoeff = 5.275 * tan(slopeAngleRad);
    return windKoeff * slopeKoeff;
}

double Math::calculateWindKoef(const cell *c, directions InvestigatedDirection) const
{
    clock_t start = clock();
    auto wind = c->getWind().get();
    if (wind == nullptr)
    {
        return double(1);
    }

    WindMetaData meta = {wind->directionDifference(InvestigatedDirection), wind->getWindSpeed()};
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

int round_to_4(int digit){
    int round_to = 4;
    return (digit + round_to - 1) & -round_to;
}

double Math::calculateGroundSlopeKoef(directions InvestigatedDirection, int altitudeDifference) const
{
    clock_t start = clock();

    SlopeMetaData meta = {bool(static_cast<int>(InvestigatedDirection) / int(2)), altitudeDifference};
    
    auto a = slope_result_[meta];
    if (a != 0)
    {
        clock_t per_iteration = clock() - start;
        slope_timer = per_iteration+slope_timer;
        slope_counter++;
        slope_counter_[meta]++;
        slope_result_[meta] = a;
        result_slope_[a]++;
        return a;
    }

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
    slope_result_[meta] = result;

    clock_t per_iteration = clock() - start;
    slope_timer = per_iteration+slope_timer;
    slope_counter++;
    return result;
}

double Math::calculateBiomassKoef(const cell *c) const
{
    int coeff = -1;
    switch (c->getBiomass())
    {
        case 0 ... 100:
            coeff = -0.4;
            break;
        case 101 ... 150:
            coeff = 0;
            break;
        case 151 ... 300:
            coeff = 0.3;
            break;
        default:
            throw std::invalid_argument("received strange value in biomass");
            break;
    }
    return double(1+coeff);
}

double Math::calculateMoistureKoeff(int moisture_percentage) const
{
    clock_t start = clock();
    moisture_percentage = round_to_4(moisture_percentage);
    auto a = moisture_result_[moisture_percentage];
    if (a != 0){
        clock_t per_iteration = clock() - start;
        moisture_timer_ = per_iteration+moisture_timer_;
        moisture_counter_++;
        return moisture_result_[moisture_percentage];
    };

    double result = 2 * exp(-0.111 * moisture_percentage / 100);
    moisture_result_[moisture_percentage] = result;
    clock_t per_iteration = clock() - start;
    moisture_timer_ = per_iteration+moisture_timer_;
    moisture_counter_++;
    return result;
}

bool Math::willSpread(const cell *c, directions InvestigatedDirection, int altitudeDifference) const
{
    double fireKoeff = calculateWindKoef(c, InvestigatedDirection);
    double slopeKoeff = calculateGroundSlopeKoef(InvestigatedDirection, altitudeDifference);
    double biomassKoeff = calculateBiomassKoef(c);
    double moistureKoeff = calculateMoistureKoeff(c->getWind().get()->getMoistureCoeff());
    int result = fireKoeff * slopeKoeff * biomassKoeff * moistureKoeff * 100 / 2;
    result_overall[result]++;
    return result > (rand() % 100);
}

bool Math::willSpreadThroughOne(const cell *c, directions InvestigatedDirection, int altitudeDifference) const
{
    double fireKoeff = calculateWindKoef(c, InvestigatedDirection);
    double slopeKoeff = calculateGroundSlopeKoef(InvestigatedDirection, altitudeDifference);
    return int(fireKoeff * slopeKoeff * 100) + (rand() % 30) > throughPercentage();
}

bool to_bool(std::string const &s)
{
    return s != "0";
}

Math::Math()
    :slope_timer(0), slope_counter(0), wind_timer_(0), wind_counter_(0), moisture_timer_(0), moisture_counter_(0)
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
    std::ifstream csvFile3{"data/moisture.csv"};
    while (std::getline(csvFile3, row))
    {
        std::stringstream rowStream(row);
        std::string col;
        std::vector<std::string> cols;
        while (std::getline(rowStream, col, ','))
        {
            cols.push_back(col);
        }
        moisture_result_[atoi(cols[0].c_str())] = atof(cols[1].c_str());
    }
    csvFile3.close();
}

Math::~Math()
{
    printf("total slope time %f, overall counts %d\n", (double)(slope_timer), slope_counter);
    printf("total wind time %f, overall counts %d\n", (double)(wind_timer_), wind_counter_);
    printf("total moisture time %f, overall counts %d\n", (double)(moisture_timer_), moisture_counter_);
    // printf("slope profiling\n");
    // std::map<int, int> slope_distribution;

    // for (auto &&i : moisture_result_)
    // {
    //     printf("%d,%f\n", i.first, i.second);
    // }
    // for (auto &&i : wind_result_)
    // {
    //     printf("%d,%f,%f\n", i.first.direction_difference_, i.first.wind_speed_, i.second);
    // }
    // for (auto &&i : slope_result_)
    // {
    //     printf("%d,%d,%f\n", i.first.diagonal_direction_, i.first.altitude_difference_, i.second);
    // }
    // for (auto &&i : result_slope_)
    // {
    //     // printf("%.4f;%d\n", i.first, i.second);
    //     printf("%.4f\n", i.first);

    // };
    
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