//
// Created by masque on 2022/1/12.
//

#include "CollisionModel.h"

#include "blog.h"

CollisionModel::CollisionModel():world(nullptr),
mWidth(0),
mHeight(0),
mRatio(50)
{
    slogd("new collision model");
}

CollisionModel::~CollisionModel() {
    delete world;
}

void CollisionModel::createNewWorld() {
    b2Vec2 gravity(0.0f, -9.8f);
    delete world;
    world = new b2World(gravity);
    world->SetAllowSleeping(false);
    world->SetContinuousPhysics(true);
    bodyMap.clear();
}

void CollisionModel::startWorld() {
    world->Step(1.0f / 50.0f, 10, 5);
}

void CollisionModel::updateBounds(int width, int height) {
    createNewWorld();
    mWidth = width;
    mHeight = height;
    b2BodyDef bodyDef;
    bodyDef.type = b2_staticBody;
    //左下
    bodyDef.position.Set(0, 0);
    b2Body* body = world->CreateBody(&bodyDef);
    b2EdgeShape groundBox;
    groundBox.Set(b2Vec2(0,0), b2Vec2(static_cast<float>(mWidth) / mRatio, 0));
    body->CreateFixture(&groundBox, 0);

    // top
    groundBox.Set(b2Vec2(0,static_cast<float>(mHeight) / mRatio), b2Vec2(static_cast<float>(mWidth) / mRatio, static_cast<float>(mHeight) / mRatio));
    body->CreateFixture(&groundBox, 0);

    // left
    groundBox.Set(b2Vec2(0, static_cast<float>(mHeight) / mRatio), b2Vec2(0,0));
    body->CreateFixture(&groundBox, 0);

    // right
    groundBox.Set(b2Vec2(static_cast<float>(mWidth) / mRatio, static_cast<float>(mHeight) / mRatio), b2Vec2(static_cast<float>(mWidth) / mRatio,0));
    body->CreateFixture(&groundBox, 0);
}

b2Body* CollisionModel::createBody(float x, float y, float width, float height, int viewAddress) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    slogd("body init width = %f, height = %f", width, height);
    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(width / 2 / mRatio, height / 2 / mRatio);
    b2CircleShape circleShape;
    circleShape.m_radius = width / 2 / mRatio;
    bodyDef.position.Set((x + width / 2) / mRatio, (y + height / 2) / mRatio);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &polygonShape;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.5f;
    fixtureDef.density = 3;
    b2Body* body = world->CreateBody(&bodyDef);
    body->CreateFixture(&fixtureDef);
    bodyMap.insert(std::pair<int, long>(viewAddress, reinterpret_cast<uintptr_t>(body)));
    slogd("body init x = %f, y = %f", (x + width / 2), (y + height / 2));
    return body;
}

b2Body* CollisionModel::createRectangleBody(float x, float y, float width, float height,
                                            int viewAddress) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    slogd("body init width = %f, height = %f", width, height);
    b2PolygonShape polygonShape;
    polygonShape.SetAsBox(width / 2 / mRatio, height / 2 / mRatio);
    b2CircleShape circleShape;
    circleShape.m_radius = width / 2 / mRatio;
    bodyDef.position.Set((x + width / 2) / mRatio, (y + height / 2) / mRatio);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.5f;
    fixtureDef.density = 3;
    b2Body* body = world->CreateBody(&bodyDef);
    body->CreateFixture(&fixtureDef);
    bodyMap.insert(std::pair<int, long>(viewAddress, reinterpret_cast<uintptr_t>(body)));
    slogd("body init x = %f, y = %f", (x + width / 2), (y + height / 2));
    return body;
}

void CollisionModel::applyLinearImpulse(float x, float y) {
    slogd("sensor x = %f, sensor y = %f", x, y);
    if (world != nullptr) {
        world->SetGravity(b2Vec2(x * 10, y * 10));
    }
}

float *CollisionModel::queryPosition(int view) {
    auto *pos = new float(3);
    std::map<int, long>::iterator iter;
    iter = bodyMap.find(view);
    if (iter != bodyMap.end()) {
        for (b2Body *body = world->GetBodyList(); body; body = body->GetNext()) {
            if (body->GetType() == b2_dynamicBody) {
                if (reinterpret_cast<uintptr_t>(body) == iter->second) {
                    float x = body->GetPosition().x * mRatio;
                    float y = body->GetPosition().y * mRatio;
                    pos[0] = x;
                    pos[1] = y;
                    slogd("p = %p, new x = %f, new y = %f", body, x, y);
                }
            }
        }
    }

    return pos;
}

float CollisionModel::queryAngle(int view){
    std::map<int, long>::iterator iter;
    iter = bodyMap.find(view);
    if (iter != bodyMap.end()) {
        for (b2Body *body = world->GetBodyList(); body; body = body->GetNext()) {
            if (body->GetType() == b2_dynamicBody) {
                if (reinterpret_cast<uintptr_t>(body) == iter->second) {
                    slogd("p = %p, new angle = %f", body, body->GetAngle());
                    return body->GetAngle();
                }
            }
        }
    }

    return 0;
}