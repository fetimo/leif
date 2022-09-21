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
    static CarbonData fromByteArray(const QByteArray &data);
    static CarbonData fromApiError(const QVariantHash &errorHash);
    static CarbonData fromApiResponse(const QVariantHash &replyHash);
    static QVariantHash flatJsonHash(const QJsonObject &object);

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

/* static */
CarbonData UkPrivate::fromByteArray(const QByteArray &data)
{
    if(data.isEmpty())
    {
        return CarbonData::error("Empty reply received.");
    }

    QJsonParseError parseError;

    QJsonDocument json = QJsonDocument::fromJson(data, &parseError);
    if(json.isNull())
    {
        QString errMsg("The supplied data(%1) seems not to be a valid JSON text. Error: %2(#%3) at %4.");
        errMsg = errMsg.arg(data, parseError.errorString()).arg(parseError.error).arg(parseError.offset);

        return CarbonData::error(errMsg);
    }

    if(json.isEmpty() || !json.isObject())
    {
        return CarbonData::error("Empty or invalid JSON received by API.");
    }

    QJsonObject reply = json.object();
    QVariantHash hash = UkPrivate::flatJsonHash(reply);

    // There is a chance that we have received an error response. In that case
    // there will be only one key named "error" containing a code and a message.
    if(reply.contains(QStringLiteral("error")))
    {
        return UkPrivate::fromApiError(hash);
    }

    // Now we can assume we have the actual data
    return UkPrivate::fromApiResponse(hash);
}

/* static */
CarbonData UkPrivate::fromApiError(const QVariantHash &errorHash)
{
    if(errorHash.isEmpty())
    {
        return CarbonData::error("An unspecified API error was received.");
    }

    QString errorCode = errorHash.value(QStringLiteral("code"), QStringLiteral("unknown")).toString();
    QString message = errorHash.value(QStringLiteral("message"), QStringLiteral("none")).toString();

    QString errMsg("The API returned an error: %1(%2).");
    errMsg = errMsg.arg(message, errorCode);

    return CarbonData::error(errMsg);
}

/* static */
CarbonData UkPrivate::fromApiResponse(const QVariantHash &replyHash)
{
    if(replyHash.isEmpty())
    {
        return CarbonData::error("Response data seems corrupt.");
    }

    QString fromStr = replyHash.value(QStringLiteral("from")).toString();
    QString toStr = replyHash.value(QStringLiteral("to")).toString();
    int forecast = replyHash.value(QStringLiteral("forecast"), 0).toInt();

    const QString dateTimeFormat = QStringLiteral("yyyy-MM-ddThh:mmZ");
    QDateTime from = fromStr.isEmpty() ? QDateTime::currentDateTime() : QDateTime::fromString(fromStr, dateTimeFormat);
    QDateTime to   = toStr.isEmpty() ? QDateTime::currentDateTime() : QDateTime::fromString(toStr, dateTimeFormat);

    return CarbonData::ok(forecast, from, to);
}

/* static */
QVariantHash UkPrivate::flatJsonHash(const QJsonObject &object)
{
    QList<QVariantMap> check;
    check << object.toVariantMap();

    QVariantHash flatHash;

    for(int i = 0; i < check.count(); i++)
    {
        QVariantMap checkMap = check.at(i);

        const QStringList keys = checkMap.keys();

        for(const QString &key : keys)
        {
            QVariant value = checkMap.value(key);

            if(value.type() == QVariant::Type::Map)
            {
                check << value.toMap();
            }
            else
            {
                flatHash.insert(key, value);
            }
        }
    }

    return flatHash;
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

    QUrl url = QString("https://api.carbonintensity.org.uk/regional/regionid/%1").arg(regionCode(region));
    QNetworkReply *reply = d->network->get(QNetworkRequest(url));

    Utilities::await(reply, &QNetworkReply::finished);

    if(reply->error() != QNetworkReply::NoError)
    {
        QString msg("Network request could not be processed. Error: %1.");
        msg = msg.arg(reply->error());
        qDebug(qPrintable(msg));

        reply->deleteLater();
        reply = nullptr;

        return CarbonData::error(msg);
    }

    QByteArray data = reply->readAll();

    reply->close();
    reply->deleteLater();
    reply = nullptr;

    return UkPrivate::fromByteArray(data);
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
