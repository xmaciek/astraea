#pragma once

#include <QObject>

class HistoryEvent : public QObject {
    Q_OBJECT
public:
    HistoryEvent();
    virtual ~HistoryEvent();

    // returns new negative action
    virtual HistoryEvent* operator ! () const;
};
