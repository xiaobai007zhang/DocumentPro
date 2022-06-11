#ifndef CENTERWIDGET_H
#define CENTERWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QSyntaxHighlighter>

namespace Ui {
class CenterWidget;
}
//test
class CenterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CenterWidget(QWidget *parent = nullptr);
    ~CenterWidget();

public:
    void initWidget();
    void resizeWidget();

    //void highlightBlock(const QString &text);


public:
    QTextEdit *m_textEdit;
    QWidget *m_wid;

private:
    Ui::CenterWidget *ui;
};

#endif // CENTERWIDGET_H
