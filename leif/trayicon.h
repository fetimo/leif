#ifndef TRAYICON_H
#define TRAYICON_H

#include <QSystemTrayIcon>

#include "models/trayiconmodel.h"

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

class TrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    TrayIcon(TrayIconModel *model, QObject *parent = nullptr);
    explicit TrayIcon(const QIcon &icon, QObject *parent = nullptr);
    virtual ~TrayIcon() = default;

private Q_SLOTS:
    void onSessionCarbonChanged();
    void onTotalCarbonChanged();
    void onCarbonUsageLevelChanged();
    void onChargeForecastChanged();
    void onResetStatsClicked();
    void onPreferencesClicked();
    void onConfiguredChanged();
    void doCheckConfigured();

private:
    void setupMenu();
    void connectModel();

    static QString sessionCarbonLabel(double value);
    static QString totalCarbonLabel(double value);
    static QString carbonLabel(const QString &labelText, double value);
    static QString intensityLabel(TrayIconModel::CarbonUsageLevel usageLevel);
    static QString chargeForecastLabel(TrayIconModel::ChargeForecast chargeForecast);

    static QString co2Unit();
    static QString usageLevelToString(TrayIconModel::CarbonUsageLevel usageLevel);
    static QString usageLevelToIconName(TrayIconModel::CarbonUsageLevel usageLevel);

private:
    struct TrayIconData
    {
        TrayIconModel *model;
        QAction *notConfiguredAction;
        QAction *sessionCarbonAction;
        QAction *totalCarbonAction;
        QAction *carbonUsageLevelAction;
        QAction *chargeForecastAction;
    } d;
};

#endif // TRAYICON_H
