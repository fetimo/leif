/**
 * @brief Defines the CarbonPluginManager class.
 *
 * The CarbonPluginManager class allows us to get information on all available
 * plugins.
 *
 * @author Dariusz Scharsig
 *
 * @date 28.09.2022
 * @copyright Tim Stone
 */
#ifndef CARBONPLUGINMANAGER_H
#define CARBONPLUGINMANAGER_H

#include <interfaces/IDataProvider.h>

class CarbonPlugin;
class CarbonPluginManagerPrivate;

class CarbonPluginManager : public IDataProvider
{
public:
    static CarbonPluginManager *Instance();
    static void Destroy();

    bool hasPlugins() const;
    QList<QLocale::Country> territories() const;
    QStringList territoryNames() const;
    QStringList regionIds(const QLocale::Country territory) const;
    QString translatedRegion(const QLocale::Country country, const QString &regionId) const;

    bool loadPlugin(const QLocale::Country country);
    bool hasCurrentPlugin() const;
    CarbonPlugin * currentPlugin() const;
    virtual CarbonData carbonPerKiloWatt(const QLocale::Country country, const QString &region) override;

private:
    CarbonPluginManager();
    ~CarbonPluginManager();
private:

    Q_DISABLE_COPY_MOVE(CarbonPluginManager)
    CarbonPluginManagerPrivate *d;
};

#endif // CARBONPLUGINMANAGER_H
