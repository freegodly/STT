/*******************************************************************
* Copyright (C) 2014, All right reserved.

* \file     Highlighter.cpp
* \version  1.0
* \author   NingJian (freegodly@gmail.com)
* \brief    

* \detail   

* TODO     
* \history  2014-4-15 created by NingJian
* 
* \note    
/*******************************************************************/

#include "Highlighter.h"


Highlighter::Highlighter(QTextDocument  *parent)
    : QSyntaxHighlighter(parent)
{
    highlightingRules.clear();
    //关于高亮代码的注释  

    commentExpression =  QRegExp("--[^[].*$");  
    commentStartExpression = QRegExp("--\\[\\[");  
    commentEndExpression = QRegExp("\\]\\]");  
    multiLineCommentFormat.setForeground(Qt::darkGreen);
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (HighlightingRule rule, highlightingRules) 
    {
        QRegExp expression(rule.pattern);
        int index = text.indexOf(expression);
        while (index >= 0) 
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = text.indexOf(expression, index + length);
        }
    }

    // 设置当前的Block状态  
    setCurrentBlockState(0);  
    int startIndex = 0;  
    //多行进行扫描设置  
    if (previousBlockState() != 1)  
    {
        startIndex = commentExpression.indexIn(text);  
        setFormat(startIndex, commentExpression.matchedLength(), multiLineCommentFormat);  
        startIndex = commentStartExpression.indexIn(text, startIndex + commentExpression.matchedLength()); 
    }


    setCurrentBlockState(0);  
    startIndex = 0;  
    //多行进行扫描设置  
    if (previousBlockState() != 1)  
        startIndex = commentStartExpression.indexIn(text);  
    while (startIndex >= 0) {  
        int endIndex = commentEndExpression.indexIn(text, startIndex);  
        int commentLength;  
        if (endIndex == -1) {  
            setCurrentBlockState(1);  
            commentLength = text.length() - startIndex;  
        } else {  
            commentLength = endIndex - startIndex  
                + commentEndExpression.matchedLength();  
        }  
        setFormat(startIndex, commentLength, multiLineCommentFormat);  
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);  
    }  


}

void Highlighter::setColorText(const QString &str, const QColor &color)
{
    HighlightingRule rule;
    rule.pattern = QRegExp("\\b"+str+"\\b");
    QTextCharFormat format;
    format.setForeground(color);
    rule.format = format;
    highlightingRules.append(rule);
}

void Highlighter::setRegxColorText(const QString &str, const QColor &color)
{
    HighlightingRule rule;
    rule.pattern = QRegExp(str);
    QTextCharFormat format;
    format.setForeground(color);
    rule.format = format;
    highlightingRules.append(rule);
}

void Highlighter::clearRules()
{
    highlightingRules.clear();
}
