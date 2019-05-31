#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include "generatemap.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:

    QLabel *width_label_;
    QLabel *height_label_;
    QLineEdit *width_edit_;
    QLineEdit *height_edit_;

    QPushButton *generate_button_;
    QPushButton *start_button_;
    QPushButton *display_track_;
    GenerateMap *map_;

    QGroupBox *control_group_;
    QGroupBox *map_group_;


    bool is_start_=0;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initWidget();
    void connectSigal();

public slots:
    void onGenerateButton();
    void onStartButton();
    void onDisplayButton();
    void onEditChange();
};

#endif // MAINWINDOW_H
