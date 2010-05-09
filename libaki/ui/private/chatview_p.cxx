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
    doc.reset(new QDomDocument(QLatin1String("html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\""
        "    \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\"")));
    createShell();
}

QString
ChatViewPrivate::createShell()
{
    QDomElement html = doc->createElement(QLatin1String("html"));
    html.setAttribute(QLatin1String("xmlns"), QLatin1String("http://www.w3.org/1999/xhtml"));
    html.setAttribute(QLatin1String("lang"), QLatin1String("en"));
    html.setAttribute(QLatin1String("xml:lang"), QLatin1String("en"));
    QDomElement head = doc->createElement(QLatin1String("head"));
    QDomElement title = doc->createElement(QLatin1String("title"));
    title.appendChild(doc->createTextNode(QLatin1String("Aki IRC Client")));
    QDomElement metaCT = doc->createElement(QLatin1String("meta"));
    metaCT.setAttribute(QLatin1String("http-equiv"), QLatin1String("Content-Type"));
    metaCT.setAttribute(QLatin1String("content"), QLatin1String("application/xhtml+xml; charset=UTF-8"));
    QDomElement metaAC = doc->createElement(QLatin1String("meta"));
    metaAC.setAttribute(QLatin1String("http-equiv"), QLatin1String("Accept-Content"));
    metaAC.setAttribute(QLatin1String("content"), QLatin1String("utf-8"));
    head.appendChild(title);
    head.appendChild(metaCT);
    head.appendChild(metaAC);
    QDomElement body = doc->createElement(QLatin1String("body"));
    body.appendChild(log(messageTime(), QString()));
    html.appendChild(head);
    html.appendChild(body);

    doc->appendChild(html);
    _q->setContent(doc->toByteArray(4), QLatin1String("application/xhtml+xml"));
    doc->clear();
    return QString();
}

QString
ChatViewPrivate::unescape(const QString& str)
{
    QString tmp = str;
    tmp.replace(QLatin1String("&gt;"), QLatin1String(">"));
    tmp.replace(QLatin1String("&lt;"), QLatin1String("<"));
    tmp.replace(QLatin1String("&amp;"), QLatin1String("&"));
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

    QWebElement log = mainFrame()->findFirstElement(QLatin1String("body log"));
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
    msg.replace(QRegExp(QLatin1String("<[^>]*>")), QLatin1String(""));
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

    result.replace(QLatin1String("src=\""), QLatin1String("src\"file://"));
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

    if (hit.linkElement().tagName() == QLatin1String("A") &&
        hit.linkElement().hasAttribute(QLatin1String("href"))) {
        KUrl url(hit.linkUrl());
        QString href = url.url().remove(QLatin1String("about:blank"));

        if (href[0] == QLatin1Char('#')) {
            emit _q->userUrlClicked(href.remove(QLatin1Char('#')));
        } else {
            QMenu* menu = new QMenu(_q);
            menu->setTitle(i18n("Normal"));

            QAction* action = new QAction(KIcon(QLatin1String("edit-copy")),
                                          i18n("Copy Link Address"), menu);
            connect(action, SIGNAL(triggered(bool)),
                    SLOT(copyUrlTriggered()));
            menu->addAction(action);

            action = new QAction(KIcon(QLatin1String("document-save-as")),
                                 i18n("Save Link As..."), menu);
            connect(action, SIGNAL(triggered(bool)),
                    SLOT(saveAsTriggered()));
            menu->addAction(action);
            menu->exec(QCursor::pos());
        }
    } else {
        QMenu* menu = new QMenu(_q);

        QAction* action = new QAction(KIcon(QLatin1String("edit-copy")),
                                            i18n("Copy"), menu);
        connect(action, SIGNAL(triggered(bool)),
                SLOT(copyTriggered()));
        menu->addAction(action);

        action = new QAction(KIcon(QLatin1String("edit-find")),
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
    QDomElement element = doc->createElement(QLatin1String("span"));
    element.setAttribute(QLatin1String("class"), klass);
    element.appendChild(doc->createTextNode(text));
    doc->appendChild(element);
    QString str = doc->toString(4);
    doc->clear();
    return str;
}

QDomElement
ChatViewPrivate::createEvent(const QString& name)
{
    QDomElement element = doc->createElement(QLatin1String("event"));
    element.setAttribute(QLatin1String("id"), randomString());
    element.setAttribute(QLatin1String("name"), name);
    element.setAttribute(QLatin1String("occurred"), messageTime());
    return element;
}

QDomElement
ChatViewPrivate::awayMessage(const QString& message)
{
    QDomText text = doc->createTextNode(message);
    QDomElement element = doc->createElement(QLatin1String("away-message"));
    element.appendChild(text);
    return element;
}

QDomElement
ChatViewPrivate::ban(const QString& mask)
{
    QDomText text = doc->createTextNode(mask);
    QDomElement element = doc->createElement(QLatin1String("ban"));
    element.appendChild(text);
    return element;
}

QDomElement
ChatViewPrivate::by(const Aki::Irc::NickInfo& sender, const QString& klass, bool self)
{
    QDomText text = doc->createTextNode(sender.nick());
    QDomElement element = doc->createElement(QLatin1String("by"));

    if (self) {
        element.setAttribute(QLatin1String("self"), QLatin1String("yes"));
    }

    if (!klass.isEmpty() && !klass.isNull()) {
        element.setAttribute(QLatin1String("class"), klass);
    }

    element.setAttribute(QLatin1String("hostmask"), sender.hostmask());
    element.setAttribute(QLatin1String("identifier"), sender.user());
    element.appendChild(text);
    return element;
}

QDomElement
ChatViewPrivate::envelope(bool ignored)
{
    QDomElement element = doc->createElement(QLatin1String("envelop"));
    if (ignored) {
        element.setAttribute(QLatin1String("ignored"), QLatin1String("yes"));
    }
    return element;
}

QDomElement
ChatViewPrivate::log(const QString& begin, const QString& source)
{
    QDomElement element = doc->createElement(QLatin1String("log"));
    element.setAttribute(QLatin1String("begin"), begin);
    element.setAttribute(QLatin1String("source"), source);
    return element;
}

QDomElement
ChatViewPrivate::message(const QString& msg, bool highlight, bool action, bool notice)
{
    QDomText text = doc->createTextNode(msg);
    QDomElement element = doc->createElement(QLatin1String("message"));

    if (highlight) {
        element.setAttribute(QLatin1String("highlight"), QLatin1String("yes"));
    }
    if (action) {
        element.setAttribute(QLatin1String("action"), QLatin1String("yes"));
    }
    if (notice) {
        element.setAttribute(QLatin1String("type"), QLatin1String("notice"));
    }
    element.setAttribute(QLatin1String("received"), messageTime());
    element.setAttribute(QLatin1String("id"), randomString());
    element.appendChild(text);
    return element;
}

QDomElement
ChatViewPrivate::old(const QString& oldNick)
{
    QDomText text = doc->createTextNode(oldNick);
    QDomElement element = doc->createElement(QLatin1String("old"));
    element.appendChild(text);
    return element;
}

QDomElement
ChatViewPrivate::reason(const QString& reason)
{
    QDomText text = doc->createTextNode(reason);
    QDomElement element = doc->createElement(QLatin1String("reason"));
    element.appendChild(text);
    return element;
}

QDomElement
ChatViewPrivate::sender(const QString& nickname, const QString& username,
                        const QString& hostmask, const QString& klass, bool self,
                        const QString& buddy)
{
    QDomText text = doc->createTextNode(nickname);
    QDomElement element = doc->createElement(QLatin1String("sender"));
    element.appendChild(text);
    if (!buddy.isEmpty() && !buddy.isNull()) {
        element.setAttribute(QLatin1String("buddy"), buddy);
    }

    element.setAttribute(QLatin1String("hostmask"), hostmask);
    element.setAttribute(QLatin1String("identifier"), username);

    if (self) {
        element.setAttribute(QLatin1String("self"), QLatin1String("yes"));
    }

    if (!klass.isEmpty() && !klass.isNull()) {
        element.setAttribute(QLatin1String("class"), klass);
    }

    return element;
}

QDomElement
ChatViewPrivate::topic(const QString& topic)
{
    QDomText text = doc->createTextNode(topic);
    QDomElement element = doc->createElement(QLatin1String("topic"));
    element.appendChild(text);
    return element;
}

QDomElement
ChatViewPrivate::who(const Aki::Irc::NickInfo& sender, bool self,
                     const QString& klass)
{
    QDomText text = doc->createTextNode(sender.nick());
    QDomElement element = doc->createElement(QLatin1String("who"));

    if (!klass.isEmpty() && !klass.isNull()) {
        element.setAttribute(QLatin1String("class"), klass);
    }

    if (self) {
        element.setAttribute(QLatin1String("self"), QLatin1String("yes"));
    }

    element.setAttribute(QLatin1String("hostmask"), sender.hostmask());
    element.appendChild(text);
    return element;
}
