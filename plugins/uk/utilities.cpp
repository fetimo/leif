/**
 * @brief Implements the utility class methods.
 *
 * @author Dariusz Scharsig
 *
 * @date 21.09.2022
 */
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QScopeGuard>
#include <QUrl>

#include "utilities.h"

/**
 * @brief Requests the carbon data from the national grid API.
 *
 * For this to happen we need to provide this static method with a valid
 * \p network access manager and a \p regionID.
 *
 * \remark This method blocks until the reply is ready, but nor longer than 5
 * seconds.
 *
 * @param network The network access manager to use for the call.
 * @param regionID The region ID to ask the API for.
 * @return The carbon data as a CarbonData object.
 */
CarbonData Utilities::requestCarbonData(QNetworkAccessManager *network, int regionID)
{
    if(network == nullptr)
    {
        return CarbonData::error(QStringLiteral("No network object provided."));
    }

    QUrl url = QString("https://api.carbonintensity.org.uk/regional/regionid/%1").arg(regionID);
    QNetworkReply *reply = network->get(QNetworkRequest(url));

    // This quarantees that reply will be cleaned up no matter when and how
    // we exit this method.
    auto cleanup = qScopeGuard([=]() { reply->deleteLater();});

    Utilities::awaitSignal(reply, &QNetworkReply::finished);
    if(!reply->isFinished())
    {
        return CarbonData::error(QStringLiteral("API didn't reply in time."));
    }

    if(reply->error() != QNetworkReply::NoError)
    {
        QString msg("Network request could not be processed. Error: %1.");
        msg = msg.arg(reply->error());

        return CarbonData::error(msg);
    }

    QByteArray data = reply->readAll();

    return Utilities::fromByteArray(data);
}

/**
 * @brief Creates a \c CarbonData response from a JSON \p data reply.
 *
 * This static method will transform an API JSON response, which is avaialable
 * as byte array \p data into a \c CarbonData object. For this it will parse
 * the JSON, check if it has an error response and call fromApiError() or
 * fromApiResponse().
 *
 * \sa fromApiError()
 * \sa fromApiResponse()
 *
 * @param data The JSON API response.
 * @return The parsed reply as a CarbonData object.
 */
CarbonData Utilities::fromByteArray(const QByteArray &data)
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
    QVariantHash hash = Utilities::flatJsonHash(reply);

    // There is a chance that we have received an error response. In that case
    // there will be only one key named "error" containing a code and a message.
    if(reply.contains(QStringLiteral("error")))
    {
        return Utilities::fromApiError(hash);
    }

    // Now we can assume we have the actual data
    return Utilities::fromApiResponse(hash);
}

/**
 * @brief Creates a \c CarbonData reply from an API error response.
 *
 * Call this static method if your JSON response contains an error code sent by
 * the API. It will return a verbose error object.
 *
 * @param errorHash The flat JSON error code sent by the API.
 * @return The CarbonData object containing the error information.
 */
CarbonData Utilities::fromApiError(const QVariantHash &errorHash)
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

/**
 * @brief Creates a CarbonData object from the API reply.
 *
 * We need the \p replyHash to be a flattened \c QVariantHash. We extract the
 * forecast, from and to values and return the correct \c CarbonData object.
 *
 * \remark
 * If one of the dates (from, to) is not present we use the current date
 * and time.
 *
 * \sa flatJsonHash()
 *
 * \param replyHash The flattend reply hash with the API reply.
 * \return The CarbonData reply.
 */
/* static */
CarbonData Utilities::fromApiResponse(const QVariantHash &replyHash)
{
    if(replyHash.isEmpty())
    {
        return CarbonData::error("Response data seems corrupt.");
    }

    QString fromStr = replyHash.value(QStringLiteral("from")).toString();
    QString toStr = replyHash.value(QStringLiteral("to")).toString();
    int forecast = replyHash.value(QStringLiteral("forecast"), 0).toInt();

    const QString dateTimeFormat = Utilities::dateTimeFormat();
    QDateTime from = fromStr.isEmpty() ? QDateTime::currentDateTime() : QDateTime::fromString(fromStr, dateTimeFormat);
    QDateTime to   = toStr.isEmpty() ? QDateTime::currentDateTime() : QDateTime::fromString(toStr, dateTimeFormat);

    return CarbonData::ok(forecast, from, to);
}

/**
 * @brief Flattens a JSON object.
 *
 * This method creates a variant hash of key/value pairs. Objects are
 * "unwinded". This is used to make parsing API calls very simple, if for
 * instance the structure of a reply is not relevant.
 *
 * @param object The JSON object we want flattened.
 * @return A variant hash with all key value pairs as a QVariantHash object.
 */
/* static */
QVariantHash Utilities::flatJsonHash(const QJsonObject &object)
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
 * @brief Returns the date time format used by the National Grid API.
 *
 * @return The date time format string as a QString object.
 */
QString Utilities::dateTimeFormat()
{
    return QStringLiteral("yyyy-MM-ddThh:mmZ");
}
