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

    /*! \fn readInputData(string dirRF, string dirZRV)
        \brief Чтение входных данных из файлов с пролетами над РФ и фалов с ЗРВ
        \param [in] dirRF абсолютный путь к папке с файлами пролетов над РФ
        \param [in] dirZRV абсолютный путь к папке с файлами с зонами видимости
        \return false - если возникли проблемы при работе с файлами. Иначе - true.
    */
    bool readInputData(string dirRF, string dirZRV);

    /*! \fn planning()
        \brief Чтение входных данных из файлов с пролетами над РФ и фалов с ЗРВ
        \param [in] makeRFsortFile необходимость создать отсортированный файл с пролетами над РФ. Для отладки
        \param [in] makeZRVsortFile необходимость создать отсортированный файл с зонами видимости. Для отладки
        \param [in] makeShootFile необходимость создавать и наполнять файл со сведениями о фотосъемке. Для отладки
        \param [in] makeUploadFile необходимость создавать и наполнять файл со сведениями о сбросе. Для отладки
    */
    void planning(bool makeRFsortFile, bool makeZRVsortFile, bool makeShootFile, bool makeUploadFile);

    /*! \fn set_prolety_sorted_file()
        \brief Устанавливает путь и имя к файлу с отсортированными полетами над РФ
        \param [in] prolety_sorted_file
    */
    void set_prolety_sorted_file(string prolety_sorted_file);

    /*! \fn set_zrv_sorted_file()
        \brief Устанавливает путь и имя к файлу с отсортированными зонами видимости
        \param [in] zrv_sorted_file
    */
    void set_zrv_sorted_file(string zrv_sorted_file);

    /*! \fn set_result_file()
        \brief Устанавливает путь и маску имени файлов с окончательным результотом. Кол-во файлов будет равно кол-ву пунктов
        \param [in] result_file
    */
    void set_result_file(string result_file);

    /*! \fn set_upload_file()
        \brief Устанавливает путь и имя к файлу с промежуточными сведениями о сброшенных данных. Наполняется по мере выполнения
        \param [in] upload_file
    */
    void set_upload_file(string upload_file);

    /*! \fn set_check_pos_upload()
        \brief Устанавливает путь и имя к файлу с промежуточными сведениями о фотосъемке. Наполняется по мере выполнения
        \param [in] check_pos_upload
    */
    void set_check_pos_upload(int check_pos_upload);

    /*! \fn set_shoot_file()
        \brief Устанавливает частоту выдачи промежуточных сведений о фотосъемке. Каждые N раз
        \param [in] shoot_file
    */
    void set_shoot_file(string shoot_file);

    /*! \fn set_check_pos_shoot()
        \brief Устанавливает частоту выдачи промежуточных сведений о сбросе данных. Каждые N раз
        \param [in] check_pos_shoot
    */
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
