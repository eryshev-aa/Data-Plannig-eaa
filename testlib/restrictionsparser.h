#ifndef RESTRICTIONSPARSER_H
#define RESTRICTIONSPARSER_H

#include <mylib.h>

#include <QFile>
#include <QTextStream>

using namespace std;

/*!
 * \brief Класс для парсина файла с ограничениями
 */
class RestrictionsParser
{
public:
    RestrictionsParser();

    /*! Чтение файла с ограничениями для пунктов ПИ
    * \param [in] restrictionsFile Фалйл с ограничениями
    * \return вектор органичений для пунктов ПИ.
    */
    vector <Mylib::Ppi> makePpi(QFile *restrictionsFile);

private:
    vector <Mylib::Ppi> m_kp = {};
};

#endif // RESTRICTIONSPARSER_H
