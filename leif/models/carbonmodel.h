#ifndef CARBONMODEL_H
#define CARBONMODEL_H

#include <QObject>

class CarbonModel : public QObject
{
    Q_OBJECT
public:
    explicit CarbonModel(QObject *parent = nullptr);

signals:

};

#endif // CARBONMODEL_H
