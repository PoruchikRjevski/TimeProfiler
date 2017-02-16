#include "timeprofiler.h"

TimeProfiler::TimeProfiler()
{
    return;
}

bool TimeProfiler::initProfiler(void)
{
    this->resetAll();

    if (this->getFrequency()) {
        return true;
    }

    cout << "error init TimeProfiler";

    return false;
}

bool TimeProfiler::start(void)
{
    if (QueryPerformanceCounter(&this->_start)) {
        return true;
    }

    cout << "error start TimeProfiler";

    return false;
}

bool TimeProfiler::stop()
{
    if (QueryPerformanceCounter(&this->_stop)) {
        _longlong tempDiff = this->_stop.QuadPart - this->_start.QuadPart;

        this->addTiming(tempDiff);

        this->_timingsRaw.iterations++;

        return true;
    }

    cout << "error stop TimeProfiler";

    return false;
}

void TimeProfiler::getTimings(_ushort timeType, timingsFinal &timings, _ushort sortType)
{
    _ulong typeMult = this->getMultByType(timeType);

    this->getAverageTimings(typeMult, timings);

    this->getVectorTimings(typeMult, timings);

    this->sortVector(sortType, timings);

    return;
}

bool TimeProfiler::getFrequency(void)
{
    if (QueryPerformanceFrequency(&this->_freq)) {
        return true;
    }

    cout << "error get freq TimeProfiler";

    return false;
}

void TimeProfiler::addTiming(_longlong &diff)
{
    this->addAverageTiming(diff);

    this->addTimingToMap(diff);

    return;
}

void TimeProfiler::resetAll(void)
{
    this->_freq.QuadPart = 0;
    this->_start.QuadPart = 0;
    this->_stop.QuadPart = 0;

    this->_timingsRaw.aver.QuadPart = 0;
    this->_timingsRaw.max.QuadPart = 0;
    this->_timingsRaw.min.QuadPart = 0;
    this->_timingsRaw.timingsRawMap.clear();
    this->_timingsRaw.iterations = 0;

    this->_timingsFinal.aver = 0;
    this->_timingsFinal.max = 0;
    this->_timingsFinal.min = 0;
    this->_timingsFinal.timingsVec.clear();

    return;
}

void TimeProfiler::addAverageTiming(_longlong &diff)
{
    if (this->_timingsRaw.max.QuadPart == 0
            || this->_timingsRaw.max.QuadPart < diff) {
        this->_timingsRaw.max.QuadPart = diff;
    }

    if (this->_timingsRaw.min.QuadPart == 0
            || this->_timingsRaw.min.QuadPart > diff) {
        this->_timingsRaw.min.QuadPart = diff;
    }

    if (this->_timingsRaw.aver.QuadPart == 0) {
        this->_timingsRaw.aver.QuadPart = diff;
    }
    else {
        this->_timingsRaw.aver.QuadPart = (this->_timingsRaw.aver.QuadPart + diff)/2;
    }

    return;
}

void TimeProfiler::addTimingToMap(_longlong &diff)
{
    map<_longlong, _ulonglong>::iterator tempMappedElement
            = this->_timingsRaw.timingsRawMap.find(diff);

    if (tempMappedElement != this->_timingsRaw.timingsRawMap.end()) {
        tempMappedElement->second++;
    }
    else {
        this->_timingsRaw.timingsRawMap.insert(pair<_longlong, _ulonglong>(diff, 1));
    }

    return;
}

void TimeProfiler::getAverageTimings(_ushort mult, timingsFinal &timings)
{
    timings.max = static_cast<double>(this->_timingsRaw.max.QuadPart);
    timings.max = mult * timings.max / this->_freq.QuadPart;

    timings.min = static_cast<double>(this->_timingsRaw.min.QuadPart);
    timings.min = mult * timings.min / this->_freq.QuadPart;

    timings.aver = static_cast<double>(this->_timingsRaw.aver.QuadPart);
    timings.aver = mult * timings.aver / this->_freq.QuadPart;

    return;
}

void TimeProfiler::getVectorTimings(_ushort mult, timingsFinal &timings)
{
    for (auto &i : this->_timingsRaw.timingsRawMap) {
        float tmpPercent = static_cast<float>(i.second) * HUNDRED_PERCENT;
        tmpPercent = tmpPercent / this->_timingsRaw.iterations;

        double tmpTime = static_cast<double>(i.first);
        tmpTime = mult * tmpTime / this->_freq.QuadPart;

        timings.timingsVec.push_back(pair<float, double>(tmpPercent, tmpTime));
    }

    return;
}

void TimeProfiler::sortVector(_ushort sortType, timingsFinal &timings)
{
    switch (sortType) {
    case BY_RATIO: {
        sort(timings.timingsVec.begin(), timings.timingsVec.end(),
                  [](pair<float, double> &a, pair<float, double> &b)
                    { return a.first > b.first; }
            );
    } break;
    case BY_TIME: {
        sort(timings.timingsVec.begin(), timings.timingsVec.end(),
                  [](pair<float, double> &a, pair<float, double> &b)
                    { return a.second > b.second; }
            );
    } break;
    }

    return;
}

_ulonglong TimeProfiler::getMultByType(_ushort &type)
{
    switch (type) {
    case MU_SEC: {
        return MULT_MU_SEC;
    } break;
    case M_SEC: {
        return MULT_M_SEC;
    } break;
    }

    return MULT_SEC;
}
