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
    for (auto entry: prolet)
    {
        time (&t);
        strftime (start, 80, "%d.%m.%Y %H:%M:%S.",&entry.tm_start);
        strftime (end, 80, "%d.%m.%Y %H:%M:%S.",&entry.tm_end);

        fout << access << ". " << entry.satellite << " " << start << entry.milisecs_start <<" " << end << entry.milisecs_end << " " << entry.vitok << "\n";
        access ++;
    }

    fout.close();
}
