#include "centerwidget.h"
#include "ui_centerwidget.h"
#include <QDebug>
#include <QPushButton>
#include <QRegularExpression>
#include "myhighlighter.h"

CenterWidget::CenterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CenterWidget)
{
    ui->setupUi(this);

}

CenterWidget::~CenterWidget()
{
    delete ui;
}

void CenterWidget::initWidget()
{
    m_wid = new QWidget(this);
    m_textEdit = new QTextEdit(m_wid);
//    QTextCursor cursor = m_textEdit->textCursor();
//    QTextImageFormat image;
//    QTextTableFormat table;
//    image.setName(QString("I:\\vs2019-project\\DSystem\\picture\\boy4.png"));
//    image.setWidth(200);
//    image.setHeight(200);
//    table.setWidth(1);
//    table.setBorder(1);
//    table.setCellSpacing(2);

//    cursor.insertImage(image);
//    cursor.insertTable(3,3,table);

}

void CenterWidget::resizeWidget()
{
    m_wid->resize(width(),height());
    m_textEdit->resize(m_wid->width(),m_wid->height());

}

//void CenterWidget::highlightBlock(const QString &text)
//{
//          QTextCharFormat myClassFormat;
//          myClassFormat.setFontWeight(QFont::Bold);
//          myClassFormat.setForeground(Qt::yellow);


//          QRegularExpression expression(text);
//          QRegularExpressionMatchIterator i = expression.globalMatch(m_textEdit->toPlainText());
//          while (i.hasNext())
//          {
//            QRegularExpressionMatch match = i.next();
//            setFormat(match.capturedStart(), match.capturedLength(), myClassFormat);

//          }


//}
