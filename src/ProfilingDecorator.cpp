#include "../include/ProfilingDecorator.h"
#include "ProfilingDecorator.h"
#include <iostream>

ProfilingDecorator::ProfilingDecorator(Math *formula)
    : primary_class(formula), counter(0), positiveCounter(0)
{
}

bool ProfilingDecorator::willSpread(const cell *c, directions InvestigatedDirection, int altitudeDifference) const
{
    auto result = this->primary_class->willSpread(c, InvestigatedDirection, altitudeDifference);
    this->counter++;
    if (result)
    {
        this->positiveCounter++;
    };
    return result;
}

bool ProfilingDecorator::willSpreadThroughOne(const cell *c, directions InvestigatedDirection, int altitudeDifference) const
{
    return this->primary_class->willSpread(c, InvestigatedDirection, altitudeDifference);
}

ProfilingDecorator::~ProfilingDecorator()
{
    std::cout << "Overall: " << this->counter << " Positive: " << this->positiveCounter << std::endl;
    primary_class->~Math();
}
