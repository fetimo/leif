#include <QJsonObject>
#include <QJsonArray>

#include "carbonplugindata.h"

Utils::CarbonPluginData::CarbonPluginData(const QString &name,
                                          const QString &description,
                                          const QList<Territory> &territories)
{
    d.name = name;
    d.description = description;
    d.territories = territories;
}

bool Utils::CarbonPluginData::isValid()
{
    return(!name().isEmpty() && !territoryList().isEmpty());
}

QString Utils::CarbonPluginData::name() const
{
    return d.name;
}

QString Utils::CarbonPluginData::description() const
{
    return d.description;
}

QList<Utils::Territory> Utils::CarbonPluginData::territoryList() const
{
    return d.territories;
}

QList<QLocale::Country> Utils::CarbonPluginData::territories() const
{
    QList<QLocale::Country> territories;
    for(const Territory &territory : const_cast<const QList<Territory> &>(d.territories))
    {
        if(territory.isValid())
        {
            territories << territory.country();
        }
    }

    return territories;
}

QStringList Utils::CarbonPluginData::territoryNames() const
{
    QStringList territoryNames;
    for(const Territory &territory : const_cast<const QList<Territory> &>(d.territories))
    {
        if(territory.isValid())
        {
            territoryNames << QLocale::countryToString(territory.country());
        }
    }

    return territoryNames;
}

QList<Utils::TranslatedString> Utils::CarbonPluginData::regionList(const QLocale::Country country) const
{
    QList<TranslatedString> regions;
    for(const Territory &territory : const_cast<const QList<Territory> &>(d.territories))
    {
        if(territory.isValid() && territory.country() == country)
        {
            regions = territory.regions();
            break;
        }
    }

    return regions;
}

QStringList Utils::CarbonPluginData::regionIds(const QLocale::Country country) const
{
    const QList<TranslatedString> regions = regionList(country);
    QStringList regionIdList;

    for(const TranslatedString &region : regions)
    {
        if(!region.isEmpty())
        {
            regionIdList << region.id();
        }
    }

    return regionIdList;
}

QString Utils::CarbonPluginData::translatedRegion(const QLocale::Country country, const QString &regionId) const
{
    const QList<TranslatedString> regions = regionList(country);
    QString translated;

    for(const TranslatedString &region : regions)
    {
        if(!region.isEmpty() && region.id() == regionId)
        {
            translated = region.translatedId();
            break;
        }
    }

    return translated;
}

Utils::CarbonPluginData Utils::CarbonPluginData::fromJson(const QJsonValue &json)
{
    if(json.isNull() || !json.isObject())
    {
        return CarbonPluginData();
    }

    QJsonObject jsonObject = json.toObject();

    QString name = jsonObject.value(QStringLiteral("name")).toString();
    QString description = jsonObject.value(QStringLiteral("description")).toString();
    QList<Territory> territories = Territory::fromJsonArray(jsonObject.value(QStringLiteral("territories")));

    return CarbonPluginData(name, description, territories);
}
