#include "clog.h"
#include <QFile>
#include <QTextStream>

CLog::CLog()
{

}

//!记录日志
void CLog::PrintLog(const QString &log)
{
    QString curDateTime = QDateTime::currentDateTime().toString("yyyy/MM.dd hh:mm:ss");

    QString text = curDateTime + " ## " +log;

    QFile file("../Log/log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);

    if(!file.isOpen()){
        return;
    }

    QTextStream stream(&file);
    stream<<text;

    file.close();

}
