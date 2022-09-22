#include <QDir>
#include <QPluginLoader>

#include "carbonplugin.h"

CarbonPlugin::CarbonPlugin(const QString &fileName)
{
    d.loader = new QPluginLoader(fileName);
    d.pluginData = Utils::CarbonPluginData::fromJson(d.loader->metaData().value(QStringLiteral("MetaData")));
    d.pluginInterface = nullptr;
}

CarbonPlugin::~CarbonPlugin()
{
    d.pluginInterface = nullptr;

    if(d.loader != nullptr)
    {
        d.loader->unload();

        delete d.loader;
        d.loader = nullptr;
    }
}

QString CarbonPlugin::errorString() const
{
    Q_ASSERT(d.loader != nullptr);

    return d.loader->errorString();
}

QString CarbonPlugin::fileName() const
{
    Q_ASSERT(d.loader != nullptr);

    return d.loader->fileName();
}

bool CarbonPlugin::isLoaded() const
{
    Q_ASSERT(d.loader != nullptr);

    return d.loader->isLoaded();
}

bool CarbonPlugin::load()
{
    Q_ASSERT(d.loader != nullptr);

    return d.loader->load();

}

bool CarbonPlugin::unload()
{
    Q_ASSERT(d.loader != nullptr);

    d.pluginInterface = nullptr;

    return d.loader->unload();
}


Utils::CarbonPluginData CarbonPlugin::pluginData() const
{
    return d.pluginData;
}

CarbonData CarbonPlugin::carbonPerKiloWatt(const QLocale::Country country, const QString &region)
{
    Q_ASSERT(d.loader != nullptr);

    if(!isLoaded())
    {
        return CarbonData::error(tr("The plugin for the country: '%1' is not loade yet.")
                                 .arg(QLocale::countryToString(country)));
    }

    if(d.pluginInterface == nullptr)
    {
        d.pluginInterface = qobject_cast<IDataProvider*>(d.loader->instance());
    }

    if(d.pluginInterface == nullptr)
    {
        return CarbonData::error(tr("The plugin for the country: '%1' seems not "
                                    "to be a valid carbon data provider plugin.")
                                 .arg(QLocale::countryToString(country)));
    }

    return d.pluginInterface->carbonPerKiloWatt(country, region);
}

QList<CarbonPlugin *> CarbonPlugin::getPlugins()
{
    QString pluginPath = QCoreApplication::applicationDirPath() + QStringLiteral("/plugins");

    QDir dir(pluginPath);
    const QStringList fileNames = dir.entryList(QDir::Files | QDir::Readable | QDir::NoDotAndDotDot);

    QList<CarbonPlugin*> plugins;

    for(const QString &fileName : fileNames)
    {
        if(QLibrary::isLibrary(fileName))
        {
            QString filePath = pluginPath + QStringLiteral("/") + fileName;
            plugins << new CarbonPlugin(filePath);
        }
    }

    return plugins;
}
