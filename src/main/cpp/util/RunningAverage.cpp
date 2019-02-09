
#include "util/RunningAverage.h"

template <class T>
RunningAverage<T>::RunningAverage(size_t capacity)
    : m_Capacity(capacity)
    , m_Count(0)
    , m_Index(0)
    , m_Total(0.0)
    , m_Average(0.0)
{
    m_Values = new T[capacity];
}

template <class T>
RunningAverage<T>::~RunningAverage() {
    delete m_Values;
}

template <class T>
void RunningAverage<T>::PushValue(T val) {
    m_Total -= m_Values[m_Index];
    m_Total += val;

    m_Values[m_Index] = val;
    m_Count = std::min(m_Count, m_Capacity);
    m_Index = (m_Index + 1) % m_Capacity;

    if (0 < m_Count) {
        m_Average = m_Total / m_Count;
    }
}

template <class T>
T RunningAverage<T>::GetAverage() {
    return m_Average;
}
