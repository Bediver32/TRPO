# Лабораторная работа № 6

Откройте `primelist_qt.pro` в Qt Creator. Приложения используют Artistic Style с отступом 4 пробела и стилем Java. Библиотека использует стиль BSD с отступом 2 пробела. Правила сохранены в файле `.astylerc` каждого проекта.

## Qt Creator

Включите модуль **Beautifier** через **Справка → О модулях** и перезапустите IDE. В настройках **Стилизатор → Artistic Style** укажите путь к `AStyle.exe`, выберите правила из файла проекта `.astylerc` и включите форматирование при сохранении. Чтобы проверить настройки, откройте C++-файл и вызовите **Стилизатор → Artistic Style → Форматировать текущий файл**.

Добавьте каталог с `AStyle.exe` в PATH. Для коммитов из Qt Creator можно указать этот каталог в настройках **Управление версиями → Git → Добавить в начало PATH**. Для коммитов из терминала проверьте доступность команды `AStyle --version`.

## Перенос в два локальных репозитория

Клонируйте ветку `lab6` в отдельную папку `6-исходники`. Сделайте копию своей папки для сдачи ЛР5 под именем `6`, сохранив обе папки `.git` внутри приложений.

```powershell
git clone --branch lab6 https://github.com/Bediver32/TRPO.git "C:\Users\Danil\Desktop\TRPO\lab\6-исходники"
```

Откройте PowerShell в папке `6` и выполните команды по одной строке.

```powershell
$src = 'C:\Users\Danil\Desktop\TRPO\lab\6-исходники'
$tmp = Join-Path $env:TEMP ('trpo-lab6-' + [guid]::NewGuid())
$cli = git -C $src format-patch -o "$tmp\cli" c07604e..HEAD -- primelist/
$gui = git -C $src format-patch -o "$tmp\gui" c07604e..HEAD -- primelist_gui/
git -C primelist am -p2 $cli
git -C primelist_gui am -p2 $gui
Copy-Item "$src\primelist_lib\.astylerc","$src\primelist_lib\primelist.cpp","$src\primelist_lib\primelist_lib.pro" .\primelist_lib\
git -C primelist config --local core.hooksPath .githooks
git -C primelist_gui config --local core.hooksPath .githooks
```

В новой копии отдельного репозитория приложения настройку `core.hooksPath` нужно выполнить один раз. Правила `.astylerc` и сценарий обработчика хранятся в истории Git. При коммите сценарий проверяет все отслеживаемые `.cpp` и `.h`, форматирует их и добавляет результат в коммит. Если в исходнике есть изменения вне индекса, коммит останавливается, чтобы сохранить эти изменения отдельно. Перед повторным коммитом добавьте их в индекс или временно уберите через stash.

Для проверки измените пробелы около оператора в C++-файле, добавьте файл через `git add` и выполните `git commit`. Обработчик напечатает `pre-commit: formatted …`, а в коммите окажется отформатированная версия.
