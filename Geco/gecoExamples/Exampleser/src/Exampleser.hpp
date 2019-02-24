#include "rack.hpp" 

using namespace rack;
extern Plugin *plugin;
extern Model *modelbuf;

struct switch_0 : SVGSwitch, ToggleSwitch {
switch_0() {
    addFrame(SVG::load(assetPlugin(plugin,"res/switch_0.svg")));
    addFrame(SVG::load(assetPlugin(plugin,"res/switch_1.svg")));
    sw->wrap();
    box.size = sw->box.size;
    }
};
struct jack : SVGPort{
jack() {
    background->svg = SVG::load(assetPlugin(plugin,"res/jack.svg"));
    background->wrap();
    box.size = background->box.size;
    }
};
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
struct button : SVGSwitch, MomentarySwitch {
    button(){
    addFrame(SVG::load(assetPlugin(plugin,"res/button.svg")));
    addFrame(SVG::load(assetPlugin(plugin,"res/buttonPushed.svg")));
    sw->wrap();
    box.size = sw->box.size;
    }
};
