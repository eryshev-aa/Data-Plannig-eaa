#include "outputresult.h"

#include <sstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

OutputResult::OutputResult(std::string out_file_name)
{
    m_out_file_name = out_file_name;
}

void OutputResult::makeResultFile(std::vector <proletZRV::AnswerData> answerData){
    int  access = 1;
    std::ofstream fout;
    std::string current_path = m_out_file_name+"-Facility-"+answerData[0].ppi+".txt";
    fout.open(current_path);
    std::string current_ppi=answerData[0].ppi;
    std::vector <std::string> put_ppi;
    put_ppi.reserve(1);
    put_ppi.push_back(current_ppi);
    time_t t;
    char start [80];
    char end [80];

    fout << " Access  *  Start Time(UTCG)       *   Stop Time(UTCG)       * dur(s)  *sat_n   * Data(Gbit)" << std::endl;
    fout << "--------------------------------------------------------------------------------------------" << std::endl;
    fout << std::flush;

    for (auto answer: answerData)
    {
        if (answer.ppi!=current_ppi) {
            current_path=m_out_file_name+"-Facility-"+answer.ppi+".txt";
            current_ppi=answer.ppi;
            fout.close();
            fout.open(current_path, std::fstream::app | std::fstream::out);
            auto it =std::find_if(put_ppi.begin(),put_ppi.end(), [&current_ppi](const std::string& a){
                return a==current_ppi;
            });
            if (it==put_ppi.end()) {
                put_ppi.push_back(current_ppi);
                fout << " Access  *  Start Time(UTCG)       *   Stop Time(UTCG)       * dur(s)    *sat_n * Data(Gbit)" << std::endl;
                fout << "--------------------------------------------------------------------------------------------" << std::endl;
                fout << std::flush;
            }
            time (&t);
            strftime (start, 80, "%d.%m.%Y %H:%M:%S.", &answer.tm_start);
            strftime (end, 80, "%d.%m.%Y %H:%M:%S.", &answer.tm_end);

            fout << std::setw(6) << std::setprecision(3) << std::right;
            fout << access << "   "
                 << start << makeOutputStringMsec(answer.milisecs_start) << "   "
                 << end << makeOutputStringMsec(answer.milisecs_end) << "   "
                 << std::setw(7) << std::fixed << std::right << TimeDifference(answer) << "   "
                 << answer.satellite << "   "
                 << answer.transfered_inf
                 << std::endl;
        } else {
            time (&t);
            strftime (start, 80, "%d.%m.%Y %H:%M:%S.", &answer.tm_start);
            strftime (end, 80, "%d.%m.%Y %H:%M:%S.", &answer.tm_end);

            fout << std::setw(6) << std::setprecision(3) << std::right;
            fout << access << "   "
                 << start << makeOutputStringMsec(answer.milisecs_start) << "   "
                 << end << makeOutputStringMsec(answer.milisecs_end) << "   "
                 << std::setw(7) << std::fixed << std::right << TimeDifference(answer) << "   "
                 << answer.satellite << "   "
                 << answer.transfered_inf
                 << std::endl;
        }
        access ++;
    }

    fout.close();
}

void OutputResult::makeProletRFFile(std::string out_file_name, std::vector <proletRF::TimeZoneRF> prolet){
    int  access = 1;
    std::ofstream fout;

    fout.open(out_file_name);
    time_t t;
    char start [80];
    char end [80];

    fout << " Access  *sat_n   *  Start Time(UTCG)       *   Stop Time(UTCG)       * dur" << std::endl;
    fout << "---------------------------------------------------------------------------" << std::endl;
    fout << std::flush;

    for (auto entry: prolet)
    {
        time (&t);
        strftime (start, 80, "%d.%m.%Y %H:%M:%S.",&entry.tm_start);
        strftime (end, 80, "%d.%m.%Y %H:%M:%S.",&entry.tm_end);

        fout << std::setw(6) << std::setprecision(3) << std::right;
        fout << access << "   "
             << entry.satellite << "   "
             << start << makeOutputStringMsec(entry.milisecs_start) << "   "
             << end << makeOutputStringMsec(entry.milisecs_end) << "   "
             << std::setw(7) << std::fixed << std::right << entry.duration
             << std::endl;

        access ++;
    }

    fout.close();
}

void OutputResult::makeZRVFile(std::string out_file_name, std::vector <proletZRV::ZRV> zrv){
    int  access = 1;
    std::ofstream fout;

    fout.open(out_file_name);
    time_t t;
    char start [80];
    char end [80];

    fout << " Access  *sat_n   *  Start Time(UTCG)       *   Stop Time(UTCG)       * dur    *PPI       " << std::endl;
    fout << "------------------------------------------------------------------------------------------" << std::endl;
    fout << std::flush;

    for (auto current_zrv: zrv)
    {
        time (&t);
        strftime (start, 80, "%d.%m.%Y %H:%M:%S.",&current_zrv.tm_start);
        strftime (end, 80, "%d.%m.%Y %H:%M:%S.",&current_zrv.tm_end);

        fout << std::setw(6) << std::setprecision(3) << std::right;
        fout << access << "   "
             << current_zrv.satellite << "   "
             << start << makeOutputStringMsec(current_zrv.milisecs_start) << "   "
             << end << makeOutputStringMsec(current_zrv.milisecs_end) << "   "
             << std::setw(7) << std::fixed << std::right << current_zrv.duration << "   "
             << std::setw(10) << std::left << current_zrv.ppi
             << std::endl;

        access ++;
    }

    fout.close();
}

std::string OutputResult::makeOutputStringMsec(int msec) {
    std::string  res;

    if (msec == 0) {
        res = "000";
    } else if (msec > 0 && msec < 10) {
        res = "00" + std::to_string(msec);
    } else if (msec > 9 && msec < 100) {
        res = "0" + std::to_string(msec);
    } else {
        res = std::to_string(msec);
    }

    return res;
}

double OutputResult::TimeDifference(proletZRV::AnswerData zone) {
    time_t first = mktime(&(zone.tm_end)) * 1000 + zone.milisecs_end;
    time_t second = mktime(&(zone.tm_start)) * 1000 + zone.milisecs_start;
    double differ = (first - second);

    return differ / 1000;
}
