#include "historyevent.hpp"

HistoryEvent::HistoryEvent() { }
HistoryEvent::~HistoryEvent() {}
HistoryEvent* HistoryEvent::operator ! () const {
    return new HistoryEvent();
}
