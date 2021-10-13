#include "app.h"
#include "buildinfo.h"

int
main(int argc, char* argv[])
{
  auto fonts = across::getFonts();
  QApplication::setOrganizationName("arktoria");
  QApplication::setApplicationName(APP_NAME);
  QApplication::setOrganizationDomain("arktoria.org");
  QApplication::setApplicationDisplayName("V2Ray Client");
  QApplication::setFont(QFont(fonts, 10));

  across::Application app(argc, argv);

  return app.run();
}
