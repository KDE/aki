#ifndef AKI_SINGLETON_HPP
#define AKI_SINGLETON_HPP

#include <QtCore/QObject>

namespace Aki
{
template <typename T>
class Singleton : public QObject
{
public:
    static T* self()
    {
        if (!_instance) {
            _instance = new T;
        }
        return _instance;
    }
protected:
    static T* _instance;
};
} // End of namespace Aki.

#endif // AKI_SINGLETON_HPP
