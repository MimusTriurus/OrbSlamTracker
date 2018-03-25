#ifndef ORBSLAMTRACKERWRAPPER_H
#define ORBSLAMTRACKERWRAPPER_H

#include <QtGlobal>

#if defined( Q_CC_MSVC )
#define FUNCTION_EXPORT __declspec( dllexport )
#else
#define FUNCTION_EXPORT
#endif

#include <QtCore>
#include <opencv2/core.hpp>

namespace OrbSlamTrackerWrapper {

extern "C" {
    FUNCTION_EXPORT void init( const char *assetsPath, int regim, bool showViewer = false );

    FUNCTION_EXPORT void track( bool value );

    FUNCTION_EXPORT float *position( );

    FUNCTION_EXPORT float *rotation( );

    FUNCTION_EXPORT void localDevice( bool value );

    FUNCTION_EXPORT void setLogCallback( void ( *func )( const char *value ) );
}

}

#endif // ORBSLAMTRACKERWRAPPER_H
