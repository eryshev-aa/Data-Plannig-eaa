#ifndef MYLIB_H
#define MYLIB_H

#include "mylib_global.h"

#include <vector>
#include <utility>
#include <string>
#include <iostream>
#include <algorithm>

#include "tableprolet.h"

#include <QFile>

using namespace std;

/*!
 * \brief Класс планировщика
 */
class MYLIB_EXPORT Mylib{
public:
    //! Пустой конструктор
    /*!
     *
     */
    Mylib();

    /*! \fn bool readInputData(string dirRF, string dirZRV)
        \brief Чтение входных данных из файлов с пролетами над РФ и фалов с ЗРВ
        \param [in] dirRF абсолютный путь к папке с файлами пролетов над РФ
        \param [in] dirZRV абсолютный путь к папке с файлами с зонами видимости
        \return false - если возникли проблемы при работе с файлами. Иначе - true.
    */
    bool readInputData(string dirRF, string dirZRV);

    /*! \fn bool planning()
        \brief Чтение входных данных из файлов с пролетами над РФ и фалов с ЗРВ
        \param [in] dirRF абсолютный путь к папке с файлами пролетов над РФ
        \param [in] dirZRV абсолютный путь к папке с файлами с зонами видимости
        \return false - если возникли проблемы в работе. Иначе - true.
    */
    bool planning();

private:


private:
    vector <proletRF::TimeZoneRF> m_proletyRF;
    vector <proletZRV::ZRV> m_proletyZRV;
    vector <proletRF::Satellite> m_sattelites_list;
    vector <proletZRV::AnswerData> m_answer;
};


#endif // MYLIB_H
