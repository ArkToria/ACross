#include "app.h"
#include "buildinfo.h"

int main(int argc, char *argv[]) {
    auto fonts = across::FONT_LIST();
    QApplication::setApplicationName(APP_NAME);
    QApplication::setApplicationDisplayName("V2Ray Client");
    QApplication::setFont(QFont(fonts, 11));

    across::Application app(argc, argv);
    if (!app.initialize()) {
        return app.getExitReason();
    }

    return app.run();
}
