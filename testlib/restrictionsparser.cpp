#include "restrictionsparser.h"

#include <sstream>
#include <utility>
RestrictionsParser::RestrictionsParser()
{

}

vector <Mylib::Ppi> RestrictionsParser::makePpi(QFile *restrictionsFile) {
    if (!restrictionsFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout << "File restrictions open error" << endl;
    }

    int i = 0;
    QTextStream in(restrictionsFile);

    while (!in.atEnd()){
        int ppi2=0;
        int tmp;

        string tmpLn = in.readLine().toStdString();
        if (tmpLn.find("#") != std::string::npos) { //добавил в файл ограничений комментарий, используя символ #. Поэтому такие строки пропускаем.
            continue;
        }

        istringstream l(tmpLn);
        l >> tmp;

        if (ppi2 == tmp){
            int t;
            l >> t;
            m_kp[i-1].kp.push_back(t);
            vector <int> temp;

            while (l >> t){
                temp.push_back(t);
            }

            m_kp[i-1].agreed.push_back(temp);

            for(int j = 0; j < i; j++){
                m_kp[i-1].isbusy.push_back("0000-00-00 00:00:00");
                m_kp[i-1].dopelganger.push_back(make_pair("0000-00-00 00:00:00",0));
            }
            //kp[i-1].isbusy.resize(kp[i-1].kp.size());
        } else {
            i++;
            int t;
            l >> t;

            Mylib::Ppi v;
            v.ppi=tmp;
            v.kp.push_back(t);

            vector <int> temp;

            while(l >> t){
                temp.push_back(t);
            }

            v.agreed.push_back(temp);
            v.isbusy.push_back("0000-00-00 00:00:00"); // add by EAA
            v.dopelganger.push_back(make_pair("0000-00-00 00:00:00", 0));
            m_kp.push_back(v);
        }
        ppi2 = tmp;
    }

    restrictionsFile->close();

    return m_kp;
}
