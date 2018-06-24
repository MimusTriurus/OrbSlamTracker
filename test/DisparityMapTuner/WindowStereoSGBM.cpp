#include "WindowStereoSGBM.h"
#include <QFileDialog>

WindowStereoSGBM::WindowStereoSGBM( QWidget *parent ) : QWidget( parent ) {
    initInterface( );

    subscribe( );

    setBaseValues( );
}

void WindowStereoSGBM::initInterface( ) {
    this->setWindowTitle( "Stereo SGBM" );
    this->setLayout( &_layout );

    _layout.addWidget( &_lblPrefilterCap         , 0, 0 );
    _layout.addWidget( &_lblBlockSize            , 1, 0 );
    _layout.addWidget( &_lblMinDisparity         , 2, 0 );
    _layout.addWidget( &_lblNumDisparities       , 3, 0 );
    _layout.addWidget( &_lblUniquenessRatio      , 4, 0 );
    _layout.addWidget( &_lblSpeckleWindowSize    , 5, 0 );
    _layout.addWidget( &_lblSpeckleRange         , 6, 0 );
    _layout.addWidget( &_lblDisp12MaxDiff        , 7, 0 );

    _layout.addWidget( &_sldrPrefilterCap        , 0, 1 );
    _layout.addWidget( &_sldrBlockSize           , 1, 1 );
    _layout.addWidget( &_sldrMinDisparity        , 2, 1 );
    _layout.addWidget( &_sldrNumDisparities      , 3, 1 );
    _layout.addWidget( &_sldrUniquenessRatio     , 4, 1 );
    _layout.addWidget( &_sldrSpeckleWindowSize   , 5, 1 );
    _layout.addWidget( &_sldrSpeckleRange        , 6, 1 );
    _layout.addWidget( &_sldrDisp12MaxDiff       , 7, 1 );

    _layout.addWidget( &_btnOpen                 , 8, 0, 1, 2 );
    _layout.addWidget( &_btnOpenCam              , 9, 0, 1, 2 );
}

void WindowStereoSGBM::setBaseValues( ) {
    _sgbm = cv::StereoSGBM::create( 0, 16, 3 );
    _sgbm->setMode( cv::StereoSGBM::MODE_SGBM );

    _sldrPrefilterCap.setRange      ( 1, 63 );
    _sldrBlockSize.setRange         ( 5, 255 );
    _sldrMinDisparity.setRange      ( -300, 100 );
    _sldrNumDisparities.setRange    ( 16, 256 );
    _sldrUniquenessRatio.setRange   ( 0, 30 );
    _sldrSpeckleWindowSize.setRange ( 0, 250 );
    _sldrSpeckleRange.setRange      ( 0, 40 );
    _sldrDisp12MaxDiff.setRange     ( -1, 100 );

    _sldrPrefilterCap.setValue      ( 1 );
    _sldrBlockSize.setValue         ( 13 );
    _sldrMinDisparity.setValue      ( -2 );
    _sldrNumDisparities.setValue    ( 80 );
    _sldrUniquenessRatio.setValue   ( 0 );
    _sldrSpeckleWindowSize.setValue ( 0 );
    _sldrSpeckleRange.setValue      ( 0 );
    _sldrDisp12MaxDiff.setValue     ( 100 );
}

void WindowStereoSGBM::subscribe( ) {
    connect( &_sldrPrefilterCap     , SIGNAL( valueChanged( int ) ), SLOT( onChangePrefilterCap     ( int ) ) );
    connect( &_sldrBlockSize        , SIGNAL( valueChanged( int ) ), SLOT( onChangeBlockSize        ( int ) ) );
    connect( &_sldrMinDisparity     , SIGNAL( valueChanged( int ) ), SLOT( onChangeMinDisparity     ( int ) ) );
    connect( &_sldrNumDisparities   , SIGNAL( valueChanged( int ) ), SLOT( onChangeNumDisparities   ( int ) ) );
    connect( &_sldrUniquenessRatio  , SIGNAL( valueChanged( int ) ), SLOT( onChangeUniquenessRatio  ( int ) ) );
    connect( &_sldrSpeckleWindowSize, SIGNAL( valueChanged( int ) ), SLOT( onChangeSpeckleWindowSize( int ) ) );
    connect( &_sldrSpeckleRange     , SIGNAL( valueChanged( int ) ), SLOT( onChangeSpeckleRange     ( int ) ) );
    connect( &_sldrDisp12MaxDiff    , SIGNAL( valueChanged( int ) ), SLOT( onChangeDisp12MaxDiff    ( int ) ) );

    connect( &_btnOpen              , SIGNAL( clicked( bool ) )    , SLOT( onOpenFrame( ) ) );
    connect( &_btnOpenCam           , SIGNAL( clicked( bool ) )    , SLOT( onOpenStereoCam( ) ) );
    connect( &_tmrGetFrames         , SIGNAL( timeout( ) )         , SLOT( update( ) ) );
}

void WindowStereoSGBM::compute( ) {
    if ( _leftFrame.empty( ) | _rightFrame.empty( ) )
        return;
    _sgbm->compute( _leftFrame, _rightFrame, _map );
    _map.convertTo( _map8, CV_8U, 255 / ( _sldrNumDisparities.value( ) * 16. ) );
    cv::applyColorMap( _map8, _map8, cv::COLORMAP_JET );
    imshow( "depth", _map8 );
//    _originAndResult = cv::Mat( _leftFrame.size( ).height, _leftFrame.size( ).width * 2, _leftFrame.type( ) );
//    //_originAndResult.convertTo( _map8, CV_8U, 255 / ( _sldrNumDisparities.value( ) * 16. ) );
//    auto lRect = cv::Rect( 0, 0, _leftFrame.size( ).width, _leftFrame.size( ).height );
//    auto rRect = cv::Rect( _leftFrame.size( ).width, 0, _leftFrame.size( ).width, _leftFrame.size( ).height );
//    cv::Mat lF = _originAndResult( lRect );
//    cv::Mat rF = _originAndResult( rRect );
//    //cout << _leftFrame.size( ) << " == " << _map8.size( ) << endl;
//    _leftFrame.copyTo( lF );
//    _map8.copyTo( rF );
//    cv::imshow( "stereo", _originAndResult );
}

void WindowStereoSGBM::onChangePrefilterCap( int i ) {
    _lblPrefilterCap.setText( "PrefilterCap:" + QString::number( i ) );
    _sgbm->setPreFilterCap( i );
    compute( );
}

void WindowStereoSGBM::onChangeBlockSize( int i ) {
    if ( _leftFrame.empty( ) )
        return;
    int cn = _leftFrame.channels( );
    int v = ( ( i % 2 ) == 0 ) ? i + 1: i;
    _lblBlockSize.setText( "BlockSize:" + QString::number( v ) );
    _sgbm->setP1( 8 * cn * v * v );
    _sgbm->setP2( 32 * cn * v * v );
    _sgbm->setBlockSize( v );
    compute( );
}

void WindowStereoSGBM::onChangeMinDisparity( int i ) {
    _lblMinDisparity.setText( "MinDisparity:" + QString::number( i ) );
    _sgbm->setMinDisparity( i );
    compute( );
}

void WindowStereoSGBM::onChangeNumDisparities( int i ) {
    i = i / 16;
    i = i * 16;
    _lblNumDisparities.setText( "NumDisparities:" + QString::number( i ) );
    _sgbm->setNumDisparities( i );
    compute( );
}

void WindowStereoSGBM::onChangeUniquenessRatio( int i ) {
    _lblUniquenessRatio.setText( "UniquenessRatio:" + QString::number( i ) );
    _sgbm->setUniquenessRatio( i );
    compute( );
}

void WindowStereoSGBM::onChangeSpeckleWindowSize( int i ) {
    _lblSpeckleWindowSize.setText( "SpeckleWindowSize:" + QString::number( i ) );
    _sgbm->setSpeckleWindowSize( i );
    compute( );
}

void WindowStereoSGBM::onChangeSpeckleRange( int i ) {
    _lblSpeckleRange.setText( "SpeckleRange:" + QString::number( i ) );
    _sgbm->setSpeckleRange( i );
    compute( );
}

void WindowStereoSGBM::onChangeDisp12MaxDiff( int i ) {
    _lblDisp12MaxDiff.setText( "Disp12MaxDiff:" + QString::number( i ) );
    _sgbm->setDisp12MaxDiff( i );
    compute( );
}

void WindowStereoSGBM::onOpenFrame( ) {
    QString path = QFileDialog::getOpenFileName( 0, "Open Left frame", "", "*.jpg *.png" );
    _leftFrame = cv::imread( path.toStdString( ), 0 );

    path = QFileDialog::getOpenFileName( 0, "Open Right frame", "", "*.jpg *.png" );
    _rightFrame = cv::imread( path.toStdString( ), 0 );

    if ( _rightFrame.empty( ) | _leftFrame.empty( ) )
        return;

    onChangePrefilterCap       ( _sldrPrefilterCap.value( ) );
    onChangeBlockSize          ( _sldrBlockSize.value( ) );
    onChangeMinDisparity       ( _sldrMinDisparity.value( ) );
    onChangeNumDisparities     ( _sldrNumDisparities.value( ) );
    onChangeUniquenessRatio    ( _sldrUniquenessRatio.value( ) );
    onChangeSpeckleWindowSize  ( _sldrSpeckleWindowSize.value( ) );
    onChangeSpeckleRange       ( _sldrSpeckleRange.value( ) );
    onChangeDisp12MaxDiff      ( _sldrDisp12MaxDiff.value( ) );

    cv::imshow( "left", _leftFrame );
    cv::imshow( "right", _rightFrame );

    compute( );
}

void WindowStereoSGBM::onOpenStereoCam( ) {
    _stereoCam.init( QString( ) );
    _stereoCam.start( );
    _tmrGetFrames.start( 60 );
}

void WindowStereoSGBM::update( ) {
    _stereoCam.read( _leftFrame, _rightFrame );

    if ( _rightFrame.empty( ) | _leftFrame.empty( ) )
        return;

    onChangePrefilterCap       ( _sldrPrefilterCap.value( ) );
    onChangeBlockSize          ( _sldrBlockSize.value( ) );
    onChangeMinDisparity       ( _sldrMinDisparity.value( ) );
    onChangeNumDisparities     ( _sldrNumDisparities.value( ) );
    onChangeUniquenessRatio    ( _sldrUniquenessRatio.value( ) );
    onChangeSpeckleWindowSize  ( _sldrSpeckleWindowSize.value( ) );
    onChangeSpeckleRange       ( _sldrSpeckleRange.value( ) );
    onChangeDisp12MaxDiff      ( _sldrDisp12MaxDiff.value( ) );

    cv::imshow( "left", _leftFrame );
    cv::imshow( "right", _rightFrame );

    compute( );
}
