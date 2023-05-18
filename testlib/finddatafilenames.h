#ifndef FINDDATAFILENAMES_H
#define FINDDATAFILENAMES_H

#include <QObject>
#include <QSettings>

using namespace std;

/*!
 * \brief Класс поиска файлов в данными
 */
class FindDataFileNames : public QObject
{
    Q_OBJECT
public:
    explicit FindDataFileNames(QObject *parent = nullptr);
    //~FindDataFileNames();

    /*! Получить список с путями и названиями файлов с данными
    * \param [in] set Описание конфигурационного файла
    * \param [in] appDirPath Путь до испольняемого файла программы
    * \return вектор с путями и названиями файлов с данными.
    */
    vector<string> getDataFileNames(QSettings &set, QString appDirPath);

signals:

};

#endif // FINDDATAFILENAMES_H
