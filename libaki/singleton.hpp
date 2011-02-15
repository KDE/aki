#ifndef AKI_SINGLETON_HPP
#define AKI_SINGLETON_HPP

#include "aki.hpp"
#include <QtCore/QObject>

namespace Aki
{
/**
 * Singleton is a class that will create a one time instance and is available
 * in a global scope rather than a scope of a class. It is not copyable.
 *
 * To make a class a Singlton.
 *
 * foo.hpp
 * @code
 * class Foo : public Singleton<Foo>
 * {
 *     Q_OBJECT
 * public:
 *     ....
 * private:
 *     AKI_DECLARE_SINGLETON(Foo)
 * };
 * @endcode
 *
 * foo.cxx
 * @code
 * AKI_INIT_SINGLETON(Foo)
 *
 * ...
 * @endcode
 */
template <typename T>
class LIBAKI_EXPORT Singleton
    : public QObject
{
public:
    /**
     * Gets the instance of the Singleton.
     * This will create an instance of the class if it has not already
     * done so else it will return the instance.
     *
     * @return Instance of the class.
     */
    static T* self()
    {
        if (!_instance) {
            _instance = new T;
        }
        return _instance;
    }
protected:
    static T* _instance;
}; // End of class Singleton.
} // End of namespace Aki.

#define AKI_DECLARE_SINGLETON(klass) \
    friend class Aki::Singleton<klass>; \
    klass(); \
    ~klass(); \
    Q_DISABLE_COPY(klass);

#define AKI_INIT_SINGLETON(klass) \
    template<> klass* Aki::Singleton<klass>::_instance = 0;

#endif // AKI_SINGLETON_HPP
