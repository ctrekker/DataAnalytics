#include <iostream>

#include "log.h"

using namespace std;

int main() {
    Log log(true, "latest.log");

    log.warning("I'm warning you!!!");
    log.error("Uh oh...");
    log.info("Just to let you know...");
    log.debug("Here's some really overly specific debug log output for you");

    return 0;
}
