#include "editor.hpp"

#include <QAction>
#include <QDir>
#include <QFileDialog>
#include <QIcon>
#include <QKeySequence>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPointer>


Editor::Editor( int argc, char** argv ) :
    m_structureView( this ),
    m_starSystem( 0 )
{
    QMenuBar* menuBar = new QMenuBar( this );
    setMenuBar( menuBar );

    QMenu* file = menuBar->addMenu( tr( "File" ) );

    QAction* open = file->addAction( tr( "Open" ) );
    open->setIcon( QIcon::fromTheme( "document-open" ) );
    open->setShortcut( QKeySequence( Qt::CTRL | Qt::Key_O ) );
    connect( open, &QAction::triggered, this, &Editor::openFileDialog );

    QAction* save = file->addAction( tr( "Save" ) );
    save->setIcon( QIcon::fromTheme( "document-save" ) );

    QAction* saveAs = file->addAction( tr( "Save as" ) );
    saveAs->setIcon( QIcon::fromTheme( "document-save-as" ) );

    file->addSeparator();

    QAction* quit = file->addAction( tr( "Quit" ) );
    quit->setIcon( QIcon::fromTheme( "application-exit" ) );
    quit->setShortcut( QKeySequence( Qt::CTRL | Qt::Key_Q ) );
    connect( quit, &QAction::triggered, this, &Editor::quit );


    QMenu* edit = menuBar->addMenu( tr( "Edit" ) );

    QAction* undo = edit->addAction( tr( "Undo" ) );
    undo->setIcon( QIcon::fromTheme( "edit-undo" ) );
    undo->setShortcut( QKeySequence( Qt::CTRL | Qt::Key_Z ) );

    QAction* redo = edit->addAction( tr( "Redo" ) );
    redo->setIcon( QIcon::fromTheme( "edit-redo" ) );
    redo->setShortcut( QKeySequence( Qt::CTRL | Qt::SHIFT | Qt::Key_Z ) );

    edit->addSeparator();

    QAction* addEntity = edit->addAction( tr( "Add Entity" ) );
    addEntity->setIcon( QIcon::fromTheme( "list-add" ) );

    QAction* removeEntity = edit->addAction( tr( "Remove Entity" ) );
    removeEntity->setIcon( QIcon::fromTheme( "list-remove" ) );


    QMenu* help = menuBar->addMenu( tr( "Help" ) );
    QAction* about = help->addAction( tr( "About" ) );
    about->setIcon( QIcon::fromTheme( "help-about" ) );
    QAction* contents = help->addAction( tr( "What's this?" ) );
    contents->setIcon( QIcon::fromTheme( "help-contents" ) );
    help->addSeparator();
    QAction* debugView = help->addAction( tr( "Debug view" ) );
    debugView->setIcon( QIcon::fromTheme( "script-error" ) );
    debugView->setShortcut( Qt::Key_F9 );
    connect( debugView, &QAction::triggered, this, &Editor::openDebugView );

    setCentralWidget( &m_structureView );
}

void Editor::openFileDialog() {
    QPointer<QFileDialog> dialog = new QFileDialog( this, tr( "Open star system file..." ), QDir::home().absolutePath() );
    dialog->setAcceptMode( QFileDialog::AcceptOpen );
    if ( dialog->exec() && dialog ) {
        QFileInfo tmpFileInfo( dialog->selectedFiles()[0] );
        std::string errOut;
        Entry* tmpEntry = new Entry();
        if ( !Entry::fromFile( tmpFileInfo.absoluteFilePath().toUtf8().data(), tmpEntry, &errOut ) ) {
            delete tmpEntry;
            QPointer<QMessageBox> msg = new QMessageBox(
                QMessageBox::Critical, tr( "Parsing error" ), QString( errOut.c_str() ), QMessageBox::Ok, this );
            msg->exec();
            return;
        }
        m_currentStarSystemFile = tmpFileInfo;
        delete m_starSystem;
        m_starSystem = tmpEntry;
        m_structureView.setEntry( m_starSystem );
    }
}

void Editor::openDebugView() {
    m_debugView.setData( m_currentStarSystemFile, *m_starSystem );
    m_debugView.exec();
}
