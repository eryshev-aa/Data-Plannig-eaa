#include <mylib.h>

#include <iostream>

#include <QCoreApplication>
#include <QSettings>
#include <QFile>
#include <QTextStream>

#include <finddatafilenames.h>
#include <restrictionsparser.h>
#include <zrvparser.h>

using namespace std;

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

//    QSettings settings("settings.ini", QSettings::IniFormat); // читаем файл настроек из папки сборки. Нужно его туда скопировать!

//    FindDataFileNames fd;
//    vector <string> dataFileNames = fd.getDataFileNames(settings, a.applicationDirPath());// формируем вектор с путями и названиями файлов с данными.

//    QString restrictionsFile; //абсолютный путь и имя файла с ограничениями
//    QString resultFile;
//    if (settings.status() == QSettings::NoError) {
//        settings.beginGroup("input data");
//        restrictionsFile = a.applicationDirPath() + "/" + settings.value("pathRestrictions").toString() + "/" + settings.value("restrictionFileName").toString();
//        settings.endGroup();
//        settings.beginGroup("output data");
//        resultFile = a.applicationDirPath() + "/" + settings.value("result").toString();
//        settings.endGroup();
//    } else {
//        cout << "Settings file read error. App close!" << endl;
//        return 0;
//    }

    string dirRF  = "/home/user/qt_projects/planer_new/DATA_Files/Russia2Constellation/";
    string dirZRV = "/home/user/qt_projects/planer_new/DATA_Files/Facility2Constellation/";


    Mylib lib;
    lib.readInputData(dirRF,dirZRV);
    //lib.readInputData(dirRF,dirZRV);

//    vector <Mylib::Ppi> kp = lib.makePpi(restrictionsFile.toStdString()); // подаем абсолютный путь и имя файла с ограничениями для пунктов передачи информации
//    vector <Mylib::Time> segments = lib.readZRV(dataFileNames); //подаем вектор с абсолютный путями и имаами файлов с зонами видимости

//    if (kp.empty() || segments.empty()) {
//        cout << "Data error. App close!" << endl;
//        return 0;
//    }

//    std::vector<Mylib::AnsVector> t = lib.planning(segments,kp); // вызываем основную функцию планирования

//    std::vector<Mylib::AnsVector> r;

//    for(const auto& seg:t){
//        r.push_back(seg);
//    }

//    if (!lib.sendReady(resultFile.toStdString(), r)) {
//        return 0;
//    } else {
//        std::cout << "Finish!" << std::endl;
//    }

    //return a.exec();
    a.exit();
}


