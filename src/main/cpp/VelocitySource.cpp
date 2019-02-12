#include "VelocitySource.h"

VelocitySource::VelocitySource(frc::Encoder &encoder) : frc::PIDSource() {
    m_pidSource = frc::PIDSourceType::kRate;
    m_Encoder = &encoder;
}

double VelocitySource::PIDGet() {
    return m_Encoder->GetRate();
}
