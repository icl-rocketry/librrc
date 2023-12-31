#pragma once

#include "nrcremoteactuatorbase.h"

#include <librnp/rnp_networkmanager.h>
#include <librnp/rnp_packet.h>
// #include <esp_task.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <functional>
#include <atomic>
#include <array>

class NRCRemotePyro : public NRCRemoteActuatorBase<NRCRemotePyro>
{

public:
    NRCRemotePyro(uint8_t firePin,uint8_t contPin,RnpNetworkManager &networkmanager);
    void setup();
    ~NRCRemotePyro();

protected:
    friend class NRCRemoteActuatorBase;
    friend class NRCRemoteBase;
    const uint8_t _firePin;
    const uint8_t _contPin;
    bool _contCheckOverride;
    // std::atomic<bool> _taskDeleted = false;

    //Task Control stuff
    /**
     * @brief Task Handle
     * 
     */
    TaskHandle_t offTaskHandle = nullptr;

    /**
     * @brief Task control block
     * 
     */
    StaticTask_t offTaskTCB;

    /**
     * @brief Task stack size
     * 
     */
    static constexpr int offTaskStackSize = 100;
    /**
     * @brief Task stack
     * 
     */
    std::array<StackType_t,offTaskStackSize> taskStack;
    /**
     * @brief Atomic off time for task 
     * 
     */
    std::atomic<uint32_t> offTime;
    std::atomic<bool> offTimeUpdated;

    


    void execute_impl(packetptr_t packetptr);
    void arm_impl(packetptr_t packetptr);
    void getstate_impl(packetptr_t packetptr);

    void updateContinuity();

    bool spawnOffTask();

};