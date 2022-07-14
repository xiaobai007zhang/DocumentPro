#ifndef PLUGINVIEW_H
#define PLUGINVIEW_H

#include <QObject>
#define PluginViewInfo "PluginView1.0"

class PluginView
{
    // Q_OBJECT
public:
    ~PluginView(){};
    PluginView();
    virtual void showTime() = 0;
};

Q_DECLARE_INTERFACE(PluginView, PluginViewInfo)
#endif // PLUGINVIEW_H
