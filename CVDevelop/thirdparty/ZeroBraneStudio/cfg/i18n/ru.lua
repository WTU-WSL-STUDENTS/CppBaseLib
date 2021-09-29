return {
  [0] = function(c) c = (c-9)%100 < 9 and 9 or (c-1)%10 return c == 0 and 1 or c < 4 and 2 or 3 end, -- plural
  ["%s event failed: %s"] = "%s обработчик события вернул ошибку: %s", -- src\editor\package.lua
  ["%s%% formatted..."] = "%s%% отформатировано...", -- src\editor\print.lua
  ["%s%% loaded..."] = "%s%% загружено...", -- src\editor\commands.lua
  ["&About"] = "&О программе", -- src\editor\menu_help.lua
  ["&Add Watch"] = "&Добавить выражение", -- src\editor\debugger.lua
  ["&Break"] = "Пр&ервать", -- src\editor\menu_project.lua
  ["&Close Page"] = "&Закрыть", -- src\editor\gui.lua, src\editor\menu_file.lua
  ["&Community"] = nil, -- src\editor\menu_help.lua
  ["&Compile"] = "&Компилировать", -- src\editor\menu_project.lua
  ["&Copy Value"] = "Копировать значение", -- src\editor\debugger.lua
  ["&Copy"] = "&Копировать", -- src\editor\gui.lua, src\editor\editor.lua, src\editor\menu_edit.lua
  ["&Default Layout"] = "Вид по &умолчанию", -- src\editor\menu_view.lua
  ["&Delete Watch"] = "&Удалить выражение", -- src\editor\debugger.lua
  ["&Delete"] = "&Удалить", -- src\editor\filetree.lua
  ["&Documentation"] = "Документация", -- src\editor\menu_help.lua
  ["&Edit Project Directory"] = "&Редактировать папку проекта", -- src\editor\filetree.lua
  ["&Edit Value"] = "&Редактировать значение", -- src\editor\debugger.lua
  ["&Edit Watch"] = "&Редактировать выражение", -- src\editor\debugger.lua
  ["&Edit"] = "&Правка", -- src\editor\menu_edit.lua
  ["&File"] = "&Файл", -- src\editor\menu_file.lua
  ["&Find"] = "&Найти", -- src\editor\menu_search.lua
  ["&Fold/Unfold All"] = "Св&ернуть/развернуть все", -- src\editor\menu_edit.lua
  ["&Frequently Asked Questions"] = "&Часто задаваемые вопросы", -- src\editor\menu_help.lua
  ["&Getting Started Guide"] = nil, -- src\editor\menu_help.lua
  ["&Help"] = "&Справка", -- src\editor\menu_help.lua
  ["&New Directory"] = "&Новая папка", -- src\editor\filetree.lua
  ["&New"] = "Соз&дать", -- src\editor\menu_file.lua
  ["&Open..."] = "&Открыть...", -- src\editor\menu_file.lua
  ["&Output/Console Window"] = "Окно &вывода/консоли", -- src\editor\menu_view.lua
  ["&Paste"] = "В&ставить", -- src\editor\gui.lua, src\editor\editor.lua, src\editor\menu_edit.lua
  ["&Print..."] = "Печать...", -- src\editor\print.lua
  ["&Project Page"] = "Страница проекта", -- src\editor\menu_help.lua
  ["&Project"] = "Пр&оект", -- src\editor\menu_project.lua
  ["&Redo"] = "Верну&ть", -- src\editor\gui.lua, src\editor\editor.lua, src\editor\menu_edit.lua
  ["&Rename"] = "Переименовать", -- src\editor\filetree.lua
  ["&Replace"] = "За&менить", -- src\editor\menu_search.lua
  ["&Run"] = "За&пустить", -- src\editor\menu_project.lua
  ["&Save"] = "&Сохранить", -- src\editor\gui.lua, src\editor\menu_file.lua
  ["&Search"] = "По&иск", -- src\editor\menu_search.lua
  ["&Select Command"] = "Выбрать команду", -- src\editor\gui.lua
  ["&Sort"] = "&Cортировать", -- src\editor\menu_edit.lua
  ["&Stack Window"] = "Окно &стека", -- src\editor\menu_view.lua
  ["&Start Debugger Server"] = "Запустить сервер отла&дки", -- src\editor\menu_project.lua
  ["&Status Bar"] = "Панель состояния", -- src\editor\menu_view.lua
  ["&Tool Bar"] = "Панель инструментов", -- src\editor\menu_view.lua
  ["&Tools"] = nil, -- src\editor\package.lua
  ["&Tutorials"] = "&Обучающие материалы", -- src\editor\menu_help.lua
  ["&Undo"] = "&Отменить", -- src\editor\gui.lua, src\editor\editor.lua, src\editor\menu_edit.lua
  ["&View"] = "&Вид", -- src\editor\menu_view.lua
  ["&Watch Window"] = "Окно &выражений", -- src\editor\menu_view.lua
  ["About %s"] = "О %s", -- src\editor\menu_help.lua
  ["Add To Scratchpad"] = "Добавить в черновик", -- src\editor\editor.lua
  ["Add Watch Expression"] = "Добавить выражение", -- src\editor\editor.lua
  ["All files"] = "Все файлы", -- src\editor\commands.lua
  ["Allow external process to start debugging"] = "Разрешить внешнему процессу начать отладку", -- src\editor\menu_project.lua
  ["Analyze the source code"] = "Проанализировать исходный код", -- src\editor\inspect.lua
  ["Analyze"] = "Анализировать", -- src\editor\inspect.lua
  ["Auto Complete Identifiers"] = "Автодополнение идентификаторов", -- src\editor\menu_edit.lua
  ["Auto complete while typing"] = "Автоматически дополнять идентификаторы при наборе", -- src\editor\menu_edit.lua
  ["Binary file is shown as read-only as it is only partially loaded."] = "Двоичный файл открыт только для чтения, так как загружен частично.", -- src\editor\commands.lua
  ["Bookmark"] = "Закладка", -- src\editor\menu_edit.lua
  ["Break execution at the next executed line of code"] = "Прервать выполнение на следующей строке", -- src\editor\toolbar.lua, src\editor\menu_project.lua
  ["Breakpoint"] = "Точка останова", -- src\editor\menu_project.lua
  ["C&lear Console Window"] = "Очистить ок&но консоли", -- src\editor\gui.lua
  ["C&lear Output Window"] = "Очистить ок&но вывода", -- src\editor\gui.lua, src\editor\menu_project.lua
  ["C&omment/Uncomment"] = "Зако&мментировать/раскомментировать", -- src\editor\menu_edit.lua
  ["Can't evaluate the expression while the application is running."] = "Невозможно вычеслить значение выражения пока приложение выполняется.", -- src\editor\debugger.lua
  ["Can't open file '%s': %s"] = "Ошибка открытия файла '%s': %s", -- src\editor\findreplace.lua, src\editor\package.lua, src\editor\inspect.lua, src\editor\outline.lua
  ["Can't process auto-recovery record; invalid format: %s."] = "Ошибка обработки записи автоматического восстановления; неверный формат: %s.", -- src\editor\commands.lua
  ["Can't replace in read-only text."] = "Ошибка замены в тексте не доступном для записи.", -- src\editor\findreplace.lua
  ["Can't run the entry point script ('%s')."] = "Ошибка выполнения стартового скрипта ('%s').", -- src\editor\debugger.lua
  ["Can't start debugger server at %s:%d: %s."] = "Невозможно запустить сервер отладки %s:%d: %s", -- src\editor\debugger.lua
  ["Can't start debugging for '%s'."] = "Невозможно начать отладку для '%s'.", -- src\editor\debugger.lua
  ["Can't start debugging session due to internal error '%s'."] = "Невозможно начать отладочную сессию из-за внутренней ошибки '%s'.", -- src\editor\debugger.lua
  ["Can't start debugging without an opened file or with the current file not being saved."] = "Невозможно начать отладку без открытого файла или с несохраненным текущим файлом.", -- src\editor\debugger.lua
  ["Can't stop debugger server as it is not started."] = "Невозможно остановить сервер отладки пока он не запущен", -- src\editor\debugger.lua
  ["Cancelled by the user."] = "Отменено пользователем.", -- src\editor\findreplace.lua
  ["Choose a directory to map"] = "Выберите папку для добавления в список", -- src\editor\filetree.lua
  ["Choose a project directory"] = "Выберите папку проекта", -- src\editor\toolbar.lua, src\editor\menu_project.lua, src\editor\filetree.lua
  ["Choose a search directory"] = "Выберите папку поиска", -- src\editor\findreplace.lua
  ["Choose..."] = "Выбрать...", -- src\editor\findreplace.lua, src\editor\menu_project.lua, src\editor\filetree.lua
  ["Clear Bookmarks In File"] = "Убрать закладки файла", -- src\editor\markers.lua
  ["Clear Bookmarks In Project"] = "Убрать закладки проекта", -- src\editor\markers.lua
  ["Clear Breakpoints In File"] = "Убрать точки останова файла", -- src\editor\markers.lua
  ["Clear Breakpoints In Project"] = "Убрать точки останова проекта", -- src\editor\markers.lua
  ["Clear Items"] = "Очистить список", -- src\editor\findreplace.lua, src\editor\menu_file.lua
  ["Clear items from this list"] = "Удалить элементы из списка", -- src\editor\menu_file.lua
  ["Clear the output window before compiling or debugging"] = "Очистить окно вывода перед компиляцией или отладкой", -- src\editor\menu_project.lua
  ["Close &Other Pages"] = "Закрыть &остальные вкладки", -- src\editor\gui.lua
  ["Close A&ll Pages"] = "Закрыть &все вкладки", -- src\editor\gui.lua
  ["Close Search Results Pages"] = "Закрыть окна с результатами поиска", -- src\editor\gui.lua
  ["Close the current editor window"] = "Закрыть текущее окно редактирования", -- src\editor\menu_file.lua
  ["Co&ntinue"] = "Пр&одолжить", -- src\editor\menu_project.lua
  ["Col: %d"] = "Стб: %d", -- src\editor\editor.lua
  ["Command Line Parameters..."] = "Параметры командной строки...", -- src\editor\gui.lua, src\editor\menu_project.lua
  ["Command line parameters"] = "Параметры командной строки", -- src\editor\menu_project.lua
  ["Comment or uncomment current or selected lines"] = "Закомментировать или раскомментировать текущую или выделенные строки", -- src\editor\menu_edit.lua
  ["Compilation error"] = "Ошибка компиляции", -- src\editor\commands.lua, src\editor\debugger.lua
  ["Compilation successful; %.0f%% success rate (%d/%d)."] = "Компиляция завершена успешно; процент успеха: %.0f%% (%d/%d).", -- src\editor\commands.lua
  ["Compile the current file"] = "Скомпилировать текущий файл", -- src\editor\toolbar.lua, src\editor\menu_project.lua
  ["Complete &Identifier"] = "Дополнить &идентификатор", -- src\editor\menu_edit.lua
  ["Complete the current identifier"] = "Дополнить текущий идентификатор", -- src\editor\menu_edit.lua
  ["Consider removing backslash from escape sequence '%s'."] = "Рассмотрите вариант удаления backslash из строки '%s'.", -- src\editor\commands.lua
  ["Copy Full Path"] = "Скопировать полный путь", -- src\editor\gui.lua, src\editor\filetree.lua
  ["Copy selected text to clipboard"] = "Скопировать выделенный текст в буфер обмена", -- src\editor\menu_edit.lua
  ["Correct &Indentation"] = "Откорректировать отступ", -- src\editor\menu_edit.lua
  ["Couldn't activate file '%s' for debugging; continuing without it."] = "Невозможно открыть файл '%s' для отладки; выполнение будет продолжено без него.", -- src\editor\debugger.lua
  ["Create an empty document"] = "Создать новый документ", -- src\editor\toolbar.lua, src\editor\menu_file.lua
  ["Cu&t"] = "Вы&резать", -- src\editor\gui.lua, src\editor\editor.lua, src\editor\menu_edit.lua
  ["Cut selected text to clipboard"] = "Вырезать выделенный текст в буфер обмена", -- src\editor\menu_edit.lua
  ["Debugger server started at %s:%d."] = "Сервер отладки запущен на %s:%d.", -- src\editor\debugger.lua
  ["Debugger server stopped at %s:%d."] = "Сервер отладки остановлен %s:%d.", -- src\editor\debugger.lua
  ["Debugging session completed (%s)."] = "Отладочная сессия завершена (%s).", -- src\editor\debugger.lua
  ["Debugging session started in '%s'."] = "Отладочная сессия запущена в '%s'.", -- src\editor\debugger.lua
  ["Debugging suspended at '%s:%s' (couldn't activate the file)."] = "Отладка остановлена в '%s:%s' (невозможно активировать файл).", -- src\editor\debugger.lua
  ["Detach &Process"] = "Отсоединить процесс", -- src\editor\menu_project.lua
  ["Disable Indexing For '%s'"] = "Запретить индексирование для '%s'", -- src\editor\outline.lua
  ["Do you want to delete '%s'?"] = "Удалить '%s'?", -- src\editor\filetree.lua
  ["Do you want to overwrite it?"] = "Переписать его?", -- src\editor\commands.lua
  ["Do you want to reload it?"] = "Перезагрузить его?", -- src\editor\editor.lua
  ["Do you want to save the changes to '%s'?"] = "Сохранить изменения в '%s'?", -- src\editor\commands.lua
  ["E&xit"] = "Вы&ход", -- src\editor\menu_file.lua
  ["Enable Indexing"] = "Разрешить индекcирование", -- src\editor\outline.lua
  ["Enter Lua code and press Enter to run it."] = "Введите код на Lua и нажмите Enter для выполнения.", -- src\editor\shellbox.lua
  ["Enter command line parameters"] = "Введите параметры командной строки", -- src\editor\menu_project.lua
  ["Enter replacement text"] = "Введите текст замены", -- src\editor\editor.lua
  ["Error while loading API file: %s"] = "Ошибка загрузки файла определений API: %s", -- src\editor\autocomplete.lua
  ["Error while loading configuration file: %s"] = "Ошибка загрузки файла конфигурации: %s", -- src\editor\style.lua
  ["Error while processing API file: %s"] = "Ошибка обработки файла определений API: %s", -- src\editor\autocomplete.lua
  ["Error while processing configuration file: %s"] = "Ошибка обработки файла конфигурации: %s", -- src\editor\style.lua
  ["Error"] = "Ошибка", -- src\editor\package.lua
  ["Evaluate In Console"] = "Выполнить в консоли", -- src\editor\editor.lua
  ["Execute the current project/file and keep updating the code to see immediate results"] = "Запустить текущий проект/файл и продолжать вносить изменения в код с немедленным выводом результатов", -- src\editor\menu_project.lua
  ["Execute the current project/file"] = "Запустить текущий проект/файл", -- src\editor\toolbar.lua, src\editor\menu_project.lua
  ["Execution error"] = "Ошибка выполнения", -- src\editor\debugger.lua
  ["Exit program"] = "Выйти из программы", -- src\editor\menu_file.lua
  ["File '%s' has been modified on disk."] = "Файл '%s' был изменен на диске.", -- src\editor\editor.lua
  ["File '%s' has more recent timestamp than restored '%s'; please review before saving."] = "Файл '%s' имеет более позднее время модификации, чем восстановленный '%s'; пожалуйста просмотрите его перед сохранением.", -- src\editor\commands.lua
  ["File '%s' is missing and can't be recovered."] = "Файл '%s' отсутствует и не может быть восстановлен.", -- src\editor\commands.lua
  ["File '%s' no longer exists."] = "Файл '%s' больше не существует.", -- src\editor\menu_file.lua, src\editor\editor.lua
  ["File already exists."] = "Файл уже существует.", -- src\editor\commands.lua
  ["File history"] = "История файлов", -- src\editor\menu_file.lua
  ["Find &In Files"] = "Н&айти в файлах", -- src\editor\menu_search.lua
  ["Find &Next"] = "Найти &далее", -- src\editor\menu_search.lua
  ["Find &Previous"] = "Найти &ранее", -- src\editor\menu_search.lua
  ["Find and insert library function"] = "Найти и вставить библиотечную функцию", -- src\editor\menu_search.lua
  ["Find and replace text in files"] = "Найти и заменить текст в файлах", -- src\editor\menu_search.lua
  ["Find and replace text"] = "Найти и заменить текст", -- src\editor\toolbar.lua, src\editor\menu_search.lua
  ["Find in files"] = "Найти в файлах", -- src\editor\toolbar.lua
  ["Find next"] = "Найти далее", -- src\editor\toolbar.lua
  ["Find text in files"] = "Найти текст в файлах", -- src\editor\menu_search.lua
  ["Find text"] = "Найти текст", -- src\editor\toolbar.lua, src\editor\menu_search.lua
  ["Find the earlier text occurence"] = "Найти предыдущее вхождение текста", -- src\editor\menu_search.lua
  ["Find the next text occurrence"] = "Найти следующее вхождение текста", -- src\editor\menu_search.lua
  ["Fold or unfold all code folds"] = "Свернуть или развернуть все блоки кода", -- src\editor\menu_edit.lua
  ["Fold or unfold current line"] = "Свернуть или развернуть текущую строку кода", -- src\editor\menu_edit.lua
  ["Fold/Unfold Current &Line"] = "Свернуть/развернуть текущую строку", -- src\editor\menu_edit.lua
  ["Formatting page %d..."] = "Отформатировано страницы %d...", -- src\editor\print.lua
  ["Found %d instance."] = {"Найдено %d совпадение.", "Найдено %d совпадения.", "Найдено %d совпадений."}, -- src\editor\findreplace.lua
  ["Found auto-recovery record and restored saved session."] = "Найдена запись авто-восстановления и восстановлена сохраненная сессия.", -- src\editor\commands.lua
  ["Found match in '%s'."] = "Найдено совпадение в '%s'.", -- src\editor\findreplace.lua
  ["Full &Screen"] = "Во весь экр&ан", -- src\editor\menu_view.lua
  ["Go To Definition"] = "Перейти к определению", -- src\editor\editor.lua
  ["Go To File..."] = "Перейти к файлу...", -- src\editor\menu_search.lua
  ["Go To Line..."] = "Перейти к строке...", -- src\editor\menu_search.lua
  ["Go To Next Bookmark"] = "Перейти к следующей закладке", -- src\editor\menu_edit.lua
  ["Go To Next Breakpoint"] = "Перейти к следующей точке останова", -- src\editor\menu_project.lua
  ["Go To Previous Bookmark"] = "Перейти к предыдущей закладке", -- src\editor\menu_edit.lua
  ["Go To Previous Breakpoint"] = "Перейти к предыдущей точке останова", -- src\editor\menu_project.lua
  ["Go To Symbol..."] = "Перейти к символу...", -- src\editor\menu_search.lua
  ["Go to file"] = "Перейти к файлу", -- src\editor\menu_search.lua
  ["Go to line"] = "Перейти к строке", -- src\editor\menu_search.lua
  ["Go to symbol"] = "Перейти к символу", -- src\editor\menu_search.lua
  ["Hide '.%s' Files"] = "Спрятать '.%s' файлы", -- src\editor\filetree.lua
  ["INS"] = "ВСТ", -- src\editor\editor.lua
  ["Ignore and don't index symbols from files in the selected directory"] = "Игнорировать и не индексировать файлы в выбранной папке", -- src\editor\outline.lua
  ["Ignored error in debugger initialization code: %s."] = "Проигнорирована ошибка в коде инициализации отладчика: %s.", -- src\editor\debugger.lua
  ["Indexing %d files: '%s'..."] = "Индексировано %d: '%s'...", -- src\editor\outline.lua
  ["Indexing completed."] = "Индексирование закончено.", -- src\editor\outline.lua
  ["Insert Library Function..."] = "Вставить библиотечную функцию...", -- src\editor\menu_search.lua
  ["Known Files"] = "Файлы Lua", -- src\editor\commands.lua
  ["Ln: %d"] = "Стр: %d", -- src\editor\editor.lua
  ["Local console"] = "Локальная консоль", -- src\editor\gui.lua, src\editor\shellbox.lua
  ["Lua &Interpreter"] = "&Интерпретатор Lua", -- src\editor\menu_project.lua
  ["Map Directory..."] = "Добавить папку в список", -- src\editor\filetree.lua
  ["Mapped remote request for '%s' to '%s'."] = "Удаленный запрос для '%s' отображен на '%s'.", -- src\editor\debugger.lua
  ["Markers Window"] = "Окно маркеров", -- src\editor\menu_view.lua
  ["Markers"] = "Маркеры", -- src\editor\markers.lua
  ["Match case"] = "Совпадение регистра", -- src\editor\toolbar.lua
  ["Match whole word"] = "Совпадение целого слова", -- src\editor\toolbar.lua
  ["Mixed end-of-line encodings detected."] = "Обнаружены смешанные символы конца строки.", -- src\editor\commands.lua
  ["Navigate"] = "Перейти", -- src\editor\menu_search.lua
  ["New &File"] = "Новый файл", -- src\editor\filetree.lua
  ["OVR"] = "ЗАМ", -- src\editor\editor.lua
  ["Open With Default Program"] = "Открыть используя программу по умолчанию", -- src\editor\filetree.lua
  ["Open an existing document"] = "Открыть существующий документ", -- src\editor\toolbar.lua, src\editor\menu_file.lua
  ["Open file"] = "Открыть файл", -- src\editor\commands.lua
  ["Outline Window"] = "Окно структуры", -- src\editor\menu_view.lua
  ["Outline"] = "Структура", -- src\editor\outline.lua
  ["Output (running)"] = "Вывод (запущен)", -- src\editor\debugger.lua, src\editor\output.lua
  ["Output (suspended)"] = "Вывод (приостановлен)", -- src\editor\debugger.lua
  ["Output"] = "Вывод", -- src\editor\debugger.lua, src\editor\output.lua, src\editor\gui.lua, src\editor\settings.lua
  ["Page Setup..."] = "Параметры страницы...", -- src\editor\print.lua
  ["Paste text from the clipboard"] = "Вставить текст из буфера обмена", -- src\editor\menu_edit.lua
  ["Preferences"] = "Настройки", -- src\editor\menu_edit.lua
  ["Prepend '!' to force local execution."] = "Укажите '=' в начале выражения для выполнения в локальной консоли", -- src\editor\shellbox.lua
  ["Prepend '=' to show complex values on multiple lines."] = "Укажите '=' в начале выражения для отображения сложных значений на нескольких строках.", -- src\editor\shellbox.lua
  ["Press cancel to abort."] = "Нажмите Отмена для завершения.", -- src\editor\commands.lua
  ["Print the current document"] = "Печать текущего документа", -- src\editor\print.lua
  ["Program '%s' started in '%s' (pid: %d)."] = "Программа '%s' запущена в '%s' (pid: %d).", -- src\editor\output.lua
  ["Program can't start because conflicting process is running as '%s'."] = "Программа не может быть запущена из-за конфликтующего процесса, выполняющегося как '%s'.", -- src\editor\output.lua
  ["Program completed in %.2f seconds (pid: %d)."] = "Программа завершена за %.2f секунд (pid: %d).", -- src\editor\output.lua
  ["Program starting as '%s'."] = "Программа запускается как '%s'.", -- src\editor\output.lua
  ["Program stopped (pid: %d)."] = "Программа завершена (pid: %d).", -- src\editor\debugger.lua
  ["Program unable to run as '%s'."] = "Программа не может быть запущена как '%s'.", -- src\editor\output.lua
  ["Project Directory"] = "Папка проекта", -- src\editor\menu_project.lua, src\editor\filetree.lua
  ["Project history"] = "История проектов", -- src\editor\menu_file.lua
  ["Project"] = "Проект", -- src\editor\filetree.lua
  ["Project/&FileTree Window"] = "Окно &проекта/списка файлов", -- src\editor\menu_view.lua
  ["Provide command line parameters"] = "Установить параметры командной строки", -- src\editor\menu_project.lua
  ["Queued %d files to index."] = "Помещено %d файлов в очередь на индексирование.", -- src\editor\commandbar.lua
  ["R/O"] = "R/O", -- src\editor\editor.lua
  ["R/W"] = "R/W", -- src\editor\editor.lua
  ["Re&place In Files"] = "Замени&ть в файлах", -- src\editor\menu_search.lua
  ["Re-indent selected lines"] = "Исправить отступ в выделенных строчках", -- src\editor\menu_edit.lua
  ["Reached end of selection and wrapped around."] = "Достигнут конец выделенного текста и произведен возврат к началу.", -- src\editor\findreplace.lua
  ["Reached end of text and wrapped around."] = "Достигнут конец текста и произведен возврат к началу.", -- src\editor\findreplace.lua
  ["Recent Files"] = "Недавние файлы", -- src\editor\menu_file.lua
  ["Recent Projects"] = "Недавние проекты", -- src\editor\menu_file.lua
  ["Redo last edit undone"] = "Вернуть последнее отмененное изменение", -- src\editor\menu_edit.lua
  ["Refresh Index"] = "Обновить индекс", -- src\editor\outline.lua
  ["Refresh Search Results"] = "Обновить результаты поиска", -- src\editor\gui.lua
  ["Refresh indexed symbols from files in the selected directory"] = "Обновить индекс символов из файлов в выбранной папке", -- src\editor\outline.lua
  ["Refresh"] = "Oбновление", -- src\editor\filetree.lua
  ["Refused a request to start a new debugging session as there is one in progress already."] = "Отказано в запросе на запуск новой отладочной сессии, поскольку одна сессия уже выполняется.", -- src\editor\debugger.lua
  ["Regular expression"] = "Регулярное выражение", -- src\editor\toolbar.lua
  ["Remote console"] = "Удаленная консоль", -- src\editor\shellbox.lua
  ["Rename All Instances"] = "Переименовать все совпадения", -- src\editor\editor.lua
  ["Replace All Selections"] = "Заменить все выделенные фрагменты", -- src\editor\editor.lua
  ["Replace all"] = "Заменить все", -- src\editor\toolbar.lua
  ["Replace next instance"] = "Заменить следующее совпадение", -- src\editor\toolbar.lua
  ["Replaced %d instance."] = {"Заменено %d совпадение.", "Заменено %d совпадения.", "Заменено %d совпадений."}, -- src\editor\findreplace.lua
  ["Replaced an invalid UTF8 character with %s."] = "Некорректный символ UTF8 заменен на %s.", -- src\editor\commands.lua
  ["Reset to default layout"] = "Установить расположение окон по умолчанию", -- src\editor\menu_view.lua
  ["Run As Scratchpad"] = "Запустить как черновик", -- src\editor\menu_project.lua
  ["Run To Cursor"] = "Выполнить до курсора", -- src\editor\menu_project.lua, src\editor\editor.lua
  ["Run as Scratchpad"] = "Запустить как черновик", -- src\editor\toolbar.lua
  ["Run to cursor"] = "Выполнить до курсора", -- src\editor\toolbar.lua, src\editor\menu_project.lua
  ["S&top Debugging"] = "&Завершить отладку", -- src\editor\menu_project.lua
  ["S&top Process"] = "&Завершить процесс", -- src\editor\menu_project.lua
  ["Save &As..."] = "Сохранить &как...", -- src\editor\gui.lua, src\editor\menu_file.lua
  ["Save A&ll"] = "Сохранить &все", -- src\editor\menu_file.lua
  ["Save Changes?"] = "Сохранить изменения?", -- src\editor\commands.lua
  ["Save all open documents"] = "Сохранить все открытые документы", -- src\editor\toolbar.lua, src\editor\menu_file.lua
  ["Save file as"] = "Сохранить файл как", -- src\editor\commands.lua
  ["Save file?"] = "Сохранить файл?", -- src\editor\commands.lua
  ["Save the current document to a file with a new name"] = "Сохранить текущий документ в файл под новым именем", -- src\editor\menu_file.lua
  ["Save the current document"] = "Сохранить текущий документ", -- src\editor\toolbar.lua, src\editor\menu_file.lua
  ["Saved auto-recover at %s."] = "Сохранено авто-восст в %s.", -- src\editor\commands.lua
  ["Scratchpad error"] = "Ошибка в черновике", -- src\editor\debugger.lua
  ["Search direction"] = "Направление поиска", -- src\editor\toolbar.lua
  ["Search in selection"] = "Поиск в выделенном тексте", -- src\editor\toolbar.lua
  ["Search in subdirectories"] = "Поиск во вложенных папках", -- src\editor\toolbar.lua
  ["Searching for '%s'."] = "Поиск '%s'.", -- src\editor\findreplace.lua
  ["Searching in '%s'."] = "Поиск в '%s'.", -- src\editor\findreplace.lua
  ["Sel: %d/%d"] = "Выд: %d/%d", -- src\editor\editor.lua
  ["Select &All"] = "Выделить &все", -- src\editor\gui.lua, src\editor\editor.lua, src\editor\menu_edit.lua
  ["Select And Find Next"] = "Выделить и найти далее", -- src\editor\menu_search.lua
  ["Select And Find Previous"] = "Выделить и найти ранее", -- src\editor\menu_search.lua
  ["Select all text in the editor"] = "Выделить весь текст в редакторе", -- src\editor\menu_edit.lua
  ["Select the word under cursor and find its next occurrence"] = "Выделить слово под курсором и найти далее", -- src\editor\menu_search.lua
  ["Select the word under cursor and find its previous occurrence"] = "Выделить слово под курсором и найти ранее", -- src\editor\menu_search.lua
  ["Set As Start File"] = "Установить как файл запуска", -- src\editor\gui.lua, src\editor\filetree.lua
  ["Set From Current File"] = "Установить по текущему файлу", -- src\editor\menu_project.lua
  ["Set To Project Directory"] = "Установить папку проекта", -- src\editor\findreplace.lua
  ["Set To Selected Directory"] = "Установить выбранную папку как проект", -- src\editor\filetree.lua
  ["Set project directory from current file"] = "Установить папку проекта по текущему файлу", -- src\editor\toolbar.lua, src\editor\menu_project.lua
  ["Set project directory to the selected one"] = "Установить выбранную папку как папку проекта", -- src\editor\filetree.lua
  ["Set search directory"] = "Установить папку поиска", -- src\editor\toolbar.lua
  ["Set the interpreter to be used"] = "Установить используемый интерпретатор", -- src\editor\menu_project.lua
  ["Set the project directory to be used"] = "Установить используемую папку проекта", -- src\editor\menu_project.lua, src\editor\filetree.lua
  ["Settings: System"] = "Установки: Системы", -- src\editor\menu_edit.lua
  ["Settings: User"] = "Установки: Пользователя", -- src\editor\menu_edit.lua
  ["Show &Tooltip"] = "Показать &подсказку", -- src\editor\menu_edit.lua
  ["Show All Files"] = "Показать все файлы", -- src\editor\filetree.lua
  ["Show Hidden Files"] = "Показать спрятанные файлы", -- src\editor\filetree.lua
  ["Open Containing Folder"] = "Показать файл в папке", -- src\editor\gui.lua, src\editor\filetree.lua
  ["Show all files"] = "Показать все файлы", -- src\editor\filetree.lua
  ["Show context"] = "Показать контекст", -- src\editor\toolbar.lua
  ["Show files previously hidden"] = "Показать ранее спрятанные файлы", -- src\editor\filetree.lua
  ["Show multiple result windows"] = "Показать результаты поиска в разных окнах", -- src\editor\toolbar.lua
  ["Show tooltip for current position; place cursor after opening bracket of function"] = "Показать подсказку в текущей позиции; переместите курсор в позицию после открывающей скобки функции", -- src\editor\menu_edit.lua
  ["Show/Hide the status bar"] = "Показать/спрятать панель состояния", -- src\editor\menu_view.lua
  ["Show/Hide the toolbar"] = "Показать/спрятать панель инструментов", -- src\editor\menu_view.lua
  ["Sort By Name"] = "Сортировать по имени", -- src\editor\outline.lua
  ["Sort selected lines"] = "Отсортировать выделенные строки", -- src\editor\menu_edit.lua
  ["Source"] = "Исходный код", -- src\editor\menu_edit.lua
  ["Stack"] = "Стек", -- src\editor\debugger.lua
  ["Start &Debugging"] = "Начать &отладку", -- src\editor\menu_project.lua
  ["Start or continue debugging"] = "Начать или продолжить отладку", -- src\editor\toolbar.lua, src\editor\menu_project.lua
  ["Step &Into"] = "&Войти", -- src\editor\menu_project.lua
  ["Step &Over"] = "&Следующая строка", -- src\editor\menu_project.lua
  ["Step O&ut"] = "В&ыйти", -- src\editor\menu_project.lua
  ["Step into"] = "Войти в функцию", -- src\editor\toolbar.lua, src\editor\menu_project.lua
  ["Step out of the current function"] = "Выйти из текущей функции", -- src\editor\toolbar.lua, src\editor\menu_project.lua
  ["Step over"] = "Перейти на следующую строку", -- src\editor\toolbar.lua, src\editor\menu_project.lua
  ["Stop debugging and continue running the process"] = "Завершить отладку и продолжить текущий процесс", -- src\editor\toolbar.lua, src\editor\menu_project.lua
  ["Stop the currently running process"] = "Завершить текущий процесс", -- src\editor\toolbar.lua, src\editor\menu_project.lua
  ["Switch to or from full screen mode"] = "Переключить полноэкранный режим", -- src\editor\menu_view.lua
  ["Symbol Index"] = "Индекс символов", -- src\editor\outline.lua
  ["Text not found."] = "Текст не найден.", -- src\editor\findreplace.lua
  ["The API file must be located in a subdirectory of the API directory."] = "Файл определений API должен быть расположен внутри папки API.", -- src\main.lua
  ["Toggle Bookmark"] = "Установить/Снять закладку", -- src\editor\markers.lua, src\editor\menu_edit.lua
  ["Toggle Breakpoint"] = "Установить/Снять точку останова", -- src\editor\markers.lua, src\editor\menu_project.lua
  ["Toggle bookmark"] = "Установить/Снять закладку", -- src\editor\toolbar.lua, src\editor\menu_edit.lua, src\editor\markers.lua
  ["Toggle breakpoint"] = "Установить/Снять точку останова", -- src\editor\markers.lua, src\editor\toolbar.lua
  ["Tr&ace"] = "Т&рассировка", -- src\editor\menu_project.lua
  ["Trace execution showing each executed line"] = "Отслеживать выполнение, показывая каждую выполненную строку", -- src\editor\menu_project.lua
  ["Unable to create directory '%s'."] = "Ошибка создания папки '%s'.", -- src\editor\filetree.lua
  ["Unable to create file '%s'."] = "Ошибка создания файла '%s'.", -- src\editor\filetree.lua
  ["Unable to delete directory '%s': %s"] = "Ошибка удаления папки '%s': %s", -- src\editor\filetree.lua
  ["Unable to delete file '%s': %s"] = "Ошибка удаления файла '%s': %s", -- src\editor\filetree.lua
  ["Unable to load file '%s'."] = "Ошибка загрузки файла '%s'.", -- src\editor\commands.lua
  ["Unable to rename file '%s'."] = "Ошибка переименования файла '%s'.", -- src\editor\filetree.lua
  ["Unable to save file '%s': %s"] = "Ошибка сохранения файла '%s': %s", -- src\editor\commands.lua
  ["Unable to stop program (pid: %d), code %d."] = "Невозможно завершить программу (pid: %d), код %d.", -- src\editor\debugger.lua
  ["Undo last edit"] = "Отменить последнее действие", -- src\editor\menu_edit.lua
  ["Unmap Directory"] = "Убрать папку из списка", -- src\editor\filetree.lua
  ["Unset '%s' As Start File"] = "Отменить '%s' как файл запуска", -- src\editor\gui.lua, src\editor\filetree.lua
  ["Updated %d file."] = {"Обновлен %d файл.", "Обновлено %d файла.", "Обновлено %d файлов."}, -- src\editor\findreplace.lua
  ["Updating symbol index and settings..."] = "Обновляется индекс символов и установки...", -- src\editor\outline.lua
  ["Use %s to close."] = "Используйте %s для закрытия.", -- src\editor\findreplace.lua
  ["Use '%s' to see full description."] = "Используйте '%s' для полного описания.", -- src\editor\editor.lua
  ["Use '%s' to show line endings and '%s' to convert them."] = "Используйте '%s' для отображения символов конца строки и '%s' для их преобразования.", -- src\editor\commands.lua
  ["Use 'clear' to clear the shell output and the history."] = "Используйте команду 'clear' для очистки содержимого окна и истории.", -- src\editor\shellbox.lua
  ["Use 'reset' to clear the environment."] = "Используйте команду 'reset' для очистки среды переменных.", -- src\editor\shellbox.lua
  ["Use Shift-Enter for multiline code."] = "Используйте Shift-Enter для многострочного кода.", -- src\editor\shellbox.lua
  ["View the markers window"] = "Показать окно маркеров", -- src\editor\menu_view.lua
  ["View the outline window"] = "Показать окно структуры", -- src\editor\menu_view.lua
  ["View the output/console window"] = "Показать окно вывода/консоли", -- src\editor\menu_view.lua
  ["View the project/filetree window"] = "Показать окно проекта/списка файлов", -- src\editor\menu_view.lua
  ["View the stack window"] = "Показать окно стека", -- src\editor\toolbar.lua, src\editor\menu_view.lua
  ["View the watch window"] = "Показать окно выражений", -- src\editor\toolbar.lua, src\editor\menu_view.lua
  ["Watch"] = "Выражение", -- src\editor\debugger.lua
  ["Welcome to the interactive Lua interpreter."] = "Добро пожаловать в интерактивный интерпретатор Lua.", -- src\editor\shellbox.lua
  ["Wrap around"] = "Продолжить сначала", -- src\editor\toolbar.lua
  ["You must save the program first."] = "Вы должны сначала сохранить программу.", -- src\editor\commands.lua
  ["Zoom In"] = "Приблизить", -- src\editor\menu_view.lua
  ["Zoom Out"] = "Удалить", -- src\editor\menu_view.lua
  ["Zoom to 100%"] = "Установить 100%", -- src\editor\menu_view.lua
  ["Zoom"] = "Установить масштаб", -- src\editor\menu_view.lua
  ["on line %d"] = "в строке %d", -- src\editor\debugger.lua, src\editor\editor.lua, src\editor\commands.lua
  ["traced %d instruction"] = {"выполнена %d инструкция", "выполнено %d инструкции", "выполнено %d инструкций"}, -- src\editor\debugger.lua
  ["unknown error"] = "неизвестная ошибка", -- src\editor\debugger.lua
}
