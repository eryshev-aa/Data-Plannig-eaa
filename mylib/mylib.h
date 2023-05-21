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

    //ниже все старые функции. Пока их не удаляю
    /*! \struct Time
        \brief Структура для ЗРВ

        Описывает зону разиовидимости видимости (ЗРВ, ZRV) для КА
    */
    struct Time{
        string start_time; ///< время начала зоны в формате YYYY-MM-DD HH:mm:ss
        string end_time;   ///< время конца зоны в формате YYYY-MM-DD HH:mm:ss
        string max_start_time; ///< время максимального начала......... м.б.: время в которое закончится передача осавшегося объема данных?
        string end_transfer; ///< время конца передачи инфомации в формате YYYY-MM-DD HH:mm:ss
        int satellite; ///< номер КА
        int ppi; ///< номер пункта приема информации
        int bandwidth; ///<пропускня способность КА (Mbit/s)
        int transmission;///<количество информации для передачи с КА (Mbit)
        int work_time; ///<время которое будет затрачено на передачу данных с КА (in seconds)
    };

    /*! \struct Ppi
        \brief Структура с описанием пункта приема информации (ППИ)

    */
    struct Ppi{
        int ppi; ///< номер пункта приема информации
        vector <int> kp; ///< номера комплектов приема на конкретном пункте приема информации
        vector <vector<int>> agreed; ///< номера КА с которыми может рабоать конкретный комплект приема на конкретном пункте приема информации
        vector <string> isbusy;
        vector<pair<string, int>> dopelganger;
    };

    /*! \struct AnsVector
        \brief Структура результата
    */
    struct AnsVector{
        int ppi; ///< номер пункта приема информации
        int kp;  ///< номер комплекта приема на конкретном пункте приема информации
        string start_time; ///< время начала зоны в формате YYYY-MM-DD HH:mm:ss
        string end_time;   ///< время конца зоны в формате YYYY-MM-DD HH:mm:ss
        int satellite; ///< номер КА;
        int bandwidth; ///<пропускня способность КА // Mbit
        int transmission;///<количество информации для передачи с КА // Mbit
    };

    /*! \fn vector <Mylib::Ppi> makePpi(string file)
        \brief Чтение файла с ограничениями для пунктов ПИ
        \param [in] restrictionsFile абсолютный путь и имя файла ограничений
        \return вектор органичений для пунктов ПИ.
    */
    vector <Mylib::Ppi> makePpi(string file);

    /*! \fn vector <Mylib::Time> readZRV(vector<string> files)
        \brief Чтение файлов с данными по ЗРВ для пунктов ПИ
        \param [in] files Абсолютные пути к файлам с ЗРВ
        \return вектор с ЗРВ для всех КА
    */
    vector <Mylib::Time> readZRV(vector<string> files);

    /*! \fn vector<AnsVector> planning(vector <Mylib::Time> segments, vector <Mylib::Ppi> kp)
        \brief Планирование сеансов
        \param [in] segments - поступивший КА
        \param [in] kp - пункты приема информации, разбитые по комплекам. Пример: {120 5 {1 2 3}}, {102 3 {4 5 3}}, {103 6 {1 2 3}}
        \return Вектор содержащий начало и конец сеанса, номер пункта приема информации, номер комплекта и номер  КА
    */
    vector<AnsVector> planning(vector <Mylib::Time> segments, vector <Mylib::Ppi> kp);

    /*! \fn bool sendReady(string outFileName, vector<Mylib::AnsVector> answer);
        \brief Вывод результата в файл
        \param [in] outfile Файл для вывода результата
        \param [in] answer Вектор с ответами
        \return false - если возникли проблемы при работе с выходным файлом. Иначе - true.
    */
    bool sendReady(string outFileName, vector<Mylib::AnsVector> answer);

private:
    /*! Сравнение 2-х временных отрезков
    * \param [in] timeA Первое время
    * \param [in] timeB Второе время
    * \return true если время начала 1-го отрезка раньше времени начала 2-го ??? иначе false
    */
    static bool compareTimes(const Time& timeA, const Time& timeB);

    /*! Сравнение 2-х временных отрезков в итоговом векторе
    * \param [in] vecA Первое время
    * \param [in] vecB Второе время
    * \return true если время начала 1-го отрезка раньше времени начала 2-го ??? иначе false
    */
    static bool compareAnsTimes(const AnsVector& vecA,const AnsVector& vecB);

    static bool compareInTransmission(const Time& timeA, const Time& timeB);

    /*! 1-ое ограничение: приданный КА
    * \param [in] segments - поступившие КА
    * \param [in] kp -  пункты приема информации, разбитые по комплектам. Пример: {120 5 {1 2 3}}, {102 3 {4 5 3}}, {103 6 {1 2 3}}
    * \param [in] answer - вектор хранящий информацию о КА которые сможет обработать приданнный Кп
    * \изменяет поступившие вектора
    */
    void DodrySatellite(vector<Mylib::Time>& sec, vector<Mylib::Ppi>& kp, vector<Mylib::AnsVector>& answer);

    /*! 2-ое ограничение: обслуживание КА
    * \param [in] segments - поступившие КА
    * \param [in] kp -  пункты приема информации, разбитые по комплектам. Пример: {120 5 {1 2 3}}, {102 3 {4 5 3}}, {103 6 {1 2 3}}
    * \return вектор из пары одного элемента из kp и одного элемента из segments
    */
    vector<pair<Mylib::Time, Mylib::Ppi>> ServiceSatellite(vector<Mylib::Time>segments, vector<Mylib::Ppi>kp);



private:
    vector <proletRF::TimeZoneRF> m_proletyRF;
    vector <proletZRV::ZRV> m_proletyZRV;
    vector <proletRF::Satellite> m_sattelites_list;

    //ниже все старое. Пока их не удаляю
    vector <Mylib::Ppi> m_kp = {};
    vector <Mylib::Time> m_segments = {};
};


#endif // MYLIB_H
