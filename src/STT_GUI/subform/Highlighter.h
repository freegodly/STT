/*******************************************************************
 * Copyright (C) 2014, All right reserved.

 * \file     Highlighter.h
 * \version  1.0
 * \author   NingJian (freegodly@gmail.com)
 * \brief    
   
 * \detail   

 * TODO     
 * \history  2014-4-15 created by NingJian
 * 
 * \note    
 /*******************************************************************/

#ifndef Highlighter_h
#define Highlighter_h


#include <QSyntaxHighlighter>
#include <QString>
#include <QPlainTextEdit>
#include <QTextDocument>
#include <QRegExp>
class Highlighter : public QSyntaxHighlighter
{
public:
    Highlighter(QTextDocument  *parent = 0);
    void highlightBlock(const QString &text);
    //添加指定规则，对指定的关键字进行，特定颜色的显示
    void setColorText(const QString &str, const QColor &color);
    void setRegxColorText(const QString &str, const QColor &color);
    //清空所有规则
    void clearRules();
    QRegExp commentStartExpression;  
    QRegExp commentEndExpression;  
    QRegExp commentExpression;  
    QTextCharFormat multiLineCommentFormat;  
private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;
};

#endif
