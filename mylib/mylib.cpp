#include "mylib.h"
#include <time.h>
#include <iomanip>
#include <sstream>

#include <QTextStream>
#include <QFileInfo>

#include "inputfileshandler.h"

#include "outputresult.h"
#include <iostream>

Mylib::Mylib()
{
    std::cout << "Start Mylib" << std::endl;
}

void Mylib::set_prolety_sorted_file(string prolety_sorted_file) {
    m_prolety_sorted_file = prolety_sorted_file;
}

void Mylib::set_zrv_sorted_file(string zrv_sorted_file) {
    m_zrv_sorted_file = zrv_sorted_file;
}

void Mylib::set_result_file(string result_file) {
    m_result_file = result_file;
}

void Mylib::set_upload_file(string upload_file) {
    m_upload_file = upload_file;

}
void Mylib::set_shoot_file(string shoot_file) {
    m_shoot_file = shoot_file;
}

void Mylib::set_check_pos_upload(int check_pos_upload) {
    m_check_pos_upload = check_pos_upload;
}

void Mylib::set_check_pos_shoot(int check_pos_shoot) {
    m_check_pos_shoot = check_pos_shoot;
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

void Mylib::planning(bool makeRFsortFile, bool makeZRVsortFile, bool makeShootFile, bool makeUploadFile){
    double time_spent = 0.0;
    clock_t begin = clock();

    TableZRV zrv(m_upload_file, m_shoot_file, m_check_pos_upload, m_check_pos_shoot);
    m_proletyZRV = zrv.SortZRV(m_proletyZRV);

    //создаем файл для промежуточных результатов фотосъемки
    std::ofstream fout;
    if (makeShootFile) {
        fout.open(m_shoot_file, std::fstream::out);

        fout << " Access  *  Start Time(UTCG)       *   Stop Time(UTCG)       * dur(s)   *sat_n    * Filled inf." << std::endl;
        fout << "-----------------------------------------------------------------------------------------------" << std::endl;
        fout.close();

    }

    //создаем файл для промежуточных результатов сброса данных
    if (makeUploadFile) {
        fout.open(m_upload_file, std::fstream::out);

        fout << " Access  *  Start Time(UTCG)       *   Stop Time(UTCG)       * dur(s)   *sat_n   * ppi          * drop(Gbyte)  * tank %" << std::endl;
        fout << "-----------------------------------------------------------------------------------------------------------------------" << std::endl;
        fout.close();
    }

    OutputResult out(m_result_file);

    if (makeRFsortFile) {
        out.makeProletRFFile(m_prolety_sorted_file, m_proletyRF);
    }
    if (makeZRVsortFile) {
        out.makeZRVFile(m_zrv_sorted_file, m_proletyZRV);
    }

    zrv.AnalyzeTask(m_proletyRF, m_proletyZRV, m_sattelites_list, m_answer);

    //m_answer = zrv.SortAnswer(m_answer); // не требуется, т.к. теперь вывод по отдельным пунктам
    out.makeResultFile(m_answer);
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    // вычислить прошедшее время, найдя разницу (end - begin)
    std::cout << "The elapsed time is " << time_spent << " sec" << std::endl;
}
