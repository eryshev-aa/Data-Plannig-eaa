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
    OutputResult(std::string out_file_name);
    void makeResultFile(std::vector<proletZRV::AnswerData> answerData);


    //методы для проверки промежуточных действий
    void makeProletRFFile(std::string out_file_name, std::vector <proletRF::TimeZoneRF> prolet);
    void makeZRVFile(std::string out_file_name, std::vector <proletZRV::ZRV> zrv);

private:
    std::string m_out_file_name;

    std::string makeOutputStringMsec(int msec);
    double TimeDifference(proletZRV::AnswerData zone);
};

#endif // OUTPUTRESULT_H
