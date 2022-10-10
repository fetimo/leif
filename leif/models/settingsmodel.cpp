#include "leifsettings.h"
#include "settingsmodel.h"

SettingsModel::SettingsModel(QObject *parent)
    : QObject{parent}
{}

QLocale::Country SettingsModel::country() const
{
    LeifSettings *settings = LeifSettings::Instance();
    if(settings == nullptr)
    {
        return QLocale::AnyCountry;
    }

    return settings->country();
}

void SettingsModel::setCountry(const QLocale::Country &newCountry)
{
    LeifSettings *settings = LeifSettings::Instance();
    if(settings == nullptr)
    {
        return;
    }

    if(settings->country() != newCountry)
    {
        settings->saveCountry(newCountry);
        emit countryChanged();
    }
}

QString SettingsModel::regionId() const
{
    LeifSettings *settings = LeifSettings::Instance();
    if(settings == nullptr)
    {
        return QString();
    }

    return settings->regionId();
}

void SettingsModel::setRegionId(const QString &newRegionId)
{
    LeifSettings *settings = LeifSettings::Instance();
    if(settings == nullptr)
    {
        return;
    }

    if(settings->regionId() != newRegionId)
    {
        settings->saveRegionId(newRegionId);
        emit regionIdChanged();
    }
}

int SettingsModel::lifetimeCarbon() const
{
    LeifSettings *settings = LeifSettings::Instance();
    if(settings == nullptr)
    {
        return 0;
    }

    return settings->lifeTimeCarbon();
}

void SettingsModel::setLifetimeCarbon(int newLifetimeCarbon)
{
    LeifSettings *settings = LeifSettings::Instance();
    if(settings == nullptr)
    {
        return;
    }

    if(settings->lifeTimeCarbon() != newLifetimeCarbon)
    {
        settings->saveLifetimeCarbon(newLifetimeCarbon);
        emit lifetimeCarbonChanged();
    }
}
