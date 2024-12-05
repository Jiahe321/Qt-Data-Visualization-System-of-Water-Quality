#include <QApplication>
#include "Window.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QTranslator translator;
    if (translator.load(QLocale::system(), "myapp", "_", ":/i18n")) {
        app.installTranslator(&translator);
    }
    WaterSampleWindow window;
    window.show();

    return app.exec();
}
