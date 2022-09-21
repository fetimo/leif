/**
 * @brief Defines the IDataProvider interface.
 *
 * The IDataProvider interface provides the information needed to determine the
 * CO2 cost of energy in a specific country/region.
 *
 * \sa IDataProvider
 *
 * \author Dariusz Scharsig
 *
 * \date 16.09.2022
 */
#ifndef IDATAPROVIDER_H
#define IDATAPROVIDER_H

#include <QLocale>
#include <QStringList>
#include <carbondata.h>

#define IDataProvider_iid "org.leif.DataProvider.IDataProvider/1.0"

class IDataProvider
{
public:
    virtual ~IDataProvider() = default;
    virtual CarbonData carbonPerKiloWatt(const QLocale::Country country, const QString &region) = 0;
};

Q_DECLARE_INTERFACE(IDataProvider, IDataProvider_iid)

#endif // IDATAPROVIDER_H
