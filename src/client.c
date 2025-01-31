#include "client.h"
#include "resource.h"
#include "scene_0.h"

void client_init() {
    fw_resource_init();
    scene_0_init();
}

void client_execute(fw_timer_data *time) {
    scene_0_execute(time);
}
