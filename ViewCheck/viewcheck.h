#ifndef VIEWCHECK_H
#define VIEWCHECK_H

#include "ViewCheck_global.h"

#include "pluginview.h"

#include "attribute.h"
#include "drawingpapereditor.h"

#include <QFile>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QScrollArea>
#include <QTextStream>
#include <QWidget>

namespace Ui
{
    class ViewCheck;
}

class VIEWCHECK_EXPORT ViewCheck : public QWidget, public PluginView
// class VIEWCHECK_EXPORT ViewCheck : public QWidget
{
    Q_OBJECT
    Q_INTERFACES(PluginView)
    Q_PLUGIN_METADATA(IID PluginViewInfo)

public:
    explicit ViewCheck(QWidget* parent = nullptr);
    ~ViewCheck();

    void showTime() override;

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void initViewCheck();

    void loadStyleSheet(const QString& fileName);

    void initConnection();

    void initTitleWidget();

    void contextMenuEvent(QContextMenuEvent* event) override;

private slots:
    void slot_loadImage();
    void slot_attribute();
    void slot_uploadBtnClicked();
    void slot_resetPosBtnClicked();

private:
    QPushButton* loadBtn;

    //中心窗体和头部窗体
    QWidget* m_centerWidget;
    QWidget* m_titleWidget;

    QtDrawingPaperEditor* center;

    QPushButton* uploadBtn;
    QPushButton* resetPosBtn;

    Attribute* attribute;

    PixStatus M_FLAG;

private:
    Ui::ViewCheck* ui;
};

#endif // VIEWCHECK_H
