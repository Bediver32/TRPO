#include <QtTest>

#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSignalSpy>

#include <memory>

#include "dialog.h"

class PrimeListGuiTest : public QObject {
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void createsDialog();
    void acceptsLowerBoundInput();
    void acceptsUpperBoundInput();
    void calculateButtonEmitsConnectedSignal();
    void tabCyclesThroughControls();

private:
    std::unique_ptr<Dialog> dialog;
    QLineEdit * lower = nullptr;
    QLineEdit * upper = nullptr;
    QPushButton * calculate = nullptr;
    QListWidget * results = nullptr;
};

void PrimeListGuiTest::init() {
    dialog = std::make_unique<Dialog>();
    lower = dialog->findChild<QLineEdit *>("lineEditA");
    upper = dialog->findChild<QLineEdit *>("lineEditB");
    calculate = dialog->findChild<QPushButton *>("pushButton");
    results = dialog->findChild<QListWidget *>("listWidget");
    QVERIFY(lower);
    QVERIFY(upper);
    QVERIFY(calculate);
    QVERIFY(results);
    dialog->show();
    dialog->activateWindow();
    QVERIFY(QTest::qWaitForWindowActive(dialog.get()));
}

void PrimeListGuiTest::cleanup() {
    dialog.reset();
}

void PrimeListGuiTest::createsDialog() {
    QVERIFY(dialog->isVisible());
}

void PrimeListGuiTest::acceptsLowerBoundInput() {
    QTest::mouseClick(lower, Qt::LeftButton);
    QTest::keyClicks(lower, "123");
    QCOMPARE(lower->text(), QStringLiteral("123"));
}

void PrimeListGuiTest::acceptsUpperBoundInput() {
    QTest::mouseClick(upper, Qt::LeftButton);
    QTest::keyClicks(upper, "456");
    QCOMPARE(upper->text(), QStringLiteral("456"));
}

void PrimeListGuiTest::calculateButtonEmitsConnectedSignal() {
    // Снимаем существующее соединение, чтобы проверить кнопку без вычисления.
    QVERIFY(QObject::disconnect(calculate, SIGNAL(clicked()), dialog.get(), SLOT(calculate())));
    QSignalSpy clicks(calculate, &QPushButton::clicked);
    QVERIFY(clicks.isValid());
    QTest::mouseClick(calculate, Qt::LeftButton);
    QCOMPARE(clicks.count(), 1);
}

void PrimeListGuiTest::tabCyclesThroughControls() {
    QTest::mouseClick(lower, Qt::LeftButton);
    QVERIFY(lower->hasFocus());
    QTest::keyClick(lower, Qt::Key_Tab);
    QVERIFY(upper->hasFocus());
    QTest::keyClick(upper, Qt::Key_Tab);
    QVERIFY(calculate->hasFocus());
    QTest::keyClick(calculate, Qt::Key_Tab);
    QVERIFY(results->hasFocus());
    QTest::keyClick(results, Qt::Key_Tab);
    QVERIFY(lower->hasFocus());
}

QTEST_MAIN(PrimeListGuiTest)
#include "tst_primelist_gui.moc"
