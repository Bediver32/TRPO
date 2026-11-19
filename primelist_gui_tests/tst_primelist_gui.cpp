#include <QtTest>

#include <QAbstractButton>
#include <QApplication>
#include <QEvent>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QSignalSpy>
#include <QTimer>

#include <limits>
#include <memory>

#include "dialog.h"
#include "translation.h"

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
    void localizesDialog_data();
    void localizesDialog();

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
    QString errorButtonText;
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
    errorButtonText.clear();
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
            if(const auto *button = box->button(QMessageBox::Ok)) {
                errorButtonText = button->text();
            }
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

void PrimeListGuiTest::localizesDialog_data() {
    QTest::addColumn<QString>("localeName");
    QTest::addColumn<QStringList>("interfaceText");
    QTest::addColumn<QStringList>("messages");
    const int maximum = std::numeric_limits<int>::max();
    QTest::newRow("ru_RU") << QStringLiteral("ru_RU")
                           << QStringList{QStringLiteral("Простые числа"), QStringLiteral("Нижняя граница A:"),
                                          QStringLiteral("Верхняя граница B:"), QStringLiteral("Вычислить"),
                                          QStringLiteral("Ошибка"), QStringLiteral("ОК")}
                           << QStringList{QStringLiteral("Параметр A введён неверно: поле пустое."),
                                          QStringLiteral("Параметр B введён неверно: значение не является десятичным целым числом."),
                                          QStringLiteral("Параметр A (нижняя граница) должен быть натуральным числом от 1 до %1.").arg(maximum),
                                          QStringLiteral("Параметр B (верхняя граница) должен быть натуральным числом от 1 до %1.").arg(maximum),
                                          QStringLiteral("Верхняя граница B должна быть больше нижней границы A.")};
    const QStringList englishInterface{QStringLiteral("Prime numbers"), QStringLiteral("Lower bound A:"),
                                       QStringLiteral("Upper bound B:"), QStringLiteral("Calculate"),
                                       QStringLiteral("Error"), QStringLiteral("OK")};
    const QStringList englishMessages{
        QStringLiteral("Parameter A is invalid: the field is empty."),
        QStringLiteral("Parameter B is invalid: the value is not a decimal integer."),
        QStringLiteral("Parameter A (lower bound) must be a positive integer from 1 to %1.").arg(maximum),
        QStringLiteral("Parameter B (upper bound) must be a positive integer from 1 to %1.").arg(maximum),
        QStringLiteral("Upper bound B must be greater than lower bound A.")};
    QTest::newRow("en_US") << QStringLiteral("en_US") << englishInterface << englishMessages;
    QTest::newRow("unsupported locale") << QStringLiteral("de_DE") << englishInterface << englishMessages;
}

void PrimeListGuiTest::localizesDialog() {
    QFETCH(QString, localeName);
    QFETCH(QStringList, interfaceText);
    QFETCH(QStringList, messages);
    QTranslator translator;
    QVERIFY(loadInterfaceTranslation(translator, QLocale(localeName)));
    QVERIFY(qApp->installTranslator(&translator));
    Dialog localized;
    const auto *labelA = localized.findChild<QLabel *>("labelA");
    const auto *labelB = localized.findChild<QLabel *>("labelB");
    auto *inputA = localized.findChild<QLineEdit *>("lineEditA");
    auto *inputB = localized.findChild<QLineEdit *>("lineEditB");
    auto *button = localized.findChild<QPushButton *>("pushButton");
    QVERIFY(labelA);
    QVERIFY(labelB);
    QVERIFY(inputA);
    QVERIFY(inputB);
    QVERIFY(button);
    const QStringList labels{localized.windowTitle(), labelA->text(), labelB->text(), button->text()};
    QCOMPARE(labels, interfaceText.mid(0, 4));
    const QList<QPair<QString, QString>> invalidInputs{
        {QString(), QStringLiteral("10")},
        {QStringLiteral("1"), QStringLiteral("abc")},
        {QStringLiteral("0"), QStringLiteral("10")},
        {QStringLiteral("1"), QStringLiteral("0")},
        {QStringLiteral("2"), QStringLiteral("2")}
    };
    for(const auto &input : invalidInputs) {
        inputA->setText(input.first);
        inputB->setText(input.second);
        button->click();
        QCOMPARE(errorTitle, interfaceText.at(4));
        QCOMPARE(errorButtonText, interfaceText.at(5));
    }
    QCOMPARE(errorMessages, messages);
}

QTEST_MAIN(PrimeListGuiTest)
#include "tst_primelist_gui.moc"
