#ifndef AKI_SCRIPT_HPP
#define AKI_SCRIPT_HPP

#include <QtCore/QObject>

namespace Kross
{
class Action;
} // End of namespace Kross.

namespace Aki
{
class Script : public QObject
{
    Q_OBJECT
public:
    class MetaData {
    public:
        QString author;
        QString comment;
        QString email;
        QString icon;
        QString interpreter;
        QString name;
        QString website;
    }; // End of class MetaData.

    explicit Script(QObject* parent = 0);
    ~Script();
    bool execute();
    bool isEnabled() const;
    bool isRunning() const;
    bool loadDesktopFile(const QString& file);
    Aki::Script::MetaData metaData() const;
    void setCode(const QString& code);
    void setEnabled(bool enable);
    void setInterpreter(const QString& interpreter);
    void stop();
private:
    Kross::Action* _action;
    Script::MetaData _meta;
    QString _file;
    QString _actionName;
    bool _isRunning;
    bool _isEnabled;
}; // End of class Script.
} // End of namespace Aki.

#endif // AKI_SCRIPT_HPP
