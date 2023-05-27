#ifndef OUTPUTRESULT_H
#define OUTPUTRESULT_H

#include "tableprolet.h"

#include <fstream>
#include <iostream>

template <typename SATELLITE_TYPE>
auto as_integer(SATELLITE_TYPE const value)
    -> typename std::underlying_type<SATELLITE_TYPE>::type
{
    return static_cast<typename std::underlying_type<SATELLITE_TYPE>::type>(value);
}
/*!
 * \brief Класс для вывода результатов в файл
 */
class OutputResult
{
public:
    /*! \fn Конструктор
        \brief Задает путь и маску имени файлов с окончательным результотом.
        \param [in] out_file_name путь и маску имени файлов с окончательным результотом
    */
    OutputResult(std::string out_file_name);

    /*! \fn makeResultFile(std::vector<proletZRV::AnswerData> answerData)
        \brief Создание файлов с результатами работы программы. Кол-во файлов будет равно кол-ву пунктов
        \param [in] answerData вектор структур AnswerData
    */
    void makeResultFile(std::vector<proletZRV::AnswerData> answerData);

    /*! \fn makeProletRFFile(std::string out_file_name, std::vector <proletRF::TimeZoneRF> prolet)
        \brief Создание файла с пролетами над РФ отсортированного по времени начала пролета
        \param [in] out_file_name абсолютный путь и имя файла
        \param [in] prolet отсортированный вектор структур TimeZoneRF
    */
    void makeProletRFFile(std::string out_file_name, std::vector <proletRF::TimeZoneRF> prolet);

    /*! \fn makeZRVFile(std::string out_file_name, std::vector <proletZRV::ZRV> zrv)
        \brief Создание файла с зонами радиовидимости отсортированного по времени начала ЗРВ
        \param [in] out_file_name абсолютный путь и имя файла
        \param [in] zrv отсортированный вектор структур ZRV
    */
    void makeZRVFile(std::string out_file_name, std::vector <proletZRV::ZRV> zrv);

private:
    std::string m_out_file_name;

    std::string makeOutputStringMsec(int msec);
    double TimeDifference(proletZRV::AnswerData zone);
};

#endif // OUTPUTRESULT_H
