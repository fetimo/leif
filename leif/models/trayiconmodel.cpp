#include "models/trayiconmodel.h"
#include "models/settingsmodel.h"

class TrayIconModelPrivate
{
    TrayIconModelPrivate();
    ~TrayIconModelPrivate();

    int sessionCarbon;
    TrayIconModel::CarbonUsageLevel carbonUsageLevel;
    TrayIconModel::ChargeForecast chargeForecast;
    QQmlApplicationEngine *qmlEngine;
    SettingsModel *settingsModel;

    friend class TrayIconModel;
};

TrayIconModelPrivate::TrayIconModelPrivate():
    sessionCarbon{0},
    carbonUsageLevel{TrayIconModel::Medium},
    chargeForecast{TrayIconModel::ChargeWhenNeeded},
    qmlEngine{new QQmlApplicationEngine},
    settingsModel{new SettingsModel}
{}

TrayIconModelPrivate::~TrayIconModelPrivate()
{
    delete qmlEngine;
    qmlEngine = nullptr;

    delete settingsModel;
    settingsModel = nullptr;
}

TrayIconModel::TrayIconModel(QObject *parent /* = nullptr */):
    QObject(parent),
    d{new TrayIconModelPrivate}
{
    connect(d->settingsModel, &SettingsModel::lifetimeCarbonChanged, this, &TrayIconModel::totalCarbonChanged);
    connect(d->settingsModel, &SettingsModel::countryChanged, this, &TrayIconModel::configuredChanged);
}

TrayIconModel::~TrayIconModel()
{
    delete d;
    d = nullptr;
}

int TrayIconModel::sessionCarbon() const
{
    Q_ASSERT(d != nullptr);
    return d->sessionCarbon;
}

void TrayIconModel::setSessionCarbon(int newSessionCarbon)
{
    Q_ASSERT(d != nullptr);

    if (d->sessionCarbon == newSessionCarbon)
        return;

    d->sessionCarbon = newSessionCarbon;
    emit sessionCarbonChanged();
}

int TrayIconModel::totalCarbon() const
{
    Q_ASSERT(d != nullptr);

    if(d->settingsModel == nullptr)
    {
        return 0;
    }

    return d->settingsModel->lifetimeCarbon();
}

TrayIconModel::CarbonUsageLevel TrayIconModel::carbonUsageLevel() const
{
    Q_ASSERT(d != nullptr);

    return d->carbonUsageLevel;
}

void TrayIconModel::setCarbonUsageLevel(CarbonUsageLevel newCarbonUsageLevel)
{
    Q_ASSERT(d != nullptr);

    if (d->carbonUsageLevel == newCarbonUsageLevel)
        return;
    d->carbonUsageLevel = newCarbonUsageLevel;
    emit carbonUsageLevelChanged();
}

TrayIconModel::ChargeForecast TrayIconModel::chargeForecast() const
{
    Q_ASSERT(d != nullptr);

    return d->chargeForecast;
}

void TrayIconModel::setChargeForecast(ChargeForecast newChargeForecast)
{
    Q_ASSERT(d != nullptr);

    if (d->chargeForecast == newChargeForecast)
        return;
    d->chargeForecast = newChargeForecast;
    emit chargeForecastChanged();
}

void TrayIconModel::resetStats()
{
    Q_ASSERT(d != nullptr);

    if(d->settingsModel == nullptr)
    {
        return;
    }

    d->settingsModel->setLifetimeCarbon(0);
    setSessionCarbon(0);
}

void TrayIconModel::showDialog()
{
    Q_ASSERT(d != nullptr);

    if(d->qmlEngine != nullptr)
    {
        d->qmlEngine->load("qml/main.qml");
    }
}

bool TrayIconModel::configured() const
{
    Q_ASSERT(d != nullptr);

    if(d->settingsModel == nullptr)
    {
        return false;
    }

    return d->settingsModel->country() != QLocale::AnyCountry;
}
