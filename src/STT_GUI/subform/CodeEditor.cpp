/*******************************************************************
 * Copyright (C) 2014, All right reserved.

 * \file     CodeEditor.cpp
 * \version  1.0
 * \author   NingJian (freegodly@gmail.com)
 * \brief

 * \detail

 * TODO
 * \history  2014-4-15 created by NingJian
 *
 * \note
 /*******************************************************************/

//#include <QtWidgets>
#include <QTextBlock>
#include <QPainter>
#include "codeeditor.h"
#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>
#include <QApplication>
#include <QStringListModel>
#include <QDebug>
#include "stt_global.h"
#include <QStandardItemModel>
#include <QStandardItem>
#include <QList>
//![constructor]



CodeEditor::CodeEditor(QWidget *parent)
{
    highlighter = new Highlighter(this->document());


    completer_model = new QStandardItemModel(this);


    QStringList lua_funs;
    lua_funs<<"and"<<"break"<<"do"<<"else"<<"elseif"<<"end"<<"false"<<"for"<<"function"<<"if"<<"in"<<"local"<<"nil"<<"not" \
           <<"or"<<"repeat"<<"return"<<"then"<<"true"<<"until"<<"while"<<"error";


    for (int i = 0 ; i < lua_funs.count() ; i++ )
    {
        highlighter->setColorText(lua_funs[i],  Qt::blue );
        f_list.append(new QStandardItem(QIcon(":/icon/lua.ico"),lua_funs[i]));

    }
    //completer_model->appendColumn(lua_f_list);

    QStringList moudles_funs;

    std::map<std::string,TEST_FUN_INFO>::iterator iter;
    for(iter = G_Test_Fun_Info.begin(); iter != G_Test_Fun_Info.end(); iter++)
    {
        TEST_FUN_INFO tf= iter->second;
        moudles_funs.append(tf.fun_name.c_str());
    }


    for (int i = 0 ; i < moudles_funs.count() ; i++ )
    {
       // highlighter->setColorText(moudles_funs[i],  Qt::magenta );
       highlighter->setRegxColorText("^\\b"+moudles_funs[i]+"\\b",  Qt::magenta );
       f_list.append(new QStandardItem(QIcon(":/icon/moudles.ico"),moudles_funs[i]));
    }

     highlighter->setRegxColorText("\\[.+\\]",  Qt::darkRed );

    completer_model->clear();
    completer_model->appendColumn(f_list);



    lineNumberArea = new LineNumberArea(this);
    //初始化重要
    c=nullptr;
    initCompleter();
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this,SIGNAL(textChanged()),this,SLOT(updateColor()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

}

//![constructor]

//![extraAreaWidth]

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

//![extraAreaWidth]

//![slotUpdateExtraAreaWidth]

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

//![slotUpdateExtraAreaWidth]

//![slotUpdateRequest]

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

//![slotUpdateRequest]

//![resizeEvent]

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

//![resizeEvent]

//![cursorPositionChanged]

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

//![cursorPositionChanged]

//![extraAreaPaintEvent_0]

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(),  Qt::lightGray/*Qt::darkGray*/);

    //![extraAreaPaintEvent_0]

    //![extraAreaPaintEvent_1]
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    //![extraAreaPaintEvent_1]

    //![extraAreaPaintEvent_2]
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(/*Qt::black*/Qt::darkGray);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void CodeEditor::updateColor()
{
    //	highlighter->setDocument(this->document());
    this->update();
}

void CodeEditor::setCompleter( QCompleter *completer )
{
    if (c)
        QObject::disconnect(c, 0, this, 0);

    c = completer;

    if (!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));
}

QCompleter * CodeEditor::completer() const
{
    return c;
}

void CodeEditor::insertCompletion( const QString &completion )
{
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

QString CodeEditor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    //tc.select(QTextCursor::BlockUnderCursor);
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

void CodeEditor::focusInEvent( QFocusEvent *e )
{
    if (c)
        c->setWidget(this);
    //QTextEdit::focusInEvent(e);
    QPlainTextEdit::focusInEvent(e);
}

void CodeEditor::keyPressEvent( QKeyEvent *e )
{
    if (c && c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
        switch (e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
        default:
            break;
        }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!c || !isShortcut) // do not process the shortcut when we have a completer
        QPlainTextEdit::keyPressEvent(e);
    //! [7]

    //! [8]
    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!c || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    //static QString eow("~!@#$%^&*()+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 1
                        || eow.contains(e->text().right(1)))) {
        c->popup()->hide();
        return;
    }

    if (completionPrefix != c->completionPrefix()) {
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0)
                + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
}

void CodeEditor::initCompleter()
{
    QCompleter *completer = new QCompleter(this);
    completer->setModel(completer_model);
    //completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    this->setCompleter(completer);
}

//QAbstractItemModel * CodeEditor::modelFromFile()
//{
//    QFile file(mode_filename);
//    if (!file.open(QFile::ReadOnly))
//        return new QStringListModel(c);

//#ifndef QT_NO_CURSOR
//    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
//#endif
//    QStringList words;

//    while (!file.atEnd()) {
//        QByteArray line = file.readLine();
//        if (!line.isEmpty())
//            words << line.trimmed();
//    }

//#ifndef QT_NO_CURSOR
//    QApplication::restoreOverrideCursor();
//#endif
//    return new QStringListModel(words, c);
//}

//![extraAreaPaintEvent_2]

