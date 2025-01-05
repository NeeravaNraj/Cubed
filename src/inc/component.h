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

#endif // !COMPONENT_H
