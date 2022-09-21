#ifndef CARBONPLUGINDATA_H
#define CARBONPLUGINDATA_H

#include <QJsonDocument>
#include <QLocale>

class CarbonPluginData
{
public:
    bool initFromJson(const QJsonObject &json);
    bool hasError() const;
    QString error() const;

    QString name() const;
    QString description() const;

    QStringList territoriesString() const;
    QList<QLocale::Country> territories() const;
    QStringList regions() const;

private:
    void setError(const QString &error);

private:
    class CarbonPluginDataPrivate
    {
    public:
        bool hasError = false;
        QString error;
        QString name;
        QString description;
        QList<QLocale::Country> territories;
        QStringList regions;
    } d;

};

#endif // CARBONPLUGINDATA_H
