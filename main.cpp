#include <QApplication>
#include "Window.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    WaterSampleWindow window;
    window.show();

    return app.exec();
}
