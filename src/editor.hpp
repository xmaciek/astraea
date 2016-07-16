#pragma once

#include <QFileInfo>
#include <QMainWindow>

#include "entry.hpp"
#include "structureview.hpp"

class Editor : public QMainWindow {
    Q_OBJECT

public:
    Editor( int argc, char** argv );

private:
    StructureView m_structureView;

    Entry* m_starSystem;
    QFileInfo m_currentStarSystemFile;

private slots:
    void openFileDialog();

signals:
    void quit();
};


