#include "../include/common.h"

void f_delete(int index) {
    // delete text
    for(int a = 1; a <= index + 5; a++) {
        printf("\b \b");
    }
}