#ifndef OUTPUTRESULT_H
#define OUTPUTRESULT_H

#include "tableprolet.h"

#include <fstream>
#include <iostream>

/*!
 * \brief Класс для вывода результатов в файл
 */
class OutputResult
{
public:
    OutputResult(std::string out_file_name);
    bool makeResultFile();


    //методы для проверки промежуточных действий
    void makeProletRFFile(std::string out_file_name, std::vector <proletRF::TimeZoneRF> prolet);

private:
    std::string m_out_file_name;
};

#endif // OUTPUTRESULT_H
