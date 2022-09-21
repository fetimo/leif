#ifndef UTILITIES_H
#define UTILITIES_H

#include <QEventLoop>

class Utilities
{
public:
    template <class Sender>
    static void await(Sender *sender, void (Sender::*signal)())
    {
        if(sender == nullptr || signal == nullptr)
        {
            return;
        }

        QEventLoop waitLoop;
        QObject::connect(sender, signal, &waitLoop, &QEventLoop::quit);
        waitLoop.exec();
    }
};

#endif // UTILITIES_H
