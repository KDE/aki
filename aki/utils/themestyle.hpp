#ifndef AKI_THEMESTYLE_HPP
#define AKI_THEMESTYLE_HPP

#include <QtCore/QHash>
#include <QtCore/QObject>

namespace Aki
{
class ThemeStyle : public QObject
{
    Q_OBJECT
public:
    typedef QHash<QString,QString> VariantStyles;
    explicit ThemeStyle(const QString& name, QObject* parent = 0);
    ~ThemeStyle();
    QString baseHref() const;
    bool isValid() const;
    QString footerHtml() const;
    QString headerHtml() const;
    QString incomingContentHtml() const;
    QString incomingNextContentHtml() const;
    QString outgoingContentHtml() const;
    QString outgoingNextContentHtml() const;
    void reloadTheme();
    QString statusHtml() const;
    QString styleName() const;
    Aki::ThemeStyle::VariantStyles variants() const;
private:
    enum DirectionMethod
    {
        Incoming,
        Outgoing,
        None
    }; // End of enum DirectionMethod.

    QString directory(const QString& htmlFile, DirectionMethod direction = Aki::ThemeStyle::None) const;
    QString readHtmlFile(const QString& htmlFile);
    void loadStyle(const QString& name);
    void readStyles();
    void readVariants();
private:
    Aki::ThemeStyle::VariantStyles _variants;
    QString _incomingContentHtml;
    QString _incomingNextContentHtml;
    QString _outgoingContentHtml;
    QString _outgoingNextContentHtml;
    QString _footerHtml;
    QString _headerHtml;
    QString _statusHtml;
    QString _styleName;
    QString _baseHref;
}; // End of class ThemeStyle.
} // End of namespace Aki.

#endif // AKI_THEMESTYLE_HPP
