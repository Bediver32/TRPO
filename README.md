# Лабораторная работа № 7 — исключения, ветки и слияние

Оба приложения обрабатывают ошибки через исключения C++. Консоль выводит сообщение в stderr и возвращает именованный код из enum ExitCode. GUI показывает сообщение в диалоговом окне. Статическая библиотека проверяет числовые ограничения: обе границы должны быть натуральными, помещаться в int, а верхняя граница должна быть строго больше нижней.

## Сборка и проверка

Откройте primelist_qt.pro в Qt Creator и соберите проект комплектом Desktop Qt MinGW. Для консольного приложения задайте аргументы в **Проекты → Настройки запуска → Параметры командной строки**.

| Аргументы | Код | Результат |
| --- | --- | --- |
| 1 10 | 0 | 2, 3, 5, 7 |
| 1 | 1 | Не хватает верхней границы |
| 1 10abc | 2 | Неверный формат параметра B |
| 0 10 | 3 | Параметр A должен быть натуральным |
| 2 2 или 10 1 | 3 | B должна быть строго больше A |

Код 4 используется для других исключений стандартной библиотеки. В Windows CMD код завершения можно вывести командой echo %ERRORLEVEL% на следующей строке. Для GUI проверьте A = 1, B = 10, затем A = 2, B = 2 и A = abc, B = 10.

## Два локальных репозитория для сдачи

Оставьте клон ветки lab7 в папке 7-исходники. Скопируйте свою папку ЛР6 в папку 7, сохранив .git внутри primelist и primelist_gui. Откройте PowerShell в папке 7. Ниже команды для чистой копии ЛР6, в которой ветки exceptions ещё нет. При ошибке остановитесь на указанной команде.

~~~powershell
& {
    $ErrorActionPreference = 'Stop'
    $src = 'C:\Users\Danil\Desktop\TRPO\lab\7-исходники'
    $env:Path = 'C:\Qt\astyle-3.6.26-x64;' + $env:Path

    function Run-Git {
        git @args
        if ($LASTEXITCODE -ne 0) { throw "Ошибка Git: $args" }
    }

    function Apply-Source($repo, $rev, $file) {
        $blob = Run-Git -C $repo rev-parse --verify "${rev}:${repo}/${file}"
        Run-Git -C $repo update-index --cacheinfo "100644,$blob,$file"
        Run-Git -C $repo restore --worktree -- $file
    }

    foreach ($repo in 'primelist', 'primelist_gui') {
        if (-not (Test-Path "$repo\.git")) { throw "В $repo отсутствует .git" }
        if (Run-Git -C $repo status --porcelain) { throw "Есть изменения в $repo" }
        if (Run-Git -C $repo branch --list exceptions) { throw "Ветка exceptions уже есть в $repo" }
    }

    foreach ($repo in 'primelist', 'primelist_gui') {
        Run-Git -C $repo fetch --no-tags $src HEAD
    }

    $cliMain = Run-Git -C primelist branch --show-current
    Run-Git -C primelist switch -c exceptions
    Apply-Source primelist 7796f5f main.cpp
    Run-Git -C primelist commit -m 'Lab 7: handle command-line errors with exceptions'
    Run-Git -C primelist switch $cliMain
    Apply-Source primelist 3069bc6 main.cpp
    Run-Git -C primelist commit -m 'Lab 7: name the CLI exit codes'

    # Готовое разрешение конфликта сохранено в d70de7a.
    Run-Git -C primelist merge --no-ff --no-commit -s ours exceptions
    Apply-Source primelist d70de7a main.cpp
    Run-Git -C primelist commit -m 'Merge exceptions and retain named exit codes'
    Apply-Source primelist 5a1a000 main.cpp
    Run-Git -C primelist commit -m 'Lab 7: map exception types to named exit codes'

    $guiMain = Run-Git -C primelist_gui branch --show-current
    Run-Git -C primelist_gui switch -c exceptions
    Apply-Source primelist_gui 229fbb6 dialog.cpp
    Run-Git -C primelist_gui commit -m 'Lab 7: handle GUI input errors with exceptions'
    Run-Git -C primelist_gui switch $guiMain
    Run-Git -C primelist_gui merge --no-ff -m 'Merge exceptions' exceptions

    Copy-Item "$src\primelist_lib\primelist.cpp","$src\primelist_lib\primelist.h" .\primelist_lib\
    Apply-Source primelist c54ab21 main.cpp
    Run-Git -C primelist commit -m 'Lab 7: delegate numeric validation to the library'
    Apply-Source primelist_gui c54ab21 dialog.cpp
    Run-Git -C primelist_gui commit -m 'Lab 7: delegate numeric validation to the library'

    Run-Git -C primelist log --graph --oneline --all -9
    Run-Git -C primelist_gui log --graph --oneline --all -7
}
~~~

Проверьте, что в обоих графах есть ветка exceptions и коммит слияния с двумя родителями. Для сдачи упакуйте папку 7 с исходниками библиотеки и обеими папками .git внутри приложений. Каталог сборки в архив не нужен.
