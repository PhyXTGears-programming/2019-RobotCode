#pragma once

#include "util/StopWatch.h"

class Delay {
    public:
        Delay(double period) : m_IsEnabled(false), m_Period(period) {}

        void Start() {
            m_IsEnabled = true;
            m_Watch.Reset();
        }
        void Stop() { m_IsEnabled = false; }

        // Delay is only done if it is enabled.  Sometimes we work with
        // sequential delays while the Robot checks both concurrently.
        bool IsDone() { return m_IsEnabled && m_Period <= m_Watch.Elapsed(); }
        double Elapsed() { return m_Watch.Elapsed(); }
        double Remaining() { return m_Period - m_Watch.Elapsed(); }

        void setPeriod(double period) { m_Period = period; }
        double getPeriod() { return m_Period; }

    private:
        bool m_IsEnabled;
        double m_Period;
        StopWatch m_Watch;
};
