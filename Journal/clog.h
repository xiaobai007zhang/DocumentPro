#ifndef CLOG_H
#define CLOG_H

#include <QDateTime>

#include "Journal_global.h"
#include "plugin.h"

class JOURNAL_EXPORT CLog : public QObject, public Plugin
{
    Q_OBJECT
    Q_INTERFACES(Plugin)
    Q_PLUGIN_METADATA(IID PluginInfo)
public:
    CLog();

public:
    void PrintLog(const QString& log) override;
    void errorLog(const QString& log) override;
};

#endif // CLOG_H
