/**
 * @brief Defines the Utilities class.
 *
 * The Utilities static class is a collection of useful methods we use to get
 * and parse the API results.
 *
 * @author Dariusz Scharsig
 *
 * @date 21.09.2022
 */
#ifndef UTILITIES_H
#define UTILITIES_H

#include <QEventLoop>
#include <QTimer>

#include "carbondata.h"

QT_BEGIN_NAMESPACE
class QNetworkAccessManager;
QT_END_NAMESPACE

class Utilities
{
public:
    /**
     * @brief Implements a blocking call which waits for \p signal to be emitted.
     *
     * This static helper method creates a blocking call which will wait until
     * \p sender's \p signal is emitted or \p milliseconds go by. If \p milliseconds
     * is set to -1 the method will wait indefinitely.
     *
     * @param sender The signal sender
     * @param signal The signal that needs to be emitted.
     * @param milliseconds Wait timeout in milliseconds.
     */
    template <class Sender>
    static void awaitSignal(Sender *sender, void (Sender::*signal)(), int milliseconds = 5000)
    {
        if(sender == nullptr || signal == nullptr)
        {
            return;
        }

        QEventLoop waitLoop;
        QObject::connect(sender, signal, &waitLoop, &QEventLoop::quit);

        if(milliseconds != -1)
        {
            QTimer::singleShot(milliseconds, &waitLoop, &QEventLoop::quit);
        }

        waitLoop.exec();
    }

    static CarbonData requestCarbonData(QNetworkAccessManager *network, int regionID);
    static CarbonData fromByteArray(const QByteArray &data);
    static CarbonData fromApiError(const QVariantHash &errorHash);
    static CarbonData fromApiResponse(const QVariantHash &replyHash);
    static QVariantHash flatJsonHash(const QJsonObject &object);
};

#endif // UTILITIES_H
