/**
 * @brief Implements the Translation class.
 *
 * @sa Translation
 *
 * @author Dariusz Scharsig
 *
 * @date 22.09.2022
 */
#include <QJsonArray>
#include <QJsonObject>

#include "translation.h"

Utils::Translation::Translation()
{}

Utils::Translation::Translation(const QString &string, const QString &localeName):
    Translation(string, QLocale(localeName))
{}

Utils::Translation::Translation(const QString &string, const QLocale &locale)
{
    d.string = string;
    d.locale = locale;
}

bool Utils::Translation::isEmpty() const
{
    return string().isEmpty();
}

QString Utils::Translation::string() const
{
    return d.string;
}

void Utils::Translation::setString(const QString &string)
{
    d.string = string;
}

QLocale Utils::Translation::locale() const
{
    return d.locale;
}

void Utils::Translation::setLocale(const QLocale &locale)
{
    d.locale = locale;
}

Utils::Translation Utils::Translation::fromJson(const QJsonValue &json)
{
    if(json.isNull() || !json.isObject())
    {
        return Translation();
    }

    QString locale = json.toObject().value(QStringLiteral("locale")).toString();
    QString string = json.toObject().value(QStringLiteral("string")).toString();

    return Translation(string, locale);
}

QList<Utils::Translation> Utils::Translation::fromJsonArray(const QJsonValue &json)
{
    if(json.isNull() || !json.isArray())
    {
        return QList<Translation>();
    }

    const QJsonArray jsonArray = json.toArray();
    QList<Translation> translations;

    for(const QJsonValue &value : jsonArray)
    {
        Translation translation = Translation::fromJson(value);
        if(!translation.isEmpty())
        {
            translations << translation;
        }
    }

    return translations;
}
