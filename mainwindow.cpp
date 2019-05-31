#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QRegExp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setWindowTitle("PathPlaining");
    this->setWindowIcon(QIcon(QString(":/new/prefix1/image/robot _icon.png")));
    this->resize(800, 600);

    initWidget();
    connectSigal();
}

MainWindow::~MainWindow()
{

}

void MainWindow::initWidget()
{
    width_label_ = new QLabel("宽度");
    width_label_->setFixedWidth(50);

    height_label_ = new QLabel("高度");
    height_label_->setFixedWidth(50);

    width_edit_ = new QLineEdit("50");
    width_edit_->setFixedWidth(100);

    height_edit_ = new QLineEdit("50");
    height_edit_->setFixedWidth(100);

    generate_button_ = new QPushButton("生成地图");
    generate_button_->setFixedWidth(100);

    display_track_ = new QPushButton("绘制轨迹");
    display_track_->setFixedWidth(100);
    display_track_->setDisabled(true);

    start_button_ = new QPushButton();
    start_button_->setFixedSize(60, 60);
    start_button_->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/image/button3.png);}");

    control_group_ = new QGroupBox("控制区");
    map_group_ = new QGroupBox("地图");

    map_ = new GenerateMap();

    QHBoxLayout *hrizon_layout1 = new QHBoxLayout;
    QHBoxLayout *hrizon_layout2 = new QHBoxLayout;

    hrizon_layout1->addWidget(width_label_);
    hrizon_layout1->addWidget(width_edit_);

    hrizon_layout2->addWidget(height_label_);
    hrizon_layout2->addWidget(height_edit_);

    QVBoxLayout *vertical_layout1 = new QVBoxLayout;
    QVBoxLayout *vertical_layout2 = new QVBoxLayout;

    vertical_layout1->addLayout(hrizon_layout1);
    vertical_layout1->addLayout(hrizon_layout2);
    vertical_layout1->addSpacerItem(new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Fixed));
    vertical_layout1->addWidget(generate_button_, Qt::AlignRight);
    vertical_layout1->addWidget(display_track_, Qt::AlignRight);
    vertical_layout1->addWidget(start_button_, Qt::AlignRight);
    vertical_layout1->addSpacerItem(new QSpacerItem(10, 300, QSizePolicy::Fixed, QSizePolicy::Fixed));

    vertical_layout2->addWidget(map_);

    map_group_->setLayout(vertical_layout2);
    control_group_->setLayout(vertical_layout1);

    QHBoxLayout *main_layout = new QHBoxLayout;
    main_layout->addWidget(map_group_);
    main_layout->addWidget(control_group_);
    main_layout->setStretch(0, 4);
    main_layout->setStretch(1, 1);

    QWidget *central_widget = new QWidget;
    central_widget->setLayout(main_layout);

    this->setCentralWidget(central_widget);
}

void MainWindow::connectSigal()
{
    connect(width_edit_, SIGNAL(textChanged(QString)), this, SLOT(onEditChange()));
    connect(height_edit_, SIGNAL(textChanged(QString)), this, SLOT(onEditChange()));
    connect(generate_button_, SIGNAL(clicked(bool)), this, SLOT(onGenerateButton()));
    connect(display_track_, SIGNAL(clicked(bool)), this, SLOT(onDisplayButton()));
    connect(start_button_, SIGNAL(clicked(bool)), this, SLOT(onStartButton()));
}

void MainWindow::onGenerateButton()
{
    map_->creatMap(width_edit_->text().toInt(), height_edit_->text().toInt());
    update();
}

void MainWindow::onStartButton()
{
    if(is_start_)
    {
        start_button_->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/image/button3.png);}");
        is_start_ = false;
    }
    else
    {
        start_button_->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/image/button1.png);}");
        is_start_ = true;
    }
}

void MainWindow::onDisplayButton()
{

}

void MainWindow::onEditChange()
{
    QLineEdit *line_edit = (QLineEdit*)sender();

    QRegExp rx("[0-9]+");

    if(!rx.exactMatch(line_edit->text()))
    {
       line_edit->setStyleSheet("background-color:red");
    }
    else
    {
       line_edit->setStyleSheet(0);
    }
}
