#pragma once

#include <QFileInfo>
#include <QMainWindow>

#include "debugview.hpp"
#include "entry.hpp"
#include "structureview.hpp"

class Editor : public QMainWindow {
    Q_OBJECT

public:
    Editor( int argc, char** argv );

private:
    DebugView m_debugView;
    StructureView m_structureView;

    Entry* m_starSystem;
    QFileInfo m_currentStarSystemFile;

private slots:
    void openDebugView();
    void openFileDialog();

signals:
    void quit();
};


