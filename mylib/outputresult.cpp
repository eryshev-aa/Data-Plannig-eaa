#include "outputresult.h"

#include <sstream>
#include <iostream>
#include <iomanip>

OutputResult::OutputResult(std::string out_file_name)
{
    m_out_file_name = out_file_name;
}

bool OutputResult::makeResultFile(){
    std::ofstream fout;

    fout.open(m_out_file_name);

    fout.close();

    return true;
}

void OutputResult::makeProletRFFile(std::string out_file_name, std::vector <proletRF::TimeZoneRF> prolet){
    int  access = 1;
    std::ofstream fout;

    fout.open(out_file_name);
    time_t t;
    char start [80];
    char end [80];
    std::string milisecs_start_out;
    std::string milisecs_end_out;

    fout << " Access  *sat_n   *  Start Time(UTCG)       *   Stop Time(UTCG)       * dur     * vit * task" << std::endl;
    fout << "--------------------------------------------------------------------------------------------" << std::endl;
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
             << std::setw(7) << std::fixed << std::right << entry.duration << "   "
             << entry.vitok << "     "
             << as_integer(entry.task)
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
