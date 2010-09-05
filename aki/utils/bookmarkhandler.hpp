#ifndef AKI_BOOKMARKHANDLER_HPP
#define AKI_BOOKMARKHANDLER_HPP

#include <QObject>
#include <KBookmarkOwner>

class KActionCollection;
class KBookmarkMenu;
class KMenu;

namespace Aki
{
class BookmarkHandler : public QObject, public KBookmarkOwner
{
    Q_OBJECT
public:
    explicit BookmarkHandler(KMenu* menu, KActionCollection* actionCollection,
                             QObject* parent = 0);
    ~BookmarkHandler();
    virtual void openBookmark(const KBookmark& bookmark, Qt::MouseButtons mousebutton,
                              Qt::KeyboardModifiers modifiers);
private:
    KBookmarkManager* _bookmarkManager;
    KBookmarkMenu* _bookmarkMenu;
}; // End of class BookmarkHandler.
} // End of namespace Aki.

#endif // AKI_BOOKMARKHANDLER_HPP
