#include <QObject>
//#include <QApplication>


#include <QTextCodec>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include "lclog.h"


namespace LOG
{
    lclog* g_lclog = 0;
    lclog::lclog (QString _strPath): m_file(0), m_stream(0), m_dirpath(_strPath)
    {
        newLog();
        m_LogisOn =true;

    }
    void lclog::setPath(const QString& _strPath)
    {
        m_dirpath = _strPath;
    }

    void lclog::newLog()
    {
        QString strPah = "Log";
        if(m_dirpath != "")
            strPah = m_dirpath +"//Log";
        QDir dir(strPah);
        if (!dir.exists())
        {
            QDir dir1;
            dir1.mkpath(strPah);
           // QDir::mkpath(strPah);
            // dir.mkpath(strPah);
             qDebug() << "Log strPah<<:" << dir1.absolutePath();

        }
        m_strCurDate = QDate::currentDate().toString("yyyyMMdd");
        QString fileName(dir.path()+"/"+m_strCurDate+".log");
        m_curLogPath = fileName;
        m_file =new QFile(fileName);
        qDebug() << "Log File Pah<<:" << dir.path()+"/"+m_strCurDate+".log";
        if(m_file->exists())
        {
            m_file->open(QIODevice::Append | QIODevice::Text);
        }
        else
            m_file->open(QIODevice::ReadWrite | QIODevice::Text);
        m_stream = new QTextStream(m_file);
    }

    void lclog::closeLog()
    {
        if(m_stream)
        {
            delete m_stream;
            m_stream = 0;
        }
        if(m_file)
        {
            m_file->close();
            delete m_file;
        }
    }

    lclog::~lclog()
    {
        closeLog();

    }

    lclog* lclog::instance(QString _strPath)
    {
        if(g_lclog==0)
        {
            g_lclog = new lclog(_strPath);
        }
        return g_lclog;
    }

    void lclog::freeData()
    {
        if(g_lclog != 0)
            delete g_lclog;
        g_lclog = 0;
    }

    void lclog::LogInfo(const QString& _LogType,const QString& _LogContent)
    {
        if(!m_LogisOn)
            return;
        QString strDate = QDate::currentDate().toString("yyyyMMdd");
        if(strDate != m_strCurDate)
        {
            closeLog();
            newLog();
        }

        qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz")+\
                    " ["+_LogType+"] "+_LogContent;
        *m_stream << QTime::currentTime().toString("hh:mm:ss.zzz")+\
                    " ["+_LogType+"] "+_LogContent << "\n";
        m_stream->flush();

    }

}
