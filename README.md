# Planer

Планировщик передачи данных от КА на станции

# Входные данные
1. Файлы с пролетами спутников над РФ
2. Файлы с зонами радиовидимости 

# Как подключить
Для подключения библиотеки в Qt Creator выполните следующие действия:
1. Создайте новый проект с использованием состемы сборки "qmake"
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
