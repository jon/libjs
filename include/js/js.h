/**
 * A simple interface to Linux Joystick devies
 *
 * @file js.h
 * @author Jon Olson <jon@damogran.com>
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Event type tag for button press and release events
 */
#define JS_EVENT_BUTTON 0x01

/**
 * Event type tag for axis motion events
 */
#define JS_EVENT_AXIS 0x02

/**
 * A Joystick event as defined by the Linux kernel
 */
struct js_event {
    uint32_t time; ///< Event timestamp
    int16_t value; ///< Event value (position for axes, 0/1 for buttons)
    uint8_t type; ///< Event type
    uint8_t number; ///< Affected joystick actuator (i.e., which axis moved, which button changed)
};

/**
 * A friendly type for struct js_event
 */
typedef struct js_event js_event_t;

/**
 * The state of an attached joystick, based on all events received to date
 */
struct js_state {
    uint8_t buttons[256]; ///< State of all joystick buttons (note: most joysticks have less than 256 buttons). Can be treated as booleans where True is pressed
    double axes[256]; ///< Position of all joystick axes. These values are clamped between -1.0 and 1.0
};

/**
 * A friendly type for struct js_state
 */
typedef struct js_state js_state_t;

/**
 * An open joystick
 */
struct js {
    int fd; ///< File descriptor of the event interface
    js_state_t state; ///< Button and axis state vector
};

/**
 * A friendly type for struct js
 */
typedef struct js js_t;

/**
 * Open a joystick by index
 *
 * @param index The index of the joystick to open. This will try to directly open the Linux input device with the associated index
 * @return Pointer to a newly allocated js_t or NULL if allocation or opening the device failed
 */
js_t *js_open(uint8_t index);

/**
 * Opens the first available joystick
 *
 * @return Pointer to a newly allocated js_t for the first Joystick or NULL if allocation or opening failed
 */
js_t *js_open_first();

/**
 * Polls the joystick for a single event. Note that this does no process on the event and does not
 * update the joystick's state. Additionally, multiple calls to this function
 * will overwrite the static js_event_t.
 *
 * @param js The joystick to poll
 * @return An event received from the joystick.
 */
js_event_t *js_poll_event(js_t *js);

/**
 * Polls the requested joystick for all events since the last poll and updates its state.
 *
 * @param js The joystick to poll
 * @return 0 on success, -1 on failure
 */
int js_poll_state(js_t *js);

/**
 * Close and free a js_t
 *
 * @param js The joystick to close
 */
void js_close(js_t *js);

#ifdef __cplusplus
}
#endif

