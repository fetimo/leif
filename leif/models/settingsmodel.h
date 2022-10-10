#ifndef SETTINGSMODEL_H
#define SETTINGSMODEL_H

#include <QObject>
#include <QLocale>
#include <QQmlEngine>

class SettingsModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QLocale::Country country READ country WRITE setCountry NOTIFY countryChanged)
    Q_PROPERTY(QString regionId READ regionId WRITE setRegionId NOTIFY regionIdChanged)
    Q_PROPERTY(int lifetimeCarbon READ lifetimeCarbon WRITE setLifetimeCarbon NOTIFY lifetimeCarbonChanged)
    QML_ELEMENT

public:
    explicit SettingsModel(QObject *parent = nullptr);

    QLocale::Country country() const;
    void setCountry(const QLocale::Country &newCountry);

    QString regionId() const;
    void setRegionId(const QString &newRegionId);

    int lifetimeCarbon() const;
    void setLifetimeCarbon(int newLifetimeCarbon);

signals:
    void countryChanged();
    void regionIdChanged();
    void lifetimeCarbonChanged();
};

#endif // SETTINGSMODEL_H
