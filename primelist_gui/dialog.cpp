#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>
#include <QString>

#include <climits>
#include <stdexcept>

#include "primelist.h"

namespace {

int parseNatural(const QString &text, const QString &parameter) {
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
    if(number < 1 || number > INT_MAX) {
        throw std::out_of_range(Dialog::tr("Параметр %1 должен быть натуральным числом от 1 до %2.")
                                .arg(parameter).arg(INT_MAX).toStdString());
    }
    return static_cast<int>(number);
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
        const int a = parseNatural(ui->lineEditA->text(), QStringLiteral("A"));
        const int b = parseNatural(ui->lineEditB->text(), QStringLiteral("B"));
        if(b <= a) {
            throw std::out_of_range(tr("Верхняя граница B должна быть больше нижней границы A.").toStdString());
        }
        const auto primes = primelist(a, b);
        ui->listWidget->clear();
        for(int n : primes) {
            ui->listWidget->addItem(QString::number(n));
        }
    } catch(const std::exception &error) {
        QMessageBox::critical(this, tr("Ошибка"), QString::fromUtf8(error.what()));
    }
}
