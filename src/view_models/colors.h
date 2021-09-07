#ifndef COLORS_H
#define COLORS_H

#include <QObject>
#include <QQmlPropertyMap>

namespace across {
namespace setting {
class Colors : public QQmlPropertyMap
{
  Q_OBJECT
public:
  explicit Colors(QQmlPropertyMap* parent = nullptr);
};
}
}

#endif // COLORS_H
