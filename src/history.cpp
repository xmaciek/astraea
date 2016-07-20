#include "historyevent.hpp"
#include "history.hpp"

#include <cassert>

History::History( QObject* parent ) : QObject( parent ) {
    // let there be light!
    m_diary.push_back( new HistoryEvent() );
    m_bookMark = m_diary.begin();
}

void History::undo() {
    const bool canGoBack = m_bookMark != m_diary.begin();
    emit undoAvailable( canGoBack );
    emit redoAvailable( true );
    if ( !canGoBack ) {
        return;
    }
    HistoryEvent* reverseHistoryEvent = !(**m_bookMark);
    m_bookMark--;
    emit performHistoryEvent( reverseHistoryEvent );
    reverseHistoryEvent->deleteLater();
}

void History::redo() {
    bool canGoForward = m_bookMark != m_diary.end()--;
    if ( !canGoForward ) {
        return;
    }
    m_bookMark++;
    canGoForward = m_bookMark != m_diary.end();
    emit performHistoryEvent( *m_bookMark );
    emit redoAvailable( canGoForward );
    emit undoAvailable( true );
}

static void clearAfter( std::vector<HistoryEvent*>::iterator from, std::vector<HistoryEvent*>* diary ) {
    assert( diary );
    assert( from != diary->end() );
    std::vector<HistoryEvent*>::iterator it = ++from;
    while ( it != diary->end() ) {
        (*it)->deleteLater();
        it++;
    }
    diary->erase( from, diary->end() );
}

void History::addHistoryEvent( HistoryEvent* a ) {
    assert( !dynamic_cast<HistoryEvent*>( a ) && "History is not accepting base HistoryEvent class" );
    clearAfter( m_bookMark, &m_diary );
    m_diary.push_back( a );
    m_bookMark++;
    emit performHistoryEvent( a );
    emit undoAvailable( true );
    emit redoAvailable( false );
}

void History::clear() {
    emit undoAvailable( false );
    emit redoAvailable( false );
    assert( !m_diary.empty() );
    m_bookMark = m_diary.begin();
    clearAfter( m_bookMark, &m_diary );
}
