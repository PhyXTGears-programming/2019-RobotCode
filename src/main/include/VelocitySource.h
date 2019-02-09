#pragma once

#include <frc/Encoder.h>
#include <frc/PIDSource.h>

class VelocitySource : frc::PIDSource {
    public:
        VelocitySource(frc::Encoder &encoder);

        inline void SetPIDSourceType(frc::PIDSourceType pidSource) {}
        inline frc::PIDSourceType GetPIDSourceType() { return frc::PIDSourceType::kRate; }
        double GetPID();
    private:
        frc::Encoder* m_Encoder;
};
