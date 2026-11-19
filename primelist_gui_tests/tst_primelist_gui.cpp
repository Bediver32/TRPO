#include <QtTest>

#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>

#include <memory>

#include "dialog.h"

class PrimeListGuiTest : public QObject {
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void createsDialog();

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

QTEST_MAIN(PrimeListGuiTest)
#include "tst_primelist_gui.moc"
