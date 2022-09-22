/**
 * @brief Implements the Territory class.
 *
 * @sa Territory
 *
 * @author Dariusz Scharsig
 *
 * @date 22.09.2022
 */
#include <QJsonArray>
#include <QJsonObject>

#include "territory.h"

Utils::Territory::Territory():
    Utils::Territory(QLocale::AnyCountry, QString(), QList<TranslatedString>())
{}

Utils::Territory::Territory(const QLocale::Country country, const QString &description, const QList<TranslatedString> &regions)
{
    d.country = country;
    d.description = description;
    d.regions = regions;
}

bool Utils::Territory::isValid() const
{
    return country() != QLocale::AnyCountry;
}

QLocale::Country Utils::Territory::country() const
{
    return d.country;
}

QString Utils::Territory::description() const
{
    return d.description;
}

bool Utils::Territory::hasRegions() const
{
    return d.regions.count() > 0;
}

QList<Utils::TranslatedString> Utils::Territory::regions() const
{
    return d.regions;
}

Utils::Territory Utils::Territory::fromJson(const QJsonValue &json)
{
    if(json.isNull() || !json.isObject())
    {
        return Territory();
    }

    QJsonObject territoryObject = json.toObject();

    if(!territoryObject.contains(QStringLiteral("territory")))
    {
        return Territory();
    }

    QLocale::Country country = static_cast<QLocale::Country>(territoryObject.value(QStringLiteral("territory")).toInt());
    QString description = territoryObject.value(QStringLiteral("description")).toString();
    QList<TranslatedString> regions = TranslatedString::fromJsonArray(territoryObject.value(QStringLiteral("regions")));

    return Territory(country, description, regions);
}

QList<Utils::Territory> Utils::Territory::fromJsonArray(const QJsonValue &json)
{
    if(json.isNull() || !json.isArray())
    {
        return QList<Territory>();
    }

    QList<Territory> territories;
    const QJsonArray jsonArray = json.toArray();

    for(const QJsonValue &value : jsonArray)
    {
        Territory territory = Territory::fromJson(value);
        if(territory.isValid())
        {
            territories << territory;
        }
    }

    return territories;
}
