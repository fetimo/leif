#include <QTimer>

#include <Windows.h>
#include <ntstatus.h>
#include <powerbase.h>
#include <devguid.h>
#include <batclass.h>

#include "powerinfo.h"

#define BATTERY_UNKNOWN_RATE    0x80000000

class PowerInfoPrivate
{
    PowerInfoPrivate();
    ~PowerInfoPrivate() = default;

    static bool hasBattery(PSYSTEM_BATTERY_STATE pBatteryState);
    static bool batteryCharging(PSYSTEM_BATTERY_STATE pBatteryState);
    static bool batteryDischarging(PSYSTEM_BATTERY_STATE pBatteryState);
    static bool batteryFullyCharged(PSYSTEM_BATTERY_STATE pBatteryState);

    // DATA
    int checkIntervalInMinutes;
    int lastCapacity;
    int currentCapacity;
    int currentChargeRate;
    int currentDischargeRate;
    int avarageDischargeRate;
    PowerInfo::State state;


    friend class PowerInfo;
};

PowerInfoPrivate::PowerInfoPrivate():
    checkIntervalInMinutes {5},
    lastCapacity {0},
    currentCapacity {0},
    currentChargeRate {0},
    currentDischargeRate {0},
    avarageDischargeRate {0},
    state {PowerInfo::Unknown}
{}

bool PowerInfoPrivate::hasBattery(PSYSTEM_BATTERY_STATE pBatteryState)
{
    if(pBatteryState == nullptr)
    {
        return false;
    }

    return pBatteryState->BatteryPresent;
}

bool PowerInfoPrivate::batteryCharging(PSYSTEM_BATTERY_STATE pBatteryState)
{
    if(pBatteryState == nullptr)
    {
        return false;
    }

    return pBatteryState->Charging;
}

bool PowerInfoPrivate::batteryDischarging(PSYSTEM_BATTERY_STATE pBatteryState)
{
    if(pBatteryState == nullptr)
    {
        return false;
    }

    return pBatteryState->Discharging;
}

bool PowerInfoPrivate::batteryFullyCharged(PSYSTEM_BATTERY_STATE pBatteryState)
{
    if(pBatteryState == nullptr)
    {
        return false;
    }

    if(!pBatteryState->AcOnLine)
    {
        return false;
    }

    if(pBatteryState->Rate != BATTERY_UNKNOWN_RATE && pBatteryState->Rate == 0)
    {
        return true;
    }

    return pBatteryState->Discharging == false;
}


PowerInfo::PowerInfo(QObject *parent /* = nullptr */):
    QObject(parent),
    d {new PowerInfoPrivate}
{
    QTimer *checkTimer = new QTimer(this);
    checkTimer->setInterval(d->checkIntervalInMinutes * 1000);
    checkTimer->setSingleShot(false);
    connect(checkTimer, &QTimer::timeout, this, &PowerInfo::checkLevels);
}

PowerInfo::~PowerInfo()
{
    delete d;
    d = nullptr;
}

PowerInfo::State PowerInfo::state() const
{
    Q_ASSERT(d != nullptr);

    return d->state;
}

float PowerInfo::powerDrawInWatts()
{
    updateState();

    float consumption = 0;

    switch(state())
    {
    case(PowerInfo::Unknown):
        // Shouldn't happen. Some issue with access
        break;

    case(PowerInfo::NoBattery):
        // We can't get any information. There is no battery in the system.
        consumption = PowerInfo::noBatteryPowerEstimate();
        break;

    case(PowerInfo::FullyCharged):
        // There is no live data. We need to return the last discharge value
        // as it is the most accurate power consumption value we have.
        consumption = avarageDischargeConsumption();
        break;

    case(PowerInfo::Charging):
        // We are charging. We will return the current charge rate
        consumption = chargeConsumption();
        break;

    case(PowerInfo::Discharging):
        // Since we are discharging, we are not using any power from the grid.
        break;
    }

    return consumption;
}

void PowerInfo::checkLevels()
{
    Q_ASSERT(d != nullptr);

    updateState();

    if(state() == PowerInfo::Discharging)
    {
        if(d->currentDischargeRate == 0)
        {
            if(d->lastCapacity > 0)
            {
                d->avarageDischargeRate = d->lastCapacity - d->currentCapacity;
            }

            d->lastCapacity = d->currentCapacity;
        }
    }
}

/**
 * @brief Checks the current SYSTEM_BATTERY_STATE.
 */
void PowerInfo::updateState()
{
    Q_ASSERT(d != nullptr);

    SYSTEM_BATTERY_STATE batteryState = {0};
    NTSTATUS result = CallNtPowerInformation(SystemBatteryState, NULL, 0, &batteryState, sizeof(SYSTEM_BATTERY_STATE));

    d->state = PowerInfo::Unknown;

    if(result != STATUS_SUCCESS)
    {
        return;
    }

    if(!PowerInfoPrivate::hasBattery(&batteryState))
    {
        d->state = PowerInfo::NoBattery;
        return;
    }

    if(PowerInfoPrivate::batteryFullyCharged(&batteryState))
    {
        d->state = PowerInfo::FullyCharged;
        return;
    }

    if(PowerInfoPrivate::batteryCharging(&batteryState))
    {
        d->state = PowerInfo::Charging;
        d->currentChargeRate = batteryState.Rate;
        return;
    }

    if(PowerInfoPrivate::batteryDischarging(&batteryState))
    {
        d->state = PowerInfo::Discharging;
        if(batteryState.Rate != BATTERY_UNKNOWN_RATE)
        {
            d->currentDischargeRate = batteryState.Rate;
        }
        else
        {
            d->currentCapacity = batteryState.RemainingCapacity;
        }

        return;
    }
}

float PowerInfo::avarageDischargeConsumption()
{
    Q_ASSERT(d != nullptr);

    if(d->currentDischargeRate > 0 )
    {
        return d->currentDischargeRate;
    }

    return d->avarageDischargeRate;
}

float PowerInfo::chargeConsumption()
{
    Q_ASSERT(d != nullptr);

    return d->currentChargeRate / 1000;
}


float PowerInfo::noBatteryPowerEstimate()
{
    return 340;
}
