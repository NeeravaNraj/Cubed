#ifndef COMPONENT_H
#define COMPONENT_H

typedef struct Component {
    const char* name;
    void* implementor;
    void* parent;

    void (*init)(struct Component*);
    void (*update)(struct Component*, float);
    void (*render)(struct Component*);
    void (*deinit)(struct Component*);
} Component;

enum ComponentNames {
    CubedSpriteRenderer = 0,
    CubedRigidBody,
    CubedBoxCollider,
};

extern const char* component_names[];
#endif // !COMPONENT_H
