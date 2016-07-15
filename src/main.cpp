#include <QApplication>
#include "editor.hpp"

int main( int argc, char** argv ) {
    QApplication app( argc, argv );
    Editor editor( argc, argv );
    editor.show();
    return app.exec();
}
