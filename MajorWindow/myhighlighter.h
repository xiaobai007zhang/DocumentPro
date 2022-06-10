#ifndef MYHIGHLIGHTER_H
#define MYHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class MyHighLighter : public QSyntaxHighlighter
{
public:
    MyHighLighter(QTextDocument *parent = nullptr,QString findText = "");

    void highlightBlock(const QString &text);

private:
    QString m_findText;
};

#endif // MYHIGHLIGHTER_H
