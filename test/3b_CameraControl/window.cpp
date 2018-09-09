#include "window.h"
#include <QDebug>
#include <QString>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "vertex.h"
#include "input.h"
#include <QVector3D>

//#include "Eigen/Eigen"

// Front Verticies
#define VERTEX_FTR Vertex( QVector3D( 0.5f,  0.5f,  0.5f), QVector3D( 1.0f, 0.0f, 0.0f ) )
#define VERTEX_FTL Vertex( QVector3D(-0.5f,  0.5f,  0.5f), QVector3D( 0.0f, 1.0f, 0.0f ) )
#define VERTEX_FBL Vertex( QVector3D(-0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 1.0f ) )
#define VERTEX_FBR Vertex( QVector3D( 0.5f, -0.5f,  0.5f), QVector3D( 0.0f, 0.0f, 0.0f ) )

// Back Verticies
#define VERTEX_BTR Vertex( QVector3D( 0.5f,  0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 0.0f ) )
#define VERTEX_BTL Vertex( QVector3D(-0.5f,  0.5f, -0.5f), QVector3D( 0.0f, 1.0f, 1.0f ) )
#define VERTEX_BBL Vertex( QVector3D(-0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 0.0f, 1.0f ) )
#define VERTEX_BBR Vertex( QVector3D( 0.5f, -0.5f, -0.5f), QVector3D( 1.0f, 1.0f, 1.0f ) )

// Create a colored cube
static const Vertex sg_vertexes[] = {
  // Face 1 (Front)
    VERTEX_FTR, VERTEX_FTL, VERTEX_FBL,
    VERTEX_FBL, VERTEX_FBR, VERTEX_FTR,
  // Face 2 (Back)
    VERTEX_BBR, VERTEX_BTL, VERTEX_BTR,
    VERTEX_BTL, VERTEX_BBR, VERTEX_BBL,
  // Face 3 (Top)
    VERTEX_FTR, VERTEX_BTR, VERTEX_BTL,
    VERTEX_BTL, VERTEX_FTL, VERTEX_FTR,
  // Face 4 (Bottom)
    VERTEX_FBR, VERTEX_FBL, VERTEX_BBL,
    VERTEX_BBL, VERTEX_BBR, VERTEX_FBR,
  // Face 5 (Left)
    VERTEX_FBL, VERTEX_FTL, VERTEX_BTL,
    VERTEX_FBL, VERTEX_BTL, VERTEX_BBL,
  // Face 6 (Right)
    VERTEX_FTR, VERTEX_FBR, VERTEX_BBR,
    VERTEX_BBR, VERTEX_BTR, VERTEX_FTR
};

#undef VERTEX_BBR
#undef VERTEX_BBL
#undef VERTEX_BTL
#undef VERTEX_BTR

#undef VERTEX_FBR
#undef VERTEX_FBL
#undef VERTEX_FTL
#undef VERTEX_FTR

/*******************************************************************************
 * OpenGL Events
 ******************************************************************************/

#include <math.h>
#include <QMatrix3x3>

#include <OrbSlamTrackerWrapper.h>

Window::Window( ) {
  m_transform.translate( 0.0f, 0.0f, -5.0f );
  QString configFilePath{ qApp->applicationDirPath( ) };
  OrbSlamTrackerWrapper::init( configFilePath.toUtf8( ), 1, true );
  OrbSlamTrackerWrapper::setLogCallback( onReceiveLog );
//  OrbSlamTrackerWrapper::localDevice( false );
  OrbSlamTrackerWrapper::track( true );
}

Window::~Window( ) {

}

void Window::initializeGL( ) {
    initializeOpenGLFunctions( );
    connect( context( ), SIGNAL( aboutToBeDestroyed( ) ), this, SLOT( teardownGL( ) ), Qt::DirectConnection );
    connect(this, SIGNAL( frameSwapped( ) ), this, SLOT( update( ) ) );
    printVersionInformation( );

    glEnable( GL_CULL_FACE );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    m_program = new QOpenGLShaderProgram( );
    m_program->addShaderFromSourceFile( QOpenGLShader::Vertex, ":/shaders/simple.vert" );
    m_program->addShaderFromSourceFile( QOpenGLShader::Fragment, ":/shaders/simple.frag" );
    m_program->link( );
    m_program->bind( );

    u_modelToWorld = m_program->uniformLocation( "modelToWorld" );
    u_worldToCamera = m_program->uniformLocation( "worldToCamera" );
    u_cameraToView = m_program->uniformLocation( "cameraToView" );

    m_vertex.create( );
    m_vertex.bind( );
    m_vertex.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_vertex.allocate( sg_vertexes, sizeof( sg_vertexes ) );

    m_object.create( );
    m_object.bind( );
    m_program->enableAttributeArray( 0 );
    m_program->enableAttributeArray( 1 );
    m_program->setAttributeBuffer( 0, GL_FLOAT, Vertex::positionOffset( ), Vertex::PositionTupleSize, Vertex::stride( ) );
    m_program->setAttributeBuffer( 1, GL_FLOAT, Vertex::colorOffset( ), Vertex::ColorTupleSize, Vertex::stride( ) );

    m_object.release( );
    m_vertex.release( );
    m_program->release( );
}

void Window::resizeGL( int width, int height ) {
  m_projection.setToIdentity( );
  m_projection.perspective( 60.0f, width / float(height), 0.1f, 1000.0f );
}

void Window::paintGL( ) {
    glClear( GL_COLOR_BUFFER_BIT );

    m_program->bind( );
    m_program->setUniformValue( u_worldToCamera, m_camera.toMatrix( ) );
    m_program->setUniformValue( u_cameraToView, m_projection );

    m_object.bind( );
    m_program->setUniformValue( u_modelToWorld, m_transform.toMatrix( ) );
    glDrawArrays( GL_TRIANGLES, 0, sizeof( sg_vertexes ) / sizeof( sg_vertexes[ 0 ] ) );
    m_object.release( );

    m_program->release( );
}

void Window::teardownGL( ) {
    m_object.destroy( );
    m_vertex.destroy( );
    delete m_program;
}

#include <QtMath>

void Window::update( ) {
    float delta = ( QTime::currentTime( ).msec( ) - _last );
    static float transSpeed = 0.05f;
    //transSpeed = 1.0f;
    static const float rotSpeed = 1;

    float *pos = OrbSlamTrackerWrapper::position( );

//    QVector3D translation( pos[ 0 ], 0, 0 );
    QVector3D translation( pos[ 0 ], -pos[ 1 ], pos[ 2 ] );
    __int64 curNow = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now( ).time_since_epoch( ) ).count( ) / 1000.0;
    delta = 1;
    translation = QQuaternion::slerp( QQuaternion( 1, m_camera.translation( ) ), QQuaternion( 1, translation ), curNow ).toEulerAngles( );
    m_camera.setTranslation( translation * transSpeed );

    float *rotArr{ OrbSlamTrackerWrapper::rotation( ) };
//    __int64 curNow = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::system_clock::now( ).time_since_epoch( ) ).count( ) / 1000.0;

    QQuaternion rotation{ rotArr[ 0 ], -rotArr[ 1 ], rotArr[ 2 ], rotArr[ 3 ] };
    rotation = QQuaternion::slerp( m_camera.rotation( ), rotation, curNow );
    m_camera.setRotation( rotation );

    _last = QTime::currentTime( ).msec( );

    QOpenGLWindow::update( );
}

/*******************************************************************************
 * Private Helpers
 ******************************************************************************/

void Window::printVersionInformation( ) {
    QString glType;
    QString glVersion;
    QString glProfile;

    // Get Version Information
    glType = ( context( )->isOpenGLES( ) ) ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char*>( glGetString( GL_VERSION ) );

    // Get Profile Information
    #define CASE( c ) case QSurfaceFormat::c: glProfile = #c; break
    switch ( format( ).profile( ) ) {
    CASE( NoProfile );
    CASE( CoreProfile );
    CASE( CompatibilityProfile );
    }
    #undef CASE
    // qPrintable() will print our QString w/o quotes around it.
    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}

void Window::onReceiveLog( const char *hypothesis ) {
    qDebug( ) << "log:" << hypothesis;
}
