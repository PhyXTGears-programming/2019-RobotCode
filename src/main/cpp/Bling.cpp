#include "Bling.h"

Bling::Bling () {
    // TODO
}

void Bling::RunBling () {
    if (m_SeqDelay.IsDone()) {
        m_Pattern = Default;
        m_SeqDelay.Stop();
    }
    m_Blinkin.Set(m_PwmMap[m_Pattern]);
}

void Bling::SetBling (BlingPattern p) {
    m_Pattern = p;
    m_SeqDelay.Start();
}