/**
 * @brief Implements the CarbonPluginManager class.
 *
 * @author Dariusz Scharsig
 *
 * @date 28.09.2022
 * @copyright Tim Stone
 */
#include "carbonplugin.h"
#include "carbonpluginmanager.h"

class CarbonPluginManagerPrivate
{
    CarbonPluginManagerPrivate();
    ~CarbonPluginManagerPrivate();
    void initPluginMap();
    CarbonPlugin *pluginForTerritory(const QLocale::Country territory) const;

    // DATA
    CarbonPlugin *currentPlugin;
    QList<CarbonPlugin*> plugins;
    QHash<QLocale::Country, int> pluginMap;
    static CarbonPluginManager *Instance;

    friend class CarbonPluginManager;
};


CarbonPluginManagerPrivate::CarbonPluginManagerPrivate():
    currentPlugin{nullptr}
{}

CarbonPluginManagerPrivate::~CarbonPluginManagerPrivate()
{
    currentPlugin = nullptr;

    qDeleteAll(plugins);
    plugins.clear();
    pluginMap.clear();
}

void CarbonPluginManagerPrivate::initPluginMap()
{
    pluginMap.clear();
    for(int i = 0; i < plugins.count(); ++i)
    {
        const CarbonPlugin *plugin = plugins.at(i);
        if(plugin == nullptr)
        {
            continue;
        }

        const QList<QLocale::Country> territories = plugin->pluginData().territories();

        for(const QLocale::Country &territory : territories)
        {
            if(!pluginMap.contains(territory))
            {
                pluginMap.insert(territory, i);
            }
        }
    }
}

CarbonPlugin *CarbonPluginManagerPrivate::pluginForTerritory(const QLocale::Country territory) const
{
    int index = pluginMap.value(territory, -1);
    if(index < 0)
    {
        return nullptr;
    }

    return plugins.at(index);
}

CarbonPluginManager * CarbonPluginManagerPrivate::Instance = nullptr;


CarbonPluginManager *CarbonPluginManager::Instance()
{
    if(CarbonPluginManagerPrivate::Instance == nullptr)
    {
        CarbonPluginManagerPrivate::Instance = new CarbonPluginManager;
    }

    return CarbonPluginManagerPrivate::Instance;
}

void CarbonPluginManager::Destroy()
{
    if(CarbonPluginManagerPrivate::Instance != nullptr)
    {
        delete CarbonPluginManagerPrivate::Instance;
        CarbonPluginManagerPrivate::Instance = nullptr;
    }
}

bool CarbonPluginManager::hasPlugins() const
{
    Q_ASSERT(d != nullptr);

    return !d->plugins.isEmpty();
}

QList<QLocale::Country> CarbonPluginManager::territories() const
{
    Q_ASSERT(d != nullptr);

    if(!hasPlugins())
    {
        return QList<QLocale::Country>();
    }

    QList<QLocale::Country> list;
    for(int i = 0; i < d->plugins.count(); ++i)
    {
        list.append(d->plugins.at(i)->pluginData().territories());
    }

    return list;
}

QStringList CarbonPluginManager::territoryNames() const
{
    Q_ASSERT(d != nullptr);

    if(!hasPlugins())
    {
        return QStringList();
    }

    QStringList list;
    for(int i = 0; i < d->plugins.count(); ++i)
    {
        list.append(d->plugins.at(i)->pluginData().territoryNames());
    }

    return list;
}

QStringList CarbonPluginManager::regionIds(const QLocale::Country territory) const
{
    Q_ASSERT(d != nullptr);

    if(!hasPlugins())
    {
        return QStringList();
    }

    const CarbonPlugin *plugin = d->pluginForTerritory(territory);
    if(plugin == nullptr)
    {
        return QStringList();
    }

    return plugin->pluginData().regionIds(territory);
}

QString CarbonPluginManager::translatedRegion(const QLocale::Country country, const QString &regionId) const
{
    Q_ASSERT(d != nullptr);

    if(!hasPlugins())
    {
        return QString();
    }

    const CarbonPlugin *plugin = d->pluginForTerritory(country);
    if(plugin == nullptr)
    {
        return QString();
    }

    return plugin->pluginData().translatedRegion(country, regionId);
}

bool CarbonPluginManager::loadPlugin(const QLocale::Country country)
{
    Q_ASSERT(d != nullptr);

    if(!hasPlugins())
    {
        return false;
    }

    CarbonPlugin *plugin = d->pluginForTerritory(country);
    if(plugin == nullptr)
    {
        return false;
    }

    if(plugin->isLoaded())
    {
        return true;
    }

    return plugin->load();
}

bool CarbonPluginManager::hasCurrentPlugin() const
{
    Q_ASSERT(d != nullptr);

    return currentPlugin() != nullptr;
}

CarbonPlugin *CarbonPluginManager::currentPlugin() const
{
    Q_ASSERT(d != nullptr);

    return d->currentPlugin;
}

CarbonData CarbonPluginManager::carbonPerKiloWatt(const QLocale::Country country, const QString &region)
{
    Q_ASSERT(d != nullptr);

    if(!hasCurrentPlugin())
    {
        return CarbonData::error("No country/region selected yet.");
    }

    return currentPlugin()->carbonPerKiloWatt(country, region);
}

CarbonPluginManager::CarbonPluginManager():
    d{new CarbonPluginManagerPrivate}
{
    d->plugins = CarbonPlugin::getPlugins();
    d->initPluginMap();
}

CarbonPluginManager::~CarbonPluginManager()
{
    delete d;
    d = nullptr;
}
