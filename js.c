
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <js/js.h>

#define MAX_JOYSTICKS 256
#define JOYSTICK_DIR "/dev/input"
#define JOYSTICK_FORMAT JOYSTICK_DIR "/js%d"

static js_t *js_alloc() {
    js_t *js;
   
    js = (js_t *)malloc(sizeof(js_t));
    if (js == NULL)
        return NULL;

    memset(js, 0, sizeof(js_t));

    return js;
}

js_t *js_open(uint8_t index) {
    char js_name[64];

    snprintf(js_name, 63, JOYSTICK_FORMAT, index);
    js_name[63] = '\0'; // Never useful, but we're careful folk around here

    js_t *js = js_alloc();
    if (js == NULL)
        goto fail;

    js->fd = open(js_name, O_RDONLY);
    if (js->fd < 0)
        goto fail;

    return js;

fail:

    free(js);
    return NULL;
}

js_t *js_open_first() {
    DIR *inputs;
    js_t *js = NULL;

    inputs = opendir("/dev/input");
    if (inputs == NULL)
        return NULL;

    struct dirent *input;
    do {
        input = readdir(inputs);
        if (input == NULL)
            break; // Could just be end of list, but if we get end of list
                   // without returning, that IS a failure
       
        unsigned int index;
        if (sscanf(input->d_name, JOYSTICK_DIR "/js%u", &index) > 0) {
            // Match
            js = js_alloc();
            if (js == NULL)
                break;

            js->fd = open(input->d_name, O_RDONLY);
            if (js->fd < 0) {
                free(js);
                break;
            } 
        }
    } while (input != NULL);    

    closedir(inputs);
    return js; // Either a valid js or NULL
}

js_event_t *js_poll_event(js_t *js) {
    static js_event_t event;
    assert(js != NULL);

    int len = read(js->fd, &event, sizeof(event));
    if (len < (int)sizeof(event))
        return NULL;    

    return &event;
}

int js_poll_state(js_t *js) {
    js_event_t *event = js_poll_event(js);

    if (event == NULL)
        return -1;

    switch (event->type) {
        case JS_EVENT_AXIS:
            js->state.axes[event->number] = (double)event->value / 32768.0;
            break;
        case JS_EVENT_BUTTON:
            js->state.buttons[event->number] = event->value;
            break;
        default:
            break;
    }

    return 0;
}
    

void js_close(js_t *js) {
    assert(js != NULL);

    close(js->fd);
    free(js);
}

