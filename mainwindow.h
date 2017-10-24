#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMessageBox>
#include <QDebug>
#include <QClipboard>
#include <QFileDialog>
#include <QtGui>
#include <QFont>
#include <QFontDialog>
#include <QSettings>
#include<QUndoCommand>
#include<QUndoStack>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    MainWindow *next=NULL;
    MainWindow *prev=NULL;

    QFont font;
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    Ui::MainWindow *ui;
    QUndoStack *undoStack = new QUndoStack(this);
    bool isChanged=false;
    bool isSaved=true;
    QString m_name="New File";
    QString m_path;
    int doCount=0;
    QSettings *settings = new QSettings("font.conf",QSettings::NativeFormat);

private slots:
    void on_pushButton_Create_clicked();
    void on_pushButton_Copy_clicked();
    void on_pushButton_Paste_clicked();
    void on_pushButton_Undo_clicked();
    void on_pushButton_Redo_clicked();
    void on_pushButton_Cut_clicked();
    void on_pushButton_ChFont_clicked();
    void on_pushButton_Exit_clicked();
    void on_pushButton_Close_clicked();

    void on_textEdit_textChanged();
    void on_pushButton_Save_clicked();
    void on_action_Save_file_as_triggered();
    void on_action_About_Qt_triggered();
    void on_pushButton_Open_clicked();
    void on_action_About_programm_triggered();
    void on_textEdit_copyAvailable(bool b);
};

#endif // MAINWINDOW_H
