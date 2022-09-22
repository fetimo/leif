#ifndef CARBONPLUGINDATA_H
#define CARBONPLUGINDATA_H

#include <QJsonValue>
#include <QLocale>

#include "territory.h"
#include "translatedstring.h"

namespace Utils {
class CarbonPluginData
{
public:
    CarbonPluginData() = default;
    CarbonPluginData(const QString &name, const QString &description, const QList<Territory> &territories);
    ~CarbonPluginData() = default;

    bool isValid();

    QString name() const;
    QString description() const;

    QList<Territory> territoryList() const;
    QList<QLocale::Country> territories() const;
    QStringList territoryNames() const;
    QList<TranslatedString> regionList(const QLocale::Country country) const;
    QStringList regionIds(const QLocale::Country country) const;
    QString translatedRegion(const QLocale::Country country, const QString &regionId) const;

    static CarbonPluginData fromJson(const QJsonValue &json);

private:
    class CarbonPluginDataPrivate
    {
    public:
        QString name;
        QString description;
        QList<Territory> territories;
    } d;
};
}

#endif // CARBONPLUGINDATA_H
