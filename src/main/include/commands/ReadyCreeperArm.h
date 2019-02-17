#pragma once

#include <frc/commands/Command.h>

class ReadyCreeperArm : public frc::Command {
 public:
  ReadyCreeperArm();
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;
};
