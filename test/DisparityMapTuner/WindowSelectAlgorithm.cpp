#include "WindowSelectAlgorithm.h"
#include "WindowStereoBM.h"
#include "WindowStereoSGBM.h"

WindowSelectAlgorithm::WindowSelectAlgorithm( QWidget *parent ) : QWidget( parent ) {
    this->setLayout( &_layout );
    _layout.addWidget( &_btnStereoBM );
    _layout.addWidget( &_btnStereoSGBM );

    connect( &_btnStereoBM, SIGNAL( clicked( bool ) ), SLOT( createStereoBM( ) ) );
    connect( &_btnStereoSGBM, SIGNAL( clicked( bool ) ), SLOT( createStereoSGBM( ) ) );
}

void WindowSelectAlgorithm::createStereoBM( ) {
    this->close( );
    auto w = new WindowStereoBM( );
    w->show( );
}

void WindowSelectAlgorithm::createStereoSGBM( ) {
    this->close( );
    auto w = new WindowStereoSGBM( );
    w->show( );
}
