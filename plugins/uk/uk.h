/**
 * @brief Defines the UK plugin.
 *
 * The UK plugin provides carbon data for the entire UK and subregions.
 *
 * @author Dariusz Scharsig
 *
 * @date 19.09.2022
 */
#ifndef UK_H
#define UK_H

#include <QObject>
#include <QtPlugin>
#include <interfaces/IDataProvider.h>
#include <carbondata.h>

#include "uk_global.h"

class UkPrivate;

class UK_EXPORT Uk : public QObject, public IDataProvider
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID IDataProvider_iid FILE "uk.json")
    Q_INTERFACES(IDataProvider)
public:
    explicit Uk(QObject *parent = nullptr);
    virtual ~Uk();

    CarbonData carbonPerKiloWatt(const QLocale::Country country, const QString &region);

private:
    void initialize();
    bool hasRegion(const QString &region) const;
    int regionCode(const QString &region) const;
    QStringList vaiableRegions() const;

private:
    Q_DISABLE_COPY_MOVE(Uk)
    UkPrivate *d;
};

#endif // UK_H
