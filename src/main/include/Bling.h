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
            ClimbReady,
            Climb,
            SizeValue
        };

    private:
        const double m_PwmMap [SizeValue] = {
            [Default]                   = -0.99, // Rainbow Forest Palette
            [CargoIntakePattern]        =  0.65, // Orange
            [CargoShootShipPattern]     = -0.39, // Color Waves, Lava Palette
            [CargoShootRocketPattern]   = -0.57, // Large Fire
            [IntakeRotateShip]          = -0.91, // Rainbow - Forest
            [IntakeRotateRocket]        =  0.75, // Dark Green
            [HatchPattern]              = -0.07, // Strobe Gold
            [ClimbReady]                = -0.13, // Breathe Gray
            [Climb]                     = -0.99, // Rainbow
        };

        const double m_DelayMap [SizeValue] = {
            [Default]                   = 2.0,
            [CargoIntakePattern]        = 2.0,
            [CargoShootShipPattern]     = 2.0,
            [CargoShootRocketPattern]   = 2.0,
            [IntakeRotateShip]          = 2.0,
            [IntakeRotateRocket]        = 2.0,
            [HatchPattern]              = 2.0,
            [ClimbReady]                = 3.0,
            [Climb]                     = 10.0,
        };

    public:
        Bling();
        void RunBling();
        void SetBling(BlingPattern p);

    private:
        // Blinkin
        frc::Spark m_Blinkin{kBlinkinPin};

        BlingPattern m_Pattern = Default;
        Delay m_SeqDelay{0};
};