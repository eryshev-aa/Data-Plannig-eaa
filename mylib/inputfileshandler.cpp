#include "inputfileshandler.h"

#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <algorithm>

using namespace file_RF;

InputFileRFHandler::InputFileRFHandler()
{

}

bool InputFileRFHandler::make_RF_trace_list(string dirPath) {
    string line;

    for (const auto & entry : std::filesystem::directory_iterator(dirPath)) {
        //std::cout << "!========== " << entry.path() << std::endl;
        ifstream in_RF_file(entry.path());

        if (in_RF_file.is_open())
        {
            getline(in_RF_file, line);
            while (std::getline(in_RF_file, line))
            {
                if ((line.find("Civil Air Patrol Use Only") != std::string::npos) ||
                    (line.find("AreaTarget-Russia-To-Satellite") != std::string::npos ) ||
                    (line.find("------") != std::string::npos) ||
                    (line.find("Access") != std::string::npos) ||
                    (line.find("Min Duration") != std::string::npos) ||             // В нашем алгоритме
                    (line.find("Max Duration") != std::string::npos) ||             // эти значения
                    (line.find("Mean Duration") != std::string::npos) ||            // не используются.
                    (line.find("Total Duration") != std::string::npos) ||           // Но может их не просто так дали...?
                    (line.find("Global Statistics") != std::string::npos) ||
                    (line[0] == '\0')) {

                    continue;
                } else if (line.find("Russia-To-KinoSat_") != std::string::npos) {
                    int sat_number = std::stoi(line.substr(18,6));  // получаем номер КА
                    std::cout << sat_number << std::endl;
                } else {
                    string rf_trace_start_time = line.substr(29,23);
                    string rf_trace_end_time   = line.substr(57,23);
                    string rf_trace_duration_str = line.substr(91,7);
                    size_t ptr = -1;
                    double rf_trace_duration   = std::stod(rf_trace_duration_str, &ptr);
                    if (ptr = 3) {
                        std::replace(rf_trace_duration_str.begin(), rf_trace_duration_str.end(), '.', ','); // у меня в Линукс почему-то работает на "," а не на "."
                        rf_trace_duration = std::stod(rf_trace_duration_str);
                    }
                    std::cout << rf_trace_start_time << "==" << rf_trace_end_time << "==" << rf_trace_duration << std::endl;
                }
            }
        } else
        {
            return false;
        }
        in_RF_file.close();
    }


    return true;
}
