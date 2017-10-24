#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    {//icons for buttons

    QPixmap pixmap(":/icons/undo.png");
    QIcon ButtonIcon(pixmap);
    ui->pushButton_Undo->setIcon(ButtonIcon);
    pixmap=QPixmap(":/icons/redo.png");
    ButtonIcon=QIcon(pixmap);
    ui->pushButton_Redo->setIcon(ButtonIcon);
    pixmap=QPixmap(":/icons/text.png");
    ButtonIcon=QIcon(pixmap);
    ui->pushButton_ChFont->setIcon(ButtonIcon);
    pixmap=QPixmap(":/icons/create.png");
    ButtonIcon=QIcon(pixmap);
    ui->pushButton_Create->setIcon(ButtonIcon);
    pixmap=QPixmap(":/icons/save.png");
    ButtonIcon=QIcon(pixmap);
    ui->pushButton_Save->setIcon(ButtonIcon);
    pixmap=QPixmap(":/icons/open.png");
    ButtonIcon=QIcon(pixmap);
    ui->pushButton_Open->setIcon(ButtonIcon);
    pixmap=QPixmap(":/icons/paste.png");
    ButtonIcon=QIcon(pixmap);
    ui->pushButton_Paste->setIcon(ButtonIcon);
    pixmap=QPixmap(":/icons/cut.png");
    ButtonIcon=QIcon(pixmap);
    ui->pushButton_Cut->setIcon(ButtonIcon);
    pixmap=QPixmap(":/icons/copy.png");
    ButtonIcon=QIcon(pixmap);
    ui->pushButton_Copy->setIcon(ButtonIcon);
    pixmap=QPixmap(":/icons/close.png");
    ButtonIcon=QIcon(pixmap);
    ui->pushButton_Close->setIcon(ButtonIcon);
    pixmap=QPixmap(":/icons/exit.png");
    ButtonIcon=QIcon(pixmap);
    ui->pushButton_Exit->setIcon(ButtonIcon);
    }

    ui->pushButton_Copy->setEnabled(false);
    ui->pushButton_Cut->setEnabled(false);
    ui->pushButton_Redo->setEnabled(false);
    ui->pushButton_Undo->setEnabled(false);
    if( QClipboard* c = QApplication::clipboard())
    if(c->text().isEmpty()){
        ui->pushButton_Paste->setEnabled(false);
    }

    {         //saving font settings
        QSettings settings(QCoreApplication::applicationDirPath() + QDir::separator() + "nastr.ini", QSettings::IniFormat);
        settings.beginGroup("MainWindow");
        font = qvariant_cast<QFont>(settings.value("font"));
        ui->textEdit->setFont(font);
        settings.endGroup();
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_Create_clicked()
{
    MainWindow *w=new MainWindow;
    //w->ui->textEdit->setFont(font);
    w->prev=this;
    w->next=this->next;
    if(this->next!=NULL)
      this->next->prev=w;
    this->next=w;
    w->show();
}

void MainWindow::on_pushButton_Copy_clicked()
{

    if( QClipboard* c = QApplication::clipboard() ) {
            c->disconnect( this );
            c->setText( ui->textEdit->textCursor().selectedText() );
            ui->pushButton_Paste->setEnabled(true);
        }
}

void MainWindow::on_pushButton_Paste_clicked()
{
    if( QClipboard* c = QApplication::clipboard() ) {
              ui->textEdit->textCursor().insertText(c->text());

        }
}

void MainWindow::on_pushButton_Undo_clicked()
{
    ui->textEdit->undo();
}

void MainWindow::on_pushButton_Redo_clicked()
{
    ui->textEdit->redo();
}

void MainWindow::on_pushButton_Cut_clicked()
{
    if( QClipboard* c = QApplication::clipboard() ) {
            c->disconnect( this );
            c->setText( ui->textEdit->textCursor().selectedText() );
            ui->pushButton_Paste->setEnabled(true);
        }
    ui->textEdit->textCursor().deleteChar();
}

void MainWindow::on_pushButton_ChFont_clicked()
{
    bool ok;
    font = QFontDialog::getFont(&ok, QFont("Times", 12), this, QString::fromUtf8("Выберите шрифт"));
    if (ok) {
        ui->textEdit->setFont(font);
    }
    {
         QSettings settings(QCoreApplication::applicationDirPath() + QDir::separator() + "nastr.ini", QSettings::IniFormat);
         settings.beginGroup("MainWindow");
         settings.setValue("font", font);
         settings.endGroup();
    }

}

void MainWindow::on_pushButton_Exit_clicked()
{
    if(this->next!=NULL){
        this->next->prev=NULL;
        this->next->on_pushButton_Exit_clicked();
    }
    if(this->prev!=NULL){
        this->prev->next=NULL;
        this->prev->on_pushButton_Exit_clicked();
    }
    this->close();
}

void MainWindow::on_pushButton_Close_clicked()
{
    close();
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(!isChanged)
    {
        event->accept();
    }
    else{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, m_name+QString::fromUtf8(" not saved"),
                              QString::fromUtf8("Do you want to save the file before exit?"),
                              QMessageBox::Yes |QMessageBox::No | QMessageBox::Cancel,QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            on_pushButton_Save_clicked();
            if(isSaved)
                event->accept();
            else
                event->ignore();

        }
        else if (reply == QMessageBox::No){
            event->accept();
        }
        else
            event->ignore();

    }
}

void MainWindow::on_textEdit_textChanged()
{
    ui->pushButton_Undo->setEnabled(ui->textEdit->document()->isUndoAvailable());
    ui->pushButton_Redo->setEnabled(ui->textEdit->document()->isRedoAvailable());
    isChanged=true;
    isSaved=false;
    this->setWindowTitle("*"+m_name+" - InstaPaper");

}

void MainWindow::on_pushButton_Save_clicked()
{
    if(m_path.isEmpty())
    {
      m_path = QFileDialog::getSaveFileName(0, "Save file in", "", "*.txt *.doc");
    }
    if(!m_path.isEmpty())
    {
        m_name=m_path.split("/")[m_path.split("/").length()-1];
        this->setWindowTitle(m_name+" - InstaPaper");
        QFile f(m_path);
        if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qFatal("Couldn't open file ");
        }
        QTextStream ts (&f);
        ts<<ui->textEdit->toPlainText();
        f.close();
        isSaved=true;
        isChanged=false;
    }
}

void MainWindow::on_action_Save_file_as_triggered()
{
    QString str = QFileDialog::getSaveFileName(0, "Save file as", "", "*.txt *.doc");
    if(!str.isEmpty())
    {
        m_path=str;
        m_name=m_path.split("/")[m_path.split("/").length()-1];
        this->setWindowTitle(m_name+" - InstaPaper");
        QFile f(str);
        if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qFatal("Couldn't open file ");
        }
        QTextStream ts (&f);
        ts<<ui->textEdit->toPlainText();
        f.close();
        isChanged=false;
        isSaved=true;
    }
}

void MainWindow::on_action_About_Qt_triggered()
{
    QMessageBox::aboutQt(this, "Title About Qt");
}

void MainWindow::on_pushButton_Open_clicked()
{
    bool no=false;
    if(isChanged){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, m_name+QString::fromUtf8(" not saved"),
                              QString::fromUtf8("Do you want to save the file changes before exit?"),
                              QMessageBox::Yes |QMessageBox::No | QMessageBox::Cancel,QMessageBox::No);

        if (reply == QMessageBox::Yes)
        {
            on_pushButton_Save_clicked();
        }
        else if (reply == QMessageBox::Cancel){
            return;
        }
        else
            no=true;
    }
    if(!(isSaved || no))
        return;
    QString str = QFileDialog::getOpenFileName(0, "Open file", "", "*.txt *.doc *.cpp *.h");
    if (!str.isEmpty()){
    QFile f(str);
    if(!f.open(QIODevice::WriteOnly | QIODevice::ReadOnly))
    {
        qFatal("Couldn't open file ");
    }
    QTextStream in(&f);
    ui->textEdit->setText(in.readAll());
    f.close();
    m_path=str;
    m_name=m_path.split("/")[m_path.split("/").length()-1];
    this->setWindowTitle(m_name+" - InstaPaper");
    isChanged=false;
    isSaved=true;
    }
}

void MainWindow::on_action_About_programm_triggered()
{
   QFile f(QString(":/about.txt"));
   if(!f.open(QIODevice::ReadOnly))
   {
       qFatal("Couldn't open file ");
   }
   QTextStream in(&f);
   QMessageBox::about(this, "Title", in.readAll());
   f.close();
}


void MainWindow::on_textEdit_copyAvailable(bool b)
{
    if(b==true){
        ui->pushButton_Copy->setEnabled(true);
        ui->pushButton_Cut->setEnabled(true);
    }
    else{
        ui->pushButton_Copy->setEnabled(false);
        ui->pushButton_Cut->setEnabled(false);
        }
}
