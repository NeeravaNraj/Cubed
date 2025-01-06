#include <time.h>
#include "inc/common.h"

int get_id(int counter) {
    time_t now = time(NULL);
    return ((short int)now << 16) | counter + 32683;
}
