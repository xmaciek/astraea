#pragma once

#include <QObject>
#include <vector>

class HistoryEvent;

class History : public QObject {
    Q_OBJECT
public:
    History( QObject* parent = 0 );

public slots:
    void undo();
    void redo();
    void addHistoryEvent( HistoryEvent* a );
    void clear();

signals:
    void performHistoryEvent( const HistoryEvent* );
    void undoAvailable( bool );
    void redoAvailable( bool );

private:
    std::vector<HistoryEvent*> m_diary;
    std::vector<HistoryEvent*>::iterator m_bookMark;
};
