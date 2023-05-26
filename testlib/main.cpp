#include <mylib.h>

#include <iostream>

#include <QCoreApplication>

using namespace std;

int main(int argc, char *argv[]) {

    QCoreApplication a(argc, argv);


//    string dirRF  = "/home/user/qt_projects/ProfIT-Data-Plannig/DATA_Files/Russia2Constellation/";
//    string dirZRV = "/home/user/qt_projects/ProfIT-Data-Plannig/DATA_Files/Facility2Constellation/";

    string dirRF  = "/home/user1/ProfIT-Data-Plannig/DATA_Files/Russia2Constellation/";
    string dirZRV = "/home/user1/ProfIT-Data-Plannig/DATA_Files/Facility2Constellation/";


//    string dirRF  = "/home/anton/ProfIT-Data-Plannig/DATA_Files/Russia2Constellation/";
//    string dirZRV = "/home/anton/ProfIT-Data-Plannig/DATA_Files/Facility2Constellation/";

//    string dirRF  = "C:/Users/erysh/Documents/ProfIT-Data-Plannig/DATA_Files/Russia2Constellation/";
//    string dirZRV = "C:/Users/erysh/Documents/ProfIT-Data-Plannig/DATA_Files/Facility2Constellation/";

    Mylib lib;

    if (lib.readInputData(dirRF,dirZRV)) {
        lib.planning();
    }



    std::cout << "Finish!" << std::endl;

    //return a.exec();
    a.exit();
}


