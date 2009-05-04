
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define JS_EVENT_BUTTON 0x01
#define JS_EVENT_AXIS 0x02

struct js_event {
    uint32_t time;
    int16_t value;
    uint8_t type;
    uint8_t number;
};
typedef struct js_event js_event_t;

struct js_state {
    uint8_t buttons[256];
    double axes[256];
};
typedef struct js_state js_state_t;

struct js {
    int fd;
    js_state_t state;
};
typedef struct js js_t;


typedef int JS_HANDLE;

js_t *js_open(uint8_t index);
js_t *js_open_first();

js_event_t *js_poll_event(js_t *js);
int js_poll_state(js_t *js);

void js_close(js_t *js);

#ifdef __cplusplus
}
#endif

