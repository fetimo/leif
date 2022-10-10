#ifndef LEIFSETTINGS_H
#define LEIFSETTINGS_H

#include <QLocale>
#include <QObject>

class LeifSettingsPrivate;

class LeifSettings : public QObject
{
    Q_OBJECT
    explicit LeifSettings(QObject *parent = nullptr);
    virtual ~LeifSettings();

public:
    static LeifSettings * Instance();
    static void Destroy();

    void saveLocation(const QLocale::Country &country, const QString &regionId);
    void saveCountry(const QLocale::Country &country);
    void saveRegionId(const QString &regionId);
    QLocale::Country country() const;
    QString regionId() const;

    void saveLifetimeCarbon(int lifeTime);
    int lifeTimeCarbon() const;
    void clearLifeTimeCarbon();

signals:
    void locationChanged(const QLocale::Country country, const QString &regionId);
    void lifeTimeCarbonChanged(int lifeTime);

private:
    Q_DISABLE_COPY_MOVE(LeifSettings);
    LeifSettingsPrivate *d;
};

#endif // LEIFSETTINGS_H
