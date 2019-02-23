#pragma once

#include <frc/GenericHID.h>
#include <frc/Joystick.h>

// We don't know which button will be what number ID yet, CHANGE LATER!
#define sClimbSequence   0

#define bRotateFloorHatchPickup 0
#define bRotateFloorCargoPickup 0
#define bRotateHatchFeederScore 0
#define bRotateRocketShot       0
#define bRotateCargoShot        0
#define bRotateGoHome           0

#define bCargoCloseShot 0
#define bCargoHighShot  0
#define bHatchGrab      0
#define bHatchRelease   0

// Generic in-line Button Board class.
// We use an `am-3753` Button Board controller.
class ButtonBoard : public frc::GenericHID { // generic inline button board class
    public:
        ButtonBoard(int id) : frc::GenericHID(id) {};

        // Button Boards don't have joysticks!
        double GetX(JoystickHand hand=kRightHand) const override { return 0; };
        double GetY(JoystickHand hand=kRightHand) const override { return 0; };

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

        // Only triggers once after the switch is thrown.
        bool GetClimbSequenceStart() { return m_ButtonPad.GetButtonPressed(sClimbSequence); };

        // Rotation position buttons.
        bool GetFloorHatchPickupPressed() { return m_ButtonPad.GetButtonPressed(bRotateFloorHatchPickup); };
        bool GetFloorCargoPickupPressed() { return m_ButtonPad.GetButtonPressed(bRotateFloorCargoPickup); };
        bool GetHatchFeederScorePressed() { return m_ButtonPad.GetButtonPressed(bRotateHatchFeederScore); };
        bool GetRocketShotPressed()       { return m_ButtonPad.GetButtonPressed(bRotateRocketShot); };
        bool GetCargoShotPressed()        { return m_ButtonPad.GetButtonPressed(bRotateCargoShot); };
        bool GetGoHomePressed()           { return m_ButtonPad.GetButtonPressed(bRotateGoHome); };

        // Action buttons.
        bool GetCargoCloseShotPressed() { return m_ButtonPad.GetButtonPressed(bCargoCloseShot); };
        bool GetCargoHighShotPressed()  { return m_ButtonPad.GetButtonPressed(bCargoHighShot); };
        bool GetHatchGrabPressed()      { return m_ButtonPad.GetButtonPressed(bHatchGrab); };
        bool GetHatchReleasePressed()   { return m_ButtonPad.GetButtonPressed(bHatchRelease); };

        double GetThrottle() { return m_FlightStick.GetThrottle(); }; // Throttle is the switch on the base of the stick
    private:
        ButtonBoard m_ButtonPad{1};     // To control the intake and creeper arms *with commands*.
        frc::Joystick m_FlightStick{2}; // To control the intake and creeper arms *manually*.
};
