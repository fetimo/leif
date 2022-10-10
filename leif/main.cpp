/**
 * @brief Defines all main application methods.
 *
 * This file contains the main() method and defined all other helper methods
 * that are used to boot up the application.
 *
 * @author Dariusz Scharsi
 *
 * @date 17.09.2022
 */
#include <QApplication>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QScopedPointer>

#include "main.h"
#include "trayicon.h"
#include "models/trayiconmodel.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false); // Very important with tray apps.

    QCoreApplication::setOrganizationName(QStringLiteral("leif"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("leif.support"));
    QCoreApplication::setApplicationName(QStringLiteral("leif carbon tracker"));

#ifdef _WIN32
    setStyleSheet();
#endif

    /*
    QScopedPointer<QTranslator> translator(new QTranslator);
    loadTranslations(translator.data());
    */

    QScopedPointer<TrayIconModel> trayModel(new TrayIconModel);
    trayModel->setCarbonUsageLevel(TrayIconModel::Medium);
    trayModel->setChargeForecast(TrayIconModel::ChargeIn30);

    QScopedPointer<TrayIcon> tray(new TrayIcon(trayModel.data()));
    tray->show();

    return app.exec();
}

void loadTranslations(QTranslator *translator)
{
    Q_ASSERT(translator != nullptr);

    if(translator->load(QLocale(), QStringLiteral("leif"), QStringLiteral("_"), QStringLiteral(":/i18n")))
    {
        QCoreApplication::installTranslator(translator);
    }
}

void setStyleSheet()
{
    qApp->setStyleSheet("QMenu {"
                        "background-color: #2B2B2B;"
                        "margin: 2px;"
                        "}"
                        "QMenu::separator {"
                        "background: gray;"
                        "height: 1px;"
                        "}"
                        "QMenu::item {"
                        "color: #f8f8f8;"
                        "padding-top: 10px;"
                        "padding-bottom: 10px;"
                        "padding-left: 30px;"
                        "padding-right: 30px"
                        "}"
                        "QMenu::item:selected {"
                        "background: #3B3B3B;"
                        "}"
                        "QMenu::item:selected:disabled {"
                        "background: #2B2B2B;"
                        "}"
                        "QMenu::item::disabled {"
                        "color: #a8a8a8;"
                        "}"
                        );
}
