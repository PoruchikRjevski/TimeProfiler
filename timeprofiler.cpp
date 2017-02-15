#include "timeprofiler.h"

TimeProfiler::TimeProfiler()
{
    return;
}

bool TimeProfiler::initProfiler(unsigned short mode)
{
    this->resetAll();

    this->_mode = mode;

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
        switch (this->_mode) {
        case MODE_AVERAGE: {
            this->_temporary = this->_stop.QuadPart - this->_start.QuadPart;
            this->addAverageTiming(this->_temporary);
        } break;
        case MODE_MAPPED: {

        } break;
        default: {
            return false;
        };
        }

        return true;
    }

    cout << "error stop TimeProfiler";

    return false;
}

bool TimeProfiler::getTimings(timingsFinal &timings, unsigned short type)
{
    unsigned long typeMult = 1;
    switch (type) {
    case MU_SEC: {
        typeMult = 1000000;
    } break;
    case M_SEC: {
        typeMult = 1000;
    } break;
    }

    switch (this->_mode) {
    case MODE_AVERAGE: {
        timings.max = static_cast<double>(this->_timingsRaw.max.QuadPart);
        timings.max = typeMult * timings.max / this->_freq.QuadPart;

        timings.min = static_cast<double>(this->_timingsRaw.min.QuadPart);
        timings.min = typeMult * timings.min / this->_freq.QuadPart;

        timings.aver = static_cast<double>(this->_timingsRaw.aver.QuadPart);
        timings.aver = typeMult * timings.aver / this->_freq.QuadPart;
    } break;
    case MODE_MAPPED: {

    } break;
    default: {
        return false;
    };
    }

    return true;
}

bool TimeProfiler::getFrequency(void)
{
    if (QueryPerformanceFrequency(&this->_freq)) {
        return true;
    }

    cout << "error get freq TimeProfiler";

    return false;
}

void TimeProfiler::resetAll(void)
{
    this->_freq.QuadPart = 0;
    this->_start.QuadPart = 0;
    this->_stop.QuadPart = 0;

    this->_timingsRaw.aver.QuadPart = 0;
    this->_timingsRaw.max.QuadPart = 0;
    this->_timingsRaw.min.QuadPart = 0;

    this->_timingsFinal.aver = 0;
    this->_timingsFinal.max = 0;
    this->_timingsFinal.min = 0;

    this->_temporary = 0;

    this->_mode = MODE_UNDEFINED;

    return;
}

void TimeProfiler::addAverageTiming(LONGLONG &diff)
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
