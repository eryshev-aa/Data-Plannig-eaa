#ifndef ZRVPARSER_H
#define ZRVPARSER_H

#include <mylib.h>

#include <QFile>
#include <QTextStream>
#include <QFileInfo>

using namespace std;

/*!
 * \brief Класс для парсина файла(-ов) с данными
 */
class ZRVparser
{
public:
    ZRVparser();

    /*! Чтение файлов с данными по ЗРВ для пунктов ПИ
    * \param [in] files Пути к файлам с ЗРВ
    * \return вектор с ЗРВ для всех КА
    */
    vector <Mylib::Time> result(vector<QString> files);

    void SendReady(QFile *ofile, vector<Mylib::AnsVector> answer);

private:
    vector <Mylib::Time> m_segments = {};

};

#endif // ZRVPARSER_H
