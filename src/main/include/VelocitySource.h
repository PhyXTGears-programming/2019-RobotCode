#pragma once

#include <frc/Encoder.h>
#include <frc/PIDSource.h>

class VelocitySource : public frc::PIDSource {
    public:
        VelocitySource(frc::Encoder &encoder);

        inline void SetPIDSourceType(frc::PIDSourceType pidSource) {}
        inline frc::PIDSourceType GetPIDSourceType() { return frc::PIDSourceType::kRate; }
        double PIDGet() override;
    private:
        frc::Encoder* m_Encoder;
};
