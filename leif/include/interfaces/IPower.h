/**
 * @brief Defines the IPower interface.
 *
 * The IPower interface defines common methods that power draw information
 * providers must implement. We will use it to calculate the current CO2
 * consumption.
 *
 * @author Dariusz Scharsig
 *
 * @date 23.09.2022
 */
#ifndef IPOWER_H
#define IPOWER_H

#include <QtPlugin>

#define IPower_iid "org.leif.DataProvider.IPower/1.0"

class IPower
{
public:
    virtual ~IPower() = default;
    virtual float powerDrawInWatts() = 0;
};

Q_DECLARE_INTERFACE(IPower, IPower_iid)

#endif // IPOWER_H
