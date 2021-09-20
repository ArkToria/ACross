#include "app.h"

int
main(int argc, char* argv[])
{
  QApplication::setOrganizationName("arktoria");
  QApplication::setApplicationName(APP_NAME);
  QApplication::setOrganizationDomain("arktoria.org");
  QApplication::setApplicationDisplayName("V2Ray Client");
  QApplication::setWindowIcon(QIcon::fromTheme("org.arktoria.across"));
  QApplication::setFont(QFont("Noto Sans", 10));

  across::Application app(argc, argv);

  return app.run();
}
