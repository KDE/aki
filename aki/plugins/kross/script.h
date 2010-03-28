#ifndef SCRIPT_H
#define SCRIPT_H

#include <QObject>
#include <QScopedPointer>

class ScriptPrivate;
class Script : public QObject
{
    Q_OBJECT
public:
    class MetaData
    {
    public:
        QString name;
        QString website;
        QString email;
        QString comment;
        QString interpreter;
        QString author;
        QString icon;
    }; // End of class MetaData.

    explicit Script(QObject *parent = 0);
    ~Script();
    bool execute() const;
    void stop();
    Script::MetaData metaData() const;
    bool isRunning() const;
    bool isEnabled() const;
    void setEnabled(bool enable);
    bool loadDesktopFile(const QString &file) const;
    void setInterpreter(const QString &interpreter);
    void setCode(const QString &code);
private:
    friend class ScriptPrivate;
    QScopedPointer<ScriptPrivate> d;
}; // End of class Script.

#endif // SCRIPT_H
