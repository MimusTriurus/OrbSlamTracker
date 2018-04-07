#include "OrbSlamTrackerWrapper.h"
#include "OrbSlamTracker.h"

namespace OrbSlamTrackerWrapper {

OrbSlamTracker *_tracker;

void init( const char *assetsPath, int regim, bool showViewer ) {
    if ( _tracker ) {
        if ( _tracker->inProgress( ) )
            _tracker->track( false );
        delete _tracker;
    }
    _tracker = new OrbSlamTracker( QString( assetsPath ), regim, showViewer );
}

void track( bool value ) {
    if ( _tracker )
        _tracker->track( value );
}

float *position( ) {
    if ( _tracker )
        return _tracker->position( ).data( );
    return nullptr;
}

float *rotation( ) {
    if ( _tracker )
        return _tracker->rotation( ).data( );
    return nullptr;
}

void setLogCallback( void ( *func )( const char * ) ) {
    if ( _tracker )
        _tracker->logMessage = func;
}

void localDevice( bool value ) {
    if ( _tracker )
        _tracker->localDevice( value );
}

}
