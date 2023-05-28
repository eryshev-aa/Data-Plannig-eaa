#include <mylib.h>

#include <iostream>

#include <QCoreApplication>

using namespace std;

int main(int argc, char *argv[]) {

    QCoreApplication a(argc, argv);

    // ВХОДНЫЕ ДАННЫЕ. ОБЯЗАТЕЛЬНО!
    // 1. Указать пути в входным данным
    // windows style
    string dirRF  = "./DATA_Files/Russia2Constellation/";
    string dirZRV = "./DATA_Files/Facility2Constellation/";

    // linux style
    //string dirRF  = "/home/anton/ProfIT-Data-Plannig/DATA_Files/Russia2Constellation/";
    //string dirZRV = "/home/anton/ProfIT-Data-Plannig/DATA_Files/Facility2Constellation/";

    // ПРОМЕЖУТОЧНЫЙ ВЫВОД РЕЗУЛЬТАТОВ.
    // 2. Указать пути и имена файлов для вывода промежуточных результатов. Данные файлы наполняются по мере выполнения
    // 2.1 Файл пролетов над РФ отсортированный по времени начала пролета для всех КА. Для отладки
    string prolety_file = "./proletRF.txt";
    //string prolety_file = "/home/anton/ProfIT-Data-Plannig/proletRF.txt";
    // 2.2 Файл со всеми зонами видимости отсортированными по времени начала. Для отладки
    string zrv_file = "./zrv.txt";
    //string zrv_file = "/home/anton/ProfIT-Data-Plannig/zrv.txt";
    // 2.3 Файл с промежуточными сведениями о сброшенных данных. Наполняется по мере выполнения
    string upload_file = "./upload_intermediate.txt";
    //string upload_file = "/home/anton/ProfIT-Data-Plannig/upload_intermediate.txt";
    // 2.4 Частота выдачи промежуточных сведений о сброшенных данных
    int upload_frequency = 1;
    // 2.5 Файл со сведениями об интервалах и объемах фотосъемки
    string shoot_file = "./shoot_intermediate.txt";
    //string shoot_file = "./home/anton/ProfIT-Data-Plannig/shoot_intermediate.txt";
    // 2.6 Частота выдачи промежуточных сведений о фотосъемке
    int shoot_frequency = 1;

    // РЕЗУЛЬТАТ. ОБЯЗАТЕЛЬНО!
    // 3. Указать путь и имя файла с результатами. Формируется в конце.
    string result_file  = "./result";
    //string result_file  = "/home/anton/ProfIT-Data-Plannig/result";

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

    std::cout << "Finish!" << std::endl;

    return a.exec();
    //a.exit();
}


