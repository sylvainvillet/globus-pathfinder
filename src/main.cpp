#include "logic/gamestate.h"
#include "ui/mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    GameState state(&app);
    MainWindow w(state);
    w.show();

    return app.exec();
}
