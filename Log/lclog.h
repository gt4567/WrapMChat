#ifndef LCLOG_H
#define LCLOG_H
#include <QString>
#include <QFile>
#include <QTextStream>
namespace LOG
{
    class lclog
    {
    private:
        lclog(QString _strPath);
    public :
     ~lclog();

    public:
      static lclog* instance(QString _strPath = "");
     static void freeData();

     void LogInfo(const QString& _LogType,const QString& _LogContent);
     QString getCurLogPath(){return m_curLogPath;}

     void setLogOn(bool _isOn =true){m_LogisOn = _isOn;}

    private:
     QFile       *m_file;
     QTextStream *m_stream;
     QString m_strCurDate;
     void newLog();
     void closeLog();
     QString m_curLogPath ;

     QString m_dirpath;
     void setPath(const QString& _strPath);

     bool m_LogisOn;

    };

}
#define LOGIN LOG::lclog::instance()
#endif // LCLOG_H
