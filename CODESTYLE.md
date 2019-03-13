
# PhyxtGears 2019 Codestyle Standards
All programmers for First Robotics Competition Team 1720 (a.k.a. PhyxtGears) must adhere to this style of programming for the 2019 FRC competition season.

### Terms
Useful terms to understand while reading.
* Declaration: telling the compiler that a variable exists, but not giving it an associated value.
* Definition: declaring a variable and initializing it.
	* DEFINITION = DECLARATION + VALUE
	* Be careful, some data types get default initialization values when declared.

# C++
### General rules
* 4 Space indentation. Spaces, no tabs.
* Both the function declaration and definition do not have a space between the name and open parentheses.
	* Example: `void Robot::RobotInit();` and
		```cpp
		void Robot::RobotInit() {
			cout << "Hello World!" << endl;
		}
		```
	* This is to distinguish between using a function from a function declaration/definition.
* Use the C++ standard "iostream" library for output.
    * `using namespace std;` is required.
    * Example: `cout << "stuff" << endl;`.
    * Do not use `cin`.
* 80 character line limit.
* Line up associated variables by equal sign when declaring.
	* See reference [1].

### Shuffleboard
Shuffleboard is the C++ and Java equivalent of LabView's Dashboard functionality, it sends data back to the Driver Station for programmers and drivers to make use of by visualizing it in various ways. Due to how Shuffleboard functions inside code compared to Dashboard, there are rules that must be followed in order to effectively use it.
* Layouts must be saved inside the project code folder in its root directory.
	* This is so that if we ever need to rollback changes, the layouts were the ones used during testing.
* All widgets must be defined in `Robot::RobotInit()`, any widgets that require interaction between the robot and the dashboard must have a member variable reserved in "Robot.h" and defined in `Robot::RobotInit()`.  Shuffleboard member variables should have the prefix `m_sbNameOfWidget`.
	* See reference [2].
* All widgets created must go to either the "Autonomous" tab or the "TeleOperated" tab.
	* This does not automatically add the widgets to the offline layout, but to a temporary, unorganized dashboard under the same name. New widgets should be added to their appropriate tabs.

### Reference
[1] Line up associated variables by equal sign:
```cpp
m_B_abcde      = false;
m_B_everything = 42;
m_B_short      = "Short!";
```
[2] Define and use Shuffleboard data.
```cpp
void  Robot::RobotInit() {
	/* Autonomous Dashboard Stuffs */
	frc::ShuffleboardTab& autoboard =  frc::Shuffleboard::GetTab("Autonomous");
	autoboard.Add("TestBool", false);
	autoboard.Add("TestString", "foo bar baz qux");
	m_sbFloater = autoboard.Add("Floater", 42.0).GetEntry(); // type = nt::NetworkTableEntry

	/* TeleOperated Dashboard Stuffs */
	frc::ShuffleboardTab& teleboard =  frc::Shuffleboard::GetTab("TeleOperated");
	m_sbWoot    = teleboard.Add("woot", 42).GetEntry();
	m_sbSintest = teleboard.Add("SinTest", 0.0).GetEntry();
}

float rads = 0;
void  Robot::TeleopPeriodic() {
	rads += 0.01;
	m_sbSintest.SetDouble(sin(rads));
}
```
