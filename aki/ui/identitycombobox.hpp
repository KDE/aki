#ifndef AKI_IDENTITYCOMBOBOX_HPP
#define AKI_IDENTITYCOMBOBOX_HPP

#include <KDE/KComboBox>

namespace Aki
{
class SqlIdentity;
/**
 * Creates a simple wrapper around SqlIdentity.
 */
class IdentityComboBox : public KComboBox
{
    Q_OBJECT
public:
    /**
     * Creates an IdentityComboBox.
     *
     * @param parent Parent of the object.
     */
    explicit IdentityComboBox(QWidget* parent = 0);
    /**
     * Destroys the object.
     */
    ~IdentityComboBox();
Q_SIGNALS:
    /**
     * This is emitted when the user changes the @p identity.
     * @note You are responsible of deleting the pointer.
     *
     * @param identity New current identity.
     */
    void identityActivated(Aki::SqlIdentity* identity);
private Q_SLOTS:
    /**
     * Catch when the new index is selected.
     */
    void slotActivated(const QString& identity);
}; // End of class IdentityComboBox.
} // End of namespace Aki.

#endif // AKI_IDENTITYCOMBOBOX_HPP
