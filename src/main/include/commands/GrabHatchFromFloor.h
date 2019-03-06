#pragma once

#include <frc/commands/Command.h>

class GrabHatchFromFloor : public frc::Command {
 public:
  GrabHatchFromFloor();
  void Initialize() override;
  void Execute() override;
  bool IsFinished() override;
  void End() override;
  void Interrupted() override;
};
