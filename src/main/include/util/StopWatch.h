#pragma once

#include <frc/Timer.h>

class StopWatch {
    public:
        StopWatch() { Reset(); }

        void Reset() { m_StartTime = frc::Timer::GetFPGATimestamp(); }
        double Elapsed() { return frc::Timer::GetFPGATimestamp() - m_StartTime; }

    private:
        double m_StartTime;
};
