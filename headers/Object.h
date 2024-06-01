//
// Created by Cafer SELLI on 7.05.2024.
//

#ifndef SAMPLEGL_OBJECT_H
#define SAMPLEGL_OBJECT_H

class Object{
public:
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void input() = 0;
    virtual void init() = 0;
    virtual void destroy() = 0;
    virtual ~Object() = default;
};

#endif //SAMPLEGL_OBJECT_H
