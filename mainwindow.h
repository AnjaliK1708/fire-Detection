#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_monitor_clicked();

    void on_SelectImage_clicked();

    void on_process_clicked();
    void on_video_clicked();

private:
    Ui::MainWindow *ui;
    QString mFilename,mFilename2;

};

#endif // MAINWINDOW_H
