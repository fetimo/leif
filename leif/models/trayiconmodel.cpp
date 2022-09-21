#include "models/trayiconmodel.h"

TrayIconModel::TrayIconModel(QObject *parent /* = nullptr */):
    QObject(parent)
{
    d.carbonUsageLevel = TrayIconModel::Medium;
    d.chargeForecast = TrayIconModel::ChargeWhenNeeded;
    d.sessionCarbon = 0;
    d.totalCarbon = 0;
    d.configured = false;
}

int TrayIconModel::sessionCarbon() const
{
    return d.sessionCarbon;
}

void TrayIconModel::setSessionCarbon(int newSessionCarbon)
{
    if (d.sessionCarbon == newSessionCarbon)
        return;
    d.sessionCarbon = newSessionCarbon;
    emit sessionCarbonChanged();
}

int TrayIconModel::totalCarbon() const
{
    return d.totalCarbon;
}

void TrayIconModel::setTotalCarbon(int newTotalCarbon)
{
    if (d.totalCarbon == newTotalCarbon)
        return;
    d.totalCarbon = newTotalCarbon;
    emit totalCarbonChanged();
}

TrayIconModel::CarbonUsageLevel TrayIconModel::carbonUsageLevel() const
{
    return d.carbonUsageLevel;
}

void TrayIconModel::setCarbonUsageLevel(CarbonUsageLevel newCarbonUsageLevel)
{
    if (d.carbonUsageLevel == newCarbonUsageLevel)
        return;
    d.carbonUsageLevel = newCarbonUsageLevel;
    emit carbonUsageLevelChanged();
}

TrayIconModel::ChargeForecast TrayIconModel::chargeForecast() const
{
    return d.chargeForecast;
}

void TrayIconModel::setChargeForecast(ChargeForecast newChargeForecast)
{
    if (d.chargeForecast == newChargeForecast)
        return;
    d.chargeForecast = newChargeForecast;
    emit chargeForecastChanged();
}

void TrayIconModel::resetStats()
{
    /// TODO
    qDebug("Reset the stats...");
}

void TrayIconModel::showDialog()
{
    d.qmlEngine.load("qml/main.qml");
}

bool TrayIconModel::configured() const
{
    return d.configured;
}

void TrayIconModel::setConfigured(bool newConfigured)
{
    if (d.configured == newConfigured)
        return;
    d.configured = newConfigured;
    emit configuredChanged();
}
