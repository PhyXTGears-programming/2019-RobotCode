#include "util/StopWatch.h"

double StopWatch::Elapsed() {
    return _Elapsed(m_StartTime, frc::Timer::GetFPGATimestamp());
}

double StopWatch::Split() {
    double currentTime = frc::Timer::GetFPGATimestamp();
    double elapsed = _Elapsed(m_StartTime, currentTime);

    m_StartTime = currentTime;

    return elapsed;
}

/* Code adapted from
 * https://github.com/wpilibsuite/allwpilib/blob/v2019.3.2/wpilibc/src/main/native/cpp/Timer.cpp
 */
double StopWatch::_Elapsed(double startTime, double stopTime) {
    if (startTime > stopTime) {
        // If the startTime is after the stopTime, then the FPGA clock rolled
        // over.  Compensate by adding the ~71 minutes that it takes to roll
        // over to the current time.
        return (frc::Timer::kRolloverTime + stopTime - startTime);
    } else {
        return stopTime - startTime;
    }
}
