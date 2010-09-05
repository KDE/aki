#include "script.hpp"
#include <aki.hpp>
#include <KDE/KConfigGroup>
#include <KDE/KDesktopFile>
#include <KDE/KMessageBox>
#include <KDE/KMimeType>
#include <KDE/Kross/Action>
#include <KDE/KUrl>
#include <QtCore/QFileInfo>
using namespace Aki;

Script::Script(QObject* parent)
    : QObject(parent)
{
}

Script::~Script()
{
}

bool
Script::execute()
{
    // Get the name of the file without the extension.
    const QString name = QFileInfo(_file).fileName();

    // Get a pointer to the mime of the file.
    KMimeType::Ptr mime = KMimeType::findByPath(_file);

    if (mime.isNull()) {
        return false;
    }

    /* Attempt creating the Kross action for the script
     * and pass the mime's icon to it and the name of the script */
    //if (!createAction(name, mime->iconName())) {
    //    return false;
    //}

    // Execute the script.
    _action->trigger();

    // Call the global function pluginLoad which is first called the initialise everything.
    if (_action->functionNames().contains("pluginLoad")) {
        _action->callFunction("pluginLoad");
    }

    // The script is running now.
    _isRunning = true;
    return true;
}

bool
Script::isEnabled() const
{
    return _isEnabled;
}

bool
Script::isRunning() const
{
    return _isRunning;
}

bool
Script::loadDesktopFile(const QString& file)
{
    // Load the desktop file for this script.
    KDesktopFile df(file);
    // Get the base url of the file.
    KUrl url(file);

    // Check and make sure this script is for our application.
    if (df.readType().trimmed() != "AkiScript") {
        KMessageBox::error(0, i18n("Unknown Script Type"));
    }

    // Read the comment of the script.
    _meta.comment = df.readComment();
    // Read the name of the script.
    _meta.name = df.readName();
    // Read the name of the icon.
    _meta.icon = df.readIcon();

    // Access the custom keys in the desktop file.
    const KConfigGroup group = df.group("Desktop Entry");

    // Get the interpret of this file.
    _meta.interpreter = group.readEntry("X-AkiScript-Interpreter");
    // Get the email of the author.
    _meta.email = group.readEntry("X-AkiScript-Author");
    // Get the website that owns the script.
    _meta.website = group.readEntry("X-AkiScript-Website");
    // Get the author of the script.
    _meta.author = group.readEntry("X-AkiScript-Author");
    // Get the mail file of the script.
    _file = group.readEntry("X-AkiScript-MainScript");
    url.setFileName(file);

    // If the icon entry is empty or missing we return the mime icon of the script.
    if (_meta.icon.isNull() || _meta.icon.isEmpty()) {
        if (_meta.interpreter == "Python") {
            _meta.icon = "text-x-python";
        } else if (_meta.interpreter == "Ruby") {
            _meta.icon = "application-x-ruby";
        } else if (_meta.interpreter == "Javascript" ||
                   _meta.interpreter == "QtScript") {
            _meta.icon = "application-x-javascript";
        }
    }

    // Check that the file is not empty of null and that it does exist.
    if (_file.isNull() || _file.isEmpty() || !QFileInfo(url.path()).exists()) {
        KMessageBox::error(0, i18n("Script does not contain the mail script or main script is empty."));
        return false;
    }

    // Reappend the url path plus the file with the extension.
    _file = url.path();
    return true;
}

Aki::Script::MetaData
Script::metaData() const
{
    return _meta;
}

void
Script::setCode(const QString& code)
{
    Q_UNUSED(code);
}

void
Script::setEnabled(bool enable)
{
    _isEnabled = enable;
}

void
Script::setInterpreter(const QString& interpreter)
{
    Q_UNUSED(interpreter);
}

void
Script::stop()
{
    // Check to see if the script is already running.
    if (!isRunning()) {
        return;
    }

    /* Call the function pluginUnload to let the script know
     * That it is stopping. */
    if (_action->functionNames().contains("pluginUnload")) {
        _action->callFunction("pluginUnload");
    }

    // Remove the action from the action collection.
    //removeAction();

    // The script is not running now.
    _isRunning = false;
}
