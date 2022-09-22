#include <QtTest>
#include <QTime>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

#include "utilities.h"

class UtilitiesTest : public QObject
{
    Q_OBJECT

public:
    UtilitiesTest();
    ~UtilitiesTest();

private slots:
    void awaitSignalEmpty();
    void awaitSignal();
    void awaitSignalTimeout();

    void dateTimeFormat();

    void flatJsonHash();
    void flatJsonHash_data();

    void fromApiResponse();
    void fromApiResponse_data();

    void fromApiError();
    void fromApiError_data();

    void fromByteArray();
    void fromByteArray_data();

private:
    QNetworkAccessManager *m_network;

};

UtilitiesTest::UtilitiesTest():
    m_network{new QNetworkAccessManager}
{}

UtilitiesTest::~UtilitiesTest()
{
    delete m_network;
    m_network = nullptr;
}

void UtilitiesTest::awaitSignalEmpty()
{
    QTime time = QTime::currentTime();

    Utilities::awaitSignal<QObject>(nullptr, nullptr);

    QVERIFY(time.msecsTo(QTime::currentTime()) < 5);
}

void UtilitiesTest::awaitSignal()
{
    QTimer timer;
    timer.setSingleShot(true);
    timer.setInterval(100);

    QTime time = QTime::currentTime();

    timer.start();
    Utilities::awaitSignal(&timer, reinterpret_cast<void (QTimer::*)()>(&QTimer::timeout));

    QVERIFY(qAbs(time.msecsTo(QTime::currentTime()) - 100) < 50);
}

void UtilitiesTest::awaitSignalTimeout()
{
    QTimer timer;
    timer.setSingleShot(true);
    timer.setInterval(1000);

    QTime time = QTime::currentTime();

    timer.start();
    Utilities::awaitSignal(&timer, reinterpret_cast<void (QTimer::*)()>(&QTimer::timeout), 100);

    QVERIFY(qAbs(time.msecsTo(QTime::currentTime()) - 100) < 50);
}

void UtilitiesTest::dateTimeFormat()
{
    QCOMPARE(Utilities::dateTimeFormat(), QStringLiteral("yyyy-MM-ddThh:mmZ"));
}

void UtilitiesTest::flatJsonHash()
{
    QFETCH(bool, empty);
    QFETCH(QString, json);
    QFETCH(int, count);

    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toLatin1(), &parseError);

    QVERIFY(!jsonDocument.isNull());

    QVERIFY(jsonDocument.isObject());

    QVariantHash flatHash = Utilities::flatJsonHash(jsonDocument.object());

    if(empty)
    {
        QVERIFY(flatHash.isEmpty());
    }
    else
    {
        QVERIFY(flatHash.contains(QStringLiteral("name")));
        QCOMPARE(flatHash.value(QStringLiteral("name")).toString(), QStringLiteral("John"));
        QCOMPARE(flatHash.count(), count);
    }
}

void UtilitiesTest::flatJsonHash_data()
{
    QTest::addColumn<bool>("empty");
    QTest::addColumn<QString>("json");
    QTest::addColumn<int>("count");

    QTest::newRow("empty") << true << "{}" << 0;
    QTest::newRow("one value") << false << "{\"name\": \"John\"}" << 1;
    QTest::newRow("two values") << false << "{\"name\": \"John\", \"city\": \"London\"}" << 2;
    QTest::newRow("nested") << false << "{\"name\": \"John\", \"address\": {\"city\": \"Glasgow\", \"country\":\"UK\"}}" << 3;
}

void UtilitiesTest::fromApiResponse()
{
    QFETCH(int, forecast);
    QFETCH(QString, from);
    QFETCH(QString, to);

    QVariantHash flatHash;
    flatHash["forecast"] = QVariant::fromValue(forecast);
    flatHash["from"] = QVariant::fromValue(from);
    flatHash["to"] = QVariant::fromValue(to);

    QString dateTimeFormat = Utilities::dateTimeFormat();
    CarbonData replyData = Utilities::fromApiResponse(flatHash);

    QVERIFY(replyData.isValid);
    QCOMPARE(replyData.co2PerKiloWattHour, forecast);

    if(from.isEmpty())
    {
        QCOMPARE(replyData.validFrom.date(), QDate::currentDate());
    }
    else
    {
        QCOMPARE(replyData.validFrom, QDateTime::fromString(from, dateTimeFormat));
    }

    if(to.isEmpty())
    {
        QCOMPARE(replyData.validTo.date(), QDate::currentDate());
    }
    else
    {
        QCOMPARE(replyData.validTo, QDateTime::fromString(to, dateTimeFormat));
    }
}

void UtilitiesTest::fromApiResponse_data()
{
    QTest::addColumn<int>("forecast");
    QTest::addColumn<QString>("from");
    QTest::addColumn<QString>("to");

    QTest::addRow("empty") << 0 << QString() << QString();
    QTest::addRow("100") << 100 << QString() << QString();
    QTest::addRow("200") << 200 << QStringLiteral("2000-12-12T12:12Z") << QStringLiteral("2000-12-12T12:12Z");
}

void UtilitiesTest::fromApiError()
{
    QFETCH(QVariant, code);
    QFETCH(QVariant, message);

    QVariantHash flatHash;

    if(!code.isNull())
        flatHash[QStringLiteral("code")] = code;

    if(!message.isNull())
        flatHash[QStringLiteral("message")] = message;

    if(code.isNull())
        code = QStringLiteral("unknown");

    if(message.isNull())
        message = QStringLiteral("none");

    CarbonData errorReply = Utilities::fromApiError(flatHash);
    qDebug() << errorReply.errorString;

    QVERIFY(errorReply.errorString.contains(code.toString()));
    QVERIFY(errorReply.errorString.contains(message.toString()));
}

void UtilitiesTest::fromApiError_data()
{
    QTest::addColumn<QVariant>("code");
    QTest::addColumn<QVariant>("message");

    QTest::addRow("code empty") << QVariant() << QVariant::fromValue(QStringLiteral("some message"));
    QTest::addRow("message empty") << QVariant::fromValue(QStringLiteral("some code")) << QVariant();
    QTest::addRow("good") << QVariant::fromValue(QStringLiteral("some code")) << QVariant::fromValue(QStringLiteral("some message"));
}

void UtilitiesTest::fromByteArray()
{
    QFETCH(QByteArray, data);
    QFETCH(QString, expected);
    QFETCH(QString, type);

    CarbonData reply = Utilities::fromByteArray(data);
    qDebug() << reply.errorString;

    if(type == QStringLiteral("invalid"))
    {
        QVERIFY(reply.errorString.contains(expected, Qt::CaseInsensitive));
    }
    else
    {
        QVERIFY(reply.isValid);
    }
}

void UtilitiesTest::fromByteArray_data()
{
    QTest::addColumn<QByteArray>("data");
    QTest::addColumn<QString>("expected");
    QTest::addColumn<QString>("type");

    QTest::addRow("empty") << QByteArray() << QStringLiteral("empty") << QStringLiteral("invalid");
    QTest::addRow("invalid") << QByteArray("invalid") << QStringLiteral("error") << QStringLiteral("invalid");
    QTest::addRow("no object") << QByteArray("{}") << QStringLiteral("corrupt") << QStringLiteral("invalid");
    QTest::addRow("error") << QByteArray("{\"error\": {\"code\": \"Some code\", \"message\": \"Some message\"}}") << QStringLiteral("Some message") << QStringLiteral("invalid");
    QTest::addRow("ok") << QByteArray("{\"data\": {\"forecast\": 123}}") << QString() << QString();
}

QTEST_MAIN(UtilitiesTest)
#include "tst_utilitiestest.moc"
