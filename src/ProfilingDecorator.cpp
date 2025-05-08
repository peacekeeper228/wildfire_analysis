#include "../include/ProfilingDecorator.h"
#include "ProfilingDecorator.h"
#include <iostream>

ProfilingDecorator::ProfilingDecorator(Math *formula)
    : primary_class_(formula), positive_counter_(0), counter_(0)
{
}

bool ProfilingDecorator::willSpread(const cell *c, directions InvestigatedDirection, int altitudeDifference) const
{
    clock_t start = clock();
    int direction_difference = c->getWind().get()->directionDifference(InvestigatedDirection);
    // if (direction_difference == 4){
    //     return false;
    // } else if (direction_difference >= 2)
    // {
    //     return 4 > (rand() % 100);
    // };
    
    auto result = this->primary_class_->willSpread(c, InvestigatedDirection, altitudeDifference);
    clock_t per_iteration = clock() - start;
    overall_timer_ = per_iteration+overall_timer_;
    metaData key = {InvestigatedDirection, altitudeDifference, c->getWind().get()->getWindDirection(), c->getWind().get()->getWindSpeed()};
    results_[key]++;
    this->counter_++;
    if (result)
    {
        results_directions_[InvestigatedDirection]++;
        this->positive_counter_++;
    };
    return result;
}

bool ProfilingDecorator::willSpreadThroughOne(const cell *c, directions InvestigatedDirection, int altitudeDifference) const
{
    return this->primary_class_->willSpreadThroughOne(c, InvestigatedDirection, altitudeDifference);
}

ProfilingDecorator::~ProfilingDecorator()
{
    std::cout << "Overall: " << this->counter_ << " Positive: " << this->positive_counter_ << std::endl;
    
    std::map<int,int> result_distribution;
    // printf("Results: altitude_difference_, wind_direction_, wind_speed_, investigated_direction_, number_of_calculations\n");
    // for (const auto &pair : results_)
    // {
    //     printf("%d,%d,%2.2f,%d;%d \n",
    //            pair.first.altitude_difference_,
    //            static_cast<int>(pair.first.wind_direction_),
    //            pair.first.wind_speed_,
    //            static_cast<int>(pair.first.investigated_direction_),
    //            pair.second);
    //     result_distribution[pair.second]++;
    // };
    printf("Overall timer: %ld\n", overall_timer_);

    // printf("Distribution\n");
    // for (auto &&i : result_distribution)
    // {
    //     printf("%d;%d\n", i.first, i.second);
    // };
    // printf("Distribution direction\n");
    // for (auto &&i : results_directions_)
    // {
    //     printf("%d;%d\n", static_cast<int>(i.first), i.second);
    // };
    
    primary_class_->~Math();
}
