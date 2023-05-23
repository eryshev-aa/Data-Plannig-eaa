#ifndef INPUTFILESHANDLER_H
#define INPUTFILESHANDLER_H

#include <iostream>
#include <vector>

#include <QDir>
#include <QFileInfo>

#include "tableprolet.h"

using namespace std;
using namespace proletRF;
using namespace proletZRV;

class InputFileRFHandler
{
public:
    InputFileRFHandler();

    bool make_proletRF(string dirPath, vector<TimeZoneRF> &rf_trace_list, vector<Satellite> &sattelites_list);
    bool make_ZRV_trace_list(string dirPath, vector<ZRV> &zrv_trace_list);

private:
    //vector <string> getFileNamesInDir_FS(string dirName); //gcc: filesistem
    vector <string> getFileNamesInDir_QDir(string dirName); //gcc+MinGW: QDir
};

#endif // INPUTFILESHANDLER_H
