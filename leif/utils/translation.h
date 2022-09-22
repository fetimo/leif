/**
 * @brief Defines the Translation class.
 *
 * The Translation utility class helps us keep track of a dynamic translation
 * that are used in JSON files for example. It is basically a QString and a
 * locale.
 *
 * @sa Translation
 *
 * @author Dariusz Scharsig
 *
 * @date 22.09.2022
 */
#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QLocale>

namespace Utils {

class Translation
{
public:
    Translation();
    Translation(const QString &string, const QString &localeName);
    Translation(const QString &string, const QLocale &locale);
    ~Translation() = default;

    bool isEmpty() const;

    QString string() const;
    void setString(const QString &string);

    QLocale locale() const;
    void setLocale(const QLocale &locale);

    static Translation fromJson(const QJsonValue &json);
    static QList<Utils::Translation> fromJsonArray(const QJsonValue &json);

private:
    struct TranslationPrivate
    {
        QString string;
        QLocale locale;
    } d;
};
}

#endif // TRANSLATION_H
