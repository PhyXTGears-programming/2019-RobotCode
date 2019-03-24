#pragma once

#include "frc/Spark.h"
#include "util/Delay.h"

#include "RobotMap.h"

class Bling {
    public:
        enum BlingPattern {
            Default,
            CargoIntakePattern,
            CargoShootShipPattern,
            CargoShootRocketPattern,
            IntakeRotateShip,
            IntakeRotateRocket,
            HatchPattern,
            Climb,
            SizeValue
        };

    private:
        const double m_PwmMap [SizeValue] = {
            [Default]                   = -0.99, // Rainbow
            [CargoIntakePattern]        =  0.65, // Orange
            [CargoShootShipPattern]     = -0.39, // Color Waves, Lava Palette
            [CargoShootRocketPattern]   = -0.57, // Large Fire
            [IntakeRotateShip]          = -0.99,
            [IntakeRotateRocket]        = -0.99,
            [HatchPattern]              = -0.07, // Strobe Gold
            [Climb]                     = -0.13, // Breathe Gray
        };

    public:
        Bling();
        void RunBling();
        void SetBling(BlingPattern p);

    private:
        // Blinkin
        frc::Spark m_Blinkin{kBlinkinPin};

        BlingPattern m_Pattern = Default;
        Delay m_SeqDelay{2};
};