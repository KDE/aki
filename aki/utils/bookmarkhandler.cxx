#include "bookmarkhandler.hpp"
#include <KDE/KActionCollection>
#include <KDE/KBookmarkManager>
#include <KDE/KBookmarkMenu>
#include <KDE/KGlobal>
#include <KDE/KStandardDirs>
using namespace Aki;

BookmarkHandler::BookmarkHandler(KMenu* menu, KActionCollection* actionCollection, QObject* parent)
    : QObject(parent),
    KBookmarkOwner()
{
    const QString bookmarkFile = KStandardDirs::locateLocal("data", "aki/bookmarks.xml");

    KBookmarkManager* manager = KBookmarkManager::managerForFile(bookmarkFile, "aki");
    manager->setUpdate(true);
    _bookmarkMenu = new KBookmarkMenu(manager, this, menu, actionCollection);
}

BookmarkHandler::~BookmarkHandler()
{
}

void BookmarkHandler::openBookmark(const KBookmark& bookmark, Qt::MouseButtons mousebutton,
                                   Qt::KeyboardModifiers modifiers)
{
    Q_UNUSED(bookmark);
    Q_UNUSED(mousebutton);
    Q_UNUSED(modifiers);
}
