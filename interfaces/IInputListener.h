//
// Created by Cafer SELLI on 11.05.2024.
//

#ifndef SAMPLEGL_IINPUTLISTENER_H
#define SAMPLEGL_IINPUTLISTENER_H

class IInputListener {
public:
    virtual void OnKeyInput(int key, int action) = 0;
};

#endif //SAMPLEGL_IINPUTLISTENER_H
