#pragma once

#include <frc/GenericHID.h>
#include <frc/Joystick.h>


// We don't know which button will be what number ID yet, CHANGE LATER!
#define Switch_ClimbSequence     0
#define Button_CreeperArmToClimb 0

#define Button_Cargo 0

#define Button_FeederHatchGrab    0
#define Button_HatchRelease       0
#define Button_FloorHatchSequence 0

#define Button_IntakeStowed      0
#define Button_IntakeCargoShot   0
#define Button_RocketShot        0
#define Button_HatchFeederScore  0
#define Button_CargoGroundPickup 0
#define Button_FloorHatchPickup  0


// Generic in-line Button Board class.
// We use an `am-3753` Button Board controller.
class ButtonBoard : public frc::GenericHID { // generic inline button board class
    public:
        // Button Boards don't have joysticks!
        double GetX(JoystickHand hand=kRightHand) const override = 0;
        double GetY(JoystickHand hand=kRightHand) const override = 0;

        // Wraps, because raw food is bad.
        bool GetButton(int button) const { return GetRawButton(button); };
        bool GetButtonPressed(int button) { return GetRawButtonPressed(button); };
        bool GetButtonReleased(int button) { return GetRawButtonReleased(button); };

        // The button board should display as a generic HID, but this is just to be sure.
        frc::GenericHID::HIDType GetType() const { return frc::GenericHID::HIDType::kHIDGamepad; };
};

// A combo class that encapsules our button board and flight stick.
// This is only for our robot operator to use.
class OperatorHID {
    public:
        OperatorHID();
    private:
        ButtonBoard m_ButtonPad{2};     // To control the intake and creeper arms *with commands*.
        frc::Joystick m_FlightStick{3}; // To control the intake and creeper arms *manually*.
};
