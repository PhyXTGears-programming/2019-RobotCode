#pragma once

#include <frc/Encoder.h>
#include <frc/PIDSource.h>

class AngleSource : public frc::PIDSource {
    public:
        enum Axis {
            kPitch,
            kRoll,
            kYaw
        };

        AngleSource(Axis axis = kPitch);

        inline void SetPIDSourceType(bool pitch = true) {}
        inline frc::PIDSourceType GetPIDSourceType() { return frc::PIDSourceType::kRate; }
        double PIDGet() override;

    private:
        Axis m_ReadAxis;
};
