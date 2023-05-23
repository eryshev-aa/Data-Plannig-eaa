#include "mylib.h"
#include <time.h>
#include <iomanip>
#include <sstream>

#include <QTextStream>
#include <QFileInfo>

#include "inputfileshandler.h"

#include "outputresult.h"
#include <iostream>
//using namespace proletRF;

Mylib::Mylib()
{

}

bool Mylib::readInputData(string dirRF, string dirZRV){
    InputFileRFHandler in;

    bool rf = in.make_proletRF(dirRF, m_proletyRF, m_sattelites_list);
    bool zrv = in.make_ZRV_trace_list(dirZRV, m_proletyZRV);

    if (rf && zrv) {
        return true;
    } else
        return false;
}

bool Mylib::planning(){
    double time_spent = 0.0;
    clock_t begin = clock();

    TableProletRF prol;
    m_proletyRF = prol.SortTable(m_proletyRF);

    TableZRV zrv;
    m_proletyZRV = zrv.SortZRV(m_proletyZRV);

    OutputResult out("/home/user/qt_projects/ProfIT-Data-Plannig/result.txt");
    out.makeProletRFFile("/home/user/qt_projects/ProfIT-Data-Plannig/proletRF.txt", m_proletyRF);
    out.makeZRVFile("/home/user/qt_projects/ProfIT-Data-Plannig/zrv.txt", m_proletyZRV);
    out.makeResultFile(m_answer);

//    OutputResult out("C:/Users/tad.INAV/Documents/ProfIT-Data-Plannig/result.txt");
//    out.makeProletRFFile("C:/Users/tad.INAV/Documents/ProfIT-Data-Plannig/proletRF.txt", m_proletyRF);
//    out.makeZRVFile("C:/Users/tad.INAV/Documents/ProfIT-Data-Plannig/zrv.txt", m_proletyZRV);

    zrv.analyze_task(m_proletyRF, m_proletyZRV, m_sattelites_list, m_answer);

    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    // вычислить прошедшее время, найдя разницу (end - begin)
    std::cout << "The elapsed time is " << time_spent << " sec" << std::endl;

//    out.makeResultFile(m_answer);

    return true;
}
