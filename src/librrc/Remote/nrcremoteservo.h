#pragma once

#include "nrcremoteactuatorbase.h"

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>
#include <librrc/Packets/servocalibrationpacket.h>

#include <math.h>
#include <Preferences.h>

typedef uint16_t counts_t;

class NRCRemoteServo : public NRCRemoteActuatorBase<NRCRemoteServo>
{

public:

    /**
     * @brief Using this constructor initialises the object using counts, i.e. the value written 
     * to the PWM generator and not the timing values like what would be given on a servo datasheet.
     * 
     * @param gpio GPIO pin number the servo control wire is connected to
     * @param channel PWM generator channel
     * @param networkmanager RNP network manager object
     * @param default_angle Angle the servo will go to when setup is called
     * @param min_angle Minimum angle the servo can reach. Note that this is not the minimum angle 
     * the servo will be restricted to, but the minimum angle the servo si capable of going to.
     * @param max_angle Maximum angle the servo can reach. Note that this is not the maxmium angle 
     * the servo will be restricted to, but the maximum angle the servo si capable of going to.
     * @param angl_lim_min Minimum angle the servo will be allowed to go to
     * @param angl_lim_min Maximum angle the servo will be allowed to go to. Note that this should be within
     * the range of the min and max angle parameters.
     * @param min_counts PWM counts corresponding to the minimum angle.
     * @param max_counts PWM counts corresponding to the maximum angle.
     */
    
    NRCRemoteServo(uint8_t gpio,
                    uint8_t channel,
                    RnpNetworkManager &networkmanager,
                    uint16_t default_angle = 0,
                    uint16_t min_angle = 0,
                    uint16_t max_angle = 180,
                    uint16_t angl_lim_min = 0,
                    uint16_t angl_lim_max = 360,
                    counts_t min_counts = counts(500),
                    counts_t max_counts = counts(2500)
                    ): 
    NRCRemoteActuatorBase(networkmanager),
        _gpio(gpio),
        _channel(channel),
        _default_angle(default_angle),
        _min_angle(min_angle),
        _max_angle(max_angle),
        _angl_lim_min(angl_lim_min),
        _angl_lim_max(angl_lim_max),
        _min_counts((uint16_t)min_counts),
        _max_counts((uint16_t)max_counts)
        {};

    void setup();

    /**
     * @brief Temporary implementation of reset. Drives servo to min angle position. This needs to be updated
     * to be configurable.
     * 
     */
    void reset();

    void setAngleLims(uint16_t minangle, uint16_t maxangle){
        _angl_lim_min = minangle;
        _angl_lim_max = maxangle;
    };

    void setHome(uint16_t homeangle){
        _default_angle = homeangle;
    };

    void goto_Angle(uint16_t angle);
    void goto_AngleHighRes(float angle);

    uint16_t getHome(){return _default_angle;};
    uint16_t getMin(){return _angl_lim_min;};
    uint16_t getMax(){return _angl_lim_max;};
    uint16_t getAngle(){return _value;};
   
protected:

    friend class NRCRemoteActuatorBase;
    friend class NRCRemoteBase;

    const uint8_t _gpio;
    const uint8_t _channel;
    uint16_t _default_angle;
    const uint16_t _min_angle;
    const uint16_t _max_angle;

    uint16_t _angl_lim_min;
    uint16_t _angl_lim_max;

    const uint16_t _min_counts;
    const uint16_t _max_counts;

    void loadCalibration();
    
    void execute_impl(packetptr_t packetptr);
    void calibrate_impl(packetptr_t packetptr);

    uint16_t angletocounts(uint16_t angle);
    float angletocountsHighRes(float angle);

    static constexpr int timer_width = 14;
    static constexpr int freq = 50;
    
    static constexpr int counts(int usec){
       return (int)(float(usec) / (float(1000000/freq)/float(std::pow(2,timer_width))));
    }
};