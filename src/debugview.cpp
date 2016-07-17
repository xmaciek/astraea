#include "debugview.hpp"

#include <QFile>
#include <QHBoxLayout>
#include <QTextStream>
#include <QVBoxLayout>

#include <sstream>


DebugView::DebugView( QWidget* parent ) :
    QDialog( parent ),
    m_lblParsed( tr( "Parsed script" ), this ),
    m_lblRaw( tr( "Raw script" ), this ),
    m_parsedData( this ),
    m_rawData( this )
{
    QHBoxLayout* mainLayout = new QHBoxLayout( this );
    QVBoxLayout* left = new QVBoxLayout();
    QVBoxLayout* right = new QVBoxLayout();

    left->addWidget( &m_lblParsed );
    left->addWidget( &m_parsedData );

    right->addWidget( &m_lblRaw );
    right->addWidget( &m_rawData );

    mainLayout->addLayout( left );
    mainLayout->addLayout( right );

    m_parsedData.setReadOnly( true );
    m_parsedData.setWordWrapMode( QTextOption::NoWrap );

    m_rawData.setReadOnly( true );
    m_rawData.setWordWrapMode( QTextOption::NoWrap );
}

void DebugView::setData( const QFileInfo& fileInfo, const Entry& entry ) {
    m_parsedData.clear();
    m_rawData.clear();
    setWindowTitle( tr( "Debug view: " ) + fileInfo.fileName() );
    std::stringstream stream;
    entry.writeToStream( stream, 4, 0 );
    m_parsedData.setPlainText( stream.str().c_str() );

    QFile file( fileInfo.absoluteFilePath() );
    file.open( QIODevice::ReadOnly | QIODevice::Text );
    QTextStream fileStream( &file );
    m_rawData.setPlainText( fileStream.readAll() );
}
