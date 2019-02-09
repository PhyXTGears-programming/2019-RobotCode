#include "VelocitySource.h"

VelocitySource::VelocitySource(frc::Encoder &encoder) : frc::PIDSource() {
    m_pidSource = frc::PIDSourceType::kRate;
    m_Encoder = &encoder;
}

double VelocitySource::GetPID() {
    return m_Encoder->GetRate();
}
