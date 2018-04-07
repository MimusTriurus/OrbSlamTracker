#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include "ConfigGetter.h"

MainWindow::MainWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    qDebug( ) << "1 == " << ConfigGetter::getParam<int>( "1", 0 );;
    qDebug( ) << "2 == " << ConfigGetter::getParam<int>( "2", 0 );;
}

MainWindow::~MainWindow( ) {
    delete ui;
}
