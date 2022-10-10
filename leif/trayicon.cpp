#include <QApplication>
#include <QHash>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>

#include "trayicon.h"

TrayIcon::TrayIcon(TrayIconModel *model, QObject *parent /* = nullptr */):
    TrayIcon(QIcon(TrayIcon::usageLevelToIconName(TrayIconModel::VeryHigh)), parent)
{
    Q_ASSERT(model != nullptr);

    d.model = model;

    setupMenu();
    connectModel();

    QTimer::singleShot(500, this, &TrayIcon::doCheckConfigured);
}

TrayIcon::TrayIcon(const QIcon &icon, QObject *parent /* = nullptr */):
    QSystemTrayIcon(icon, parent)
{
    d.model = nullptr;
    d.notConfiguredAction = nullptr;
    d.sessionCarbonAction = nullptr;
    d.totalCarbonAction = nullptr;
    d.carbonUsageLevelAction = nullptr;
    d.chargeForecastAction = nullptr;
}

void TrayIcon::onSessionCarbonChanged()
{
    Q_ASSERT(d.model != nullptr);

    d.sessionCarbonAction->setText(TrayIcon::sessionCarbonLabel(d.model->sessionCarbon()));
}

void TrayIcon::onTotalCarbonChanged()
{
    Q_ASSERT(d.model != nullptr);

    d.totalCarbonAction->setText(TrayIcon::totalCarbonLabel(d.model->totalCarbon()));
}

void TrayIcon::onCarbonUsageLevelChanged()
{
    Q_ASSERT(d.model != nullptr);

    d.carbonUsageLevelAction->setText(TrayIcon::intensityLabel(d.model->carbonUsageLevel()));

    // Also set the icon
    setIcon(QIcon(TrayIcon::usageLevelToIconName(d.model->carbonUsageLevel())));
}

void TrayIcon::onChargeForecastChanged()
{
    Q_ASSERT(d.model != nullptr);

    d.chargeForecastAction->setText(TrayIcon::chargeForecastLabel(d.model->chargeForecast()));
}

void TrayIcon::onResetStatsClicked()
{
    Q_ASSERT(d.model != nullptr);

    int answer = QMessageBox::question(nullptr, tr("Resetting stats"),
                                       tr("Do you really want to reset the "
                                          "carbon usage statistics?\n"
                                          "This can not be undone."));

    show();

    if(answer != QMessageBox::Yes)
    {
        return;
    }

    showMessage(tr("Leif Information"), tr("The CO2 usage statistics have been cleared."),
                QIcon("img/leif_128.png"));

    d.model->resetStats();
}

void TrayIcon::onPreferencesClicked()
{
    Q_ASSERT(d.model != nullptr);

    d.model->showDialog();
}

void TrayIcon::onConfiguredChanged()
{
    Q_ASSERT(d.model != nullptr);

    if(d.notConfiguredAction != nullptr)
    {
        d.notConfiguredAction->setVisible(!d.model->configured());
    }
}

void TrayIcon::doCheckConfigured()
{
    Q_ASSERT(d.model != nullptr);

    if(!d.model->configured())
    {
        showMessage(tr("We need to talk"), tr("Hey there. I would love to go to "
                                              "work, but I need to know where "
                                              "you are."), QIcon("img/leif_128.png"));
    }
}

void TrayIcon::setupMenu()
{
    QMenu *menu = new QMenu;

    d.notConfiguredAction = menu->addAction(tr("Leif is not configured yet"), this, &TrayIcon::onPreferencesClicked);
    onConfiguredChanged();

    d.sessionCarbonAction = menu->addAction(QString());
    d.sessionCarbonAction->setDisabled(true);
    onSessionCarbonChanged();

    d.totalCarbonAction = menu->addAction(QString());
    d.totalCarbonAction->setDisabled(true);
    onTotalCarbonChanged();

    d.carbonUsageLevelAction = menu->addAction(QString());
    d.carbonUsageLevelAction->setDisabled(true);
    onCarbonUsageLevelChanged();

    d.chargeForecastAction = menu->addAction(QString());
    d.chargeForecastAction->setDisabled(true);
    onChargeForecastChanged();

    menu->addAction(tr("Preferences..."), this, &TrayIcon::onPreferencesClicked);
    menu->addAction(tr("Reset stats"), this, &TrayIcon::onResetStatsClicked, Qt::Key_R);

    menu->addSeparator();
    menu->addAction(tr("Quit"), qApp, &QApplication::quit, Qt::Key_Q);

    setContextMenu(menu);
}

void TrayIcon::connectModel()
{
    Q_ASSERT(d.model != nullptr);

    connect(d.model, &TrayIconModel::sessionCarbonChanged, this, &TrayIcon::onSessionCarbonChanged);
    connect(d.model, &TrayIconModel::totalCarbonChanged, this, &TrayIcon::onTotalCarbonChanged);
    connect(d.model, &TrayIconModel::carbonUsageLevelChanged, this, &TrayIcon::onCarbonUsageLevelChanged);
    connect(d.model, &TrayIconModel::chargeForecastChanged, this, &TrayIcon::onChargeForecastChanged);
    connect(d.model, &TrayIconModel::configuredChanged, this, &TrayIcon::onConfiguredChanged);
}

QString TrayIcon::co2Unit()
{
    return QString("gCO2");
}

QString TrayIcon::sessionCarbonLabel(double value)
{
    QString labelText = tr("Session", "As in since the computer was turned on.");
    return TrayIcon::carbonLabel(labelText, value);
}

QString TrayIcon::totalCarbonLabel(double value)
{
    QString labelText = tr("Lifetime", "As in total sum of consumption");
    return TrayIcon::carbonLabel(labelText, value);
}

QString TrayIcon::carbonLabel(const QString &labelText, double value)
{
    QString label = QStringLiteral("%1: %2 \\(%3)");
    label = label.arg(labelText, QLocale().toString(value), TrayIcon::co2Unit());

    return label;
}

QString TrayIcon::intensityLabel(TrayIconModel::CarbonUsageLevel usageLevel)
{
    QString intensity = QStringLiteral("%1: %2");
    intensity = intensity.arg(tr("Current intensity", "Power usage intensity."), TrayIcon::usageLevelToString(usageLevel));

    return intensity;
}

QString TrayIcon::chargeForecastLabel(TrayIconModel::ChargeForecast chargeForecast)
{
    static QHash<TrayIconModel::ChargeForecast, QString> label;

    if(label.contains(chargeForecast))
    {
        return label.value(chargeForecast);
    }

    QString str;

    switch(chargeForecast)
    {
    case(TrayIconModel::ChargeNow):
        str = tr("Charge now");
        break;

    case(TrayIconModel::ChargeIn30):
        str = tr("Charge in 30 minutes");
        break;

    case(TrayIconModel::ChargeIn60):
        str = tr("Charge in an hour or so");
        break;

    case(TrayIconModel::ChargeWhenNeeded):
        str = tr("Charge when needed");
        break;
    }

    label.insert(chargeForecast, str);

    return str;
}

QString TrayIcon::usageLevelToString(TrayIconModel::CarbonUsageLevel usageLevel)
{
    static QHash<TrayIconModel::CarbonUsageLevel, QString> usageLevelStrings;

    if(usageLevelStrings.contains(usageLevel))
    {
        return usageLevelStrings.value(usageLevel);
    }

    QString str;
    switch(usageLevel)
    {
    case(TrayIconModel::VeryHigh):
        //: The carbon usage intensity is very high
        str = tr("Very high");
        break;

    case(TrayIconModel::High):
        str = tr("High");
        break;

    case(TrayIconModel::Medium):
        str = tr("Medium");
        break;

    case(TrayIconModel::Low):
        str = tr("Low");
        break;

    case(TrayIconModel::VeryLow):
        str = tr("Very low");
        break;

    default:
        str = tr("unknown");
        break;
    }

    usageLevelStrings.insert(usageLevel, str);

    return str;
}

QString TrayIcon::usageLevelToIconName(TrayIconModel::CarbonUsageLevel usageLevel)
{
    static QHash<TrayIconModel::CarbonUsageLevel, QString> usageLevelIconNames;

    if(usageLevelIconNames.contains(usageLevel))
    {
        return usageLevelIconNames.value(usageLevel);
    }

    QString str;
    switch(usageLevel)
    {
    case(TrayIconModel::VeryHigh):
        //: The carbon usage intensity is very high
        str = QStringLiteral("VerySadD.png");
        break;

    case(TrayIconModel::High):
        str = QStringLiteral("SadD.png");
        break;

    case(TrayIconModel::Medium):
        str = QStringLiteral("FairD.png");
        break;

    case(TrayIconModel::Low):
        str = QStringLiteral("HappyD.png");
        break;

    case(TrayIconModel::VeryLow):
        str = QStringLiteral("VeryHappyD.png");
        break;

    default:
        str = QStringLiteral("VerySadD.png");
        break;
    }

    str.prepend(QStringLiteral("img/"));

    usageLevelIconNames.insert(usageLevel, str);

    return str;
}
