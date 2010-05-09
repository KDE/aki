#ifndef SERVERVIEW_HPP
#define SERVERVIEW_HPP

#include "interfaces/ibasewindow.hpp"

class ServerView : public Aki::IBaseWindow
{
    Q_OBJECT
public:
    ServerView(const QString& title, QWidget* parent = 0);
    ~ServerView();
    virtual QWidget* widget();
private:
    
}; // End of class ServerView.

#endif // SERVERVIEW_HPP
