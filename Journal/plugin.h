#ifndef PLUGIN_H
#define PLUGIN_H

#define PluginInfo "PluginInfo1.0"
#include <QObject>

//插件抽象类
class Plugin
{
public:
    virtual ~Plugin(){};
    virtual void PrintLog(const QString&) = 0;
    virtual void errorLog(const QString&) = 0;
};

Q_DECLARE_INTERFACE(Plugin, PluginInfo)
#endif // PLUGIN_H
