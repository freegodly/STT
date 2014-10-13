/*******************************************************************
 * Copyright (C) 2014, All right reserved.

 * \file     CodeEditor.h
 * \version  1.0
 * \author   NingJian (freegodly@gmail.com)
 * \brief    
   
 * \detail   

 * TODO     
 * \history  2014-4-15 created by NingJian
 * 
 * \note    
 /*******************************************************************/

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include "Highlighter.h"

#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class QCompleter;
class LineNumberArea;
class Highlighter;
class QAbstractItemModel;

QT_END_NAMESPACE


class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:

    CodeEditor(QWidget *parent);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    Highlighter * highlighter ;

	void setCompleter(QCompleter *c);
	QCompleter *completer() const;
    //QAbstractItemModel *modelFromFile();
    QStandardItemModel *completer_model;
    QList<QStandardItem*> f_list;

protected:
    void resizeEvent(QResizeEvent *event);
	void keyPressEvent(QKeyEvent *e);
	void focusInEvent(QFocusEvent *e);
private slots:
	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect &, int);
	void insertCompletion(const QString &completion);
	void updateColor();
        
private:
	QString textUnderCursor() const;
	void initCompleter();
    QWidget *lineNumberArea;
	QCompleter *c;
	QString mode_filename;


};



class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};



#endif
