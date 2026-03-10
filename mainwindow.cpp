#include "mainwindow.h"

#include <QFileDialog>

#include "./ui_mainwindow.h"
#include "logger.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    logger = new Logger(this);

    connect(ui->btnGenerate, &QPushButton::clicked, this, &MainWindow::onBtnGenerateClicked);
    connect(ui->btnBrowse, &QPushButton::clicked, this, &MainWindow::onBtnBrowseClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onBtnBrowseClicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "Choose path");

    if (!path.isEmpty()) {
        ui->txtDestination->setText(path);
    }
}

void MainWindow::onBtnGenerateClicked()
{
    logger->generateLog(ui->txtUrl->text(), ui->txtToken->text(), ui->txtDestination->text(), 0);
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
        palette.setColor(QPalette::Window, QColor(245, 245, 245));
        palette.setColor(QPalette::WindowText, Qt::black);
        palette.setColor(QPalette::Base, QColor(255, 255, 255));
        palette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));
        palette.setColor(QPalette::ToolTipBase, QColor(255, 255, 220));
        palette.setColor(QPalette::ToolTipText, Qt::black);
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::Button, QColor(240, 240, 240));
        palette.setColor(QPalette::ButtonText, Qt::black);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, QColor(0, 120, 215));
        palette.setColor(QPalette::Highlight, QColor(0, 120, 215));
        palette.setColor(QPalette::HighlightedText, Qt::white);

        palette.setColor(QPalette::Disabled, QPalette::Text, QColor(150, 150, 150));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(150, 150, 150));

    }
    else
    {
        palette.setColor(QPalette::Window, QColor(245, 245, 245));
        palette.setColor(QPalette::WindowText, Qt::black);
        palette.setColor(QPalette::Base, QColor(255, 255, 255));
        palette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));
        palette.setColor(QPalette::ToolTipBase, QColor(255, 255, 220));
        palette.setColor(QPalette::ToolTipText, Qt::black);
        palette.setColor(QPalette::Text, Qt::black);
        palette.setColor(QPalette::Button, QColor(240, 240, 240));
        palette.setColor(QPalette::ButtonText, Qt::black);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, QColor(0, 120, 215));
        palette.setColor(QPalette::Highlight, QColor(0, 120, 215));
        palette.setColor(QPalette::HighlightedText, Qt::white);

        palette.setColor(QPalette::Disabled, QPalette::Text, QColor(150, 150, 150));
        palette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(150, 150, 150));
    }

    return palette;

}
