#include <QApplication>
#include "editor.hpp"

int main( int argc, char** argv ) {
    QApplication app( argc, argv );
    Editor editor( argc, argv );
    QObject::connect( &editor, &Editor::quit, &app, &QApplication::quit );
    editor.show();
    return app.exec();
}
