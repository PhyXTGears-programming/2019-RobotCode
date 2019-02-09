#include <cstdlib>
#include <algorithm>

template <class T>
class RunningAverage {
public:
    // Create an instance that stores the given number of elements.
    RunningAverage(size_t);
    ~RunningAverage();

    // Push a value onto the ring buffer, and drop any extra values.
    void PushValue(T);

    // Return the value of the running average.
    T GetAverage();

private:
    T *m_Values;
    size_t m_Capacity;
    size_t m_Count;
    size_t m_Index;

    T m_Total;
    T m_Average;
};
