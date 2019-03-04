#pragma once

#include <cmath>

#include <frc/GenericHID.h>
#include <frc/Joystick.h>

// Button Board Rotation Consts
#define bRotateFloorHatchPickup 1
#define bRotateFloorCargoPickup 2
#define bRotateHatchFeederScore 3
#define bRotateRocketShot       4
#define bRotateCargoShot        5
#define bRotateStowed           6
// Button Board Action Consts
#define bHatchGrab       1
#define bHatchFloor      2
#define bHatchRelease    3
#define bCargoHighShot   4
#define bCargoIntake     5
#define bCargoCloseShot  6
#define sClimbSequence   7
#define bCreeperReadyArm 8

// Flight Stick consts
#define bRetractPiston 3
#define bRetractArm    4

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
        bool GetGoHomePressed()           { return m_RotationPad.GetButtonPressed(bRotateStowed); };

        // Action buttons.
        bool GetHatchGrabPressed()       { return m_ActionPad.GetButtonPressed(bHatchGrab); };
        bool GetHatchFloorPressed()      { return m_ActionPad.GetButtonPressed(bHatchFloor); };
        bool GetHatchReleasePressed()    { return m_ActionPad.GetButtonPressed(bHatchRelease); };
        bool GetCargoCloseShotPressed()  { return m_ActionPad.GetButtonPressed(bCargoCloseShot); };
        bool GetCargoIntakePressed()     { return m_ActionPad.GetButtonPressed(bCargoIntake); };
        bool GetCargoHighShotPressed()   { return m_ActionPad.GetButtonPressed(bCargoHighShot); };
        bool GetClimbSequencePressed()   { return m_ActionPad.GetButtonPressed(sClimbSequence); };
        bool GetCreeperReadyArmPressed() { return m_ActionPad.GetButtonPressed(bCreeperReadyArm); };

        // Functions for testing, probably shouldn't keep these.
        bool GetRetractCylinder() { return m_FlightStick.GetRawButtonPressed(bRetractPiston); };
        bool GetRetractArm()      { return m_FlightStick.GetRawButtonPressed(bRetractArm); };

        double GetThrottle() { return m_FlightStick.GetThrottle(); }; // Throttle is the switch on the base of the stick
        double GetJoystickY() {
            double posY = -m_FlightStick.GetY();
            if (std::abs(posY) > 0.1) {
                return posY;
            } else {
                return 0;
            }
        };

    private:
        ButtonBoard m_ActionPad{2};     // To control the intake and creeper arms actions.
        ButtonBoard m_RotationPad{1};   // To control the intake angle.
        frc::Joystick m_FlightStick{3}; // To control the intake and creeper arms *manually*.
};
