#include "editor.hpp"

#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <QMenu>
#include <QMenuBar>


Editor::Editor( int argc, char** argv ) {
    QMenuBar* menuBar = new QMenuBar( this );
    setMenuBar( menuBar );

    QMenu* file = menuBar->addMenu( tr( "File" ) );

    QAction* open = file->addAction( tr( "Open" ) );
    open->setIcon( QIcon::fromTheme( "document-open" ) );

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
}
