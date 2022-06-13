#include "clog.h"
#include <QFile>
#include <QTextStream>

#include <QDir>
#include <QDebug>

CLog::CLog()
{

}

//!记录日志
void CLog::PrintLog(const QString &log)
{
    QString curDateTime = QDateTime::currentDateTime().toString("yyyy/MM.dd hh:mm:ss");

    QString text = curDateTime + " ## " +log;
    QDir path("../LogFile");

    if(!path.exists()){
        path.mkdir("../LogFile");
    }

    QFile file("../LogFile/log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);

    if(!file.isOpen()){
        return;
    }

    QTextStream stream(&file);
    stream<<text;
    stream<<endl;

    file.close();
}

//!打印错误信息日志
void CLog::errorLog(const QString & log)
{
    QString curDateTime = QDateTime::currentDateTime().toString("yyyy/MM.dd hh:mm:ss");
    QString text = curDateTime + " ## " +log;

    QDir path("../LogFile");

    if(!path.exists()){
        path.mkdir("../LogFile");
    }

    QFile file("../LogFile/error.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);

    if(!file.isOpen()){
        return;
    }

    QTextStream stream(&file);
    stream<<text;
    stream<<endl;

    file.close();
}
