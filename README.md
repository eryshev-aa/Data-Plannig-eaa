# Planer

Планировщик передачи данных от КА на станции

# Входные данные
1. Файлы с пролетами спутников над РФ
2. Файлы с зонами радиовидимости 

# Как использовать

1. Подключите библиотеку `mylib` к Вашему проекту 
2. Укажите абсолютные пути к папкам с фходными файлами
3. Поочередно вызовите следующие методы:
- readInputData(string dirRF, string dirZRV)
- 
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


    Mylib lib;
    lib.readInputData(dirRF,dirZRV);

    std::cout << "Finish!" << std::endl;

    a.exit();
}
```
