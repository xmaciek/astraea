#pragma once

#include <QMainWindow>

class Editor : public QMainWindow {
    Q_OBJECT

public:
    Editor( int argc, char** argv );

signals:
    void quit();
};


