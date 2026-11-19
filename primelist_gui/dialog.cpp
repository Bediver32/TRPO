#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>
#include <QAbstractButton>
#include <QString>

#include <limits>
#include <stdexcept>

#include "primelist.h"

namespace {

qlonglong parseDecimal(const QString &text, const QString &parameter) {
    const QString value = text.trimmed();
    if(value.isEmpty()) {
        throw std::invalid_argument(Dialog::tr("Параметр %1 введён неверно: поле пустое.")
                                    .arg(parameter).toStdString());
    }
    bool ok = false;
    const qlonglong number = value.toLongLong(&ok, 10);
    if(!ok) {
        throw std::invalid_argument(Dialog::tr("Параметр %1 введён неверно: значение не является десятичным целым числом.")
                                    .arg(parameter).toStdString());
    }
    return number;
}

} // namespace

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog) {
    ui->setupUi(this);
}

Dialog::~Dialog() {
    delete ui;
}

void Dialog::calculate() {
    try {
        const qlonglong a = parseDecimal(ui->lineEditA->text(), QStringLiteral("A"));
        const qlonglong b = parseDecimal(ui->lineEditB->text(), QStringLiteral("B"));
        const auto primes = primelist(a, b);
        ui->listWidget->clear();
        for(int n : primes) {
            ui->listWidget->addItem(QString::number(n));
        }
    } catch(const std::invalid_argument &error) {
        showError(QString::fromUtf8(error.what()));
    } catch(const PrimeRangeError &error) {
        switch(error.reason()) {
        case PrimeRangeError::Reason::LowerBound:
            showError(tr("Параметр A (нижняя граница) должен быть натуральным числом от 1 до %1.")
                      .arg(std::numeric_limits<int>::max()));
            break;
        case PrimeRangeError::Reason::UpperBound:
            showError(tr("Параметр B (верхняя граница) должен быть натуральным числом от 1 до %1.")
                      .arg(std::numeric_limits<int>::max()));
            break;
        case PrimeRangeError::Reason::BoundOrder:
            showError(tr("Верхняя граница B должна быть больше нижней границы A."));
            break;
        }
    } catch(const std::exception &) {
        showError(tr("Не удалось выполнить вычисление."));
    }
}

void Dialog::showError(const QString &message) {
    QMessageBox box(QMessageBox::Critical, tr("Ошибка"), message, QMessageBox::Ok, this);
    box.button(QMessageBox::Ok)->setText(tr("ОК"));
    box.exec();
}
