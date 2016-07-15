#include "structureview.hpp"

#include <QHeaderView>

#include <cassert>


StructureView::StructureView( QWidget* parent ) :
    QTreeView( parent )
{
    setModel( &m_model );
    QTreeView::header()->hide();
    connect( selectionModel(), &QItemSelectionModel::selectionChanged, this, &StructureView::emitSelectionChanged );
}

typedef QStandardItem* ItemPtr;

static ItemPtr entryToQStandardItem( const Entry* entry, QMap<quint64,ItemPtr>* itemMap, QMap<ItemPtr,quint64>* entryMap  ) {
    assert( entry );
    assert( itemMap );
    assert( entryMap );

    ItemPtr item = new QStandardItem( entry->m_name.c_str() );
    item->setEditable( false );
    itemMap->insert( entry->m_id, item );
    entryMap->insert( item, entry->m_id );
    Entries::const_iterator it = entry->m_entries.cbegin();
    while ( it != entry->m_entries.cend() ) {
        item->appendRow( entryToQStandardItem( &(*it), itemMap, entryMap ) );
        it++;
    }
    return item;
}

void StructureView::setEntry( const Entry* entry ) {
    assert( entry );
    clear();
    m_model.appendRow( entryToQStandardItem( entry, &m_itemMap, &m_entryMap ) );
}

void StructureView::clear() {
    m_itemMap.clear();
    m_entryMap.clear();
    m_model.clear();
}

void StructureView::selectEntry( quint64 id ) {
    QMap<quint64, ItemPtr>::const_iterator it = m_itemMap.find( id );
    if ( it == m_itemMap.end() ) {
        selectionModel()->clear();
    } else {
        setCurrentIndex( m_model.indexFromItem( it.value() ) );
    }
}


void StructureView::emitSelectionChanged() {
    QStandardItem* item = m_model.itemFromIndex( currentIndex() );
    QMap<ItemPtr, quint64>::const_iterator it = m_entryMap.find( item );
    assert( it != m_entryMap.end() );
    emit entrySelected( it.value() );
}

