#pragma once

#include <QMainWindow>

#include "structureview.hpp"

class Editor : public QMainWindow {
    Q_OBJECT

public:
    Editor( int argc, char** argv );

private:
    StructureView m_structureView;

signals:
    void quit();
};


