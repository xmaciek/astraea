#pragma once

#include <QMap>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeView>

#include "entry.hpp"

class StructureView : public QTreeView {
    Q_OBJECT
public:
    StructureView( QWidget* parent = 0 );
    void setEntry( const Entry* entry );

public slots:
    void selectEntry( quint64 );

private slots:
    void emitSelectionChanged();

signals:
    void entrySelected( quint64 );

private:
    QStandardItemModel m_model;

    QMap<quint64, QStandardItem*> m_itemMap;
    QMap<QStandardItem*, quint64> m_entryMap;

    void clear();
};
