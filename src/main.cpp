#include "app.h"

int
main(int argc, char* argv[])
{
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  QApplication::setOrganizationName("arktoria");
  QApplication::setApplicationName(APP_NAME);
  QApplication::setOrganizationDomain("arktoria.org");
  QApplication::setApplicationDisplayName("V2ray Client");
  QApplication::setWindowIcon(QIcon::fromTheme("org.arktoria.across"));
  QApplication::setFont(QFont("Noto Sans", 10));

  across::Application app(argc, argv);

  return app.run();
}
