#ifndef VIEWCHECK_H
#define VIEWCHECK_H

#include "ViewCheck_global.h"

#include "pluginview.h"



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
    //���ذ�ť
    QPushButton* loadBtn;

    QWidget* m_centerWidget;

    QtDrawingPaperEditor* center;

    //ͷ���򵥹��ܴ���
    QWidget* m_titleWidget;
    //�Զ���ؼ�����ʾͼƬ
    QString m_curFilePth;



    QPushButton *uploadBtn;
    QPushButton *resetPosBtn;

private:
    Ui::ViewCheck* ui;
};

#endif // VIEWCHECK_H
