#ifndef CLOG_H
#define CLOG_H

#include <QDateTime>

#include "plugin.h"
#include "Journal_global.h"

class JOURNAL_EXPORT CLog:public QObject,public Plugin
{
    Q_OBJECT
    Q_INTERFACES(Plugin)
    Q_PLUGIN_METADATA(IID Info)


public:
    CLog();

public:
    void PrintLog(const QString & log) override;
    void errorLog(const QString & log) override;
};

#endif // CLOG_H
