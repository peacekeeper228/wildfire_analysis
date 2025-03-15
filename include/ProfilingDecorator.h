#pragma once
#include <map>
#include "Math.h"

class ProfilingDecorator final: public Math
{
private:
    Math* primary_class;
    mutable int positiveCounter;
    mutable int counter;
public:
    ProfilingDecorator(Math* formula);
    bool willSpread(const cell* c, directions InvestigatedDirection, int altitudeDifference) const override;
    bool willSpreadThroughOne(const cell* c, directions InvestigatedDirection, int altitudeDifference) const override;
    ~ProfilingDecorator();
};