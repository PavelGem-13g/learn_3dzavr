//
// Created by Иван Ильин on 29.01.2021.
//

#ifndef ENGINE_ASCALE_H
#define ENGINE_ASCALE_H

#include "Animation.h"
#include "../Object.h"

class AScale final : public Animation {
private:
    const std::weak_ptr<Object> _object;
    const Vec3D _scalingValue;
    Vec3D _previousScaleFactor{1, 1, 1};

    void update() override {
        auto obj = _object.lock();

        if (obj == nullptr) {
            stop();
            return;
        }
        obj->scale(Vec3D(1/_previousScaleFactor.x(), 1/_previousScaleFactor.y(), 1/_previousScaleFactor.z()));
        Vec3D scaleFactor = Vec3D{1, 1, 1} + (_scalingValue - Vec3D(1, 1, 1))*progress();
        obj->scale(scaleFactor);
        _previousScaleFactor = scaleFactor;
    }

public:
    AScale(std::weak_ptr<Object> object, const Vec3D &s, double duration = 1, LoopOut looped = LoopOut::None,
           InterpolationType interpolationType = InterpolationType::Bezier) : Animation(duration, looped,
                                                                                        interpolationType),
                                                                              _object(object), _scalingValue(s) {
    }
};

#endif //INC_3DZAVR_ASCALE_H
