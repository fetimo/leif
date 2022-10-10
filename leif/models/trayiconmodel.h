/**
 * @brief Defines the TrayIconModel class.
 *
 * The TrayIconModel class drives the data display of the TrayIcon instance. It
 * contains information about the current power usage, co2 footprint and more.
 *
 * @author Dariusz Scharsig
 *
 * @date 18.09.2022
 */
#ifndef TRAYICONMODEL_H
#define TRAYICONMODEL_H

#include <QObject>
#include <QQmlApplicationEngine>

class TrayIconModelPrivate;

class TrayIconModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int sessionCarbon READ sessionCarbon WRITE setSessionCarbon NOTIFY sessionCarbonChanged)
    Q_PROPERTY(int totalCarbon READ totalCarbon NOTIFY totalCarbonChanged)
    Q_PROPERTY(CarbonUsageLevel carbonUsageLevel READ carbonUsageLevel WRITE setCarbonUsageLevel NOTIFY carbonUsageLevelChanged)
    Q_PROPERTY(ChargeForecast chargeForecast READ chargeForecast WRITE setChargeForecast NOTIFY chargeForecastChanged)
    Q_PROPERTY(bool configured READ configured NOTIFY configuredChanged)

public:
    enum CarbonUsageLevel { VeryLow, Low, Medium, High, VeryHigh };
    Q_ENUM(CarbonUsageLevel)

    enum ChargeForecast { ChargeNow, ChargeIn30, ChargeIn60, ChargeWhenNeeded };
    Q_ENUM(ChargeForecast)

    explicit TrayIconModel(QObject *parent = nullptr);
    virtual ~TrayIconModel();

    int sessionCarbon() const;
    void setSessionCarbon(int newSessionCarbon);

    int totalCarbon() const;

    CarbonUsageLevel carbonUsageLevel() const;
    void setCarbonUsageLevel(CarbonUsageLevel newCarbonUsageLevel);

    ChargeForecast chargeForecast() const;
    void setChargeForecast(ChargeForecast newChargeForecast);

    bool configured() const;

public Q_SLOTS:
    void resetStats();
    void showDialog();

signals:
    void sessionCarbonChanged();
    void totalCarbonChanged();
    void carbonUsageLevelChanged();
    void chargeForecastChanged();
    void configuredChanged();

private:
    TrayIconModelPrivate *d;
};

#endif // TRAYICONMODEL_H
