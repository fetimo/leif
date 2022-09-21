/**
 * @brief Defines the CarbonData reply structure.
 *
 * The CarbonData reply structure contains the cost of a kilowatt hour in
 * grams of CO2 and may contain an error, if there was an issue getting the data.
 *
 * \sa IDataProvider
 *
 * \author Dariusz Scharsig
 *
 * \date 17.09.2022
 */
#ifndef CARBONDATA_H
#define CARBONDATA_H

#include <QDateTime>
#include <QString>

struct CarbonData
{
    inline CarbonData();
    inline CarbonData(int _co2PerKiloWattHour, bool _isValid, const QString &_errorString, const QDateTime &_validFrom, const QDateTime &_validTo);

    inline static CarbonData ok(int _co2PerKiloWattHour, const QDateTime &_validFrom, const QDateTime &_validTo);
    inline static CarbonData error(const QString &_errorString);

    int co2PerKiloWattHour;
    bool isValid;
    QString errorString;
    QDateTime validFrom;
    QDateTime validTo;
};

CarbonData::CarbonData():
    CarbonData(-1, false, QString(), QDateTime(), QDateTime())
{}

CarbonData::CarbonData(int _co2PerKiloWattHour, bool _isValid, const QString &_errorString, const QDateTime &_validFrom, const QDateTime &_validTo):
    co2PerKiloWattHour {_co2PerKiloWattHour},
    isValid {_isValid},
    errorString {_errorString},
    validFrom {_validFrom},
    validTo {_validTo}
{}

/* static */
CarbonData CarbonData::ok(int _co2PerKiloWattHour, const QDateTime &_validFrom, const QDateTime &_validTo)
{
    return CarbonData(_co2PerKiloWattHour, true, QString(), _validFrom, _validTo);
}

/* static */
CarbonData CarbonData::error(const QString &_errorString)
{
    return CarbonData(-1, false, _errorString, QDateTime(), QDateTime());
}

#endif // CARBONDATA_H
