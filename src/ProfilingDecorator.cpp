#include "../include/ProfilingDecorator.h"
#include "ProfilingDecorator.h"
#include <iostream>

ProfilingDecorator::ProfilingDecorator(Math *formula)
    : primary_class_(formula), counter_(0), positive_counter_(0)
{
}

bool ProfilingDecorator::willSpread(const cell *c, directions InvestigatedDirection, int altitudeDifference) const
{
    auto result = this->primary_class_->willSpread(c, InvestigatedDirection, altitudeDifference);
    this->counter_++;
    if (result)
    {
        this->positive_counter_++;
    };
    return result;
}

bool ProfilingDecorator::willSpreadThroughOne(const cell *c, directions InvestigatedDirection, int altitudeDifference) const
{
    return this->primary_class_->willSpread(c, InvestigatedDirection, altitudeDifference);
}

ProfilingDecorator::~ProfilingDecorator()
{
    std::cout << "Overall: " << this->counter_ << " Positive: " << this->positive_counter_ << std::endl;
    primary_class_->~Math();
}
