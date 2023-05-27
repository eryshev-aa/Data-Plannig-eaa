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
        \param [in] makeRFsortFile необходимость создать отсортированный файл с пролетами над РФ. Для отладки
        \param [in] makeZRVsortFile необходимость создать отсортированный файл с зонами видимости. Для отладки
        \return false - если возникли проблемы в работе. Иначе - true.
    */
    void planning(bool makeRFsortFile, bool makeZRVsortFile, bool makeShootFile, bool makeUploadFile);

    void set_prolety_sorted_file(string prolety_sorted_file);
    void set_zrv_sorted_file(string zrv_sorted_file);
    void set_result_file(string result_file);
    void set_upload_file(string upload_file);
    void set_check_pos_upload(int check_pos_upload);
    void set_shoot_file(string shoot_file);
    void set_check_pos_shoot(int check_pos_shoot);

private:
    vector <proletRF::TimeZoneRF> m_proletyRF;
    vector <proletZRV::ZRV> m_proletyZRV;
    vector <proletRF::Satellite> m_sattelites_list;
    vector <proletZRV::AnswerData> m_answer;

    string m_prolety_sorted_file;
    string m_zrv_sorted_file;
    string m_result_file;
    string m_upload_file;
    int m_check_pos_upload;
    string m_shoot_file;
    int m_check_pos_shoot;
};


#endif // MYLIB_H
