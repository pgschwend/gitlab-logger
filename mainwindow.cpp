#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


QPalette MainWindow::setTheme(Theme theme)
{
    QPalette palette;

    if(theme == Dark)
    {

        palette.setColor(QPalette::Window, QColor(45,45,45));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(30,30,30));
        palette.setColor(QPalette::AlternateBase, QColor(45,45,45));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(45,45,45));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);
    }
    else if(theme == Light)
    {
        palette.setColor(QPalette::Window, QColor(245, 245, 245));            // Hintergrundfenster
        palette.setColor(QPalette::WindowText, Qt::black);                    // Text auf hellen Flächen
        palette.setColor(QPalette::Base, QColor(255, 255, 255));              // Eingabefelder, Textboxen
        palette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));     // Alternierende Tabellenzeilen
        palette.setColor(QPalette::ToolTipBase, QColor(255, 255, 220));       // Tooltips
        palette.setColor(QPalette::ToolTipText, Qt::black);
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::Button, QColor(240, 240, 240));            // Buttons
        palette.setColor(QPalette::ButtonText, Qt::black);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, QColor(0, 120, 215));                // Blaue Links wie Windows
        palette.setColor(QPalette::Highlight, QColor(0, 120, 215));           // Auswahl (z. B. in ListViews)
        palette.setColor(QPalette::HighlightedText, Qt::white);

        palette.setColor(QPalette::Disabled, QPalette::Text, QColor(150, 150, 150));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(150, 150, 150));

    }
    else
    {
        palette.setColor(QPalette::Window, QColor(245, 245, 245));            // Hintergrundfenster
        palette.setColor(QPalette::WindowText, Qt::black);                    // Text auf hellen Flächen
        palette.setColor(QPalette::Base, QColor(255, 255, 255));              // Eingabefelder, Textboxen
        palette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));     // Alternierende Tabellenzeilen
        palette.setColor(QPalette::ToolTipBase, QColor(255, 255, 220));       // Tooltips
        palette.setColor(QPalette::ToolTipText, Qt::black);
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::Button, QColor(240, 240, 240));            // Buttons
        palette.setColor(QPalette::ButtonText, Qt::black);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, QColor(0, 120, 215));                // Blaue Links wie Windows
        palette.setColor(QPalette::Highlight, QColor(0, 120, 215));           // Auswahl (z. B. in ListViews)
        palette.setColor(QPalette::HighlightedText, Qt::white);

        palette.setColor(QPalette::Disabled, QPalette::Text, QColor(150, 150, 150));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(150, 150, 150));
    }

    return palette;

}
