#include "app.h"

int
main(int argc, char* argv[])
{
  QApplication::setOrganizationName("arktoria");
  QApplication::setApplicationName(APP_NAME);
  QApplication::setOrganizationDomain("arktoria.org");
  QApplication::setApplicationDisplayName("V2Ray Client");
  QApplication::setFont(QFont({ "Noto Sans",
                                "Segoe UI Variable",
                                "Segoe UI",
                                "Noto Sans CJK",
                                "PingFang SC",
                                "Microsoft YaHei UI" },
                              10));

  across::Application app(argc, argv);

  return app.run();
}
