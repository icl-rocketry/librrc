#include "nrcremoteservo.h" 
#include <librrc/HAL/localpwm.h>

class NRCRemoteProp : public NRCRemoteServo<LocalPWM>
{
public:
    NRCRemoteProp(uint8_t pin,
                uint8_t channel, // current freq and res is 50 and 14
                RnpNetworkManager &networkmanager,
                std::string name = "",
                uint32_t speedResolution = 100,
                uint32_t minWidth = 1100,
                uint32_t maxWidth = 1940) : 
                NRCRemoteServo<LocalPWM>( 
                                        LocalPWM(pin, channel),
                                        networkmanager, 
                                        name, 
                                        0, // defaultAngle == startingSpeed
                                        0, // minAngle == minSpeed (off)
                                        speedResolution, // maxAngle == speedResolution (full throttle)
                                        minWidth, 
                                        maxWidth, 
                                        0, // minAngleLimit
                                        speedResolution // maxAngleLimit
                                        )
    {
    }

    void setup() 
    {
        NRCRemoteServo::setup();
    }

    void killMotor()
    {
        NRCRemoteServo::execute_base(0);
    }

protected:
    // Override the execute_base method if you need custom angle to pulse width mapping
    void execute_base(int32_t arg) 
    {
        // Custom execution logic
        // For example, modify the range mapping or angle limits

        // Or, call the base class's execute_base method to reuse existing logic
        NRCRemoteServo::execute_base(arg);
    }
};
