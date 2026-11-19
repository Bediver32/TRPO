#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>
#include <QString>

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
    } catch(const std::exception &error) {
        QMessageBox::critical(this, tr("Ошибка"), QString::fromUtf8(error.what()));
    }
}
