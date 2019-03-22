#include "AngleSource.h"

#include "Robot.h"

#include <iostream>

AngleSource::AngleSource(Axis axis) : frc::PIDSource() {
    m_pidSource = frc::PIDSourceType::kDisplacement;
    m_ReadAxis = axis;
}

double AngleSource::PIDGet() {
    switch (m_ReadAxis) {
        case kPitch:
            return Robot::m_AHRS->GetPitch();
            break;
        case kRoll:
            return Robot::m_AHRS->GetRoll();
            break;
        case kYaw:
            return Robot::m_AHRS->GetYaw();
            break;
        default:
            std::cout << "AxisSource: Invalid Read Axis" << std::endl;
            break;
    }
    return 0;
}
