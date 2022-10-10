#ifndef POWERINFO_H
#define POWERINFO_H

#include <QObject>
#include <interfaces/IPower.h>

class PowerInfoPrivate;

class PowerInfo : public QObject, public IPower
{
public:
    Q_OBJECT

public:
    enum State {Unknown, NoBattery, FullyCharged, Charging, Discharging};
    Q_ENUM(State)

    explicit PowerInfo(QObject *parent = nullptr);
    virtual ~PowerInfo();

    State state() const;
    virtual float powerDrawInWatts() override;

private slots:
    void checkLevels();

private:
    void updateState();
    float avarageDischargeConsumption();
    float chargeConsumption();

    static float noBatteryPowerEstimate();

private:
    Q_DISABLE_COPY_MOVE(PowerInfo)
    class PowerInfoPrivate *d;
};

#endif // POWERINFO_H
