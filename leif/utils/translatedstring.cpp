/**
 * @brief Implements the TranslatedString utility class.
 *
 * @sa TranslatedString
 *
 * @author Dariusz Scharsig
 *
 * @date 22.09.2022
 */

#include <QJsonArray>
#include <QJsonObject>

#include "translatedstring.h"

Utils::TranslatedString::TranslatedString(const QString &id, const QList<Translation> &translations)
{
    d.id = id;
    translate(translations);
}

bool Utils::TranslatedString::isEmpty() const
{
    return id().isEmpty();
}

QString Utils::TranslatedString::id() const
{
    return d.id;
}

QString Utils::TranslatedString::translatedId() const
{
    if(isEmpty())
    {
        return QString();
    }

    return d.translatedId;
}

Utils::TranslatedString Utils::TranslatedString::fromJson(const QJsonValue &json)
{
    if(json.isNull() || !json.isObject())
    {
        return TranslatedString();
    }

    QJsonObject jsonObject = json.toObject();

    QString id = jsonObject.value(QStringLiteral("id")).toString();

    QList<Translation> translations;

    if(jsonObject.contains(QStringLiteral("translations")))
    {
        translations = Translation::fromJsonArray(jsonObject.value(QStringLiteral("translations")));
    }

    return TranslatedString(id, translations);
}

void Utils::TranslatedString::translate(const QList<Translation> &translations)
{
    for(const Translation &translation : translations)
    {
        if(translation.isEmpty())
        {
            continue;
        }

        if(translation.locale() == QLocale())
        {
            d.translatedId = translation.string();
            break;
        }
    }
}

QList<Utils::TranslatedString> Utils::TranslatedString::fromJsonArray(const QJsonValue &json)
{
    if(json.isNull() || !json.isArray())
    {
        return QList<TranslatedString>();
    }

    QList<TranslatedString> translatedStrings;
    const QJsonArray &jsonArray = json.toArray();

    for(const QJsonValue &value : jsonArray)
    {
        TranslatedString translatedString = TranslatedString::fromJson(value);
        if(!translatedString.isEmpty())
        {
            translatedStrings << translatedString;
        }
    }

    return translatedStrings;
}
