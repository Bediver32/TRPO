import sys

from PyQt5.QtWidgets import QApplication

from dialog import Dialog

app = QApplication(sys.argv)
win = Dialog()
win.show()
sys.exit(app.exec())
