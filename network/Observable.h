#ifndef OBSERVABLE_H
#define OBSERVABLE_H
#include <QWeakPointer>

class Observer;


class Observable
{
public:
    virtual ~Observable() {}

    virtual void addObserver(QWeakPointer<Observer> observer) = 0;
    virtual void delObserver(QWeakPointer<Observer> observer) = 0;
    virtual void notifyObservers() = 0;
};

#endif // OBSERVABLE_H
