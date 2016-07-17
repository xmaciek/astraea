#pragma once

#include <QDialog>
#include <QFileInfo>
#include <QLabel>
#include <QTextEdit>

#include "entry.hpp"
class DebugView : public QDialog {
    Q_OBJECT
public:
    DebugView( QWidget* parent = 0 );
    void setData( const QFileInfo& fileInfo, const Entry& e );

private:
    QLabel m_lblParsed;
    QLabel m_lblRaw;
    QTextEdit m_parsedData;
    QTextEdit m_rawData;
};
