import os
import sys

from PyQt5 import uic
from PyQt5.QtCore import pyqtSlot
from PyQt5.QtWidgets import QDialog, QMessageBox

# В PyCharm проект primelist присоединяется через File | Open | Attach, и модуль
# становится доступен сам. Этот фрагмент нужен только для запуска из консоли:
# он добавляет в путь поиска каталог соседнего проекта primelist.
_HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.join(_HERE, "..", "primelist"))

import primelist  # noqa: E402  (модуль из проекта приложения командной строки)


def parse_natural(text: str) -> int:
    """Преобразует текст поля ввода в натуральное число или бросает ValueError."""
    text = text.strip()
    if not (text.isascii() and text.isdigit()) or int(text) < 1:
        raise ValueError("нужно ввести натуральное число")
    return int(text)


class Dialog(QDialog):
    def __init__(self):
        super().__init__(None)
        uic.loadUi(os.path.join(_HERE, "dialog.ui"), self)

    @pyqtSlot()
    def calculate(self):
        try:
            a = parse_natural(self.lineEditA.text())
            b = parse_natural(self.lineEditB.text())
        except ValueError:
            QMessageBox.warning(self, "Ошибка", "Нужно ввести два натуральных числа.")
            return

        # Границы могут быть введены в любом порядке
        if a > b:
            a, b = b, a

        self.listWidget.clear()
        for n in primelist.prime_list(a, b):
            self.listWidget.addItem(str(n))
