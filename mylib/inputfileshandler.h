#ifndef INPUTFILESHANDLER_H
#define INPUTFILESHANDLER_H

#include <iostream>
#include <vector>

#include "tableprolet.h"

using namespace std;
using namespace proletRF;
using namespace proletZRV;

class InputFileRFHandler
{
public:
    InputFileRFHandler();

    bool make_RF_trace_list(string dirPath, vector<TimeZoneRF> &rf_trace_list);
    bool make_ZRV_trace_list(string dirPath, vector<ZRV> &zrv_trace_list);

private:
    vector <string> getFileNamesInDir_FS(string dirName);
};

#endif // INPUTFILESHANDLER_H
