// Generic in-line Button Board class.

#pragma once

#include <frc/GenericHID.h>

class ButtonBoard : public frc::GenericHID {
    public:
        ButtonBoard(int id) : frc::GenericHID(id) {}

        // Button Boards don't have joysticks!
        double GetX(JoystickHand hand=kRightHand) const override { return 0; }
        double GetY(JoystickHand hand=kRightHand) const override { return 0; }

        // Wraps, because raw food is bad.
        bool GetButton(int button) const { return GetRawButton(button); }
        bool GetButtonPressed(int button) { return GetRawButtonPressed(button); }
        bool GetButtonReleased(int button) { return GetRawButtonReleased(button); }

        // The button board should display as a generic HID, but this is just to be sure.
        frc::GenericHID::HIDType GetType() const { return frc::GenericHID::HIDType::kHIDGamepad; }
};