#include "Examples.hpp"
#include <math.h>
    float getSampleRate()
    {
        return engineGetSampleRate();
    }

Plugin *plugin;
void init(rack::Plugin *p) {
    plugin = p;
    p->slug = "Hora-Examples";
#ifdef VERSION
    p->version = TOSTRING(VERSION);
#endif

    p->addModel(modelLowPassVCF);
}
