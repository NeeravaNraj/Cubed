#include "inc/player.h"
#include "inc/common.h"
#include "inc/entity.h"
#include "inc/box2d/box2d.h"
#include "inc/raylib/raylib.h"
#include "inc/box2d/math_functions.h"

const float width = 16;
const float height = 32;
const float move_speed = 15000;

void player_init(Player *player, b2WorldId world_id, b2Vec2 position) {
    b2Vec2 extent = { width / 2, height / 2 };

    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.position.x = position.x + extent.x;
    body_def.position.y = position.y + extent.y;
    body_def.type = b2_dynamicBody;
    body_def.fixedRotation = true;

    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.friction = 0.5;
    shape_def.restitution = 0.1;

    b2Polygon player_polygon = b2MakeBox(extent.x, extent.y);
    b2BodyId body_id = b2CreateBody(world_id, &body_def);
    b2CreatePolygonShape(body_id, &shape_def, &player_polygon);

    Entity entity = { .body_id = body_id, .extent = extent };
    player->entity = entity;
}

void player_handle_input(Player *player) {
    Entity entity = player->entity;
    b2Vec2 p = b2Body_GetWorldPoint(entity.body_id, (b2Vec2){-entity.extent.x, -entity.extent.y});
    print_vec2(asvec2(p));
    b2Vec2 move = {0};

    if (IsKeyDown(KEY_D)) {
        move.x = move_speed;
        b2Body_ApplyForceToCenter(player->entity.body_id, move, true);
    }

    if (IsKeyDown(KEY_A)) {
        move.x = -move_speed;
        b2Body_ApplyForceToCenter(player->entity.body_id, move, true);
    }

    if (IsKeyPressed(KEY_SPACE)) {
        move.y = -100000;
        b2Body_ApplyLinearImpulseToCenter(player->entity.body_id, move, true);
    }
}

void player_render(Player *player) {
    Entity entity = player->entity;
    b2Vec2 p = b2Body_GetWorldPoint(entity.body_id, (b2Vec2){-entity.extent.x, -entity.extent.y});
    b2Vec2 size = (b2Vec2){ entity.extent.x * 2, entity.extent.y * 2 };
    b2Rot rotation = b2Body_GetRotation(entity.body_id);
    float radians = b2Rot_GetAngle(rotation);

    Rectangle rect = { 
        .x = p.x, .y = p.y,
        .width = size.x, 
        .height = size.y
    };
    Vector2 origin = {0};

    DrawRectanglePro(rect, origin, radians, RED);
}
