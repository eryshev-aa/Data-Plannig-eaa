#ifndef INPUTFILESHANDLER_H
#define INPUTFILESHANDLER_H

#include <iostream>
#include <vector>

#include <QDir>
#include <QFileInfo>

#include "tableprolet.h"

using namespace std;
using namespace proletRF;
using namespace proletZRV;

/*!
 * \brief Класс для чтения фходных данных из файлов
 */
class InputFileRFHandler
{
public:
    InputFileRFHandler();

    /*! \fn bool make_proletRF(string dirPath, vector<TimeZoneRF> &rf_trace_list, vector<Satellite> &sattelites_list)
        \brief Создание вектора структур с пролетами над РФ (TimeZoneRF) и вектора структур КА (Satellite)
        \param [in] dirPath абсолютный путь к папке с файлами пролетов над РФ
        \param [in] rf_trace_list вектор для наполения пролетами
        \param [in] sattelites_list вектор для наполнения аппаратами
        \return false - если возникли проблемы при работе с файлами. Иначе - true.
    */
    bool make_proletRF(string dirPath, vector<TimeZoneRF> &rf_trace_list, vector<Satellite> &sattelites_list);

    /*! \fn bool readInputData(string dirRF, string dirZRV)
        \brief Создание вектора структур с зонами радиовидимости (ZRV)
        \param [in] dirPath абсолютный путь к папке с файлами ЗРВ
        \param [in] zrv_trace_list абсолютный путь к папке с файлами с зонами видимости
        \param [in] min_time время первого пролета на РФ, для удаления ЗРВ до самого первого пролета.
        \return false - если возникли проблемы при работе с файлами. Иначе - true.
    */
    bool make_ZRV_trace_list(string dirPath, vector<ZRV> &zrv_trace_list, string min_time);

private:
    //vector <string> getFileNamesInDir_FS(string dirName); //gcc: filesistem
    vector <string> getFileNamesInDir_QDir(string dirName); //gcc+MinGW: QDir
};

#endif // INPUTFILESHANDLER_H
