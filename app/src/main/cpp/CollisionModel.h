//
// Created by masque on 2022/1/12.
//

#ifndef MY_APPLICATION_COLLISIONMODEL_H
#define MY_APPLICATION_COLLISIONMODEL_H

#include "Box2D/Box2D.h"
#include <map>

class CollisionModel {
public:
    CollisionModel();
    ~CollisionModel();

    void createNewWorld();
    void startWorld();
    void updateBounds(int width, int height);
    b2Body* createBody(float x, float y, float width, float height, int viewAddress);
    b2Body* createRectangleBody(float x, float y, float width, float height, int viewAddress);
    void applyLinearImpulse(float x, float y);
    float *queryPosition(int);
    float queryAngle(int);
private:
    b2World* world;
    int mWidth;
    int mHeight;
    float mRatio;
    std::map<int, long> bodyMap;
};


#endif //MY_APPLICATION_COLLISIONMODEL_H
