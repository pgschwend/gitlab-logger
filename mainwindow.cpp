#include "mainwindow.h"

#include <QFileDialog>
#include <QSettings>
#include <QCloseEvent>
#include "./ui_mainwindow.h"
#include "logger.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QSettings settings("Logger", "Gitlab");

    // Load values from settings
    ui->txtUrl->setText(settings.value("url", "https://gitlab.example.com/project.git").toString());
    ui->txtToken->setText(settings.value("token", "").toString());
    ui->txtDestination->setText(settings.value("destination", "C:/").toString());

    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());

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
    // Take existing path from textfield
    QString currentPath = ui->txtDestination->text().trimmed();

    // If textfield is empty, take a default starting path
    if (currentPath.isEmpty()) {
        currentPath = QDir::homePath();
    }

    // Open dialog
    QString selectedFile = QFileDialog::getSaveFileName(
        this,
        "Choose destination",
        currentPath,  // <-- Choose existing path from texfield!
        "CSV Files (*.csv);;JSON Files (*.json);;All Files (*)"
    );

    // If user has not canceled, set path
    if (!selectedFile.isEmpty()) {
        ui->txtDestination->setText(selectedFile);
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


void MainWindow::closeEvent(QCloseEvent *event) {
    saveSettings();
    event->accept();
}

void MainWindow::saveSettings() {
    QSettings settings("Logger", "Gitlab");

    settings.setValue("url", ui->txtUrl->text());
    settings.setValue("token", ui->txtToken->text());
    settings.setValue("destination", ui->txtDestination->text());

    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());

    qDebug() << "Settings saved!";
}