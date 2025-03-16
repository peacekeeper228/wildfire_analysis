#pragma once
#include <map>
#include "Math.h"

class ProfilingDecorator final: public Math
{
private:
    Math* primary_class_;
    mutable int positive_counter_;
    mutable int counter_;
public:
    ProfilingDecorator(Math* formula);
    bool willSpread(const cell* c, directions InvestigatedDirection, int altitudeDifference) const override;
    bool willSpreadThroughOne(const cell* c, directions InvestigatedDirection, int altitudeDifference) const override;
    ~ProfilingDecorator();
};