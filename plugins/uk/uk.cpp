/**
 * @brief Implements the UK plugin.
 *
 * The UK plugin provides carbon data for the entire UK and subregions.
 *
 * @author Dariusz Scharsig
 *
 * @date 19.09.2022
 */
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkProxyFactory>
#include <QHash>
#include <QJsonDocument>
#include <QJsonArray>

#ifdef _DEBUG
#include <QtDebug>
#endif

#include "uk.h"
#include "utilities.h"

class UkPrivate
{
private:
    UkPrivate();
    ~UkPrivate();

    QHash<QString, int> regionHash;
    QNetworkAccessManager *network;
    friend class Uk;
};

UkPrivate::UkPrivate()
{
    network = new QNetworkAccessManager;
}

UkPrivate::~UkPrivate()
{
    delete network;
    network = nullptr;
}

/**
 * @brief Constructs the Uk plugin interface object.
 *
 * We create the network access manager and initialize the region hash.
 *
 * @param parent Optionl QObject parent pointer. Default: \c nullptr.
 */
Uk::Uk(QObject *parent /* = nullptr */):
    QObject(parent),
    d{new UkPrivate}
{
    initialize();
}

/**
 * @brief Destroys the Uk plugin interface object.
 */
Uk::~Uk()
{
    delete d;
    d = nullptr;
}

/**
 * @brief Returns the carbon emission per kilo watt hour in grams of CO2.
 *
 * You need to specify the \p country and the \p region. If the \p country is
 * not the United Kingdom or the region is empty/unknown, the method will return
 * an error CarbonData object with an appropriate message.
 *
 * \remark
 * This method blocks the execution until the data is available and read.
 *
 * \sa CarbonData
 *
 * @param country The country (always United Kingdrom).
 * @param region One of the 15 regions specified by https://api.carbonintensity.org.uk/
 * @return The CarbonData reply object.
 */
CarbonData Uk::carbonPerKiloWatt(const QLocale::Country country, const QString &region)
{
    Q_ASSERT(d != nullptr);

    if(country != QLocale::UnitedKingdom)
    {
        return CarbonData::error(QString("This plugin can only provide data for "
                                         "the United Kingdom, but %1 was "
                                         "requested.").arg(QLocale::countryToString(country)));
    }

    if(region.isEmpty())
    {
        return CarbonData::error("No region provided. This plugin requires a specific region.");
    }

    if(!hasRegion(region))
    {
        return CarbonData::error(QString("Can't provide the data for the "
                                         "region: '%1'. It is unknown.").arg(region));
    }

    return Utilities::requestCarbonData(d->network, regionCode(region));
}

/**
 * @brief Initializes a hash with all regions this plugin can handle.
 *
 * We need those to map the region name to it's ID as required by the API.
 */
void Uk::initialize()
{
    Q_ASSERT(d != nullptr);

    QNetworkProxyFactory::setUseSystemConfiguration(true);

    if(!d->regionHash.isEmpty())
    {
        return;
    }

    d->regionHash["North Scotland"] = 1;
    d->regionHash["South Scotland"] = 2;
    d->regionHash["North West England"] = 3;
    d->regionHash["North East England"] = 4;
    d->regionHash["South Yorkshire"] = 5;
    d->regionHash["North Wales, Merseyside and Cheshire"] = 6;
    d->regionHash["South Wales"] = 7;
    d->regionHash["West Midlands"] = 8;
    d->regionHash["East Midlands"] = 9;
    d->regionHash["East England"] = 10;
    d->regionHash["South West England"] = 11;
    d->regionHash["South England"] = 12;
    d->regionHash["London"] = 13;
    d->regionHash["South East England"] = 14;
    d->regionHash["England"] = 15;
    d->regionHash["Scotland"] = 16;
    d->regionHash["Wales"] = 17;
}

/**
 * @brief Checks if a region is known.
 *
 * This method will verify that the \p region is one of the regions this plugin
 * can handle.
 *
 * @param region The region we wish to check.
 * @return \arg \c true Yes, this region is known.
 *         \arg \c false No, unknown region.
 */
bool Uk::hasRegion(const QString &region) const
{
    return(vaiableRegions().contains(region));
}

/**
 * @brief Returns the region code as required by the API.
 *
 * For example, "North Scotland" has the ID: \c 1.
 *
 * \remark If an unknown \p region is provided, this method will return \c -1.
 *
 * @param region The region we want the ID for.
 * @return The region ID as an \c int.
 */
int Uk::regionCode(const QString &region) const
{
    Q_ASSERT(d != nullptr);

    if(region.isEmpty() || !hasRegion(region))
    {
        return -1;
    }

    return d->regionHash.value(region);
}

/**
 * @brief Returns a string list with all known region names.
 *
 * @return The list as a QStringList object.
 */
QStringList Uk::vaiableRegions() const
{
    Q_ASSERT(d != nullptr);

    return d->regionHash.keys();
}
