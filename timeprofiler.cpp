#include "qpctimingslib.h"

TimeProfiler::TimeProfiler()
{
    return;
}

bool TimeProfiler::initQpcTimings(void)
{
    bool result = false;

    this->clearAll();

    result = this->getFrequency();

    if(!result) {
        cout << "error init TimeProfiler";
    }

    return result;
}

bool TimeProfiler::startQpcTimings(void)
{
    bool result = false;

    result = QueryPerformanceCounter(&this->currentWorkVars.startVal);

    if(!result) {
        cout << "error start TimeProfiler";
    }

    return result;
}

bool TimeProfiler::stopQpcTimings()
{
    bool result = false;

    result = QueryPerformanceCounter(&this->currentWorkVars.finishVal);

    if(result) {
        LARGE_INTEGER diff;
        diff.QuadPart = this->currentWorkVars.finishVal.QuadPart -
                this->currentWorkVars.startVal.QuadPart;

        this->addTiming(diff);
    }

    if(!result) {
        cout << "error stop TimeProfiler";
    }

    return result;
}

double TimeProfiler::getTimingMs(int type)
{
    double result = 0;

    switch(type) {
    case MAXIMUM: {
        result = static_cast<double>(this->currentRawTimings.maxTime.QuadPart);
        result = 1000 * result / this->currentWorkVars.freq.QuadPart;
    } break;
    case MINIMUM: {
        result = static_cast<double>(this->currentRawTimings.minTime.QuadPart);
        result = 1000 * result / this->currentWorkVars.freq.QuadPart;
    } break;
    case AVERAGE: {
        result = static_cast<double>(this->currentRawTimings.avTime.QuadPart);
        result = 1000 * result / this->currentWorkVars.freq.QuadPart;
    } break;
    }

    return result;
}

bool TimeProfiler::getFrequency(void)
{
    bool result = false;

    result = QueryPerformanceFrequency(&this->currentWorkVars.freq);

    if(!result) {
        cout << "error get freq TimeProfiler";
    }

    return result;
}

void TimeProfiler::clearAll(void)
{
    this->currentRawTimings.avTime.QuadPart = 0;
    this->currentRawTimings.maxTime.QuadPart = 0;
    this->currentRawTimings.minTime.QuadPart = 0;

    this->currentWorkVars.finishVal.QuadPart = 0;
    this->currentWorkVars.freq.QuadPart = 0;
    this->currentWorkVars.startVal.QuadPart = 0;

    return;
}

void TimeProfiler::addTiming(LARGE_INTEGER &diff)
{
    if(this->currentRawTimings.maxTime.QuadPart == 0 ||
            this->currentRawTimings.maxTime.QuadPart < diff.QuadPart) {
        this->currentRawTimings.maxTime.QuadPart = diff.QuadPart;
    }

    if(this->currentRawTimings.minTime.QuadPart == 0 ||
            this->currentRawTimings.minTime.QuadPart > diff.QuadPart) {
        this->currentRawTimings.minTime.QuadPart = diff.QuadPart;
    }

    if(this->currentRawTimings.avTime.QuadPart == 0) {
        this->currentRawTimings.avTime.QuadPart = diff.QuadPart;
    }
    else {
        this->currentRawTimings.avTime.QuadPart =
                (this->currentRawTimings.avTime.QuadPart + diff.QuadPart)/2;
    }

    return;
}
