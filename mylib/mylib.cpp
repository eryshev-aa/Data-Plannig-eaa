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

    bool rf = in.make_RF_trace_list(dirRF, m_proletyRF, m_sattelites_list);
    bool zrv = in.make_ZRV_trace_list(dirZRV, m_proletyZRV);

    if (rf && zrv) {
        return true;
    } else
        return false;
}

bool Mylib::planning(){
    TableProletRF prol;
    m_proletyRF = prol.SortTable(m_proletyRF);

    TableZRV zrv;
    m_proletyZRV = zrv.SortZRV(m_proletyZRV);

    OutputResult out("/home/user/ProfIT-Data-Plannig/result.txt");
    out.makeProletRFFile("/home/user/ProfIT-Data-Plannig/proletRF.txt", m_proletyRF);
    out.makeZRVFile("/home/user/ProfIT-Data-Plannig/zrv.txt", m_proletyZRV);

//    OutputResult out("C:/Users/tad.INAV/Documents/ProfIT-Data-Plannig/result.txt");
//    out.makeProletRFFile("C:/Users/tad.INAV/Documents/ProfIT-Data-Plannig/proletRF.txt", m_proletyRF);
//    out.makeZRVFile("C:/Users/tad.INAV/Documents/ProfIT-Data-Plannig/zrv.txt", m_proletyZRV);

    zrv.analyze_task(m_proletyRF, m_proletyZRV, m_sattelites_list);

    //out.makeResultFile(m_answer);

    return true;
}
