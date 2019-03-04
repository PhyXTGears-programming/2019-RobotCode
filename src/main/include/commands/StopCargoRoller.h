#pragma once

#include <frc/commands/Command.h>

class StopCargoRoller : public frc::Command {
public:
    StopCargoRoller();
    void Initialize() override;
    void Execute() override;
    bool IsFinished() override;
    void End() override;
    void Interrupted() override;
};
