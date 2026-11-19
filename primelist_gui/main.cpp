#include "dialog.h"
#include "translation.h"

#include <QApplication>

int main(int argc, char * argv[]) {
    QApplication a(argc, argv);
    QTranslator translator;
    if(loadInterfaceTranslation(translator, QLocale::system())) {
        a.installTranslator(&translator);
    }
    Dialog w;
    w.show();
    return a.exec();
}
