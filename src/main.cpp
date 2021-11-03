#include "app.h"

int
run_app(int argc, char* argv[])
{
  auto fonts = across::getFonts();
  QApplication::setApplicationName(APP_NAME);
  QApplication::setApplicationDisplayName("V2Ray Client");
  QApplication::setFont(QFont(fonts, 11));

  across::Application app(argc, argv);
  if (!app.initialize()) {
    return app.getExitReason();
  }

  return app.run();
}

int
main(int argc, char* argv[])
{
  int exit_code = run_app(argc, argv);

#ifdef Q_CC_MINGW
  std::exit(exit_code);
#endif
  return exit_code;
}
