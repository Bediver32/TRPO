#include <QtTest>

#include <chrono>
#include <limits>
#include <stdexcept>
#include <typeinfo>
#include <vector>

#include "primelist.h"

class PrimeListTest : public QObject {
    Q_OBJECT

private slots:
    void returnsOnlyPrimes();
    void includesPrimeLowerBound();
    void includesPrimeUpperBound();
    void handlesLargestSupportedBound();
    void rejectsInvalidBounds_data();
    void rejectsInvalidBounds();
    void listsAllFiveDigitPrimesWithin100ms();
};

void PrimeListTest::returnsOnlyPrimes() {
    const std::vector<int> expected{2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    QCOMPARE(primelist(1, 30), expected);
    QVERIFY(primelist(14, 16).empty());
}

void PrimeListTest::includesPrimeLowerBound() {
    const std::vector<int> expected{11, 13};
    QCOMPARE(primelist(11, 16), expected);
}

void PrimeListTest::includesPrimeUpperBound() {
    const std::vector<int> expected{11, 13, 17};
    QCOMPARE(primelist(10, 17), expected);
}

void PrimeListTest::handlesLargestSupportedBound() {
    const std::vector<int> expected{2147483647};
    QCOMPARE(primelist(2147483646LL, 2147483647LL), expected);
}

void PrimeListTest::rejectsInvalidBounds_data() {
    QTest::addColumn<qlonglong>("lower");
    QTest::addColumn<qlonglong>("upper");
    QTest::addColumn<QString>("message");

    constexpr int maxBound = std::numeric_limits<int>::max();
    const auto tooLarge = static_cast<qlonglong>(maxBound) + 1;
    const auto lowerMessage =
        QStringLiteral("Параметр A (нижняя граница) должен быть натуральным числом от 1 до %1.").arg(maxBound);
    const auto upperMessage =
        QStringLiteral("Параметр B (верхняя граница) должен быть натуральным числом от 1 до %1.").arg(maxBound);
    const auto orderMessage =
        QStringLiteral("Верхняя граница B должна быть больше нижней границы A.");

    QTest::newRow("zero lower") << 0LL << 10LL << lowerMessage;
    QTest::newRow("negative lower") << -1LL << 10LL << lowerMessage;
    QTest::newRow("lower exceeds int") << tooLarge << 10LL << lowerMessage;
    QTest::newRow("zero upper") << 1LL << 0LL << upperMessage;
    QTest::newRow("negative upper") << 1LL << -1LL << upperMessage;
    QTest::newRow("upper exceeds int") << 1LL << tooLarge << upperMessage;
    QTest::newRow("equal limits") << 2LL << 2LL << orderMessage;
    QTest::newRow("reversed limits") << 10LL << 2LL << orderMessage;
}

void PrimeListTest::rejectsInvalidBounds() {
    QFETCH(qlonglong, lower);
    QFETCH(qlonglong, upper);
    QFETCH(QString, message);

    try {
        primelist(lower, upper);
        QFAIL("Expected std::out_of_range");
    } catch (const std::out_of_range &error) {
        QVERIFY(typeid(error) == typeid(std::out_of_range));
        QCOMPARE(QString::fromUtf8(error.what()), message);
    } catch (const std::exception &error) {
        QFAIL(error.what());
    } catch (...) {
        QFAIL("Unexpected non-standard exception");
    }
}

void PrimeListTest::listsAllFiveDigitPrimesWithin100ms() {
    constexpr int lower = 10000;
    constexpr int upper = 99999;
    constexpr auto timeLimit = std::chrono::milliseconds(100);

    const auto start = std::chrono::steady_clock::now();
    const auto actual = primelist(lower, upper);
    const auto elapsed = std::chrono::steady_clock::now() - start;
    const auto elapsedMs = std::chrono::duration<double, std::milli>(elapsed).count();

    qInfo("Five-digit prime list took %.3f ms", elapsedMs);
    const auto timingMessage = QStringLiteral("Expected at most 100 ms, measured %1 ms").arg(elapsedMs);
    QVERIFY2(elapsed <= timeLimit, qPrintable(timingMessage));

    // Независимый эталон по решету Эратосфена считается вне измеряемого интервала.
    std::vector<bool> composite(upper + 1);
    for (int divisor = 2; divisor * divisor <= upper; ++divisor) {
        if (!composite[divisor]) {
            for (int multiple = divisor * divisor; multiple <= upper; multiple += divisor) {
                composite[multiple] = true;
            }
        }
    }

    std::vector<int> expected;
    for (int number = lower; number <= upper; ++number) {
        if (!composite[number]) {
            expected.push_back(number);
        }
    }

    QCOMPARE(actual, expected);
}

QTEST_APPLESS_MAIN(PrimeListTest)
#include "tst_primelist.moc"
