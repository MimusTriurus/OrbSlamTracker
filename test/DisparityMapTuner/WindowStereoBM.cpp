#include "WindowStereoBM.h"
#include <QDebug>
#include <QFileDialog>

WindowStereoBM::WindowStereoBM( QWidget *parent ) :
    QWidget( parent )
{
    initInterface( );

    subscribe( );

    setBaseValues( );
}

WindowStereoBM::~WindowStereoBM( ) {
    _stereoCam.stop( );
}

void WindowStereoBM::initInterface( ) {
    this->setWindowTitle( "Stereo BM" );
    this->setLayout( &_layout );

    _layout.addWidget( &_lblPrefilterCap         , 0, 0 );
    _layout.addWidget( &_lblBlockSize            , 1, 0 );
    _layout.addWidget( &_lblMinDisparity         , 2, 0 );
    _layout.addWidget( &_lblNumDisparities       , 3, 0 );
    _layout.addWidget( &_lblTextureThreshold     , 4, 0 );
    _layout.addWidget( &_lblUniquenessRatio      , 5, 0 );
    _layout.addWidget( &_lblSpeckleWindowSize    , 6, 0 );
    _layout.addWidget( &_lblSpeckleRange         , 7, 0 );
    _layout.addWidget( &_lblDisp12MaxDiff        , 8, 0 );

    _layout.addWidget( &_sldrPrefilterCap        , 0, 1 );
    _layout.addWidget( &_sldrBlockSize           , 1, 1 );
    _layout.addWidget( &_sldrMinDisparity        , 2, 1 );
    _layout.addWidget( &_sldrNumDisparities      , 3, 1 );
    _layout.addWidget( &_sldrTextureThreshold    , 4, 1 );
    _layout.addWidget( &_sldrUniquenessRatio     , 5, 1 );
    _layout.addWidget( &_sldrSpeckleWindowSize   , 6, 1 );
    _layout.addWidget( &_sldrSpeckleRange        , 7, 1 );
    _layout.addWidget( &_sldrDisp12MaxDiff       , 8, 1 );

    _layout.addWidget( &_btnOpen                 , 9, 0, 1, 2 );
    _layout.addWidget( &_btnOpenCam              , 10, 0, 1, 2 );
}

void WindowStereoBM::setBaseValues( ) {
    _bm = cv::StereoBM::create( 16, 9 );

    _sldrPrefilterCap.setRange      ( 1, 63 );
    _sldrBlockSize.setRange         ( 5, 255 );
    _sldrMinDisparity.setRange      ( -300, 100 );
    _sldrNumDisparities.setRange    ( 16, 256 );
    _sldrTextureThreshold.setRange  ( 0, 1000 );
    _sldrUniquenessRatio.setRange   ( 0, 30 );
    _sldrSpeckleWindowSize.setRange ( 0, 250 );
    _sldrSpeckleRange.setRange      ( 0, 40 );
    _sldrDisp12MaxDiff.setRange     ( -1, 100 );

    _sldrPrefilterCap.setValue      ( 63 );
    _sldrBlockSize.setValue         ( 11 );
    _sldrMinDisparity.setValue      ( -3 );
    _sldrNumDisparities.setValue    ( 96 );
    _sldrTextureThreshold.setValue  ( 20 );
    _sldrUniquenessRatio.setValue   ( 9 );
    _sldrSpeckleWindowSize.setValue ( 250 );
    _sldrSpeckleRange.setValue      ( 3 );
    _sldrDisp12MaxDiff.setValue     ( 100 );
}

void WindowStereoBM::subscribe( ) {
    connect( &_sldrPrefilterCap     , SIGNAL( valueChanged( int ) ), SLOT( onChangePrefilterCap     ( int ) ) );
    connect( &_sldrBlockSize        , SIGNAL( valueChanged( int ) ), SLOT( onChangeBlockSize        ( int ) ) );
    connect( &_sldrMinDisparity     , SIGNAL( valueChanged( int ) ), SLOT( onChangeMinDisparity     ( int ) ) );
    connect( &_sldrNumDisparities   , SIGNAL( valueChanged( int ) ), SLOT( onChangeNumDisparities   ( int ) ) );
    connect( &_sldrTextureThreshold , SIGNAL( valueChanged( int ) ), SLOT( onChangeTextureThreshold ( int ) ) );
    connect( &_sldrUniquenessRatio  , SIGNAL( valueChanged( int ) ), SLOT( onChangeUniquenessRatio  ( int ) ) );
    connect( &_sldrSpeckleWindowSize, SIGNAL( valueChanged( int ) ), SLOT( onChangeSpeckleWindowSize( int ) ) );
    connect( &_sldrSpeckleRange     , SIGNAL( valueChanged( int ) ), SLOT( onChangeSpeckleRange     ( int ) ) );
    connect( &_sldrDisp12MaxDiff    , SIGNAL( valueChanged( int ) ), SLOT( onChangeDisp12MaxDiff    ( int ) ) );

    connect( &_btnOpen              , SIGNAL( clicked( bool ) ), SLOT( onOpenFrame( ) ) );
    connect( &_btnOpenCam           , SIGNAL( clicked( bool ) ), SLOT( onOpenStereoCam( ) ) );
    connect( &_tmrGetFrames         , SIGNAL( timeout( ) )     , SLOT( update( ) ) );
}

void WindowStereoBM::compute( ) {
    if ( _leftFrame.empty( ) | _rightFrame.empty( ) )
        return;
    _bm->compute( _leftFrame, _rightFrame, _map );
    _map.convertTo( _map8, CV_8U, 255 / ( _sldrNumDisparities.value( ) * 16. ) );
    cv::applyColorMap( _map8, _map8, cv::COLORMAP_JET);
    imshow( "depth", _map8 );
}

void WindowStereoBM::onChangePrefilterCap( int i ) {
    _lblPrefilterCap.setText( "PrefilterCap:" + QString::number( i ) );
    _bm->setPreFilterCap        ( _sldrPrefilterCap.value( ) );
    compute( );
}

void WindowStereoBM::onChangeBlockSize( int i ) {
    Q_UNUSED( i )
    int v = ( ( _sldrBlockSize.value( ) % 2 ) == 0 ) ? _sldrBlockSize.value( ) + 1: _sldrBlockSize.value( );
    _lblBlockSize.setText( "BlockSize:" + QString::number( v ) );
    _bm->setBlockSize           ( v );
    compute( );
}

void WindowStereoBM::onChangeMinDisparity( int i ) {
    _lblMinDisparity.setText( "MinDisparity:" + QString::number( i ) );
    _bm->setMinDisparity        ( _sldrMinDisparity.value( ) );
    compute( );
}

void WindowStereoBM::onChangeNumDisparities( int i ) {
    i = i / 16;
    i = i * 16;
    _lblNumDisparities.setText( "NumDisparities:" + QString::number( i ) );
    _bm->setNumDisparities      ( i );
    compute( );
}

void WindowStereoBM::onChangeTextureThreshold( int i ) {
    _lblTextureThreshold.setText( "TextureThreshold:" + QString::number( i ) );
    _bm->setTextureThreshold    ( _sldrTextureThreshold.value( ) );
    compute( );
}

void WindowStereoBM::onChangeUniquenessRatio( int i ) {
    _lblUniquenessRatio.setText( "UniquenessRatio:" + QString::number( i ) );
    _bm->setUniquenessRatio     ( _sldrUniquenessRatio.value( ) );
    compute( );
}

void WindowStereoBM::onChangeSpeckleWindowSize( int i ) {
    _lblSpeckleWindowSize.setText( "SpeckleWindowSize:" + QString::number( i ) );
    _bm->setSpeckleWindowSize   ( _sldrSpeckleWindowSize.value( ) );
    compute( );
}

void WindowStereoBM::onChangeSpeckleRange( int i ) {
    _lblSpeckleRange.setText( "SpeckleRange:" + QString::number( i ) );
    _bm->setSpeckleRange        ( _sldrSpeckleRange.value( ) );
    compute( );
}

void WindowStereoBM::onChangeDisp12MaxDiff( int i ) {
    _lblDisp12MaxDiff.setText( "Disp12MaxDiff:" + QString::number( i ) );
    _bm->setDisp12MaxDiff       ( _sldrDisp12MaxDiff.value( ) );
    compute( );
}

void WindowStereoBM::onOpenFrame( ) {
    QString path = QFileDialog::getOpenFileName( 0, "Open Left frame", "", "*.jpg *.png" );
    _leftFrame = cv::imread( path.toStdString( ), 0 );

    path = QFileDialog::getOpenFileName( 0, "Open Right frame", "", "*.jpg *.png" );
    _rightFrame = cv::imread( path.toStdString( ), 0 );

    onChangePrefilterCap       ( _sldrPrefilterCap.value( ) );
    onChangeBlockSize          ( _sldrBlockSize.value( ) );
    onChangeMinDisparity       ( _sldrMinDisparity.value( ) );
    onChangeNumDisparities     ( _sldrNumDisparities.value( ) );
    onChangeTextureThreshold   ( _sldrTextureThreshold.value( ) );
    onChangeUniquenessRatio    ( _sldrUniquenessRatio.value( ) );
    onChangeSpeckleWindowSize  ( _sldrSpeckleWindowSize.value( ) );
    onChangeSpeckleRange       ( _sldrSpeckleRange.value( ) );
    onChangeDisp12MaxDiff      ( _sldrDisp12MaxDiff.value( ) );

    if ( _rightFrame.empty( ) | _leftFrame.empty( ) )
        return;
    cv::imshow( "left", _leftFrame );
    cv::imshow( "right", _rightFrame );
    compute( );
}

void WindowStereoBM::onOpenStereoCam( ) {
    _stereoCam.init( QString( ) );
    _stereoCam.start( );
    _tmrGetFrames.start( 60 );
}

void WindowStereoBM::update( ) {
    _stereoCam.read( _leftFrame, _rightFrame );

    cv::cvtColor( _leftFrame, _leftFrame, cv::COLOR_BGR2GRAY );
    cv::cvtColor( _rightFrame, _rightFrame, cv::COLOR_BGR2GRAY );

    onChangePrefilterCap       ( _sldrPrefilterCap.value( ) );
    onChangeBlockSize          ( _sldrBlockSize.value( ) );
    onChangeMinDisparity       ( _sldrMinDisparity.value( ) );
    onChangeNumDisparities     ( _sldrNumDisparities.value( ) );
    onChangeTextureThreshold   ( _sldrTextureThreshold.value( ) );
    onChangeUniquenessRatio    ( _sldrUniquenessRatio.value( ) );
    onChangeSpeckleWindowSize  ( _sldrSpeckleWindowSize.value( ) );
    onChangeSpeckleRange       ( _sldrSpeckleRange.value( ) );
    onChangeDisp12MaxDiff      ( _sldrDisp12MaxDiff.value( ) );

    if ( _rightFrame.empty( ) | _leftFrame.empty( ) )
        return;
    cv::imshow( "left", _leftFrame );
    cv::imshow( "right", _rightFrame );
    compute( );
}
