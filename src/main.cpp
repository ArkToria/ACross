#include "app.h"

int
main(int argc, char* argv[])
{
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  QGuiApplication::setOrganizationName("arktoria");
  QGuiApplication::setApplicationName(APP_NAME);
  QGuiApplication::setOrganizationDomain("arktoria.org");
  QGuiApplication::setApplicationDisplayName("ACross - V2ray Client");
  QGuiApplication::setWindowIcon(QIcon::fromTheme("org.arktoria.across"));
  QGuiApplication::setFont(QFont("Noto Sans", 10));

  across::Application app(argc, argv);

  return app.run();
}
