#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QWidget *widget;
    QLabel *data_source_label;
    QLabel *video_path_label;
    QLineEdit *video_path;
    QPushButton *select_video_path;
    QPushButton *import_video_btn;
    QString video_import_path;
    QString video_dir;
    QString audio_dir;
    QLabel *audio_path_label;
    QLineEdit *audio_path;
    QPushButton *select_audio_path;
    QString audio_import_path;
    void setImportDialogLayout();
    void readPcap(QString fileName);

private slots:
    void SelectVideoPath();
    void SelectAudioPath();
    void readPcapList();

};

#endif // MAINWINDOW_H
