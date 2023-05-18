#ifndef INPUTFILESHANDLER_H
#define INPUTFILESHANDLER_H

#include <iostream>
#include <vector>

using namespace std;

namespace file_RF { // для обработки файлов с интервалами пролетов КА над РФ

/*!
   * \brief Набор навигационных систем
   */
enum class SATELLITE_TASK {
    WAIT = 0,
    SHOOTING = 1, ///< Съемка (фотографирование)
    UPLOAD = 2,   ///< Сброс данных
};

struct RF_trace_list{
    string start_time; ///< время начала зоны в формате YYYY-MM-DD HH:mm:ss
    int satellite; ///< номер КА
    string rf_trace_start_time; ///< время начала интервала в формате YYYY-MM-DD HH:mm:ss
    string rf_trace_end_time;   ///< время конца интервала в формате YYYY-MM-DD HH:mm:ss
    double rf_trace_duration;   ///< длительность интервала
};
}

class InputFileRFHandler
{
public:
    InputFileRFHandler();

    bool make_RF_trace_list(string dirPath);

private:
    void GetFilesInDirectory(vector<string> &out, const string &directory);
};

#endif // INPUTFILESHANDLER_H
