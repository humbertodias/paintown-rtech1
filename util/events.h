#ifndef _paintown_events_h
#define _paintown_events_h

/* handles global events from the system such as
 *   window manager events (press X button)
 *   keyboard/mouse/joystick input (for some backends like SDL)
 */

namespace Util{

class Thread;

class EventManager{
public:
    EventManager();
    virtual void run();
    virtual void waitForThread(Thread & thread);
    virtual ~EventManager();

private:
    enum Event{
        CloseWindow
    };

    virtual void dispatch(Event type);

#ifdef USE_SDL
    virtual void runSDL();
#endif
};

}

#endif