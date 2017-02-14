#ifndef TIMEPROFILER_H
#define TIMEPROFILER_H

#include <string>
#include <map>
#include <iostream>

#include "windows.h"

using std::string;
using std::map;
using std::cout;

struct rawTimings {
    LARGE_INTEGER maxTime;
    LARGE_INTEGER minTime;
    LARGE_INTEGER avTime;
};

struct workVars {
    LARGE_INTEGER freq;
    LARGE_INTEGER startVal;
    LARGE_INTEGER finishVal;
};

enum timingsType {
    MAXIMUM,
    MINIMUM,
    AVERAGE
};

enum profilerMode {
    UNDEFINED,
    SAMPLES,
    TRIPLE
};


class TimeProfiler
{
public:
    TimeProfiler();

    bool initQpcTimings(void);

    bool startQpcTimings(void);
    bool stopQpcTimings(void);

    double getTimingMs(int type);

private:
    unsigned short _mode;


    bool getFrequency(void);
    void clearAll(void);
    void addTiming(LARGE_INTEGER &diff);

    map<LARGE_INTEGER, unsigned long long> allTimings;

    workVars currentWorkVars;
    rawTimings currentRawTimings;
};

#endif // TIMEPROFILER_H
