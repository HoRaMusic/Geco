#include "rack.hpp" 

using namespace rack;
extern Plugin *plugin;
extern Model *modelLowPassVCF;

struct mediumKnob : SVGKnob{
mediumKnob() {
    box.size = Vec(25, 25);
    minAngle = -0.75*M_PI;
    maxAngle = 0.75*M_PI;
    setSVG(SVG::load(assetPlugin(plugin,"res/mediumKnob.svg")));
    SVGWidget *shadow = new SVGWidget();
    shadow->setSVG(SVG::load(assetPlugin(plugin, "res/mediumknobShadow.svg")));
    addChild(shadow);
    }
};
struct jack : SVGPort{
jack() {
    background->svg = SVG::load(assetPlugin(plugin,"res/jack.svg"));
    background->wrap();
    box.size = background->box.size;
    }
};
