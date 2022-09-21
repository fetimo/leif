#include <QJsonObject>

#include "carbonplugindata.h"

bool CarbonPluginData::initFromJson(const QJsonObject &json)
{
    if(json.isEmpty())
    {
        setError("The JSON object is empty.");
        return false;
    }

    if(!json.contains(QStringLiteral("MetaData")))
    {
        setError("Plugin meta data not found.");
        return false;
    }

    QJsonObject data = json.value(QStringLiteral("MetaData")).toObject();
    if(data.isEmpty())
    {
        setError("Invalid Plugin meta data.");
        return false;
    }

    d.name = data.value(QStringLiteral("name")).toString();
    d.description = data.value(QStringLiteral("description")).toString();


}

bool CarbonPluginData::hasError() const
{
    return d.hasError;
}

QString CarbonPluginData::error() const
{
    return d.error;
}

QString CarbonPluginData::name() const
{
    return d.name;
}

QString CarbonPluginData::description() const
{
    return d.description;
}

QStringList CarbonPluginData::territoriesString() const
{
    QStringList stringList;
    for(const QLocale::Country territory : const_cast<const QList<QLocale::Country> &>(d.territories))
    {
        stringList << QLocale::countryToString(territory);
    }

    return stringList;
}

QList<QLocale::Country> CarbonPluginData::territories() const
{
    return d.territories;
}

QStringList CarbonPluginData::regions() const
{
    return d.regions;
}

void CarbonPluginData::setError(const QString &error)
{
    d.error = error;
    d.hasError = true;
}
