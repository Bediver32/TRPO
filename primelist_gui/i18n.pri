CONFIG += lrelease embed_translations

TRANSLATIONS += \
    $$PWD/primelist_gui_ru_RU.ts \
    $$PWD/primelist_gui_en_US.ts

LRELEASE_DIR = $$library_config/.qm
RCC_DIR = $$library_config
HEADERS += $$PWD/translation.h
