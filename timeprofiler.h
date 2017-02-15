#ifndef TIMEPROFILER_H
#define TIMEPROFILER_H

#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

#include "windows.h"

#define MULT_SEC                1
#define MULT_M_SEC              1000
#define MULT_MU_SEC             1000000

typedef unsigned short _ushort;
typedef LONGLONG _longlong;
typedef unsigned long _ulong;
typedef unsigned long long _ulonglong;

using std::map;
using std::pair;
using std::vector;
using std::cout;
using std::sort;

enum timeType : unsigned short {
    MU_SEC,
    M_SEC,
    SEC
};

enum sortType : _ushort {
    BY_TIME,
    BY_CASES
};

// vector<pair<times, time>> - "times" - number of cases of "time"
struct timingsFinal {
    double min;
    double max;
    double aver;
    vector<pair<_ulonglong, double>> timingsVec;
};

struct timingsRaw {
    LARGE_INTEGER min;
    LARGE_INTEGER max;
    LARGE_INTEGER aver;
    map<_longlong, _ulonglong> timingsRawMap;
};

class TimeProfiler
{
public:
    TimeProfiler();

    bool initProfiler(void);
    bool start(void);
    bool stop(void);

    void getTimings(_ushort timeType, timingsFinal &timings, _ushort sortType);

private:
    void resetAll(void);
    bool getFrequency(void);

    void addTiming(_longlong &diff);
    void addAverageTiming(_longlong &diff);
    void addTimingToMap(_longlong &diff);

    void getAverageTimings(_ushort mult, timingsFinal &timings);
    void getVectorTimings(_ushort timeType, timingsFinal &timings);
    void sortVector(_ushort sortType, timingsFinal &timings);

    _ulonglong getMultByType(_ushort &type);

    timingsRaw _timingsRaw;
    timingsFinal _timingsFinal;

    LARGE_INTEGER _freq;
    LARGE_INTEGER _start;
    LARGE_INTEGER _stop;
};

#endif // TIMEPROFILER_H
