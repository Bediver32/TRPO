#include <QtTest>

#include <QApplication>
#include <QEvent>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QSignalSpy>
#include <QTimer>

#include <limits>
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
    void replacesResultsOnSecondCalculation();
    void completesKeyboardAndMouseWorkflow();
    void reportsInputErrors_data();
    void reportsInputErrors();

private:
    bool eventFilter(QObject *watched, QEvent *event) override;
    QStringList resultTexts() const;

    std::unique_ptr<Dialog> dialog;
    QLineEdit * lower = nullptr;
    QLineEdit * upper = nullptr;
    QPushButton * calculate = nullptr;
    QListWidget * results = nullptr;
    QStringList errorMessages;
    QString errorTitle;
    QMessageBox::Icon errorIcon = QMessageBox::NoIcon;
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
    errorMessages.clear();
    errorTitle.clear();
    errorIcon = QMessageBox::NoIcon;
    qApp->installEventFilter(this);
    dialog->show();
    dialog->activateWindow();
    QVERIFY(QTest::qWaitForWindowActive(dialog.get()));
}

void PrimeListGuiTest::cleanup() {
    qApp->removeEventFilter(this);
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

QStringList PrimeListGuiTest::resultTexts() const {
    QStringList texts;
    for(int row = 0; row < results->count(); ++row) {
        texts.append(results->item(row)->text());
    }
    return texts;
}

bool PrimeListGuiTest::eventFilter(QObject *watched, QEvent *event) {
    if(event->type() == QEvent::Show) {
        if(auto *box = qobject_cast<QMessageBox *>(watched)) {
            errorMessages.append(box->text());
            errorTitle = box->windowTitle();
            errorIcon = box->icon();
            // Закрываем окно после входа QMessageBox в собственный цикл событий.
            QTimer::singleShot(0, box, &QDialog::accept);
        }
    }
    return QObject::eventFilter(watched, event);
}

void PrimeListGuiTest::replacesResultsOnSecondCalculation() {
    lower->setText(QStringLiteral("10"));
    upper->setText(QStringLiteral("20"));
    QTest::mouseClick(calculate, Qt::LeftButton);
    const QStringList first{QStringLiteral("11"), QStringLiteral("13"),
                            QStringLiteral("17"), QStringLiteral("19")};
    QCOMPARE(resultTexts(), first);
    lower->setText(QStringLiteral("2"));
    upper->setText(QStringLiteral("4"));
    QTest::mouseClick(calculate, Qt::LeftButton);
    const QStringList second{QStringLiteral("2"), QStringLiteral("3")};
    QCOMPARE(resultTexts(), second);
    QVERIFY(errorMessages.isEmpty());
}

void PrimeListGuiTest::completesKeyboardAndMouseWorkflow() {
    QTest::mouseClick(lower, Qt::LeftButton);
    QVERIFY(lower->hasFocus());
    QTest::keyClicks(lower, "1");
    QTest::keyClick(lower, Qt::Key_Tab);
    QVERIFY(upper->hasFocus());
    QTest::keyClicks(upper, "10");
    QTest::mouseClick(calculate, Qt::LeftButton);
    const QStringList expected{QStringLiteral("2"), QStringLiteral("3"),
                               QStringLiteral("5"), QStringLiteral("7")};
    QCOMPARE(resultTexts(), expected);
    QVERIFY(errorMessages.isEmpty());
}

void PrimeListGuiTest::reportsInputErrors_data() {
    QTest::addColumn<QString>("leftText");
    QTest::addColumn<QString>("rightText");
    QTest::addColumn<QString>("expectedMessage");
    const auto one = QStringLiteral("1");
    const auto two = QStringLiteral("2");
    const auto ten = QStringLiteral("10");
    const auto zero = QStringLiteral("0");
    const auto negative = QStringLiteral("-1");
    const auto invalid = QStringLiteral("1.5");
    const auto overflow = QStringLiteral("999999999999999999999");
    const auto maxBound = std::numeric_limits<int>::max();
    const auto tooLarge = QString::number(static_cast<qlonglong>(maxBound) + 1);
    const auto invalidLower =
        QStringLiteral("Параметр A введён неверно: значение не является десятичным целым числом.");
    const auto invalidUpper =
        QStringLiteral("Параметр B введён неверно: значение не является десятичным целым числом.");
    const auto lowerBounds =
        QStringLiteral("Параметр A (нижняя граница) должен быть натуральным числом от 1 до %1.").arg(maxBound);
    const auto upperBounds =
        QStringLiteral("Параметр B (верхняя граница) должен быть натуральным числом от 1 до %1.").arg(maxBound);
    const auto order = QStringLiteral("Верхняя граница B должна быть больше нижней границы A.");
    QTest::newRow("empty A") << QString() << ten << QStringLiteral("Параметр A введён неверно: поле пустое.");
    QTest::newRow("empty B") << one << QString() << QStringLiteral("Параметр B введён неверно: поле пустое.");
    QTest::newRow("non-integer A") << invalid << ten << invalidLower;
    QTest::newRow("non-integer B") << one << invalid << invalidUpper;
    QTest::newRow("parse overflow A") << overflow << ten << invalidLower;
    QTest::newRow("parse overflow B") << one << overflow << invalidUpper;
    QTest::newRow("zero A") << zero << ten << lowerBounds;
    QTest::newRow("negative A") << negative << ten << lowerBounds;
    QTest::newRow("A exceeds int") << tooLarge << ten << lowerBounds;
    QTest::newRow("zero B") << one << zero << upperBounds;
    QTest::newRow("negative B") << one << negative << upperBounds;
    QTest::newRow("B exceeds int") << one << tooLarge << upperBounds;
    QTest::newRow("equal bounds") << two << two << order;
    QTest::newRow("reversed bounds") << ten << two << order;
}

void PrimeListGuiTest::reportsInputErrors() {
    QFETCH(QString, leftText);
    QFETCH(QString, rightText);
    QFETCH(QString, expectedMessage);
    lower->setText(leftText);
    upper->setText(rightText);
    QTest::mouseClick(calculate, Qt::LeftButton);
    QCOMPARE(errorMessages, QStringList{expectedMessage});
    QCOMPARE(errorTitle, QStringLiteral("Ошибка"));
    QCOMPARE(errorIcon, QMessageBox::Critical);
    QCOMPARE(results->count(), 0);
}

QTEST_MAIN(PrimeListGuiTest)
#include "tst_primelist_gui.moc"
