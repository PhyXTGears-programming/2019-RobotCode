#pragma once

#include <frc/commands/Command.h>

class GrabHatchFromLoadingStation : public frc::Command {
    public:
        GrabHatchFromLoadingStation();
        void Initialize() override;
        void Execute() override;
        bool IsFinished() override;
        void End() override;
        void Interrupted() override;

    private:
        bool m_IsHatchGrabbed = false;
};
