#pragma once
#include "Framework/BaseLevel.h"
class Scene :
    public BaseLevel
{
public:
    Scene(sf::RenderWindow& hwnd, Input& in, GameState& gs, AudioManager& aud);

    // Called when this scene becomes active (see Main.cpp game loop).
    virtual void onBegin() = 0;

    // Called when leaving this scene for another.
    virtual void onEnd() = 0;
};

