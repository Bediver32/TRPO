#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QLocale>
#include <QString>
#include <QStringList>
#include <QTranslator>

inline bool loadInterfaceTranslation(QTranslator &translator, const QLocale &locale) {
    const QStringList languages = locale.uiLanguages();
    for(const QString &language : languages) {
        switch(QLocale(language).language()) {
        case QLocale::Russian:
            return translator.load(QStringLiteral("primelist_gui_ru_RU"), QStringLiteral(":/i18n"));
        case QLocale::English:
            return translator.load(QStringLiteral("primelist_gui_en_US"), QStringLiteral(":/i18n"));
        default:
            break;
        }
    }
    return translator.load(QStringLiteral("primelist_gui_en_US"), QStringLiteral(":/i18n"));
}

#endif
