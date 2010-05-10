#include "chatview_p.hpp"
#include "ui/chatview.hpp"
#include <KDE/KApplication>
#include <KDE/KDateTime>
#include <KDE/KDebug>
#include <KDE/KFileDialog>
#include <KDE/KIcon>
#include <KDE/KToolInvocation>
#include <KDE/KUser>
#include <KDE/KIO/Job>
#include <QtGui/QClipboard>
#include <QtGui/QMenu>
#include <QtGui/QTextDocument>
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebFrame>
using namespace Aki;

ChatViewPrivate::ChatViewPrivate(ChatView* qq)
    : QObject(),
    _q(qq)
{
    KEmoticons emotes;
    emoTheme = emotes.theme();
    qsrand(std::time(0));
    doc.reset(new QDomDocument("html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\""
                               "    \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\""));
    createShell();
}

QString
ChatViewPrivate::createShell()
{
    QDomElement html = doc->createElement("html");
    html.setAttribute("xmlns", "http://www.w3.org/1999/xhtml");
    html.setAttribute("lang", "en");
    html.setAttribute("xml:lang", "en");
    QDomElement head = doc->createElement("head");
    QDomElement title = doc->createElement("title");
    title.appendChild(doc->createTextNode("Aki IRC Client"));
    QDomElement metaCT = doc->createElement("meta");
    metaCT.setAttribute("http-equiv", "Content-Type");
    metaCT.setAttribute("content", "application/xhtml+xml; charset=UTF-8");
    QDomElement metaAC = doc->createElement("meta");
    metaAC.setAttribute("http-equiv", "Accept-Content");
    metaAC.setAttribute("content", "utf-8");
    head.appendChild(title);
    head.appendChild(metaCT);
    head.appendChild(metaAC);
    QDomElement body = doc->createElement("body");
    body.appendChild(log(messageTime(), QString()));
    html.appendChild(head);
    html.appendChild(body);

    doc->appendChild(html);
    _q->setContent(doc->toByteArray(4), "application/xhtml+xml");
    doc->clear();
    return QString();
}

QString
ChatViewPrivate::unescape(const QString& str)
{
    QString tmp = str;
    tmp.replace("&gt;", ">");
    tmp.replace("&lt;", "<");
    tmp.replace("&amp;", "&");
    return tmp;
}

QWebFrame*
ChatViewPrivate::mainFrame()
{
    return _q->page()->mainFrame();
}

void
ChatViewPrivate::appendMessage(const QString& message)
{
    const int maxVert = mainFrame()->scrollBarMaximum(Qt::Vertical);
    const int value = mainFrame()->scrollBarValue(Qt::Vertical);

    userMoved = (maxVert == value) ? true : false;

    QWebElement log = mainFrame()->findFirstElement("body log");
    Q_ASSERT(log.isNull());
    log.appendInside(unescape(message));
}

void
ChatViewPrivate::appendMessage(const QDomElement& element)
{
    doc->appendChild(element);
    appendMessage(doc->toString(4));
    doc->clear();
}

QString
ChatViewPrivate::convertEmotes(const QString& str)
{
    return parseEmoticons(str, KEmoticonsTheme::StrictParse);
}

QString
ChatViewPrivate::messageTime() const
{
    // TODO Change this to deal with themes in xsl
    QTime time = KDateTime::currentLocalTime();
    QString timeStr = KGlobal::locale()->formatLocaleTime(time);
    return timeStr;
}

QString
ChatViewPrivate::stripHtml(const QString& message) const
{
    QString msg = message;
    msg.replace(QRegExp("<[^>]*>"), "");
    return msg;
}

void
ChatViewPrivate::contentsSizeChanged(const QSize& size)
{
    if (!userMoved) {
        mainFrame()->scroll(size.width(), size.height());
    }
}

QString
ChatViewPrivate::parseEmoticons(const QString& message, KEmoticonsTheme::ParseMode mode,
                                const QStringList& excludes) const
{
    // Exact code from parseEmoticons in parseEmoticons()
    // Juse added function to add file:// to the path because WebKit won't
    // load them without it.
    QList<KEmoticonsTheme::Token> tokens =
        emoTheme.tokenize(message, mode | KEmoticonsTheme::SkipHTML);
    if (tokens.isEmpty() && !message.isEmpty()) {
        return message;
    }

    QString result;

    foreach (const KEmoticonsTheme::Token& token, tokens) {
        switch (token.type) {
        case KEmoticonsTheme::Text: {
            result += token.text;
            break;
        }
        case KEmoticonsTheme::Image: {
            if (!excludes.contains(token.text)) {
                result += token.picHTMLCode;
            } else {
                result += token.text;
            }
            break;
        }
        default: {
            kWarning() << "Unknown token type. Something's broken.";
            break;
        }
        }
    }

    result.replace("src=\"", "src\"file://");
    return result;
}

void
ChatViewPrivate::linkClicked(const KUrl& url)
{
    KToolInvocation::invokeBrowser(url.url());
}

void
ChatViewPrivate::customContextMenuRequested(const QPoint& pos)
{
    QWebHitTestResult hit = mainFrame()->hitTestContent(pos);
    if (hit.isNull()) {
        return;
    }

    if (hit.linkElement().tagName() == "A" &&
        hit.linkElement().hasAttribute("href")) {
        KUrl url(hit.linkUrl());
        QString href = url.url().remove("about:blank");

        if (href[0] == QLatin1Char('#')) {
            emit _q->userUrlClicked(href.remove(QLatin1Char('#')));
        } else {
            QMenu* menu = new QMenu(_q);
            menu->setTitle(i18n("Normal"));

            QAction* action = new QAction(KIcon("edit-copy"),
                                          i18n("Copy Link Address"), menu);
            connect(action, SIGNAL(triggered(bool)),
                    SLOT(copyUrlTriggered()));
            menu->addAction(action);

            action = new QAction(KIcon("document-save-as"),
                                 i18n("Save Link As..."), menu);
            connect(action, SIGNAL(triggered(bool)),
                    SLOT(saveAsTriggered()));
            menu->addAction(action);
            menu->exec(QCursor::pos());
        }
    } else {
        QMenu* menu = new QMenu(_q);

        QAction* action = new QAction(KIcon("edit-copy"),
                                            i18n("Copy"), menu);
        connect(action, SIGNAL(triggered(bool)),
                SLOT(copyTriggered()));
        menu->addAction(action);

        action = new QAction(KIcon("edit-find"),
                             i18n("Find Text..."), menu);
        connect(action, SIGNAL(triggered(bool)),
                SLOT(findTextTriggered()));
        menu->addAction(action);
        menu->addSeparator();

        action = new QAction(i18n("Channel Topic History..."),
                             menu);
        connect(action, SIGNAL(triggered(bool)),
                SLOT(topicHistoryTriggered()));
        menu->addAction(action);

        menu->exec(QCursor::pos());
    }
}

void
ChatViewPrivate::copyUrlTriggered()
{
    QClipboard* cb = KApplication::clipboard();
    cb->setText(_q->page()->selectedText(), QClipboard::Clipboard);
}

void
ChatViewPrivate::saveAsTriggered()
{
    KUser user(KUser::UseEffectiveUID);
    const QString fileName = KFileDialog::getSaveFileName(user.homeDir(), QString(), _q,
                                                          i18n("Save As..."),
                                                          KFileDialog::ConfirmOverwrite);
    KIO::file_copy(_q->page()->selectedText(), fileName);
}

void
ChatViewPrivate::findTextTriggered()
{
    emit _q->findTextTriggered();
}

void
ChatViewPrivate::topicHistoryTriggered()
{
    // TODO topichistory signal
}

QString
ChatViewPrivate::randomString() const
{
    static const char alphanum[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    QString str;
    for (int i = 0; i < 11; ++i) {
        str.append(QLatin1Char(alphanum[qrand() % (sizeof(alphanum) - 1)]));
    }

    return str;
}

QString
ChatViewPrivate::span(const QString& klass, const QString& text)
{
    QDomElement element = doc->createElement("span");
    element.setAttribute("class", klass);
    element.appendChild(doc->createTextNode(text));
    doc->appendChild(element);
    QString str = doc->toString(4);
    doc->clear();
    return str;
}

QDomElement
ChatViewPrivate::createEvent(const QString& name)
{
    QDomElement element = doc->createElement("event");
    element.setAttribute("id", randomString());
    element.setAttribute("name", name);
    element.setAttribute("occurred", messageTime());
    return element;
}

QDomElement
ChatViewPrivate::awayMessage(const QString& message)
{
    QDomText text = doc->createTextNode(message);
    QDomElement element = doc->createElement("away-message");
    element.appendChild(text);
    return element;
}

QDomElement
ChatViewPrivate::ban(const QString& mask)
{
    QDomText text = doc->createTextNode(mask);
    QDomElement element = doc->createElement("ban");
    element.appendChild(text);
    return element;
}

QDomElement
ChatViewPrivate::by(const Aki::Irc::NickInfo& sender, const QString& klass, bool self)
{
    QDomText text = doc->createTextNode(sender.nick());
    QDomElement element = doc->createElement("by");

    if (self) {
        element.setAttribute("self", "yes");
    }

    if (!klass.isEmpty() && !klass.isNull()) {
        element.setAttribute("class", klass);
    }

    element.setAttribute("hostmask", sender.hostmask());
    element.setAttribute("identifier", sender.user());
    element.appendChild(text);
    return element;
}

QDomElement
ChatViewPrivate::envelope(bool ignored)
{
    QDomElement element = doc->createElement("envelop");
    if (ignored) {
        element.setAttribute("ignored", "yes");
    }
    return element;
}

QDomElement
ChatViewPrivate::log(const QString& begin, const QString& source)
{
    QDomElement element = doc->createElement("log");
    element.setAttribute("begin", begin);
    element.setAttribute("source", source);
    return element;
}

QDomElement
ChatViewPrivate::message(const QString& msg, bool highlight, bool action, bool notice)
{
    QDomText text = doc->createTextNode(msg);
    QDomElement element = doc->createElement("message");

    if (highlight) {
        element.setAttribute("highlight", "yes");
    }
    if (action) {
        element.setAttribute("action", "yes");
    }
    if (notice) {
        element.setAttribute("type", "notice");
    }
    element.setAttribute("received", messageTime());
    element.setAttribute("id", randomString());
    element.appendChild(text);
    return element;
}

QDomElement
ChatViewPrivate::old(const QString& oldNick)
{
    QDomText text = doc->createTextNode(oldNick);
    QDomElement element = doc->createElement("old");
    element.appendChild(text);
    return element;
}

QDomElement
ChatViewPrivate::reason(const QString& reason)
{
    QDomText text = doc->createTextNode(reason);
    QDomElement element = doc->createElement("reason");
    element.appendChild(text);
    return element;
}

QDomElement
ChatViewPrivate::sender(const QString& nickname, const QString& username,
                        const QString& hostmask, const QString& klass, bool self,
                        const QString& buddy)
{
    QDomText text = doc->createTextNode(nickname);
    QDomElement element = doc->createElement("sender");
    element.appendChild(text);
    if (!buddy.isEmpty() && !buddy.isNull()) {
        element.setAttribute("buddy", buddy);
    }

    element.setAttribute("hostmask", hostmask);
    element.setAttribute("identifier", username);

    if (self) {
        element.setAttribute("self", "yes");
    }

    if (!klass.isEmpty() && !klass.isNull()) {
        element.setAttribute("class", klass);
    }

    return element;
}

QDomElement
ChatViewPrivate::topic(const QString& topic)
{
    QDomText text = doc->createTextNode(topic);
    QDomElement element = doc->createElement("topic");
    element.appendChild(text);
    return element;
}

QDomElement
ChatViewPrivate::who(const Aki::Irc::NickInfo& sender, bool self,
                     const QString& klass)
{
    QDomText text = doc->createTextNode(sender.nick());
    QDomElement element = doc->createElement("who");

    if (!klass.isEmpty() && !klass.isNull()) {
        element.setAttribute("class", klass);
    }

    if (self) {
        element.setAttribute("self", "yes");
    }

    element.setAttribute("hostmask", sender.hostmask());
    element.appendChild(text);
    return element;
}
