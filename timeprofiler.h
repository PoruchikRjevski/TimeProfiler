#ifndef TIMEPROFILER_H
#define TIMEPROFILER_H

#include <map>
#include <iostream>

#include "windows.h"

using std::map;
using std::cout;

enum mode : unsigned short {
    MODE_UNDEFINED,
    MODE_MAPPED,
    MODE_AVERAGE
};

enum timeType : unsigned short {
    MU_SEC,
    M_SEC,
    SEC
};

struct timingsFinal {
    double min;
    double max;
    double aver;
};

struct timingsRaw {
    LARGE_INTEGER min;
    LARGE_INTEGER max;
    LARGE_INTEGER aver;
};

/// OLD


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

/// OLD




class TimeProfiler
{
public:
    TimeProfiler();

    bool initProfiler(unsigned short mode);
    bool start(void);
    bool stop(void);

    bool getTimings(timingsFinal &timings, unsigned short type);


    /// OLD







private:
    void resetAll(void);
    bool getFrequency(void);
    void addAverageTiming(LONGLONG &diff);

    unsigned short _mode;

    timingsRaw _timingsRaw;
    timingsFinal _timingsFinal;

    LONGLONG _temporary;

    LARGE_INTEGER _freq;
    LARGE_INTEGER _start;
    LARGE_INTEGER _stop;



    /// OLD
    map<LARGE_INTEGER, unsigned long long> allTimings;

    workVars currentWorkVars;
    rawTimings currentRawTimings;
};

#endif // TIMEPROFILER_H
