/**
 * @brief Defines the TranslatedString class.
 *
 * The TranslatedString class is a small utility or helper class that allows us
 * to parse and use JSON strings that may contain translations vor various
 * locales.
 *
 * @sa TranslatedString
 *
 * @author Dariusz Scharsig
 *
 * @date 22.09.2022
 */
#ifndef TRANSLATEDSTRING_H
#define TRANSLATEDSTRING_H

#include <QList>
#include <QLocale>
#include <QString>

#include "translation.h"

namespace Utils {
class TranslatedString
{
public:
    TranslatedString() = default;
    TranslatedString(const QString &id, const QList<Translation> &translations);
    ~TranslatedString() = default;

    bool isEmpty() const;

    QString id() const;
    QString translatedId() const;

    static TranslatedString fromJson(const QJsonValue &json);
    static QList<TranslatedString> fromJsonArray(const QJsonValue &json);

private:
    void translate(const QList<Translation> &translations);

private:
    struct TranslatedStringPrivate
    {
        QString id;
        QString translatedId;
    } d;
};
}

#endif // ! TRANSLATEDSTRING_H
