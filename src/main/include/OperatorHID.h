#pragma once

#include <frc/GenericHID.h>
#include <frc/Joystick.h>

// We don't know which button will be what number ID yet, CHANGE LATER!

#define bRotateFloorHatchPickup 1
#define bRotateFloorCargoPickup 2
#define bRotateHatchFeederScore 3
#define bRotateRocketShot       4
#define bRotateCargoShot        5
#define bRotateStowed           6

#define bHatchGrab       1
#define bHatchFloor      2
#define bHatchRelease    3
#define bCargoHighShot   4
#define bCargoIntake     5
#define bCargoCloseShot  6
#define sClimbSequence   7
#define bCreeperReadyArm 8

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
        bool GetClimbSequenceStart() { return m_ActionPad.GetButtonPressed(sClimbSequence); };

        // Rotation position buttons.
        bool GetFloorHatchPickupPressed() { return m_RotationPad.GetButtonPressed(bRotateFloorHatchPickup); };
        bool GetFloorCargoPickupPressed() { return m_RotationPad.GetButtonPressed(bRotateFloorCargoPickup); };
        bool GetHatchFeederScorePressed() { return m_RotationPad.GetButtonPressed(bRotateHatchFeederScore); };
        bool GetRocketShotPressed()       { return m_RotationPad.GetButtonPressed(bRotateRocketShot); };
        bool GetCargoShotPressed()        { return m_RotationPad.GetButtonPressed(bRotateCargoShot); };
        bool GetStowedPressed()           { return m_RotationPad.GetButtonPressed(bRotateStowed); };

        // Action buttons.
        bool GetCargoCloseShotPressed() { return m_ActionPad.GetButtonPressed(bCargoCloseShot); };
        bool GetCargoHighShotPressed()  { return m_ActionPad.GetButtonPressed(bCargoHighShot); };
        bool GetHatchGrabPressed()      { return m_ActionPad.GetButtonPressed(bHatchGrab); };
        bool GetHatchReleasePressed()   { return m_ActionPad.GetButtonPressed(bHatchRelease); };

        double GetThrottle() { return m_FlightStick.GetThrottle(); }; // Throttle is the switch on the base of the stick
    private:
        ButtonBoard m_ActionPad{1};     // To control the intake and creeper arms actions.
        ButtonBoard m_RotationPad{2};   // To control the intake angle.
        frc::Joystick m_FlightStick{3}; // To control the intake and creeper arms *manually*.
};
