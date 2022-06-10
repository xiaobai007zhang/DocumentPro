#ifndef CLOG_H
#define CLOG_H

#include <QDateTime>

#include "plugin.h"
#include "Journal_global.h"

class JOURNAL_EXPORT CLog:public Plugin
{

    Q_PLUGIN_METADATA(IID PluginInfo)
    Q_INTERFACES(Plugin)
    Q_CLASSINFO("Flag",2)

public:
    CLog();

public:
    void PrintLog(const QString & log) override;


//!普通成员变量
private:
    //记录当前日志的时间
    QString m_curDateTime;

};

#endif // CLOG_H
