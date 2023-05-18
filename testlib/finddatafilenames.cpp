#include "finddatafilenames.h"

#include <iostream>

#include <QDir>
#include <QFileInfo>

FindDataFileNames::FindDataFileNames(QObject *parent)
    : QObject{parent}
{

}

vector <string> FindDataFileNames::getDataFileNames(QSettings &set, QString appDirPath){
    vector <string> result;

    QString pathZRV; //относительный путь к папке с данными. В этой папке может храниться множество файлов.

    if (set.status() == QSettings::NoError) {
        set.beginGroup("input data");
        pathZRV = set.value("pathZRV").toString();
        set.endGroup();
    } else {
        cout << "Settings file read error. App close!" << endl;
        result.clear();
        return result;
    }

    if (pathZRV.isEmpty()) {
        cout << "Data folder path not set. App close!" << endl;
        result.clear();
        return result;
    } else {
        #ifdef Q_OS_WIN
            QDir dataDir(appDirPath);
        #endif
        #ifdef Q_OS_LINUX
            QDir dataDir;
        #endif

        //QDir dataDir/*(appDirPath)*/;
        dataDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot); //будем искать в каталоге сборки папку с данными
        dataDir.setSorting(QDir::Name);

        QFileInfoList dirsPathList = dataDir.entryInfoList(); //получаем список директорий
        QStringList dirsNameList;

        #ifdef Q_OS_WIN
        for (int i = 0; i < dirsPathList.size(); i++) {
            dirsNameList.append(dirsPathList.at(i).fileName());
        }
        #endif

        #ifdef Q_OS_WIN
            if (dirsNameList.contains(pathZRV)) { //проверяем, что нужная нам папке существует.
        #endif
        #ifdef Q_OS_LINUX
            if (dirsPathList.contains(pathZRV)) { //проверяем, что нужная нам папке существует.
        #endif
            QDir filesDir(appDirPath + "/" +pathZRV);
            //dataDir.setCurrent(tmp); //переходим в неё
            filesDir.setFilter(QDir::Files); //далее будем искать в папке с данными файлы
            dirsPathList = filesDir.entryInfoList(); //получаем список файлов
            if (dirsPathList.count() > 0) {
                for (int i = 0; i < dirsPathList.size(); ++i) {
                    //цикл по файлам в папке Data
                    QFileInfo fileInfo = dirsPathList.at(i);
                    result.push_back(fileInfo.absolutePath().toStdString() + "/" + fileInfo.fileName().toStdString());
                }
            } else {
                cout << "Data folder is empty. App close!" << endl;
                result.clear();
                return result;
            }
        } else {
            cout << "Data folder path not exist. App close!" << endl;
            result.clear();
            return result;
        }
        dataDir.setCurrent(dataDir.currentPath());
    }

    return result;
}
