#include "serverview.hpp"

ServerView::ServerView(const QString& title, QWidget* parent)
    : Aki::IBaseWindow(title, parent)
{
    setWindowType(ServerWindow);
}

ServerView::~ServerView()
{
}

QWidget*
ServerView::widget()
{
    return 0;
}
