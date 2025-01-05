#include <stdio.h>
#include <stdlib.h>
#include "inc/entity.h"
#include "inc/box2d/box2d.h"
#include "inc/box2d/collision.h"
#include "inc/box2d/id.h"
#include "inc/box2d/math_functions.h"
#include "inc/common.h"

/* Entity* entity_init(b2WorldId worldId, b2Vec2 position, b2Vec2 extent) { */
/*  */
/*     b2BodyDef bodyDef = b2DefaultBodyDef(); */
/*     bodyDef.type = b2_dynamicBody; */
/*     bodyDef.position = position; */
/*  */
/*     b2BodyId bodyId = b2CreateBody(worldId, &bodyDef); */
/*     b2Circle circle; */
/*     circle.center.x += boxSize / 2; */
/*     circle.center.y += boxSize / 2; */
/*     circle.radius = boxSize / 2; */
/*     /* b2Polygon box = b2MakeBox(boxExtent.x, boxExtent.y); */
/*     b2ShapeDef shapeDef = b2DefaultShapeDef(); */
/*     shapeDef.friction = 0.05; */
/*     shapeDef.restitution = 0.0; */
/*  */
/*     b2CreateCircleShape(bodyId, &shapeDef, &circle); */
/*  */
/*     Entity* entity = malloc(sizeof(Entity)); */
/*  */
/*     if (entity == NULL) { */
/*         perror("malloc"); */
/*         exit(1); */
/*     } */
/*  */
/*     entity->body_id = bodyId; */
/*     entity->extent = extent; */
/*  */
/*     return entity; */
/* } */

/* void entity_render(Entity *entity) { */
/*     b2Vec2 p = b2Body_GetWorldPoint(entity->body_id, (b2Vec2){entity->extent.x, entity->extent.y}); */
/*     b2Vec2 size = (b2Vec2){ entity->extent.x * 2, entity->extent.y * 2 }; */
/*     b2Rot rotation = b2Body_GetRotation(entity->body_id); */
/*     float radians = b2Rot_GetAngle(rotation); */
/*  */
/*     Rectangle rect = {  */
/*         .x = p.x, .y = p.y, */
/*         .width = size.x,  */
/*         .height = size.y */
/*     }; */
/*     Vector2 origin = {0}; */
/*  */
/*     DrawCircleV(asvec2(p), boxSize / 2, RED); */
/* } */

/* void entity_destructor(void* entity) { */
/*     entity_deinit(*((void**)entity)); */
/* } */

/* void entity_deinit(Entity *entity) { */
/*     Entity* e = (Entity*)entity; */
/*     b2DestroyBody(e->body_id); */
/*     e->body_id = b2_nullBodyId; */
/*  */
/*     free(e); */
/* } */
