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
    TableProletRF prol;
    m_proletyRF = prol.SortTable(m_proletyRF);

    char start_min_buff [80];
    strftime (start_min_buff, 80, "%d.%m.%Y %H:%M:%S.", &m_proletyRF[0].tm_start);
    std::string min_start(start_min_buff);
    min_start += std::to_string(m_proletyRF[0].milisecs_start);
    bool zrv = in.make_ZRV_trace_list(dirZRV, m_proletyZRV, min_start);

    if (rf && zrv) {
        return true;
    } else
        return false;
}

bool Mylib::planning(){
    double time_spent = 0.0;
    clock_t begin = clock();

    TableZRV zrv;
    m_proletyZRV = zrv.SortZRV(m_proletyZRV);
    //создаем файл для промежуточных результатов для фотографии
    std::ofstream fout;
    //fout.open("/home/user/ProfIT-Data-Plannig/shoot_intermediate.txt", std::fstream::out );
    fout.open("C:/Users/erysh/Documents/ProfIT-Data-Plannig/shoot_intermediate.txt", std::fstream::out );
    fout << " Access  *  Start Time(UTCG)       *   Stop Time(UTCG)       * dur(s)    *sat_n * Filled inf." << std::endl;
    fout << "---------------------------------------------------------------------------------------------" << std::endl;
    fout.close();

    //создаем файл для промежуточных результатов выгруза данных
    //fout.open("/home/user/ProfIT-Data-Plannig/upload_intermediate.txt", std::fstream::out);
    fout.open("C:/Users/erysh/Documents/ProfIT-Data-Plannig/upload_intermediate.txt", std::fstream::out);
    fout << " Access  *  Start Time(UTCG)       *   Stop Time(UTCG)       * dur(s)    *sat_n * ppi  * Filled inf. " << std::endl;
    fout << "-----------------------------------------------------------------------------------------------------" << std::endl;
    fout.close();

//    OutputResult out("/home/user/qt_projects/ProfIT-Data-Plannig/result.txt");
//    out.makeProletRFFile("/home/user/qt_projects/ProfIT-Data-Plannig/proletRF.txt", m_proletyRF);
//    out.makeZRVFile("/home/user/qt_projects/ProfIT-Data-Plannig/zrv.txt", m_proletyZRV);

    //OutputResult out("/home/anton/ProfIT-Data-Plannig/");
//    out.makeProletRFFile("/home/anton/ProfIT-Data-Plannig/proletRF.txt", m_proletyRF);
//    out.makeZRVFile("/home/anton/ProfIT-Data-Plannig/zrv.txt", m_proletyZRV);

//    OutputResult out("/home/user/ProfIT-Data-Plannig/result.txt");
//    out.makeProletRFFile("/home/user/ProfIT-Data-Plannig/proletRF.txt", m_proletyRF);
//    out.makeZRVFile("/home/user/ProfIT-Data-Plannig/zrv.txt", m_proletyZRV);


    OutputResult out("C:/Users/erysh/Documents/ProfIT-Data-Plannig/result.txt");
    out.makeProletRFFile("C:/Users/erysh/Documents/ProfIT-Data-Plannig/proletRF.txt", m_proletyRF);
    out.makeZRVFile("C:/Users/erysh/Documents/ProfIT-Data-Plannig/zrv.txt", m_proletyZRV);

    zrv.AnalyzeTask(m_proletyRF, m_proletyZRV, m_sattelites_list, m_answer);

    //m_answer = zrv.SortAnswer(m_answer); // не требуется, т.к. теперь вывод по отдельным пунктам
    out.makeResultFile(m_answer);
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    // вычислить прошедшее время, найдя разницу (end - begin)
    std::cout << "The elapsed time is " << time_spent << " sec" << std::endl;

//    out.makeResultFile(m_answer);

    return true;
}
