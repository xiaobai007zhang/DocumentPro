#include "myhighlighter.h"
#include <QRegularExpression>
#include <QDebug>

MyHighLighter::MyHighLighter(QTextDocument *parent,QString findText):QSyntaxHighlighter(parent),m_findText(findText)
{

}

void MyHighLighter::highlightBlock(const QString &text)
{
    qDebug()<<text<<"111111111";
    qDebug()<<m_findText;
    QTextCharFormat myClassFormat;
    myClassFormat.setFontWeight(QFont::Bold);
    myClassFormat.setForeground(Qt::blue);


    QRegularExpression expression(m_findText);

    QRegularExpressionMatchIterator i = expression.globalMatch(text);
    while (i.hasNext())
    {
      QRegularExpressionMatch match = i.next();
      setFormat(match.capturedStart(), match.capturedLength(), myClassFormat);

    }
}
