#include "gamewindow.h"
#include "ui_gamewindow.h"
#include <QPixmap>
#include <QPainter>
#include <QFont>

GameWindow::GameWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GameWindow)
{
    initiate_window();
    initiate_game_title();
    initiate_offline_button();
    initiate_online_button();
    initiate_exit_button();

    ui->setupUi(this);
}

GameWindow::~GameWindow()
{
    delete ui;
}

void GameWindow::initiate_window() { setFixedSize(800, 600); }

void GameWindow::paintEvent(QPaintEvent* event) {
    static QPixmap backpic(":/Backgrounds/images/Homepage.jpg");
    QPainter painter(this);
    painter.drawPixmap(this->rect(),backpic);
}

void GameWindow::initiate_offline_button() {
    offline_button = new QPushButton(this);
    offline_button->setGeometry(75, 165, 200, 50);
    /*QFont font;
    font.setPointSize(15);
    font.setFamily("Arial Rounded MT Bold");
    font.setBold(true);
    offline_button->setFont(font);*/
    offline_button->setStyleSheet(
                                  "QPushButton{"
                                  "background-color:white;"//color
                                  "border-style:outset;"                  //inset/outset
                                  "border-width:4px;"
                                  "border-radius:10px;"
                                  "border-color:black;"    //border color
                                  "font:bold 25px;"                       //character and size
                                  "color:black;"                //character color
                                  "padding:6px;"                          //padding
                                  "}");
    offline_button->setCursor(Qt::PointingHandCursor);
    offline_button->setText("Single Player");
    offline_button->installEventFilter(this);
    offline_clickable = true;
}

void GameWindow::initiate_online_button() {
    online_button = new QPushButton(this);
    online_button->setGeometry(75, 240, 200, 50);
    QFont font;
    font.setPointSize(15);
    font.setFamily("Arial Rounded MT Bold");
    font.setBold(true);
    online_button->setFont(font);
    online_button->setStyleSheet("QPushButton{"
                                 "background-color:white;"//color
                                 "border-style:outset;"                  //inset/outset
                                 "border-width:4px;"
                                 "border-radius:10px;"
                                 "border-color:black;"    //border color
                                 "font:bold 25px;"                       //character and size
                                 "color:black;"                //character color
                                 "padding:6px;"                          //padding
                                 "}");
    online_button->setCursor(Qt::PointingHandCursor);
    online_button->setText("Multi-Players");
    online_button->installEventFilter(this);
    online_clickable = true;
}

void GameWindow::initiate_exit_button() {
    exit_button = new QPushButton(this);
    exit_button->setGeometry(75, 315, 200, 50);
    exit_button->setStyleSheet("QPushButton{"
                                 "background-color:white;"//color
                                 "border-style:outset;"                  //inset/outset
                                 "border-width:4px;"
                                 "border-radius:10px;"
                                 "border-color:black;"    //border color
                                 "font:bold 25px;"                       //character and size
                                 "color:black;"                //character color
                                 "padding:6px;"                          //padding
                                 "}");
    exit_button->setCursor(Qt::PointingHandCursor);
    exit_button->setText("Exit");
    exit_button->installEventFilter(this);
}

void GameWindow::initiate_game_title() {
    game_title = new QPushButton(this);
    game_title->setGeometry(50, 50, 300, 100);
    QFont font;
    font.setItalic(true);
    font.setStyle(QFont::StyleItalic);
    font.setPointSize(40);
    font.setFamily("Arial Rounded MT Bold");
    font.setBold(true);
    game_title->setFont(font);
    game_title->setStyleSheet("color:Black");
    game_title->setText("USTlord");
    game_title->setFlat(true);
}
