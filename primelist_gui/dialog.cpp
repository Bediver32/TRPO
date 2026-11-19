#include "dialog.h"
#include "ui_dialog.h"

#include <QMessageBox>
#include <QString>

#include <algorithm>
#include <climits>

#include "../primelist/primelist.h"

namespace {

// Преобразует текст поля ввода в натуральное число, помещающееся в int.
// Возвращает false, если текст не является корректным натуральным числом.
bool parseNatural(const QString &text, int &value)
{
    bool ok = false;
    const qlonglong n = text.trimmed().toLongLong(&ok);
    if (!ok || n < 1 || n > INT_MAX) {
        return false;
    }
    value = static_cast<int>(n);
    return true;
}

} // namespace

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::calculate()
{
    int a = 0;
    int b = 0;
    if (!parseNatural(ui->lineEditA->text(), a) ||
        !parseNatural(ui->lineEditB->text(), b)) {
        QMessageBox::warning(this, tr("Ошибка"),
                             tr("Нужно ввести два натуральных числа (от 1 до %1).").arg(INT_MAX));
        return;
    }

    // Границы могут быть введены в любом порядке
    if (a > b) {
        std::swap(a, b);
    }

    ui->listWidget->clear();
    for (int n : primelist(a, b)) {
        ui->listWidget->addItem(QString::number(n));
    }
}
