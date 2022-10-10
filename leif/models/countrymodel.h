#ifndef COUNTRYMODEL_H
#define COUNTRYMODEL_H

#include <QObject>
#include <QAbstractItemModel>

class CountryModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CountryModel(QObject *parent = nullptr);

signals:

};

#endif // COUNTRYMODEL_H
