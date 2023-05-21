#include <mylib.h>

#include <iostream>

#include <QCoreApplication>
#include <QSettings>
#include <QFile>
#include <QTextStream>

#include <finddatafilenames.h>
//#include <restrictionsparser.h>
//#include <zrvparser.h>

using namespace std;

int main(int argc, char *argv[]) {

    QCoreApplication a(argc, argv);


    string dirRF  = "/home/user/ProfIT-Data-Plannig/DATA_Files/Russia2Constellation/";
    string dirZRV = "/home/user/ProfIT-Data-Plannig/DATA_Files/Facility2Constellation/";


    Mylib lib;

    if (lib.readInputData(dirRF,dirZRV)) {
        lib.planning();
    }



    std::cout << "Finish!" << std::endl;

    //return a.exec();
    a.exit();
}


