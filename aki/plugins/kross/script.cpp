#include "script.h"
#include "akiversion.h"
#include <KConfigGroup>
#include <KDebug>
#include <KDesktopFile>
#include <KLocale>
#include <KMessageBox>
#include <KMimeType>
#include <Kross/Action>
#include <Kross/ActionCollection>
#include <Kross/Manager>

class ScriptPrivate
{
public:
    ScriptPrivate(Script *qq)
        : q(qq),
        action(0),
        file(QString()),
        actionName(QString()),
        isRunning(false),
        isEnabled(false)
    {
    }

    bool loadDesktopFile(const QString &desktopFile)
    {
        // Load the desktop file for this script.
        KDesktopFile df(desktopFile);
        // Get the base url of the file.
        KUrl url(desktopFile);

        // Check and make sure this script is for our application.
        if (df.readType().trimmed() != "AkiScript") {
            KMessageBox::error(0, i18n("Unknown Script Type"));
            return false;
        }

        // Read the comment of the script.
        meta.comment = df.readComment();
        // Read the name of the script.
        meta.name = df.readName();
        // Read the name of the icon.
        meta.icon = df.readIcon();

        // Access the custom keys in the desktop file.
        const KConfigGroup group = df.group("Desktop Entry");

        // Get the interpreter of this file.
        meta.interpreter = group.readEntry("X-AkiScript-Interpreter");
        // Get the email of the author.
        meta.email = group.readEntry("X-AkiScript-Email");
        // Get the website that owns the script.
        meta.website = group.readEntry("X-AkiScript-Website");
        // Get the author of the script.
        meta.author = group.readEntry("X-AkiScript-Author");
        // Get the main file of the script.
        file = group.readEntry("X-AkiScript-MainScript");
        url.setFileName(file);

        // If the icon entry is emmpty or missing we return the mime icon of the script.
        if (meta.icon.isNull() || meta.icon.isEmpty()) {
            if (meta.interpreter == "Python") {
                meta.icon = "text-x-python";
            } else if (meta.interpreter == "Ruby") {
                meta.icon = "application-x-ruby";
            } else if (meta.interpreter == "Javascript" ||
                       meta.interpreter == "QtScript") {
                meta.icon = "application-x-javascript";
            }
        }

        // Check that the file is not empty or null and that it does exist.
        if (file.isNull() || file.isEmpty() || !QFileInfo(url.path()).exists()) {
            KMessageBox::error(0, i18n("Script does not contain the main script or main script is empty"));
            return false;
        }

        // Reappend the url path plus the file with the extension.
        file = url.path();
        return true;
    }

    bool createAction(const QString &name, const QString &icon)
    {
        /* Check to see if this file exists. If the file does not exist or
        the script has already a valid action exit*/
        if (!QFileInfo(file).exists() || action) {
            kDebug() << "Script doesn't exist or action already exists";
            return false;
        }

        // Set the action name.
        actionName = name;

        // Create the new action with the action name.
        action = new Kross::Action(q, actionName);
        // Set the location of the main script.
        action->setFile(file);
        // Set the mime of the script.
        action->setIconName(icon);

        // Try and get the interpreter of this file.
        const QString interpreter = Kross::Manager::self().interpreternameForFile(file);
        /* Check to see if the interpret was not valid for this script or
        if the interpreter is not available for the script.*/

        if (interpreter.isNull() || !Kross::Manager::self().interpreters().contains(interpreter)) {
            /* Since the interpreter was invalid or the interpreter is not available for the script
            delete the created action*/
            kDebug() << "Interpreter is null or not available";
            delete action;
            return false;
        }

        // Set the interpreter for this script.
        action->setInterpreter(interpreter);

        // Add the action to the action collection.
        Kross::Manager::self().actionCollection()->addAction(actionName, action);
        return true;
    }

    void removeAction()
    {
        // Access the action collection.
        Kross::ActionCollection *ac = Kross::Manager::self().actionCollection();
        // Remove the action.
        ac->removeAction(action);
        // Delete the action when it returns control back to the application.
        delete action;
        action = 0;
    }

    Script *q;
    Script::MetaData meta;
    Kross::Action *action;
    QString file;
    QString actionName;
    bool isRunning;
    bool isEnabled;
};

Script::Script(QObject *parent)
    : QObject(parent)
{
    d.reset(new ScriptPrivate(this));
}

Script::~Script()
{
    stop();
}

bool
Script::execute() const
{
    // Get the name of the file without the extension.
    const QString name = QFileInfo(d->file).fileName();

    // Get a pointer to the mime of the file.
    KMimeType::Ptr mime = KMimeType::findByPath(d->file);
    if (mime.isNull()) {
        return false;
    }

    /* Attempt creating the Kross action for the script
    and pass the mime's icon to it and the name of the script.*/
    if (!d->createAction(name, mime->iconName())) {
        return false;
    }

    // Execute the script.
    d->action->trigger();

    // Call the global function pluginLoad which is first called the initialise everything.
    if (d->action->functionNames().contains("pluginLoad")) {
        d->action->callFunction("pluginLoad");
    }

    // Check that version is correct with the version of the application.
    if (!checkVersion()) {
        // Remove the action from the collection if it is not.
        d->removeAction();
        return false;
    }

    // The script is running now.
    d->isRunning = true;
    return true;
}

void
Script::stop()
{
    // Check to see if the script is already running.
    if (!d->isRunning) {
        return;
    }

    /* Call the function pluginUnload to let the script know.
    That it is stopping. */
    if (d->action->functionNames().contains("pluginUnload")) {
        d->action->callFunction("pluginUnload");
    }

    // Remove the action from the actionCollection
    d->removeAction();

    // The script is not running now.
    d->isRunning = false;
}

bool
Script::checkVersion() const
{
    // Check that we have a valid script action.
    if (!d->action) {
        return false;
    }

    /* Check to see if script has the function pluginVersion and if it
    matches the version of this application. If the function is not found
    the script will not be loaded and deleted.*/
    if (d->action->functionNames().contains("pluginVersion")) {
        // Pass the version of the application.
        QVariantList args;
        args << AKI_VERSION_STR;
        // Get the state of if this script is compatible with this application.
        return d->action->callFunction("pluginVersion", args).toBool();
    }

    return false;
}

Script::MetaData
Script::metaData() const
{
    return d->meta;
}

bool
Script::isRunning() const
{
    return d->isRunning;
}

bool
Script::isEnabled() const
{
    return d->isEnabled;
}

void
Script::setEnabled(bool enable)
{
    d->isEnabled = enable;
}

bool
Script::loadDesktopFile(const QString &desktopFile) const
{
    return d->loadDesktopFile(desktopFile);
}
