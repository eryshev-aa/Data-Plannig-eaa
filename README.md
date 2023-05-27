# Planer

Планировщик передачи данных от КА на станции

# Входные данные
1. Файлы с пролетами спутников над РФ
2. Файлы с зонами радиовидимости 

# Как подключить
Для подключения библиотеки в Qt Creator выполните следующие действия:
1. Создайте новый проект с использованием системы сборки "qmake"
2. Добавьте в .pro файл проекта следующие строчки:
```c++
unix:!macx: LIBS += -L$$PWD/../build-mylib-Desktop-Debug/ -lmylib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-mylib-Desktop_Qt_5_15_2_MinGW_64_bit-Release/release/ -lmylib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-mylib-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug/ -lmylib

INCLUDEPATH += $$PWD/../build-mylib-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug
DEPENDPATH += $$PWD/../build-mylib-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug

INCLUDEPATH += $$PWD/../mylib

INCLUDEPATH += $$PWD/../build-mylib-Desktop-Debug
DEPENDPATH += $$PWD/../build-mylib-Desktop-Debug

DISTFILES +=

RESOURCES +=

HEADERS += \
```

# Как использовать

1. Подключите библиотеку `mylib` к Вашему проекту 
2. Укажите абсолютные пути к папкам с входными файлами
3. Укажите пути к файлам для выходных файлов
4. Укажите параметры для запуска
5. Поочередно вызовите следующие методы:
- set_prolety_sorted_file(string prolety_sorted_file)
- set_zrv_sorted_file(string zrv_file)
- set_upload_file(string upload_file)
- set_check_pos_upload(int check_pos_upload)
- set_shoot_file(string shoot_file)
- set_check_pos_shoot(int check_pos_shoot)
- set_result_file(string result_file)
- readInputData(string dirRF, string dirZRV)
- planning(bool makeRFsortFile, bool makeZRVsortFile, bool makeShootFile, bool makeUploadFile)
- 
- 

Пример:
```c++
#include <mylib.h>
#include <finddatafilenames.h>

#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    string dirRF  = "/home/user/ProfIT-Data-Plannig/DATA_Files/Russia2Constellation/";
    string dirZRV = "/home/user/ProfIT-Data-Plannig/DATA_Files/Facility2Constellation/";

	string prolety_file = "/home/anton/ProfIT-Data-Plannig/proletRF.txt";
	string zrv_file = "/home/anton/ProfIT-Data-Plannig/zrv.txt";
    string upload_file = "/home/anton/ProfIT-Data-Plannig/upload_intermediate.txt";
    int upload_frequency = 1;
    string shoot_file = "/home/anton/ProfIT-Data-Plannig/shoot_intermediate.txt";
    int shoot_frequency = 1;
    string result_file  = "/home/anton/ProfIT-Data-Plannig/result";

    Mylib lib;

    lib.set_prolety_sorted_file(prolety_file);
    lib.set_zrv_sorted_file(zrv_file);    
    lib.set_upload_file(upload_file);
    lib.set_check_pos_upload(upload_frequency);
    lib.set_shoot_file(shoot_file);
    lib.set_check_pos_shoot(shoot_frequency);
    lib.set_result_file(result_file);

    if (lib.readInputData(dirRF,dirZRV)) {
        lib.planning(true, true, true, true);
    }

	a.exit();
}
```

# Сборка библиотеки

**Сборка библиотеки под GCC:**
1. В Qt Creator открыть файлы библиотеки через .pro файл
2. Нажать правой кнопкой мыши на имя библиотеки
3. Выбрать в появившемся списке выбрать "Собрать"

**Сборка библиотеки под MingW:**
1. В Qt Creator  открыть файлы библиотеки через .pro файл
2. Зайти в файл inputfileshandler.cpp
3. Расскомментровать следующие строки:

```c++
#include <filesystem>
```

```c++
vector <string> InputFileRFHandler::getFileNamesInDir_FS(string dirName){
    vector <string> result;

    for (const auto & entry : std::filesystem::directory_iterator(dirName)) {
        result.push_back(entry.path());
    }

    return result;
}
```

В методе ***bool InputFileRFHandler::make_proletRF(string dirPath, vector<proletRF::TimeZoneRF> &rf_trace_list, vector<Satellite> &sattelites_list):***

```c++
vector <string> files = getFileNamesInDir_FS(dirPath);
```

В методе ***bool InputFileRFHandler::make_ZRV_trace_list(string dirPath, vector<ZRV> &zrv_trace_list, string min_time):***

```c++
vector <string> files = getFileNamesInDir_FS(dirPath)
```
4. В этом же файле закомментировать следующие строки:
```c++
vector <string> InputFileRFHandler::getFileNamesInDir_QDir(string dirName){
    vector <string> result;

    QFileInfoList filesList;
    QDir filesDir(QString::fromStdString(dirName));
    filesDir.setFilter(QDir::Files);
    filesList = filesDir.entryInfoList();
    if (filesList.count() > 0) {
        for (int i = 0; i < filesList.size(); ++i) {
            QFileInfo fileInfo = filesList.at(i);
            result.push_back(fileInfo.absolutePath().toStdString() + "/" + fileInfo.fileName().toStdString());
        }
    } else {
        cout << "Data folder " << dirName << " is empty!" << endl;
        result.clear();
        return result;
    }

    return result;
}
```

В методе ***bool InputFileRFHandler::make_proletRF(string dirPath, vector<proletRF::TimeZoneRF> &rf_trace_list, vector<Satellite> &sattelites_list):***

```c++
vector <string> files = getFileNamesInDir_QDir(dirPath);
```

В методе ***bool InputFileRFHandler::make_ZRV_trace_list(string dirPath, vector<ZRV> &zrv_trace_list, string min_time):***

```c++
vector <string> files = getFileNamesInDir_FS(dirPath)
```

4. Зайти в файл inputfileshandler.h
5. Расскомментировать в нем следующую строку:

```c++
vector <string> getFileNamesInDir_FS(string dirName);
```
6. В этом же файле закомментировать строку:

```c++
vector <string> getFileNamesInDir_QDir(string dirName);
```

7. Сохранить изменения в данных файлах
8. Нажать правой кнопкой мыши на имя библиотеки
9. Выбрать в появившемся списке выбрать "Собрать"
