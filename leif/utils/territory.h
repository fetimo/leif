/**
 * @brief Defines the Territory class.
 *
 * The Territory utility class defines a particular territory, that can consist
 * of several regions.
 *
 * @sa Territory
 *
 * @author Dariusz Scharsig
 *
 * @date 22.09.2022
 */
#ifndef TERRITORY_H
#define TERRITORY_H

#include <QJsonValue>
#include <QString>
#include "translatedstring.h"

namespace Utils {

class Territory
{
public:
    Territory();
    Territory(const QLocale::Country country, const QString &description, const QList<TranslatedString> &regions);
    ~Territory() = default;

    bool isValid() const;

    QLocale::Country country() const;
    QString description() const;

    bool hasRegions() const;
    QList<TranslatedString> regions() const;

    static Territory fromJson(const QJsonValue &json);
    static QList<Territory> fromJsonArray(const QJsonValue &json);

private:
    struct TerritoryPrivate
    {
        QLocale::Country country;
        QString description;
        QList<TranslatedString> regions;
    } d;
};

}

#endif // TERRITORY_H
