#pragma once

#include <frc/Timer.h>

class StopWatch {
    public:
        StopWatch() { Reset(); }

        void Reset() { m_StartTime = frc::Timer::GetFPGATimestamp(); }

        /* Return the elapsed time (in seconds) since the last reset of the
         * stop watch.
         */
        double Elapsed();

        /* Return the elapsed time (in seconds) and update the start time so
         * the next split will measure the next delta from the current time.
         */
        double Split();

    private:
        double m_StartTime;

        double _Elapsed(double startTime, double stopTime);
};
