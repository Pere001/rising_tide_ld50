
#include "game.h"

struct day_and_daytime{
    s32 day;
    f32 daytime;
};
// In theory supports negative time.
inline day_and_daytime TimeToDayAndDaytime(s32 time){
    day_and_daytime result;
    s32 timeFromStartOfDay0 = (s32)(DAY_LENGTH*INITIAL_DAYTIME) + time;
    result.day = timeFromStartOfDay0 / DAY_LENGTH;
    if (timeFromStartOfDay0 < 0)
        result.day -= 1;
    
    result.daytime = (f32)(timeFromStartOfDay0 % DAY_LENGTH) / (f32)DAY_LENGTH;
    if (result.daytime < 0)
        result.daytime += 1.f;
    Assert(result.daytime >= 0.f);
    return result;
}
inline s32 DayAndDaytimeToTime(day_and_daytime dad){
    s32 result = ((f32)dad.day + dad.daytime)*DAY_LENGTH - (s32)(DAY_LENGTH*INITIAL_DAYTIME);
    return result;
}
inline b32 DayAndDaytimeIsNight(day_and_daytime dad){
    b32 result = dad.daytime < NIGHT_END || dad.daytime >= NIGHT_START;
    return result;
}
inline b32 TimeIsNight(s32 time){
    auto dad = TimeToDayAndDaytime(time);
    b32 result = DayAndDaytimeIsNight(dad);
    return result;
}
u16 TimeGetSeaLevel(s32 time){
    s32 unclampedResult = INITIAL_SEA_LEVEL + (MAX_SEA_LEVEL - INITIAL_SEA_LEVEL)*(time/(f32)MAX_TIME);
    u16 result = (u16)Clamp(unclampedResult, 0, 65535);
    return result;
}
s32 SeaLevelGetTime(s32 seaLevel){
    s32 result = ((f32)MAX_TIME)*(seaLevel - INITIAL_SEA_LEVEL)/(f32)(MAX_SEA_LEVEL - INITIAL_SEA_LEVEL);
    return result;
}

struct days_hours_minutes{
    s32 days;
    s32 hours;
    s32 minutes;
};
days_hours_minutes TimeInFramesToDaysHoursMinutes(s32 timeInFrames){
    f32 days = timeInFrames/(f32)DAY_LENGTH;
    days_hours_minutes result;
    result.days = (s32)days;
    f32 hours = Frac(days)*24.f;
    result.hours = (s32)hours;
    f32 minutes = Frac(hours)*60.f;
    result.minutes = (s32)Round(minutes);
    return result;
}
days_hours_minutes TimeInSecondsToDaysHoursMinutes(f32 timeInSeconds){
    days_hours_minutes result = TimeInFramesToDaysHoursMinutes((s32)Round(timeInSeconds*60.f));
    return result;
}
s32 TimeInDaysHoursMinutesToFrames(days_hours_minutes timeInDHM){
    s32 result = ((timeInDHM.days*24 + timeInDHM.hours)*60 + timeInDHM.minutes)*60*60;
    return result;
}

// Converts from time in frames since game start (as in gg->time) to time in frames since
// start of day 0.
s32 GameFrameTimeToFrameTimeSinceDay0(s32 time){
    s32 result = (s32)Round(INITIAL_DAYTIME*DAY_LENGTH) + time;
    return result;
}

void AddTileWithBehaviour(tile *t){
    auto gg = &globalGame;
    Assert(gg->numTilesWithBehaviour < MAX_TILES_WITH_BEHAVIOUR);

    gg->tilesWithBehaviour[gg->numTilesWithBehaviour] = t;
    gg->numTilesWithBehaviour++;
}
void RemoveTileWithBehaviour(tile *t){
    auto gg = &globalGame;
    for(s32 i = 0; i < gg->numTilesWithBehaviour; i++){
        if (gg->tilesWithBehaviour[i] == t){
            if (i != gg->numTilesWithBehaviour - 1){
                gg->tilesWithBehaviour[i] = gg->tilesWithBehaviour[gg->numTilesWithBehaviour - 1];
            }
            gg->numTilesWithBehaviour--;
            break;
        }
    }
}

v2s TileGetGridPos(tile *t){
    s32 tileIndex = (s32)(((size_t)t - (size_t)globalGame.tiles)/sizeof(tile));
    AssertRange(0, tileIndex, TILES_X*TILES_Y -1);

    v2s result = {tileIndex % TILES_X, tileIndex / TILES_X};
    return result;
}


enum tooltip_preferred_pos{
    TooltipPos_UpRight,
    TooltipPos_UpLeft,
    TooltipPos_DownRight,
    TooltipPos_DownLeft,
};
v2 PlaceTooltip(v2 pos, v2 dim, tooltip_preferred_pos preferredPos = TooltipPos_UpRight, v2 borderMargin = V2(10.f)){
    v2 winDim = globalInput.windowDim;
    v2 finalPos = pos;
    if (preferredPos == TooltipPos_UpLeft || preferredPos == TooltipPos_DownLeft){
        finalPos.x -= dim.x;
    }
    if (preferredPos == TooltipPos_DownLeft || preferredPos == TooltipPos_DownRight){
        finalPos.y -= dim.y;
    }

    // Handle out of window.
    if (finalPos.x + dim.x + borderMargin.x > winDim.x){
        finalPos.x = winDim.x - borderMargin.x - dim.x;
    }
    if (finalPos.y + dim.y + borderMargin.y > winDim.y){
        finalPos.y = winDim.y - borderMargin.y - dim.y;
    }
    if (finalPos.x - borderMargin.x < 0){
        finalPos.x = borderMargin.x;
    }
    if (finalPos.y - borderMargin.y < 0){
        finalPos.y = borderMargin.y;
    }
    return finalPos;
}

f32 StringWidth(char *str){
    s32 maxLineLength = 0;
    s32 lineLength = 0;
    for(char *it = str; *it; it++){
        char chr = *it;
        if (chr >= 32 && chr <= 126){
            lineLength++;           
        }else if (chr == '\n'){
            maxLineLength = Max(maxLineLength, lineLength);
            lineLength = 0;
        }
    }
    maxLineLength = Max(maxLineLength, lineLength);
    return maxLineLength * (20.f - 2.f); // that's the xadvance
}

f32 StringHeight(char *str){
    s32 numLines = 1;
    for(char *it = str; *it; it++){
        char chr = *it;
        if (chr == '\n'){
            numLines++;
        }
    }
    return numLines * 32.f; // that's the xadvance
}
v2 StringDim(char *str){
    v2 result = V2(StringWidth(str), StringHeight(str));
    return result;
}
v2 CenterStringX(char *str, v2 centerPos, f32 scaleX = 1.f){
    v2 result = centerPos - V2(StringWidth(str)*scaleX/2.f, 0);
    return result;
}
v2 CenterStringXY(char *str, v2 centerPos, v2 scale = V2(1.f)){
    f32 w = StringWidth(str);
    f32 h = StringHeight(str);
    v2 result = centerPos + V2(-w*scale.x/2, (-25.f + h/2)*scale.y);
    return result;
}
v2 CenterStringY(char *str, v2 centerPos, f32 scaleY = 1.f){
    f32 h = StringHeight(str);
    v2 result = centerPos + V2(0, (-25.f + h/2)*scaleY);
    return result;
}

v2 AlignStringRight(char *str, v2 centerPos, f32 scaleX = 1.f){
    v2 result = centerPos - V2(StringWidth(str)*scaleX, 0);
    return result;
}

inline s32 WindowYGetZ(f32 y){
    f32 marginBelowWindow = 200.f;
    f32 marginAboveWindow = 100.f;
    f32 effectiveHeight = globalInput.windowDim.y + marginBelowWindow + marginAboveWindow;
    f32 normalizedY = Clamp01((y + marginBelowWindow) / effectiveHeight);
    s32 z = Lerp(ENTITY_Z_MAX, ENTITY_Z_MIN, normalizedY);
    return z;
}

void DrawTextTooltip(char *text, v2 pos, s32 z,
                     tooltip_preferred_pos preferredPos = TooltipPos_UpRight,
                     v2 borderMargin = V2(10.f), v2 textMargin = V2(10.f)){
    v2 textScale = V2(1.0f);
    v2 textDim = Hadamard(StringDim(text), textScale);
    v2 outlineMargin = V2(3.f);
    v2 tooltipDim = textDim + textMargin*2 + outlineMargin*2;
    v2 tooltipPos = PlaceTooltip(pos, tooltipDim, preferredPos);

    v4 bgCol = V4_Grey(.7f, .7f);
    v4 textCol = V4_Black();
    v4 outlineCol = V4_Black();
    DrawRect(tooltipPos + outlineMargin, tooltipDim - outlineMargin*2, bgCol, z);
    DrawRectFrame(tooltipPos + outlineMargin, tooltipDim - outlineMargin*2, outlineMargin, outlineCol, z);
    DrawString(text, tooltipPos + V2(outlineMargin.x + textMargin.x, tooltipDim.y - outlineMargin.y - textMargin.y - 25.f),
               textScale, z, textCol);
}


void DrawItem(v2 centerPos, item_id id, s32 count, s32 z = 50, v4 tintColor = V4_White()){
    auto image = &globalGame.images[TEX_ITEMS];
    v2 texPos = { (f32)((s32)id % 10)*48, image->dim.y - (1 + ((s32)id / 10))*48.f};
    DrawSpriteRect(image, texPos, V2(48), centerPos - V2(48/2), V2(1.f), z, tintColor);

    if (count > 1){
        char numStr[32];
        sprintf_s(numStr, ArrayCount(numStr), "%i", count);
        v2 strScale = V2(.75f);
        DrawString(numStr, AlignStringRight(numStr, centerPos + V2(48/2, -48/2 + 1), strScale.x), strScale, z, V4_Grey(.9f), 0, true, 1.f);
    }
}

inline tile *WorldPosGetTile(v2 pos){
    s32 tileX = Clamp(pos.x / TILE_SIZE, 0, TILES_X - 1);
    s32 tileY = Clamp(pos.y / TILE_SIZE, 0, TILES_Y - 1);
    tile *result = &globalGame.tiles[TILES_X*tileY + tileX];
    return result;
}
inline v2s WorldPosGetTilePos(v2 pos){
    v2s tilePos = {Clamp(pos.x / TILE_SIZE, 0, TILES_X - 1),
                   Clamp(pos.y / TILE_SIZE, 0, TILES_Y - 1)};
    return tilePos;
}

//
// Light Map
//
inline v4 LightMapColor(v2s tilePos){
    auto gg = &globalGame;
    v2s lightMapPos = V2S(Clamp(tilePos.x - gg->lightMapMinTile.x, 0, gg->lightMapSize.x - 1),
                          Clamp(tilePos.y - gg->lightMapMinTile.y, 0, gg->lightMapSize.y - 1));
    v4 color = gg->lightMap[gg->lightMapSize.x*lightMapPos.y + lightMapPos.x];
    return color;
}
inline v4 LightMapColor(v2 worldPos){
    v2s tilePos = WorldPosGetTilePos(worldPos);
    return LightMapColor(tilePos);
}
inline v4 LightMapColor(tile *t){
    s32 tileIndex = ((size_t)t - (size_t)globalGame.tiles) / sizeof(tile *);
    v2s tilePos = {tileIndex % TILES_X, tileIndex / TILES_X};
    AssertRange(0, tilePos.x, TILES_X - 1);
    AssertRange(0, tilePos.y, TILES_Y - 1);
    return LightMapColor(tilePos);
}


//
// Entities
//

// Only for mobs
b32 CircleEntityCollision(entity *e, v2 c, f32 r){
    f32 entityYOffset = 32.f;
    f32 entityRadius  = 32.f;
    if (e->type == Entity_Goose){
        entityYOffset = 42.f;
        entityRadius  = 42.f;
    }else if (e->type == Entity_Rabbit){
        entityYOffset = 32.f;
        entityRadius  = 30.f;
    }else if (e->type == Entity_Boar){
        entityYOffset = 40.f;
        entityRadius  = 48.f;
    }

    f32 distanceSqr = LengthSqr(e->pos + V2(0, entityYOffset) - c);
    f32 maxDistance = r + entityRadius;
    f32 maxDistanceSqr = maxDistance*maxDistance;
    b32 result = (distanceSqr <= maxDistanceSqr);
    return result;
}

b32 PlayerEntityCollision(entity *e, f32 extraRadius = 0){
    v2 c = globalGame.playerPos + V2(0, 16.f);
    f32 r = 46.f + extraRadius;
    b32 result = CircleEntityCollision(e, c, r);
    return result;
}

entity *CreateEntity(entity_type type){
    if (globalGame.numEntities >= MAX_ENTITIES)
        return 0;

    entity *e = &globalGame.entities[globalGame.numEntities];
    memset(e, 0, sizeof(entity));

    e->type = type;
    e->id = ++globalGame.lastUsedEntityId;

    globalGame.numEntities++;
    return e;
}
void DestroyEntity(entity *e){
    Assert(e > globalGame.entities && e < globalGame.entities + globalGame.numEntities);
    
    memcpy(e, &globalGame.entities[globalGame.numEntities - 1], sizeof(entity));
    globalGame.numEntities--;
}


void HurtPlayer(s32 damage, f32 hitDirection = 0.f, f32 knockbackSpeed = 0.f,
                b32 bypassArmor = false){
    auto gg = &globalGame;

    s32 finalDamage = damage;
    if (bypassArmor){
        f32 armor = 0.f;
        for (s32 i = 0; i < 3; i++){
            if (gg->inventoryArmor[i].id)
                armor += .2f;
        }
        finalDamage = (s32)Ceil(damage*Clamp01(1.f - armor));
    }

    s32 numParticles = RandomRangeS32(8, 12);
    b32 died = false;
    if (gg->playerHealth != 0){
        gg->playerHealth = MaxS32(0, gg->playerHealth - finalDamage);
        if (gg->debug.godMode){ // Immortal cheat.
            gg->playerHealth = MaxS32(1, gg->playerHealth);
        }
        if (gg->playerHealth == 0){
            gg->timeOfDeath = gg->time;
            numParticles = RandomRangeS32(100, 110);
            died = true;
        }
    }

    // Blood particles
    for(s32 i = 0; i < numParticles; i++){
        entity *e = CreateEntity(Entity_TexParticle);
        auto t = &e->data.texParticle;

        e->speed = V2LengthDir(RandomRange(1.f, 8.f), Random(2*PI));
        if (died)
            e->speed *= 2.f;
        e->pos = gg->playerPos;

        t->imageIndex = TEX_EFFECTS;
        t->texPos = V2(64.f, 0.f);
        t->texDim = V2(16.f);
        t->scale = V2(RandomRange(.4f, 1.f));
        t->z = 0;

        t->h = RandomRange(50.f, 110.f); // height from ground.
        t->hSpeed = RandomRange(-5.f, 10.f);
        t->hGravity = 1.f;

        t->speedMultiplier = RandomRange(.94f, .98f);
        t->maxLifetime = RandomRangeS32(30, 70);
        if (died)
            t->maxLifetime *= 10;
        t->creationTime = gg->time;

        t->color = V4(.95f, .1f, .1f, RandomRange(.8f, 1.f));
    }
    
    // Knockback
    gg->knockbackTimerMax = 30;
    gg->knockbackTimer = gg->knockbackTimerMax;
    gg->playerSpeed = V2LengthDir(knockbackSpeed, hitDirection);
}

b32 HasItem(inventory_item item){
    s32 itemCount = 0;
    for(s32 i = 0; i < ArrayCount(globalGame.inventoryAll); i++){
        if (globalGame.inventoryAll[i].id == item.id){
            itemCount += globalGame.inventoryAll[i].numItems;
        }
    }
    if (globalGame.holdingItem.id == item.id){
        itemCount += globalGame.holdingItem.numItems;
    }
    b32 result = (itemCount >= item.numItems);
    return result;
}

void RemoveItem(inventory_item item){
    s32 toRemove = item.numItems;
    for(s32 i = 0; i < ArrayCount(globalGame.inventoryAll); i++){
        if (globalGame.inventoryAll[i].id == item.id){
            s32 temp = Max(0, globalGame.inventoryAll[i].numItems - toRemove);
            toRemove = Max(0, toRemove - globalGame.inventoryAll[i].numItems);
            globalGame.inventoryAll[i].numItems = temp;
            if (globalGame.inventoryAll[i].numItems == 0)
                globalGame.inventoryAll[i].id = (item_id)0;
            if (!toRemove)
                break;
        }
    }
    Assert(toRemove == 0);
}
// returns the amount of items that weren't taken.
s32 TakeItem(inventory_item item){
    b32 stackable = !(globalItemProperties[(s32)item.id].flags & ItemFlag_Unstackable);

    for(s32 i = 0; i < ArrayCount(globalGame.inventoryAll); i++){
        if (globalGame.inventoryAll[i].id == item.id && stackable){
            globalGame.inventoryAll[i].numItems += item.numItems;
            return 0;
        }
    }
    for(s32 i = 0; i < ArrayCount(globalGame.inventoryAll); i++){
        if (!globalGame.inventoryAll[i].id){
            globalGame.inventoryAll[i] = item;
            return 0;
        }
    }
    return item.numItems;
}

void DropItem(inventory_item item, f32 speedDir = Random(2*PI), f32 speedLength = 14.f){
    entity *e = CreateEntity(Entity_Item);
    e->pos = globalGame.playerPos;
    e->speed = V2LengthDir(speedLength, speedDir);
    e->data.item.itemId       = item.id;
    e->data.item.numItems     = item.numItems;
    e->data.item.creationTime = globalGame.time;
}
entity *CreateItem(inventory_item item, v2 pos, v2 speed = V2(0)){
    entity *e = CreateEntity(Entity_Item);
    e->pos = pos;
    e->speed = speed;
    e->data.item.itemId       = item.id;
    e->data.item.numItems     = item.numItems;
    e->data.item.creationTime = globalGame.time;
    return e;
}
entity *CreateItem(item_id itemId, v2 pos, v2 speed = V2(0)){
    inventory_item item = {itemId, 1};
    return CreateItem(item, pos, speed);
}

void TakeItemOrDrop(inventory_item item){
    s32 excess = TakeItem(item);
    if (excess){
        item.numItems = excess;
        DropItem(item, Random(2*PI));
    }
}

// 'z': pass 0 for automatic (based on window y).
void CreateTexParticles(s32 imageIndex, v2 texRegionMin, v2 texRegionMax, v2 pos, s32 num, s32 z,
                        f32 scaleMin = .8f, f32 scaleMax = 1.1f, s32 maxLifetime = 110,
                        f32 texSizeMin = .2f, f32 texSizeMax = .5f){
    for(s32 i = 0; i < num; i++){
        auto e = CreateEntity(Entity_TexParticle);
        auto t = &e->data.texParticle;
        e->pos = pos;
        e->speed = V2LengthDir(RandomRange(2.f, 9.f), Random(2*PI));

        t->imageIndex = imageIndex;
        t->texDim.x = Round(RandomRange(texSizeMin, texSizeMax)*(texRegionMax.x - texRegionMin.x));
        t->texDim.y = Round(RandomRange(texSizeMin, texSizeMax)*(texRegionMax.y - texRegionMin.y));
        t->texPos.x = Round(RandomRange(texRegionMin.x, Max(texRegionMin.x, texRegionMax.x - t->texDim.x)));
        t->texPos.y = Round(RandomRange(texRegionMin.y, Max(texRegionMin.y, texRegionMax.y - t->texDim.y)));
        t->scale = V2(RandomRange(scaleMin, scaleMax), RandomRange(scaleMin, scaleMax));
        t->z = z;
        t->speedMultiplier = .95f;
        t->maxLifetime = maxLifetime;
        t->creationTime = globalGame.time;
        t->color = V4_White();
    }
}

//
// ENTITIES
//
#define ENTITY_STATE_IDLE 0
#define ENTITY_STATE_HIT 1 // With ENTITY_STATE_HIT stateTimer goes down instead of up
#define ENTITY_STATE_DUCK 2
#define ENTITY_STATE_AIR 3
#define ENTITY_STATE_LANDING 4
#define ENTITY_STATE_WALK 5
#define ENTITY_STATE_CHASE 6
#define ENTITY_STATE_ATTACK 7
#define ENTITY_STATE_PRE_ATTACK 8

void HurtEntity(entity *e, s32 damage, f32 hitDirection, f32 knockback = 14.f, s32 knockbackTime = 10){
    auto gg = &globalGame;
    if (e->health == 0)
        return;

    b32 died = false;
    s32 numParticles = 3 + 3*damage;

    e->health = MaxS32(0, e->health - damage);
    switch(e->type){
    case Entity_Rabbit: 
    {
        if (e->health == 0){
            died = true;
            e->deleted = true;
            for(s32 i = RandomRangeS32(1, 1); i > 0; i--){
                CreateItem(Item_RawWhiteMeat, e->pos, V2LengthDir(RandomRange(8.f, 12.f), hitDirection + RandomRange(-PI*.3f, PI*.3f)));
            }
            
            // Create Particles
            v2 texDim = V2(64.f, 112.f);
            v2 texPos = V2(0, gg->images[TEX_RABBIT].dim.y - texDim.y);
            CreateTexParticles(2, texPos, texPos + texDim, e->pos, 4, 55, .8f, 1.f);
            
        }else{
            e->state = ENTITY_STATE_HIT;
            e->stateTimer = knockbackTime;
            e->speed = V2LengthDir(knockback, hitDirection);
        }
    } break;
    
    case Entity_Goose: 
    {
        if (e->health == 0){
            died = true;
            e->deleted = true;
            for(s32 i = RandomRangeS32(1, 2); i > 0; i--){
                CreateItem(Item_RawWhiteMeat, e->pos, V2LengthDir(RandomRange(8.f, 12.f), hitDirection + RandomRange(-PI*.3f, PI*.3f)));
            }
            for(s32 i = RandomRangeS32(2, 3); i > 0; i--){
                CreateItem(Item_Feather, e->pos, V2LengthDir(RandomRange(8.f, 12.f), hitDirection + RandomRange(-PI*.3f, PI*.3f)));
            }
            
            // Create Particles
            v2 texDim = V2(160.f, 160.f);
            v2 texPos = V2(0, gg->images[TEX_GOOSE].dim.y - texDim.y);
            CreateTexParticles(5, texPos, texPos + texDim, e->pos, 4, 55, .8f, 1.f);

        }else{
            e->state = ENTITY_STATE_HIT;
            e->stateTimer = knockbackTime;
            e->speed = V2LengthDir(knockback, hitDirection);
        }
    } break;
    
    case Entity_Boar: 
    {
        if (e->health == 0){
            died = true;
            e->deleted = true;
            for(s32 i = RandomRangeS32(1, 2); i > 0; i--){
                CreateItem(Item_RawRedMeat, e->pos, V2LengthDir(RandomRange(8.f, 12.f), hitDirection + RandomRange(-PI*.3f, PI*.3f)));
            }
            for(s32 i = RandomRangeS32(1, 3); i > 0; i--){
                CreateItem(Item_Leather, e->pos, V2LengthDir(RandomRange(8.f, 12.f), hitDirection + RandomRange(-PI*.3f, PI*.3f)));
            }
            
            // Create Particles
            v2 texDim = V2(160.f, 160.f);
            v2 texPos = V2(0, gg->images[TEX_BOAR].dim.y - texDim.y);
            CreateTexParticles(7, texPos, texPos + texDim, e->pos, 4, 55, .8f, 1.f);

        }else{
            e->state = ENTITY_STATE_HIT;
            e->stateTimer = knockbackTime;
            e->speed = V2LengthDir(knockback, hitDirection);
        }
    } break;
    }

    
    // Blood particles
    if (died)
        numParticles *= 3;

    for(s32 i = 0; i < numParticles; i++){
        entity *ep = CreateEntity(Entity_TexParticle);
        auto t = &ep->data.texParticle;

        ep->speed = V2LengthDir(RandomRange(2.f, 8.f), Random(2*PI));
        ep->pos = e->pos;

        t->imageIndex = TEX_EFFECTS;
        t->texPos = V2((RandomChance(.5f) ? 12.f : 14.f)*16.f, 0.f);
        t->texDim = V2(16.f);
        t->scale = V2(RandomRange(.7f, 1.f));
        t->z = 0;

        t->h = RandomRange(20.f, 60.f); // height from ground.
        t->hSpeed = RandomRange(-5.f, 8.f);
        t->hGravity = 1.f;

        t->speedMultiplier = RandomRange(.94f, .98f);
        t->maxLifetime = RandomRangeS32(20, 50);
        t->creationTime = gg->time;

        t->color = V4(.6f, .1f, .1f, RandomRange(.8f, 1.f));
    }
}

void UpdateEntity(entity *e){
    if (e->deleted)
        return;
    auto gg = &globalGame;

    switch(e->type){
    case Entity_Rabbit:
    {
        auto t = &e->data.rabbit;
        e->stateTimer++;
        
        tile *occupiedTile = WorldPosGetTile(e->pos);

        if (e->state == ENTITY_STATE_IDLE){
            e->speed = V2(0);
            if (e->stateTimer > 10 && RandomChance(.032f)){
                e->state = ENTITY_STATE_DUCK;
                e->stateTimer = 0;
            }
        }else if (e->state == ENTITY_STATE_DUCK){
            e->speed = V2(0);
            if (e->stateTimer > 12){
                e->state = ENTITY_STATE_AIR;
                f32 moveDir = RandomRange(0, 2*PI);
                f32 moveLen = 3.f;
                if (occupiedTile->h <= gg->seaLevel){
                    //moveDir = AngleOf((V2(TILES_X, TILES_Y)*TILE_SIZE) - e->pos);
                    //moveLen = 4.f;
                }
                
                // At night run away from fire.
                if (TimeIsNight(gg->time)){
                    v4 lightCol = LightMapColor(e->pos + V2LengthDir(TILE_SIZE*2.f, moveDir));
                    if (lightCol.r > .5f){
                        moveDir *= -1.f;
                    }
                }
                e->speed = V2LengthDir(moveLen, moveDir);
                e->stateTimer = 0;
            }
        }else if (e->state == ENTITY_STATE_AIR){
            if (e->stateTimer > 20){
                e->state = ENTITY_STATE_LANDING;
                e->speed *= .1f;
                e->stateTimer = 0;
            }
        }else if (e->state == ENTITY_STATE_LANDING){
            if (e->stateTimer > 4){
                e->state = ENTITY_STATE_IDLE;
                e->stateTimer = 0;
            }
        }else if (e->state == ENTITY_STATE_HIT){
            e->stateTimer -= 2; // With ENTITY_STATE_HIT stateTimer goes down instead of up, so we have to add 2 to compensate for the normal increment.
            f32 speedLen = Length(e->speed);
            f32 speedDir = AngleOf(e->speed);
            speedLen = Max(0, speedLen - 3.f);
            e->speed = V2LengthDir(speedLen, speedDir);
            if (e->stateTimer <= 0){
                e->speed = V2(0);
                e->state = ENTITY_STATE_DUCK;
                e->stateTimer = 9;
            }
        }

        e->pos += e->speed;
        
        // Face left
        if (Abs(e->speed.x) > 0.1f)
            e->facingLeft = e->speed.x < 0;
        
        // Despawn
        if ((s32)occupiedTile->h < (s32)gg->seaLevel - 5000){
            e->deleted = true;
        }

    } break;
    
    case Entity_Goose:
    {
        auto t = &e->data.goose;
        e->stateTimer++;
        
        tile *occupiedTile = WorldPosGetTile(e->pos);


        // At noone goose can lay egg
        if (((gg->time + (s32)(DAY_LENGTH*INITIAL_DAYTIME)) % DAY_LENGTH) == (s32)(DAY_LENGTH/2)){
            if (RandomChance(.5f) && occupiedTile->h > gg->seaLevel){
                CreateItem(Item_Egg, e->pos);
            }
        }

        f32 playerDistanceSqr = LengthSqr(gg->playerPos - e->pos);
        f32 attackRange = 160.f;
        f32 stopChaseRange = 500.f;
        f32 goTowardsEggRange = TILE_SIZE*30.f;

        if (e->state == ENTITY_STATE_IDLE){
            e->speed = V2(0);
            if (e->stateTimer > 10 && RandomChance(.032f)){
                e->state = ENTITY_STATE_WALK;
                f32 moveDir = Random(2*PI);
                f32 moveLen = 3.f;
                if (RandomChance(.3f)){
                    // At night run away from fire.
                    b32 movingAwayFromFire = false;
                    if (TimeIsNight(gg->time) && RandomChance(.8f)){
                        v4 lightCol = LightMapColor(e->pos + V2LengthDir(TILE_SIZE*2.f, moveDir));
                        if (lightCol.r > .5f){
                            moveDir *= -1.f;
                            movingAwayFromFire = true;
                        }
                    }
                    // Go towards egg
                    if (!movingAwayFromFire){
                        f32 bestDistanceSqr = goTowardsEggRange*goTowardsEggRange;
                        for(s32 i = 0; i < gg->numEntities; i++){
                            entity *entityIt = &gg->entities[i];
                            if (!entityIt->deleted && entityIt->type == Entity_Item && entityIt->data.item.itemId == Item_Egg){
                                f32 distanceSqr = LengthSqr(entityIt->pos - e->pos);
                                if (distanceSqr < bestDistanceSqr){
                                    bestDistanceSqr = distanceSqr;
                                    moveDir = AngleOf(entityIt->pos - e->pos) + RandomRange(-.1f*PI, .1f*PI);
                                }
                            }
                        }
                    }
                    
                }
                if (occupiedTile->h <= gg->seaLevel){
                }
                e->speed = V2LengthDir(moveLen, moveDir);
                e->stateTimer = 0;
            }
            if (playerDistanceSqr < attackRange*attackRange && t->attackCooldown == 0){
                e->state = ENTITY_STATE_PRE_ATTACK;
                e->speed = V2(0);
                e->stateTimer = 0;
                e->facingLeft = (gg->playerPos.x < e->pos.x);
            }
        }else if (e->state == ENTITY_STATE_WALK){
            if (e->stateTimer > 15 && playerDistanceSqr < attackRange*attackRange){
                e->state = ENTITY_STATE_PRE_ATTACK;
                e->speed = V2(0);
                e->stateTimer = 0;
                e->facingLeft = (gg->playerPos.x < e->pos.x);
            }else if (e->stateTimer > 30 + RandomChance(0.012f)){
                e->state = ENTITY_STATE_IDLE;
                e->speed = V2(0);
                e->stateTimer = 0;
            }
        }else if (e->state == ENTITY_STATE_PRE_ATTACK){
            if (e->stateTimer > 20){
                e->state = ENTITY_STATE_ATTACK;
                e->stateTimer = 0;
                e->cantHurtPlayer = false;
            }
        }else if (e->state == ENTITY_STATE_ATTACK){
            f32 hitRange = 120.f;
            if (e->stateTimer > 10){
                e->state = ENTITY_STATE_IDLE;
                e->stateTimer = 0;
            }else if (!e->cantHurtPlayer && PlayerEntityCollision(e, 40.f)){
                HurtPlayer(10, AngleOf(gg->playerPos - e->pos), 20.f);
                t->lastHurtPlayerTime = gg->time;
                e->cantHurtPlayer = true;
            }
            t->attackCooldown = 30;

        }else if (e->state == ENTITY_STATE_CHASE){
            f32 speedLen = Length(e->speed);
            f32 speedDir = AngleOf(e->speed);
            f32 playerDir = AngleOf(gg->playerPos - e->pos);
            if (e->speed.x == 0 && e->speed.y == 0)
                speedDir = playerDir;
            speedLen = Min(10.f, speedLen + .3f);
            speedDir = MoveAngleTowards(speedDir, playerDir, PI*.025f);
            e->speed = V2LengthDir(speedLen, speedDir);
            f32 hitRange = 120.f;

            if (e->stateTimer > (t->angryCooldown ? 5*60 : 2*60)){ //|| playerDistanceSqr > stopChaseRange*stopChaseRange){
                e->state = ENTITY_STATE_IDLE;
                e->speed = V2(0);
                e->stateTimer = 0;
            }else if ((gg->time - t->lastHurtPlayerTime) > 30 && PlayerEntityCollision(e)){
                HurtPlayer(10, AngleOf(gg->playerPos - e->pos), 20.f);
                t->lastHurtPlayerTime = gg->time;
            }

        }else if (e->state == ENTITY_STATE_HIT){
            e->stateTimer -= 2; // With ENTITY_STATE_HIT stateTimer goes down instead of up, so we have to add 2 to compensate for the normal increment.
            f32 speedLen = Length(e->speed);
            f32 speedDir = AngleOf(e->speed);
            speedLen = Max(0, speedLen - 3.f);
            e->speed = V2LengthDir(speedLen, speedDir);
            if (e->stateTimer <= 0){
                e->state = ENTITY_STATE_CHASE;
                f32 playerDir = AngleOf(gg->playerPos - e->pos);
                e->speed = V2LengthDir(3.f, playerDir);
                e->stateTimer = 0;
            }
        }

        e->pos += e->speed;
        if (t->attackCooldown)
            t->attackCooldown--;

        if (t->angryCooldown)
            t->angryCooldown--;

        // Face left
        if (Abs(e->speed.x) > 0.1f)
            e->facingLeft = e->speed.x < 0;

        // Despawn
        if ((s32)occupiedTile->h < (s32)gg->seaLevel - 5000 && playerDistanceSqr > Square(TILE_SIZE*200)){
            e->deleted = true;
        }

    } break;

    case Entity_Boar:
    {
        auto t = &e->data.boar;
        e->stateTimer++;
        
        tile *occupiedTile = WorldPosGetTile(e->pos);

        f32 playerDistanceSqr = LengthSqr(gg->playerPos - e->pos);
        f32 attackRange = 80.f;
        f32 stopChaseRange = 500.f;
        if (e->state == ENTITY_STATE_IDLE){
            e->speed = V2(0);
            if (playerDistanceSqr < attackRange*attackRange && t->attackCooldown == 0){
                e->state = ENTITY_STATE_ATTACK;
                e->stateTimer = 0;
                e->cantHurtPlayer = false;
            }else if (e->stateTimer > 10 && RandomChance(.032f)){
                e->state = ENTITY_STATE_WALK;
                f32 moveDir = RandomRange(0, 2*PI);
                f32 moveLen = 3.f;
                //if (occupiedTile->h <= gg->seaLevel){
                    //moveDir = AngleOf((V2(TILES_X, TILES_Y)*TILE_SIZE) - e->pos);
                    //moveLen = 4.f;
                //}
                if (TimeIsNight(gg->time)){
                    // Go towards player
                    if (RandomChance(.7f)){
                        f32 approachPlayerRange = 30.f*TILE_SIZE;
                        if (playerDistanceSqr < Square(approachPlayerRange)){
                            v4 playerLightCol = LightMapColor(gg->playerPos);
                            if (playerLightCol.r < .4f){ // player pos isn't too bright (no fire)
                                moveDir = AngleOf(gg->playerPos - e->pos);
                                moveLen = 5.f;
                            }
                        }
                    }
                    // Go away from (fire) light.
                    v4 boarLightCol = LightMapColor(e->pos + V2LengthDir(TILE_SIZE*2.f, moveDir));
                    if (boarLightCol.r > .5f){
                        moveDir *= -1.f;
                    }
                }
                e->speed = V2LengthDir(moveLen, moveDir);
                e->stateTimer = 0;
            }
        }else if (e->state == ENTITY_STATE_WALK){
            if (e->stateTimer > 15 && playerDistanceSqr < attackRange*attackRange){
                e->state = ENTITY_STATE_ATTACK;
                e->speed = V2(0);
                e->stateTimer = 0;
                e->cantHurtPlayer = false;
            }else if (e->stateTimer > 30 + RandomChance(0.012f)){
                e->state = ENTITY_STATE_IDLE;
                e->speed = V2(0);
                e->stateTimer = 0;
            }
        }else if (e->state == ENTITY_STATE_ATTACK){
            f32 hitRange = 120.f;
            if (e->stateTimer > 10){
                e->state = ENTITY_STATE_IDLE;
                e->stateTimer = 0;
            }else if (!e->cantHurtPlayer && PlayerEntityCollision(e)){
                HurtPlayer(20, AngleOf(gg->playerPos - e->pos), 20.f);
                t->lastHurtPlayerTime = gg->time;
                e->cantHurtPlayer = true;
            }
            t->attackCooldown = 30;

        }else if (e->state == ENTITY_STATE_CHASE){
            f32 speedLen = Length(e->speed);
            f32 speedDir = AngleOf(e->speed);
            f32 playerDir = AngleOf(gg->playerPos - e->pos);
            if (e->speed.x == 0 && e->speed.y == 0)
                speedDir = playerDir;
            speedLen = Min(10.f, speedLen + .3f);
            speedDir = MoveAngleTowards(speedDir, playerDir, PI*.025f);
            e->speed = V2LengthDir(speedLen, speedDir);
            f32 hitRange = 120.f;

            if (e->stateTimer > 2*60 || playerDistanceSqr > stopChaseRange*stopChaseRange){
                e->state = ENTITY_STATE_IDLE;
                e->speed = V2(0);
                e->stateTimer = 0;
            }else if ((gg->time - t->lastHurtPlayerTime) > 30 && PlayerEntityCollision(e)){
                HurtPlayer(20, AngleOf(gg->playerPos - e->pos), 20.f);
                t->lastHurtPlayerTime = gg->time;
            }

        }else if (e->state == ENTITY_STATE_HIT){
            e->stateTimer -= 2; // With ENTITY_STATE_HIT stateTimer goes down instead of up, so we have to add 2 to compensate for the normal increment.
            f32 speedLen = Length(e->speed);
            f32 speedDir = AngleOf(e->speed);
            speedLen = Max(0, speedLen - 3.f);
            e->speed = V2LengthDir(speedLen, speedDir);
            if (e->stateTimer <= 0){
                e->state = ENTITY_STATE_CHASE;
                f32 playerDir = AngleOf(gg->playerPos - e->pos);
                e->speed = V2LengthDir(3.f, playerDir);
                e->stateTimer = 0;
            }
        }

        e->pos += e->speed;
        if (t->attackCooldown)
            t->attackCooldown--;

        // Face left
        if (Abs(e->speed.x) > 0.1f)
            e->facingLeft = e->speed.x < 0;
        
        // Despawn
        if ((s32)occupiedTile->h < (s32)gg->seaLevel - 5000){
            e->deleted = true;
        }

    } break;
    
    case Entity_Item:
    {
        auto t = &e->data.item;
        
        if (e->speed.x || e->speed.y){
            e->pos += e->speed;
            f32 len = Length(e->speed);
            f32 dir = AngleOf(e->speed);
            f32 newLen = Max(0, len - 1.f);
            e->speed = V2LengthDir(newLen, dir);
        }

        f32 playerDistanceSqr = LengthSqr(e->pos - gg->playerPos);
        f32 pickupDistance = 80.f;
        if (playerDistanceSqr < pickupDistance*pickupDistance && gg->time - t->creationTime > 60){
            inventory_item *emptySlot = 0;
            inventory_item *stackableSlot = 0;
            for (s32 i = ArrayCount(gg->inventoryAll) - 1; i >= 0; i--){
                inventory_item *slot = &gg->inventoryAll[i];
                if (!slot->id){
                    emptySlot = slot;
                }else if (slot->id == t->itemId){
                    stackableSlot = slot;
                }
            }

            if (stackableSlot){
                stackableSlot->numItems += t->numItems;
                e->deleted = true;
            }else if (emptySlot){
                emptySlot->numItems = t->numItems;
                emptySlot->id = t->itemId;
                e->deleted = true;
            }

            // If you pick up an egg, it'll make goose angry.
            if ((stackableSlot || emptySlot) && t->itemId == Item_Egg){
                f32 makeGooseAngryRadius = TILE_SIZE*30.f;
                f32 radiusSquared = makeGooseAngryRadius*makeGooseAngryRadius;
                for(s32 i = 0; i < gg->numEntities; i++){
                    entity *entityIt = &gg->entities[i];
                    if (!entityIt->deleted && entityIt->type == Entity_Goose){
                        f32 distanceSqr = LengthSqr(entityIt->pos - e->pos);
                        if (distanceSqr < radiusSquared){
                            entityIt->data.goose.angryCooldown = 60*7;
                            entityIt->state = ENTITY_STATE_CHASE;
                            f32 playerDir = AngleOf(gg->playerPos - entityIt->pos);
                            entityIt->speed = V2LengthDir(3.f, playerDir);
                            entityIt->stateTimer = 0;
                        }
                    }
                }
            }
        }

        // Despawn
        tile *tile_ = WorldPosGetTile(e->pos);
        if ((s32)tile_->h < (s32)(gg->seaLevel - 1500)){
            e->deleted = true;
        }

    } break;
    
    case Entity_ThrownStone:
    {
        auto t = &e->data.thrownStone;
        f32 breakChance = .3f;
        if (gg->time - t->creationTime > 17){
            //if (!RandomChance(breakChance)){
                // Create item
                entity *itemEntity = CreateEntity(Entity_Item);
                itemEntity->pos = e->pos;
                itemEntity->speed = e->speed*.3f;
                itemEntity->data.item.numItems = 1;
                itemEntity->data.item.itemId = Item_Stone;
            //}else{
            //  // Create Particles (stone broke)
            //  v2 texPos = V2(((s32)Item_Stone % 10)*48.f, gg->images[TEX_ITEMS].dim.y - (1 + ((s32)Item_Stone/10))*48.f);
            //  CreateTexParticles(3, texPos + V2(8.f), texPos + V2(48 - 8.f), e->pos, 4, 55, .8f, 1.f);
            //}

            e->deleted = true;
        }else{
            f32 hitboxRadius = 18.f;
            v2 hitboxCenter = e->pos;
            for(int i = 0; i < gg->numEntities; i++){
                entity *entityIt = gg->entities + i;
                if (entityIt->deleted)
                    continue;

                auto entityType = entityIt->type;
                if (entityType == Entity_Goose || entityType == Entity_Rabbit
                    || entityType == Entity_Boar)
                {
                    if (CircleEntityCollision(entityIt, hitboxCenter, hitboxRadius)){
                        if (!RandomChance(breakChance)){
                            // Create item
                            entity *itemEntity = CreateEntity(Entity_Item);
                            itemEntity->pos = e->pos;
                            itemEntity->speed = -.2f*e->speed;
                            itemEntity->data.item.numItems = 1;
                            itemEntity->data.item.itemId = Item_Stone;
                        }else{
                            // Create Particles (stone broke)
                            v2 texPos = V2(((s32)Item_Stone % 10)*48.f, gg->images[TEX_ITEMS].dim.y - (1 + ((s32)Item_Stone/10))*48.f);
                            CreateTexParticles(3, texPos + V2(8.f), texPos + V2(48 - 8.f), e->pos, 4, 55, .8f, 1.f);
                        }

                        HurtEntity(entityIt, 2, AngleOf(e->speed), 4.f, 4);

                        // Delete stone
                        e->deleted = true;
                        break;
                    }
                }
            }
        }


        e->pos += e->speed;


    } break;

    case Entity_TexParticle:
    {
        auto t = &e->data.texParticle;

        s32 lifetime = gg->time - t->creationTime;
        if (lifetime > t->maxLifetime){
            e->deleted = true;
        }
        auto tile_ = WorldPosGetTile(e->pos);
        if (tile_->h < gg->seaLevel){
            e->deleted = true;
        }
        e->pos += e->speed;
        e->speed *= t->speedMultiplier;

        // Gravity
        if (t->hGravity){
            t->h += t->hSpeed;
            if (t->h < 0 ){
                t->h = 0;
                t->hSpeed *= -.1f;
                e->speed *= .5f;
            }
            if (t->h){
                t->hSpeed = Max(-20.f, t->hSpeed - t->hGravity);
            }
        }


    } break;

    }

}
void RenderEntity(entity *e){
    if (e->deleted)
        return;
    auto gg = &globalGame;
    auto gi = &globalInput;
    v2 viewPos = gg->camPos - gi->windowDim/2;

    if (!PointInRect(e->pos, viewPos - V2(1000.f), viewPos + gi->windowDim + V2(1000.f)))
        return;
    
    v4 lightMapCol = LightMapColor(e->pos);

    switch(e->type){
    case Entity_Rabbit:
    {
        auto t = &e->data.rabbit;
        s32 subImage;
        if (e->state == ENTITY_STATE_IDLE) {
            subImage = 0;
        } else if (e->state == ENTITY_STATE_DUCK || e->state == ENTITY_STATE_LANDING) {
            subImage = 1;
        }else{
            subImage = 2;
        }
        auto img = &gg->images[TEX_RABBIT];
        v2 feetPos = e->pos - viewPos;

        if ( feetPos.x + 200 > 0 && feetPos.x - 200 < gi->windowDim.x &&
             feetPos.y + 200 > 0 && feetPos.y - 200 < gi->windowDim.y)
        {
            v2 texDim = V2(80, 128);
            v2 texPos = V2(texDim.x*subImage, 112);
            v2 drawPos = feetPos - V2(texDim.x/2, 16);
            tile *tile_ = WorldPosGetTile(e->pos);
            if (tile_->h <= gg->seaLevel){
                f32 submergedPixels = LerpClamp(20.f, 127.f, (gg->seaLevel - tile_->h)/500.f);
                texPos.y += submergedPixels;
                texDim.y -= submergedPixels;
                drawPos.y += 8.f;
            }
            DrawSpriteRect(img, texPos, texDim, drawPos, V2(1.f), WindowYGetZ(e->pos.y - viewPos.y), lightMapCol, e->facingLeft); 
        }

    } break;
    
    case Entity_Goose:
    {
        auto t = &e->data.goose;
        s32 subImage;
        if (e->state == ENTITY_STATE_IDLE) {
            subImage = 0;
        }else if (e->state == ENTITY_STATE_PRE_ATTACK){
            subImage = 2;
        }else if (e->state == ENTITY_STATE_ATTACK){
            subImage = 1;
        }else if (e->state == ENTITY_STATE_WALK){
            subImage = 3 + ((e->stateTimer / 10) % 2);
        }else if (e->state == ENTITY_STATE_CHASE){
            subImage = 6 + ((e->stateTimer / 6) % 2);
        }else{
            subImage = 3;
        }
        auto img = &gg->images[TEX_GOOSE];
        v2 feetPos = e->pos - viewPos;
        v2 texDim = V2(160, 160);
        v2 texPos = V2(texDim.x*(subImage % 3), gg->images[TEX_GOOSE].dim.y - texDim.y*(1 + subImage/3));

        if ( feetPos.x + 200 > 0 && feetPos.x - 200 < gi->windowDim.x &&
             feetPos.y + 200 > 0 && feetPos.y - 200 < gi->windowDim.y)
        {
            tile *tile_ = WorldPosGetTile(e->pos);
            v2 drawPos = feetPos - V2(texDim.x/2, 16);
            if (tile_->h <= gg->seaLevel){
                f32 submergedPixels = LerpClamp(32.f, 50.f, (gg->seaLevel - tile_->h)/500.f);
                texPos.y += submergedPixels;
                texDim.y -= submergedPixels;
                drawPos.y += 16.f;
            }
            DrawSpriteRect(img, texPos, texDim, drawPos, V2(1.f), WindowYGetZ(e->pos.y - viewPos.y), lightMapCol, e->facingLeft); 
        }

    } break;
    
    case Entity_Boar:
    {
        auto t = &e->data.boar;
        s32 subImage;
        if (e->state == ENTITY_STATE_IDLE) {
            subImage = 0;
        }else if (e->state == ENTITY_STATE_ATTACK){
            subImage = 1;
        }else if (e->state == ENTITY_STATE_WALK){
            subImage = 2 + ((e->stateTimer / 10) % 2);
        }else if (e->state == ENTITY_STATE_CHASE){
            subImage = 4 + ((e->stateTimer % 15) < 7 ? 0 : 1);
        }else{
            subImage = 2;
        }
        auto img = &gg->images[TEX_BOAR];
        v2 feetPos = e->pos - viewPos;
        v2 texDim = V2(160, 160);
        v2 texPos = V2(texDim.x*(subImage % 2), gg->images[TEX_BOAR].dim.y - texDim.y*(1 + subImage/2));

        if ( feetPos.x + 200 > 0 && feetPos.x - 200 < gi->windowDim.x &&
             feetPos.y + 200 > 0 && feetPos.y - 200 < gi->windowDim.y)
        {
            tile *tile_ = WorldPosGetTile(e->pos);
            v2 drawPos = feetPos - V2(texDim.x/2, 16);
            if (tile_->h <= gg->seaLevel){
                f32 submergedPixels = LerpClamp(32.f, 50.f, (gg->seaLevel - tile_->h)/500.f);
                texPos.y += submergedPixels;
                texDim.y -= submergedPixels;
                drawPos.y += 16.f;
            }
            DrawSpriteRect(img, texPos, texDim, drawPos, V2(1.f), WindowYGetZ(e->pos.y - viewPos.y), lightMapCol, e->facingLeft); 
        }

    } break;
    
    case Entity_Item:
    {
        auto t = &e->data.item;
        
        f32 osc = 8*(Sin((gg->animationTime - t->creationTime)*0.2f) + 1.f)/2.f;
        DrawItem(e->pos - viewPos + V2(0, -osc), t->itemId, t->numItems, WindowYGetZ(e->pos.y - viewPos.y), lightMapCol);

    } break;

    case Entity_ThrownStone:
    {
        DrawItem(e->pos - viewPos, Item_Stone, 1, WindowYGetZ(e->pos.y - viewPos.y), lightMapCol);

    } break;

    case Entity_TexParticle:
    {
        auto t = &e->data.texParticle;

        s32 lifetime = gg->time - t->creationTime;
        f32 alpha = LerpClamp(0.f, 1.f, 10.f*(1.f - lifetime/(f32)t->maxLifetime));

        v2 drawPos = e->pos - Hadamard(t->texDim/2, t->scale) - viewPos + V2(0, t->h);
        f32 z = (t->z > 0 ? t->z : WindowYGetZ(e->pos.y - viewPos.y));
        v4 col = Hadamard(t->color, lightMapCol);
        col.a *= alpha;
        DrawSpriteRect(&gg->images[t->imageIndex], t->texPos, t->texDim, drawPos, t->scale, z, col);

        //char str[100];
        //sprintf(str, "z:%f", z);
        //DrawString(str, drawPos + V2(40));
    } break;


    }
    
    if (gg->debugOn){
        DrawCircleLine(e->pos - viewPos, 5.f, V4_Green(), 90);
    }
}


void GuiEndOfFrameReset(){
    globalGame.guiHoveredId = 0;
    if (!globalGame.guiKeepActive){
        globalGame.guiActiveId = 0;
    }
    globalGame.guiKeepActive = 0;
}
v4 ButtonGetColor(u64 id, b32 unusable = false){
    //if (id == globalGame.guiActiveId) return V4_Grey(.3f);
    //if (id == globalGame.guiHoveredId) return V4_Grey(.8f);
    //return V4_Grey(.6f);
    if (id == globalGame.guiActiveId || unusable) return V4_Grey(.3f);
    if (id == globalGame.guiHoveredId) return V4(.8f, .92f, 1.f);
    return V4(.7f, .84f, .6f);
}
b32 DoButton(u64 id, v2 pos, v2 dim, char *text=0, s32 z = 90, b32 unusable = false,
             v2 textScale = V2(1.f)){
    b32 hovering = PointInRect(globalInput.mousePos, pos, pos + dim);

    b32 result = false;

    if (!unusable){
        if (!globalGame.guiActiveId && !globalGame.guiHoveredId){
            if (hovering){
                globalGame.guiHoveredId = id;
                if (ButtonWentDown(&globalInput.mouseButtons[0])){
                    globalGame.guiActiveId = id;
                }
            }
        }
        if (globalGame.guiActiveId == id){
            if (globalInput.mouseButtons[0].isDown){
                globalGame.guiKeepActive = true;
            }else{
                if (hovering){
                    result = true;
                    globalGame.guiHoveredId = id;
                }
                globalGame.guiActiveId = false;
            }
        }
    }

    // Draw rect
    v4 col = ButtonGetColor(id, unusable);
    v2 drawPos = RoundV2(pos);
    DrawRect(drawPos, dim, col, z);

    f32 frameSize = (globalGame.guiActiveId == id ? 4.f : 3.f);
    DrawRectFrame(drawPos, dim, V2(frameSize), V4_Black(), z);

    // Draw text
    if (text){
        v2 textPos = CenterStringXY(text, pos + dim/2, textScale) + Hadamard(V2(0, -1.f), textScale);
        if (globalGame.guiActiveId == id)
            textPos.y -= 2.f;
        else if (globalGame.guiHoveredId == id)
            textPos.y += 1.f;
        DrawString(text, textPos, textScale, z, V4_Black());
    }

    return result;
}


loaded_image LoadImageBmp(u8 *bmpData, size_t bmpSize){
// TODO: Think about endianness...

    Assert(bmpSize > sizeof(BITMAPFILEHEADER));
    auto header = (BITMAPFILEHEADER *)bmpData;
    auto infoHeader = (BITMAPINFOHEADER *)(header + 1);
    Assert(memcmp(&header->bfType, "BM", 2) == 0);

    s32 width = infoHeader->biWidth;
    s32 height = infoHeader->biHeight;
    s32 bitsPerPixel = infoHeader->biBitCount;
    s32 imageSize = infoHeader->biSizeImage;

    DebugPrintf("Info bitmap:\n");
    DebugPrintf("Width=%i Height=%i bitsPerPixel=%i imageSize=%i\n",
                width, height, bitsPerPixel, imageSize);

    u8 *src = bmpData + header->bfOffBits;
    u8 *dest = PushSize(&globalGame.permMem, width*height*4);
    
    loaded_image loadedImage = {};
    loadedImage.base = (u32 *)dest;
    loadedImage.dim = V2(width, height);

    for(s32 y = 0; y < height; y++){
        for(s32 x = 0; x < width; x++){
            u32 pixelSrc = *((u32 *)src + y*width + x); // ARGB
            //u32 pixel = (0xFF000000 & pixelSrc << 24) | // Convert from ABGR
            //          (0x00FF0000 & pixelSrc << 8)  |
            //          (0x0000FF00 & pixelSrc >> 8)  |
            //          (0x000000FF & pixelSrc >> 24);
            //u32 pixel = (0xFF000000 & pixelSrc << 16) | // Convert from BGRA
            //          (0x00FF0000 & pixelSrc)       |
            //          (0x0000FF00 & pixelSrc >> 16) |
            //          (0x000000FF & pixelSrc);
            //u32 pixel = (0xFFFFFF00 & (pixelSrc << 8)) | // Convert from ARGB
            //          (0x000000FF & (pixelSrc >> 24));
            u32 pixel = (0xFF00FF00 & (pixelSrc)) |      // ABGR to ARGB
                        (0x00FF0000 & (pixelSrc << 16)) |
                        (0x000000FF & (pixelSrc >> 16));
            *(u32 *)dest = pixel;
            dest += 4;
        }
    }

    return loadedImage;
}

inline v4 TileGetColor(tile t, u16 seaLevel = globalGame.seaLevel){
    s32 midLevel = INITIAL_SEA_LEVEL + 3500;
    s32 beachEnd = INITIAL_SEA_LEVEL + 2000;
    v4 result;
    if (t.h < seaLevel){
        s32 deepSea = seaLevel - DEEP_WATER_DEPTH;
        if (t.h < deepSea){
            result = WATER_COLOR_DEEP;
        }else{
            result = LerpV4(WATER_COLOR_DEEP, WATER_COLOR_SHALLOW, Clamp01(((f32)t.h - (f32)deepSea) / ((f32)seaLevel - (f32)deepSea)));
        }
    }else{
        v4 ground0 = V4(.72f, .7f, .3f, 1.f); // beach sand
        v4 ground1, ground2;
        if ((biome_id)t.biomeData == Biome_Prairie){
            //ground1 = V4(.1f, .38f, .18f, 1.f);
            //ground2 = V4(.72f, .85f, .53f, 1.f);
            ground1 = V4(.18f, .46f, .1f);
            ground2 = V4(.72f, .93f, .25f);
        }else if ((biome_id)t.biomeData == Biome_Forest){
            ground1 = V4(.05f, .34f, .15f);
            ground2 = V4(.52f, .85f, .53f);
        }else if ((biome_id)t.biomeData == Biome_Dry){
            //ground1 = V4(.67f, .60f, .24f);
            ground1 = V4(.4f, .34f, .13f);
            ground2 = V4(.88f, .86f, .42f);
        }else if ((biome_id)t.biomeData == Biome_Rocky){
            ground1 = V4(.38f, .37f, .34f);
            ground2 = V4(.82f, .82f, .8f);
        }

        if (t.h < midLevel){
            f32 l = (t.h - beachEnd)/(f32)(midLevel - INITIAL_SEA_LEVEL);
            result = LerpV4(ground0, ground1, ((f32)t.h - (f32)beachEnd)/ ((f32)midLevel - (f32)beachEnd));
        }else{
            f32 l = (t.h - midLevel) / (f32)(MAX_HEIGHT - midLevel);
            result = V4(Lerp(ground1.r, ground2.r, l), Lerp(ground1.g, ground2.g, l), Lerp(ground1.b, ground2.b, l), 1.f);
        }
    }

    //f32 c = Lerp(.5f, 1.f, (f32)t.h - (f32)INITIAL_SEA_LEVEL)/(60000.f - INITIAL_SEA_LEVEL);
    //v4 color = (t.h < seaLevel ? waterCol : V4(.5f*c, 1.f*c, .3f*c, 1.f));
    return result;
}


struct perlin_state{
    s32 perlinSize; // size of perlin noise grid cells, measured in terrain grid cells.
    s32 maxHeight;
    u8 *perlinIndices;
    s32 perlinCellsX;
    s32 perlinCellsY;
    b32 limitHeight;
};

struct biome_point{
    v2 pos;
    f32 h;
    biome_id biome;
};

static biome_point biomePoints[32] = {};

static v2 DEBUG_peakPos = {};

// ,,
DWORD WINAPI ThreadProc(LPVOID param){
    auto gg = &globalGame;
    gg->workerThread.output.sleeping = true;

    while(1){
        Sleep(1);
        
        if (gg->workerThread.input.startGenerating && !gg->workerThread.output.finishedGenerating){
            gg->workerThread.output.sleeping = false;

            gg->workerThread.output.progress = 0.f;

            f32 progressAfterFirstLoop = .60f;
            f32 progressAfterNormalizeLoop = .98f;

            // ,,Zero game state.
            memset(&gg->___stuffBelowThisMemberWillGetZeroedEveryNewGame___, 0, ((size_t)gg) + sizeof(game_state) - ((size_t)&gg->___stuffBelowThisMemberWillGetZeroedEveryNewGame___));
            
            // 
            // Terrain
            //
        
            // Init perlin states.
            perlin_state perlins[5] = {};
            //perlins[0].perlinSize = 8;
            //perlins[0].maxHeight  = 400;
            
            perlins[4].perlinSize = 16;
            perlins[4].maxHeight  = 1000;

            perlins[3].perlinSize = 64;
            perlins[3].maxHeight  = 2000;
            
            perlins[2].perlinSize = 128;
            perlins[2].maxHeight  = 5000;
            perlins[2].limitHeight = true;
            
            perlins[1].perlinSize = 256;
            perlins[1].maxHeight  = 23000;
            perlins[1].limitHeight = true;

            perlins[0].perlinSize = 512;
            perlins[0].maxHeight  = 8000;
            perlins[0].limitHeight = true;
        
            f32 s = 0.70710678118f;
            v2 perlinVectors[4] = {{s, s}, {-s, s}, {s, -s}, {-s, -s}};
            for(s32 i = 0; i < ArrayCount(perlins); i++){
                auto p = &perlins[i];
                s32 perlinCellsX = p->perlinCellsX = TILES_X/p->perlinSize + 1;
                s32 perlinCellsY = p->perlinCellsY = TILES_X/p->perlinSize + 1;

                u8 *indices = p->perlinIndices = PushSize(&gg->frameMem, perlinCellsX*perlinCellsY);
                for(s32 y = 0; y < perlinCellsY; y++){
                    for(s32 x = 0; x < perlinCellsX; x++){
                        indices[perlinCellsX*y + x] = RandomU8() & 0x03;
                    }
                    //indices[perlinCellsX*y]                    = (RandomU8() & 0x01 ? 3 : 3);
                    //indices[perlinCellsX*y + perlinCellsX-1] = (RandomU8() & 0x01 ? 0 : 2);
                }
                for(s32 x = 0; x < perlinCellsX; x++){
                    //indices[x]                                   = (RandomU8() & 0x01 ? 0 : 1);
                    //indices[perlinCellsX*(perlinCellsY-2) + x] = (RandomU8() & 0x01 ? 0 : 1);
                }
            }

            // Generate tiles

            f32 peakDir = RandomRange(0, 2*PI);
            s32 peakX, peakY;
            {
                v2 p = V2(.5f) + V2LengthDir(RandomRange(0, .3f), peakDir);
                peakX = TILES_X*p.x;
                peakY = TILES_Y*p.y;
            }
            DEBUG_peakPos = V2(peakX, peakY)*TILE_SIZE;

            //
            // Biome points
            //
            
            // (reset)
            for(s32 i = 0; i < ArrayCount(biomePoints); i++){
                biomePoints[i].biome = (biome_id)0;
            }

            s32 numMainPoints = 8;
            s32 numSecondaryPoints = 8;
            s32 numTerciaryPoints = ArrayCount(biomePoints) - numMainPoints - numSecondaryPoints;
            Assert(numTerciaryPoints > 0);
            //v2 biomePoints[12];
            //biome_id biomeIds[ArrayCount(biomePoints)];
            f32 worldRadius = TILES_X*TILE_SIZE/2;
            v2 worldCenter = V2(TILES_X, TILES_Y)*TILE_SIZE/2;
            //for(s32 i = 0; i < 4; i++){
            //  biomePoints[i] = worldCenter + V2LengthDir(Lerp(0.25f*worldRadius, 0.7f*worldRadius, Random01()), Random(2*PI));
            //}
            f32 randomDir = Random01()*2*PI;
            for(s32 i = 0; i < 4; i++){
                biomePoints[i].pos = worldCenter + V2LengthDir(Lerp(0.3f*worldRadius, 0.65f*worldRadius, Random01()), randomDir + i*(PI/2) + Random(PI/8));
                biomePoints[i].biome = (biome_id)(i % (s32)Biome_Count);
            }
            for(s32 i = 4; i < 8; i++){
                v2 point;
                for(s32 tryCount = 0; tryCount < 10; tryCount++){
                    point = worldCenter + V2LengthDir(Lerp(0.1f*worldRadius, 0.65f*worldRadius, Random01()), Random(2*PI));
                    f32 minDistance = worldRadius*.35f;
                    b32 tooClose = false;
                    for(s32 j = 0; j < i; j++){
                        f32 distanceSqr = LengthSqr(point - biomePoints[j].pos);
                        if (distanceSqr > Square(minDistance)){
                            tooClose = true;
                            break;
                        }
                    }
                    if (!tooClose)
                        break;
                }
                biomePoints[i].pos = point;
                biomePoints[i].biome = (biome_id)(i % (s32)Biome_Count);
            }

            for(s32 i = 0; i < numSecondaryPoints; i++){
                s32 originalPointIndex = i % numMainPoints;
                v2 originalPoint = biomePoints[originalPointIndex].pos;
                v2 point;
                for(s32 tryCount = 0; tryCount < 20; tryCount++){
                    point = originalPoint + V2LengthDir(Lerp(0.3f*worldRadius, 0.7f*worldRadius, Random01()), Random(2*PI));

                    if (LengthSqr(point - worldCenter) > Square(0.8f*worldRadius)){
                        point = worldCenter + worldRadius*V2(Lerp(-0.8, 0.8f, Random01()), Lerp(-0.8f, 0.8f, Random01()));
                    }

                    // Check other distance to other points
                    f32 minDistance = worldRadius*.15f;
                    b32 tooClose = false;
                    for(s32 j = 0; j < i; j++){
                        f32 distanceSqr = LengthSqr(point - biomePoints[j].pos);
                        if (distanceSqr > Square(minDistance)){
                            tooClose = true;
                            break;
                        }
                    }
                    if (!tooClose && LengthSqr(point - worldCenter) < Square(0.7f*worldRadius))
                        break;
                }
                biomePoints[numMainPoints + i].pos = point;
                biomePoints[numMainPoints + i].biome = biomePoints[originalPointIndex].biome;
            }
            for(s32 i = 0; i < numTerciaryPoints; i++){
                s32 originalPointIndex = i % (numMainPoints + numSecondaryPoints);
                v2 originalPoint = biomePoints[originalPointIndex].pos;
                biomePoints[numMainPoints + numSecondaryPoints + i].pos = originalPoint + V2LengthDir(Lerp(0.1f*worldRadius, 0.2f*worldRadius, Random01()), Random(2*PI));
                biomePoints[numMainPoints + numSecondaryPoints + i].biome = biomePoints[originalPointIndex].biome;
            }


            u16 maxHeight = 32768;
            for(s32 y = 0; y < TILES_Y; y++){
                for(s32 x = 0; x < TILES_X; x++){
                    tile *t = &gg->tiles[TILES_X*y + x]; 
                    t->object = (tile_object_type)0;


                    f32 h = (f32)(RandomU8() & 0x0F); // 0-15

                    f32 centerDistance = Clamp01(LengthFast(V2((f32)(x - TILES_X/2), (f32)(y - TILES_Y/2))) / (f32)(TILES_X/2));
                    const f32 peakSize = .5f;
                    f32 peakDistance = LengthFast(V2((f32)(x - peakX), (f32)(y - peakY)));
                    f32 peakDistanceNormalized = Clamp01(peakDistance / (peakSize*(TILES_X/2)));
                    //f32 smoothCenterDistance = SLerp(0, 1.f, centerDistance);
                    f32 peak = (1.f - peakDistanceNormalized);
                    f32 peakFactor = peak*peak;
                    h +=  10000*peakFactor;
                    h +=  15000*(1.f - centerDistance*centerDistance*centerDistance);
                    h +=  1000*(1.f - centerDistance);

                    // Perlin
                    f32 limitedHeightMax = Lerp(0.75f*INITIAL_SEA_LEVEL, (f32)MAX_HEIGHT,
                                           1.f - centerDistance*centerDistance*centerDistance*centerDistance);
                    for(s32 i = 0; i < ArrayCount(perlins); i++){
                        auto p = perlins[i];
                        s32 perlinY = y / p.perlinSize;
                        s32 perlinX = x / p.perlinSize;
                        v2 perlinCellPos = V2((f32)(x % p.perlinSize), (f32)(y % p.perlinSize)) / (f32)p.perlinSize;

                        f32 dots[4] = {Dot(perlinVectors[p.perlinIndices[p.perlinCellsX*perlinY + perlinX]],           perlinCellPos - V2(0.f, 0.f)),
                                       Dot(perlinVectors[p.perlinIndices[p.perlinCellsX*(perlinY + 1) + perlinX]],     perlinCellPos - V2(0.f, 1.f)),
                                       Dot(perlinVectors[p.perlinIndices[p.perlinCellsX*(perlinY + 1) + perlinX + 1]], perlinCellPos - V2(1.f, 1.f)),
                                       Dot(perlinVectors[p.perlinIndices[p.perlinCellsX*perlinY       + perlinX + 1]], perlinCellPos - V2(1.f, 0.f))};
                        //f32 linear0 = Lerp(dots[0], dots[3], perlinCellPos.x); 
                        //f32 linear1 = Lerp(dots[1], dots[2], perlinCellPos.x); 
                        //f32 linear  = Lerp(linear0, linear1, perlinCellPos.y);
                        f32 smooth0 = SmoothstepLerp(dots[0], dots[3], perlinCellPos.x); 
                        f32 smooth1 = SmoothstepLerp(dots[1], dots[2], perlinCellPos.x); 
                        f32 smooth  = SmoothstepLerp(smooth0, smooth1, perlinCellPos.y);

                        f32 newH = h + p.maxHeight*((smooth + 1.f) / 2.f);

                        if (!p.limitHeight && newH > limitedHeightMax){
                            h = limitedHeightMax;
                        }else{
                            h = newH;
                        }
                        // h = limitedHeightMax; // visualizing the height limit
                    }
                    Assert(h >= 0.f && h <= 65535.f);
                    t->h = (u16)h;
                    if (t->h > maxHeight){
                        maxHeight = t->h;
                    }
                }

                gg->workerThread.output.progress = Lerp(0.f, progressAfterFirstLoop, y/(f32)(TILES_Y - 1));
                if (gg->workerThread.input.cancelWork){
                    goto LABEL_CancelGeneration;
                }
            }

            //
            // Set biome point h
            //
            for(s32 i = 0; i < ArrayCount(biomePoints); i++){
                s32 originalPointIndex = i % (numMainPoints + numSecondaryPoints);
                tile *biomePointTile = WorldPosGetTile(biomePoints[i].pos);
                biomePoints[i].h = (f32)biomePointTile->h;
            }

            //
            // Fix edges and normalize height and add tile objects
            //
            f32 normalizeScale = (MAX_HEIGHT - INITIAL_SEA_LEVEL) / (f32)(maxHeight - INITIAL_SEA_LEVEL);
            tile_object_type plantChoices[] = {
                    TileObject_Plant1,         TileObject_Grass1, TileObject_Grass2, TileObject_Grass3,
                    TileObject_Grass4,         TileObject_Grass4, TileObject_Grass5, TileObject_Grass5,
                    TileObject_GrassAndRocks1};
            tile_object_type decorativeRockChoices[] = {
                    TileObject_GrassAndRocks1, TileObject_Rocks1, TileObject_Rocks2, TileObject_Rocks2,
                    TileObject_Rocks2,         TileObject_Rocks3, TileObject_Rocks3};

            s32 logCount = 0, rockCount = 0, treeCount = 0, flowerCount = 0;
            for(s32 y = 0; y < TILES_Y; y++){
                for(s32 x = 0; x < TILES_X; x++){
                    tile *t = &gg->tiles[TILES_X*y + x];

                    // Find closest biome
                    {
                        v2 tileWorldPos = V2(x, y)*TILE_SIZE;
                        f32 zScale = 3.3f;
                        f32 h = (f32)t->h;
                        f32 bestBiomeDistanceSqr = 999999999999.f;
                        for(s32 i = 0; i < ArrayCount(biomePoints); i++){
                            v2 posDelta = biomePoints[i].pos - tileWorldPos;
                            f32 hDelta = biomePoints[i].h - h;
                            f32 zDelta = hDelta*zScale;
                            f32 biomeDistanceSqr = Square(posDelta.x) + Square(posDelta.y) + Square(zDelta);
                            if (biomeDistanceSqr < bestBiomeDistanceSqr){
                                bestBiomeDistanceSqr = biomeDistanceSqr;
                                t->biomeData = (u8)biomePoints[i].biome;
                            }
                        }
                    }


                    if (t->h > INITIAL_SEA_LEVEL){
                        // Normalize the height
                        t->h = (u16)(INITIAL_SEA_LEVEL + (t->h - INITIAL_SEA_LEVEL)*normalizeScale);
        
                        // Put tile object based on biome.
                        if (t->h > INITIAL_SEA_LEVEL + 300 && RandomChance(.04f)){ // 4% of tiles will have something at most.
                            // Chances for each object. Chance summation must be <= 1
                            f32 cPlant = .25f;
                            f32 cDecorativeRocks = .25f;
                            f32 cStone = 0.06f;
                            f32 cStick = 0.04f;
                            f32 cRock = 0;
                            f32 cLog = 0;
                            f32 cFlower = 0;
                            f32 cMushroom = .05f;
                            f32 cTree = 0;
                            f32 cHay = .05f;
                            
                            auto biome = t->biomeData;
                            if (biome == Biome_Prairie){
                                cFlower = .08f;
                                cLog = .03f;
                                cPlant = .4f;
                            }else if (biome == Biome_Forest){
                                cTree = .09f;
                                cLog = .06f;
                                cFlower = .01f;
                            }else if (biome == Biome_Dry){
                                cHay = .08f;
                                cPlant = .15f;
                            }else{// if (biome == Biome_Rocky){
                                cStone = .12f;
                                cRock = .05f;
                                cDecorativeRocks = .4f;
                                cPlant = 0;
                            }
                            // Throw dice
                            f32 random = Random01();
                            if (random < cPlant){
                                t->object = plantChoices[RandomS32(ArrayCount(plantChoices) - 1)];
                            }else{
                                random -= cPlant;
                                if (random < cDecorativeRocks){
                                    t->object = plantChoices[RandomS32(ArrayCount(decorativeRockChoices) - 1)];
                                }else{
                                    random -= cDecorativeRocks;
                                    if (random < cStone){
                                        t->object = TileObject_Stone;
                                    }else{
                                        random -= cStone;
                                        if (random < cStick){
                                            t->object = TileObject_Stick;
                                        }else{
                                            random -= cStick;
                                            if (random < cRock){
                                                t->object = TileObject_Rock;
                                                rockCount++;
                                            }else{
                                                random -= cRock;
                                                if (random < cLog){
                                                    t->object = TileObject_Log;
                                                    logCount++;
                                                }else{
                                                    random -= cLog;
                                                    if (random < cFlower){
                                                        t->object = TileObject_Flower1;
                                                        flowerCount++;
                                                    }else{
                                                        random -= cFlower;
                                                        if (random < cMushroom){
                                                            t->object = TileObject_Mushroom;
                                                        }else{
                                                            random -= cMushroom;
                                                            if (random < cTree){
                                                                if (t->h > INITIAL_SEA_LEVEL + 1000){
                                                                    t->object = TileObject_Tree;
                                                                    t->objectData = 3;
                                                                    treeCount++;
                                                                }
                                                            }else{
                                                                random -= cTree;
                                                                if (random < cHay){
                                                                    t->object = TileObject_Hay;
                                                                }else{
                                                                    random -= cHay;
                                                                    // No object.
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    
                }
                ShuffleRandomState((u32)y + 1909454003);

                gg->workerThread.output.progress = Lerp(progressAfterFirstLoop, progressAfterNormalizeLoop, y/(f32)(TILES_Y - 1));
                if (gg->workerThread.input.cancelWork){
                    goto LABEL_CancelGeneration;
                }
            }
            DebugPrintf("Generated %i logs and %i rocks.\n", logCount, rockCount);

            // Find good player spawn pos
            {
                f32 playerPosDir = peakDir + PI;//Random(2*PI);
                v2 inlandPos = {};// = V2(TILES_X, TILES_Y)*TILE_SIZE/2;
                for(s32 i = 0; i < (s32)(TILES_X*.35f); i++){
                    v2 pos = worldCenter + V2LengthDir((f32)i*TILE_SIZE, playerPosDir);
                    tile *t = WorldPosGetTile(pos);
                    if (t->h > INITIAL_SEA_LEVEL + 1000){
                        inlandPos = pos;
                    }
                }
                // A second loop that tries to get farther but stops at any water.
                // This avoids placing player on small islands.
                for(s32 i = 0; i < (s32)(TILES_X*.3f); i++){
                    v2 pos = inlandPos + V2LengthDir((f32)i*TILE_SIZE, playerPosDir);
                    tile *t = WorldPosGetTile(pos);
                    if (t->h < INITIAL_SEA_LEVEL + 1000) 
                        break;
                    gg->playerPos = pos;
                }
            }

            gg->seaLevel = INITIAL_SEA_LEVEL;
            gg->playerHealthMax = gg->playerHealth = 100;
            gg->playerEnergy      = 1.f;
            gg->playerStomach     = 1.f;
            gg->playerTemperature = .6f;

            gg->lightMapSize = V2S(1);
            gg->lightMap[0] = V4_White();


            // Create entities
            for(s32 i = 0; i < 4000; i++){
                v2 spawnPos = V2(RandomRange(0, TILES_X*TILE_SIZE), RandomRange(0, TILES_Y*TILE_SIZE));
                tile *spawnTile = WorldPosGetTile(spawnPos);
                if (spawnTile->h > INITIAL_SEA_LEVEL + 500){
                    // Summation of all chances must be <= 1
                    f32 rabbitChance, gooseChance, boarChance;
                    if (spawnTile->biomeData == Biome_Prairie){
                        rabbitChance = .55f;
                        gooseChance = .45f;
                        boarChance = .20f;
                    }else if (spawnTile->biomeData == Biome_Forest){
                        rabbitChance = .15f;
                        gooseChance = .3f;
                        boarChance = .6f;
                    }else if (spawnTile->biomeData == Biome_Dry){
                        rabbitChance = 0.05f;
                        gooseChance = .3f;
                        boarChance = .3f;
                    }else if (spawnTile->biomeData == Biome_Rocky){
                        rabbitChance = 0;
                        gooseChance = .2f;
                        boarChance = .3f;
                    }
                    // Throw dice
                    f32 random = Random01();
                    if (random < rabbitChance){ // Rabbit
                        entity *e = CreateEntity(Entity_Rabbit);
                        e->pos = spawnPos;
                        e->health = 10;
                        ShuffleRandomState(2426252457356433 + i*264573457571);
                    }else if (random < rabbitChance + gooseChance){ // Goose
                        entity *e = CreateEntity(Entity_Goose);
                        e->pos = spawnPos;
                        e->health = 20;
                        ShuffleRandomState(242625243 + i*26455777771);
                    }else if (random < rabbitChance + gooseChance + boarChance){ // Boar
                        entity *e = CreateEntity(Entity_Boar);
                        e->pos = spawnPos;
                        e->health = 20;
                        ShuffleRandomState(242625433 + i*2645734573);
                    } // else there is no entity.

                }
            }

            ShuffleRandomState(35273564334264);
            
            // TODO: Maybe create extra trees if the tree count is low...

            // Create Notes
            for(s32 i = 0; i < 400; i++){
                v2 pos = V2(RandomRange(0, TILES_X*TILE_SIZE), RandomRange(0, TILES_Y*TILE_SIZE));
                tile *t = WorldPosGetTile(pos);
                if (t->h > INITIAL_SEA_LEVEL + 400){
                    t->object = TileObject_Note;
                    t->objectData = RandomU16(ArrayCount(globalNotes) - 1);
                }
            }



            if (!gg->workerThread.input.cancelWork){
                gg->workerThread.output.finishedGenerating = true;
                gg->workerThread.output.progress = 1.f;
            }else{
LABEL_CancelGeneration:
                gg->workerThread.output.finishedGenerating = false;
                gg->workerThread.output.progress = 0.f;
            }
            gg->workerThread.output.sleeping = true;
        }
    }
}

void GameMain(){
    auto gi = &globalInput;
    auto gg = &globalGame;
    v2 mPos = gi->mousePos;

    if (!globalGame.init){

        // Test

        globalGame.init = true;

        InitGlobalItemProperties();
    
        gg->activeSection = PS_MainMenu;

        read_file_result readResult;
        
        readResult = ReadEntireFile("data\\raft.bmp", true);
        gg->images[TEX_RAFT] = LoadImageBmp(readResult.base, readResult.size);
        LoadTexture(&gg->images[TEX_RAFT]);
        DeallocateMemory(readResult.base);
        
        
        readResult = ReadEntireFile("data\\font.bmp", true);
        gg->images[TEX_FONT] = LoadImageBmp(readResult.base, readResult.size);
        LoadTexture(&gg->images[TEX_FONT]);
        DeallocateMemory(readResult.base);
        gg->defaultFontTex = &gg->images[TEX_FONT];
        
        readResult = ReadEntireFile("data\\rabbit.bmp", true);
        gg->images[TEX_RABBIT] = LoadImageBmp(readResult.base, readResult.size);
        LoadTexture(&gg->images[TEX_RABBIT]);
        DeallocateMemory(readResult.base);
        
        readResult = ReadEntireFile("data\\items.bmp", true);
        gg->images[TEX_ITEMS] = LoadImageBmp(readResult.base, readResult.size);
        LoadTexture(&gg->images[TEX_ITEMS]);
        DeallocateMemory(readResult.base);

        readResult = ReadEntireFile("data\\tile_objects.bmp", true);
        gg->images[TEX_TILES] = LoadImageBmp(readResult.base, readResult.size);
        LoadTexture(&gg->images[TEX_TILES]);
        DeallocateMemory(readResult.base);
        
        readResult = ReadEntireFile("data\\goose.bmp", true);
        gg->images[TEX_GOOSE] = LoadImageBmp(readResult.base, readResult.size);
        LoadTexture(&gg->images[TEX_GOOSE]);
        DeallocateMemory(readResult.base);
        
        readResult = ReadEntireFile("data\\player.bmp", true);
        gg->images[TEX_PLAYER] = LoadImageBmp(readResult.base, readResult.size);
        LoadTexture(&gg->images[TEX_PLAYER]);
        DeallocateMemory(readResult.base);
        
        readResult = ReadEntireFile("data\\boar.bmp", true);
        gg->images[TEX_BOAR] = LoadImageBmp(readResult.base, readResult.size);
        LoadTexture(&gg->images[TEX_BOAR]);
        DeallocateMemory(readResult.base);
        
        readResult = ReadEntireFile("data\\gui.bmp", true);
        gg->images[TEX_GUI] = LoadImageBmp(readResult.base, readResult.size);
        LoadTexture(&gg->images[TEX_GUI]);
        DeallocateMemory(readResult.base);
        
        readResult = ReadEntireFile("data\\effects.bmp", true);
        gg->images[TEX_EFFECTS] = LoadImageBmp(readResult.base, readResult.size);
        LoadTexture(&gg->images[TEX_EFFECTS]);
        DeallocateMemory(readResult.base);

        
        
        s32 tileMemSize = TILES_X*TILES_Y*sizeof(tile);
        gg->tiles = (tile *)PushSize(&gg->permMem, tileMemSize);
        memset(gg->tiles, 0, tileMemSize);
    
        
        // Multi-threading
        gg->workerThread.handle = CreateThread(0, 0, ThreadProc, 0, 0, &gg->workerThread.id);


    }

    if (gi->keyboard.control.isDown && ButtonWentDown(&gi->keyboard.letters['G' - 'A'])){
        gg->debugOn = !gg->debugOn;
        DebugPrintf("DEBUG IS NOW %s\n", (gg->debugOn ? "ON" : "OFF"));
    }

    v4 clearCol = V4_Black();
    if (gg->activeSection == PS_MainMenu){
        clearCol = V4(.05f, .26f, .64f, 1.f);
        v2 buttonDim = V2(200.f, 46.f);
        if (DoButton(1001, gi->windowDim/2 - V2(0, 20.f) -buttonDim/2, buttonDim, "New Game")){
            gg->workerThread.output.finishedGenerating = false;
            gg->workerThread.input.startGenerating = true;
            gg->workerThread.input.cancelWork = false;
            gg->activeSection = PS_CreatingWorld;
            
            s32 tileMemSize = TILES_X*TILES_Y*sizeof(tile);
            memset(gg->tiles, 0, tileMemSize);
        }
        if (DoButton(1002, gi->windowDim/2 - V2(0, 80.f) -buttonDim/2, buttonDim, "Continue", 90, !gg->worldIsValid)){
            if (gg->worldIsValid){
                gg->activeSection = PS_World;
            }
        }
        if (DoButton(1003, gi->windowDim/2 - V2(0, 140.f) - buttonDim/2, buttonDim, "Tutorial")){
            gg->activeSection = PS_Tutorial;
        }
        
        //char titleText[30] = "Kinda High Tide";

        //char titleText[30] = "Angry Waters";
        //char titleText[30] = "Tidal Island"; 
        //char titleText[30] = "Tidal Flood"; 
        //char titleText[30] = "Final Flood";
        //char titleText[30] = "Final Tide";
        //char titleText[30] = "High Tide";
        char titleText[30] = "Rising Tide";
        //char titleText[30] = "Rising Sea"; 
        //char titleText[30] = "The Sea is Rising";
        //char titleText[30] = "Deep End Rising";

        v2 titleScale = V2(2.f);
        v2 titlePos = CenterStringXY(titleText, gi->windowDim/2 + V2(0, 80.f), titleScale);

        s32 iterations = 16;
        for(s32 i = 0; i < iterations; i++){
            v2 pos = titlePos + V2LengthDir(4.f, ((f32)i/(f32)iterations)*2.f*PI);
            DrawString(titleText, pos, titleScale, 45, V4_Black(1.f));
        }
        DrawString(titleText, titlePos, titleScale, 50, V4(.9f, 1.f, 1.f));

        { // Gradient Effect
            s32 iterations = 12;
            f32 h = Ceil(gi->windowDim.y*.3f/iterations);
            for(s32 i = 0; i < iterations; i++){
                f32 a = .8f*((f32)(iterations - i)/(f32)iterations);
                DrawRect(V2(0, i*h), V2(gi->windowDim.x, h), V4(0,0,.1f, a), 12);
            }
        }

        // Randomize seed by mouse so world is different every time
        ShuffleRandomState((u32)gi->mousePos.x);
        ShuffleRandomState((u32)gi->mousePos.y*123456789.f);
        ShuffleRandomState((u32)gi->mousePos.y*gi->mousePos.x);


    }else if (gg->activeSection == PS_CreatingWorld){
        clearCol = WATER_COLOR_DEEP;

        f32 progress = gg->workerThread.output.progress;

        char progressStr[100];
        sprintf(progressStr, "Generating world... %i%%", (s32)(100.f*progress));
        v2 strScale = V2(1.f);
        DrawString(progressStr, CenterStringXY(progressStr, gi->windowDim/2, strScale), strScale, 70, V4_White());

        // Progress bar
        {
        //  v2 dim = V2(gi->windowDim.x*1.f, 50.f);
        //  v2 p0 = V2((gi->windowDim.x - dim.x)/2, (gi->windowDim.y - dim.y)/2);
        //  v2 p1 = V2(gi->windowDim.x - p0.x, (gi->windowDim.y + dim.y)/2);

        //  v4 bgCol = V4_Black(.6f);
        //  v4 barCol = LerpV4(V4(.44f, .38f, .19f, .7f), V4(.9f, .68f, .18f, .8f), progress);
        //  // Bar background
        //  DrawRect(V2(0, p0.y), V2(p0.x, dim.y), bgCol, 50);
        //  DrawRect(V2(p0.x + dim.x*progress, p0.y), V2(dim.x*(1.f-progress) + gi->windowDim.x - p1.x, dim.y), bgCol, 50);
        //  // Progress
        //  DrawRect((gi->windowDim - dim)/2, V2(dim.x*progress, dim.y), barCol, 50);
            f32 h = 52.f;
            DrawRect(V2(0, gi->windowDim.y/2 - h/2), V2(gi->windowDim.x, h), V4_Black(.5f), 50);
        }
        DrawRect(V2(0,0), gi->windowDim, V4_Black((1.f - progress)*.5f), 40);


        // Map process
        {
            // Draw map
            v2 mapDim = V2(Min(gi->windowDim.x, gi->windowDim.y)*.9f);
            v2 mapPos = (gi->windowDim - mapDim) / 2;
            u16 seaLevel = INITIAL_SEA_LEVEL;
            //s32 mapSize = 100;
            //v2 cellDim = mapDim/mapSize;
            //for(s32 y = 0; y < mapSize; y++){
            //  for(s32 x = 0; x < mapSize; x++){
            //      s32 tileX = TILES_X*x/mapSize;
            //      s32 tileY = TILES_Y*y/mapSize;
            //      tile t = gg->tiles[TILES_X*tileY + tileX];
            //      v4 col = TileGetColor(t, seaLevel);
            //      col.a = 1.f;
            //      v2 cellPos = mapPos + V2(x*cellDim.x, y*cellDim.y);
            //      DrawRect(cellPos, cellDim, col, 10 + (y*10/mapSize));
            //  }
            //}
            // Draw map
            f32 pointSize = 1.f;
            f32 maxPointsPerAxis = 300.f;
            while(mapDim.x/pointSize > maxPointsPerAxis){
                pointSize += 1.f;
            }
            DrawMap(mapPos, mapDim, 1.f, pointSize, seaLevel, 10, false);
        }

        


        v2 buttonDim = V2(130.f, 46.f);
        if (DoButton(1201, V2(20.f, 20.f), buttonDim, "Cancel")){
            gg->workerThread.input.cancelWork = true;
            gg->workerThread.input.startGenerating = false;
            gg->activeSection = PS_MainMenu;
            gg->worldIsValid = false;
        }else if (gg->workerThread.output.progress == 1.f && gg->workerThread.output.finishedGenerating){
            gg->activeSection = PS_World;
            gg->workerThread.input.startGenerating = false;
            gg->worldIsValid = true;
        }

    }else if (gg->activeSection == PS_World){
        clearCol = V4(.1f, .3f, .7f, 1.f);
        b32 dead = (gg->playerHealth == 0);
        if (dead){
            gg->sleepWindowIsOpen = false;
            gg->inventoryOpen = false;
        }

        b32 pauseGameplay = gg->sleepWindowIsOpen || gg->noteIsOpen || gg->pause || gg->passedOutTimer; // Nothing moves, can't even access inventory, but we draw stuff.
        b32 timeIsStopped = (gg->debug.stopTime || pauseGameplay);
        if (!timeIsStopped && !dead && gg->playerEnergy == 0){
            pauseGameplay = true;
            timeIsStopped = true;
            gg->passedOutTimer = 1;

        }
        if (!timeIsStopped){
            gg->time += (gg->debug.timeX10 ? 10 : 1);

            gg->seaLevel = TimeGetSeaLevel(gg->time);
            if (!dead){
                //gg->playerEnergy = Max(0.f, gg->playerEnergy - 1.f/(60*60*30)); // 30 minute to deplete energy if standing still.
                gg->playerStomach = Max(0.f, gg->playerStomach - HUNGER_RATE);
            }
        }

        // Passed out
        if (gg->passedOutTimer){
            // Advance time
            if (gg->passedOutTimer == 60*2){
                // Sleep
                f32 recoverEnergy = .333f;
                s32 wakeupTime = gg->time + (s32)(recoverEnergy/PASSED_OUT_ENERGY_RECOVERY_RATE);
                tile *t = WorldPosGetTile(gg->playerPos);
                s32 floodTime = SeaLevelGetTime(t->h);
                gg->time = wakeupTime;
                gg->seaLevel = TimeGetSeaLevel(gg->time);
                if (wakeupTime >= floodTime){
                    gg->passedOutAndDrowned = true;
                    gg->playerHealth = 0;
                    dead = true;
                    gg->inventoryOpen = false;
                }else{
                    gg->playerEnergy = recoverEnergy;
                }
            }
            s32 maxTime = (s32)(gg->passedOutAndDrowned ? 60*7 : 60*4.5f);

            // Draw black rect
            f32 alpha = 1.f;
            if (gg->passedOutTimer < 60*1.f){
                alpha = Clamp01(gg->passedOutTimer/60.f);
            }else if (gg->passedOutTimer > maxTime - 60){
                alpha = Clamp01((maxTime - gg->passedOutTimer)/60.f);
            }
            f32 z = 97;
            if (alpha){
                DrawRect(V2(0), gi->windowDim, V4_Black(alpha), z);
            }
            // Draw strings.
            char *str1 = "You were so tired\nthat you passed out...";
            char *str2 = "and drowned.";
            f32 str1Alpha = alpha*Clamp01(((f32)gg->passedOutTimer - 60*.8f)/(60*.75f));
            f32 str2Alpha = alpha*Clamp01(((f32)gg->passedOutTimer - 60*2.5f)/(60*.75f));
            if (str1Alpha){
                DrawString(str1, CenterStringXY(str1, V2(.5f*gi->windowDim.x, .6f*gi->windowDim.y), V2(2.f)), V2(2.f), z, V4_White(str1Alpha));
            }
            if (gg->passedOutAndDrowned && str2Alpha){
                DrawString(str2, CenterStringXY(str2, V2(.5f*gi->windowDim.x, .4f*gi->windowDim.y), V2(2.f)), V2(2.f), z, V4_White()); // (not using the alpha looks better, more impact)
            }

            // Stop effect.
            if (gg->passedOutTimer > maxTime){
                gg->passedOutTimer = 0;
                if (gg->playerHealth == 0){
                    gg->timeOfDeath = gg->time;
                }
            }else{
                gg->passedOutTimer++;
            }
        }

        //
        // Sleep window
        //
        if (gg->sleepWindowIsOpen){
            v4 sleepColor = V4(1.f, .98f, .05f);

            f32 recoveryRate = (gg->sleepBedType == TileObject_HayBed ? HAYBED_ENERGY_RECOVERY_RATE :
                               (gg->sleepBedType == TileObject_SleepingBag ? SLEEPINGBAG_ENERGY_RECOVERY_RATE :
                                BED_ENERGY_RECOVERY_RATE));
            f32 energyRecoveryRate = recoveryRate;
            f32 healthRecoveryRate = recoveryRate*gg->playerHealthMax;
            f32 hungerRate = HUNGER_RATE/2;

            // Limit sleep
            f32 maxEnergyToRecover = 1.f - gg->playerEnergy;
            s32 maxHpToRecover = gg->playerHealthMax - gg->playerHealth;
            tile *playerTile = WorldPosGetTile(gg->sleepBedPos);
            s32 sinkTime = SeaLevelGetTime(playerTile->h);
            s32 stomachGoesEmptyTime = gg->time + gg->playerStomach/hungerRate;
            s32 energyGoesFullTime = gg->time + maxEnergyToRecover/recoveryRate;

            s32 maxFinalTime = Clamp((s32)sinkTime - 120, 0, MAX_TIME); // in frames
            maxFinalTime = Min(maxFinalTime, stomachGoesEmptyTime); // Can't sleep with empty stomach


            // Blackness overlay.
            DrawRect(V2(0), gi->windowDim, V4_Black(.3f), 90);
            
            // Panel
            v2 borderMargin = V2(50.f, 30.f);
            f32 extraBottomMargin = 30.f;
            v2 temp = gi->windowDim - 2.f*borderMargin - V2(0, extraBottomMargin);
            temp = V2(Max(200.f, temp.x), Max(150.f, temp.y));
            v2 panelDim = FitRatioInRect(4.f/3.f, temp);
            v2 panelPos = V2(0, extraBottomMargin) + (gi->windowDim - V2(0, extraBottomMargin))/2 - panelDim/2;
            DrawRect(panelPos, panelDim, V4_Grey(.8f), 90);
            DrawRectFrame(panelPos, panelDim, V2(3.f), V4_Black(), 90);

            // "Time has stopped"
            {
                char *str = "(time has stopped)";
                DrawString(str, CenterStringX(str, V2(panelPos.x + panelDim.x/2, panelPos.y - 40.f)), V2(1.f), 90, V4_White());
            }

            // Sea level bar (@CopyPaste from PS_Map)
            v2 barDim = V2(panelDim.x*.9f, 38.f);
            v2 barPos = panelPos + V2(panelDim.x/2, panelDim.y*.78f) - barDim/2;
            u16 seaLevel = gg->seaLevel;
            f32 seaLevelFactor = Clamp((seaLevel - INITIAL_SEA_LEVEL)/(f32)(MAX_SEA_LEVEL - INITIAL_SEA_LEVEL), 0.f, 1.f);
                // Background
            DrawRect(barPos, barDim, V4(.51f, .47f, .4f), 91);
                // Black frame
            DrawRectFrame(barPos, barDim, V2(3.f), V4_Black(), 91);
                // Filled part
            DrawRect(barPos, V2(seaLevelFactor*barDim.x, barDim.y), V4_Blue(), 91);

            s32 finalTime = gg->sleepWindowTimeAfterSleep;
            u16 finalSeaLevel = TimeGetSeaLevel(finalTime);
            f32 finalSeaLevelFactor = Clamp((finalSeaLevel - INITIAL_SEA_LEVEL)/(f32)(MAX_SEA_LEVEL - INITIAL_SEA_LEVEL), 0.f, 1.f);
            if (maxFinalTime > gg->time && PointInRect(gi->mousePos, barPos, barPos + barDim)){
                finalSeaLevelFactor = Clamp((gi->mousePos.x - barPos.x)/barDim.x, 0.f, 1.f);
                finalSeaLevel = INITIAL_SEA_LEVEL + (s32)((MAX_SEA_LEVEL - INITIAL_SEA_LEVEL)*finalSeaLevelFactor);
                finalTime = SeaLevelGetTime(finalSeaLevel);
                if (finalTime > maxFinalTime){
                    finalTime = maxFinalTime;
                    finalSeaLevel = TimeGetSeaLevel(finalTime);
                    finalSeaLevelFactor = Clamp((finalSeaLevel - INITIAL_SEA_LEVEL)/(f32)(MAX_SEA_LEVEL - INITIAL_SEA_LEVEL), 0.f, 1.f);
                }
                if (gi->mouseButtons[0].isDown){
                    gg->sleepWindowTimeAfterSleep = finalTime;
                }
            }
                // Current level mark
            DrawRect(barPos + V2(seaLevelFactor*barDim.x - 1.5f, 0), V2(3.f, barDim.y), V4_Black(), 91);
            
            if (finalSeaLevel > seaLevel){
                // Sleeped part
                DrawRect(barPos + V2(seaLevelFactor*barDim.x, 0), V2((finalSeaLevelFactor - seaLevelFactor)*barDim.x, barDim.y), sleepColor, 91);
            }
            
            // Draw Day/Night Marks
            {
                s32 day = 1;
                while(true){
                    day_and_daytime dad = {day, 0.f};
                    s32 time = DayAndDaytimeToTime(dad);
                    if (time > MAX_TIME)
                        break;
                    v2 pos = barPos + V2(Clamp01(time/(f32)MAX_TIME)*barDim.x, 0);
                    // Day start marker
                    DrawRect(pos, V2(3.f, barDim.y), V4_Black(), 91);

                    // Night highlight
                    f32 maxTimeInDays = MAX_TIME/(f32)DAY_LENGTH;
                    f32 daySizeInPixels = (barDim.x/maxTimeInDays);
                    f32 nightSizeInPixels = daySizeInPixels*((1.f - NIGHT_START) + NIGHT_END);
                    v2 nightStartPos = pos - V2(daySizeInPixels*(1.f - NIGHT_START), 0);
                    if (nightStartPos.x + nightSizeInPixels > barPos.x && nightStartPos.x < barPos.x + barDim.x){
                        nightStartPos.x = Max(barPos.x, nightStartPos.x);
                        f32 nightSize = Min(nightSizeInPixels, barPos.x + barDim.x - nightStartPos.x);
                        DrawRect(nightStartPos, V2(nightSize, barDim.y), V4_Black(.4f), 91);
                    }

                    // Day number
                    char str[24];
                    sprintf_s(str, ArrayCount(str), "day %i", day);
                    v2 strScale = V2(.5f);
                    v2 strPos = pos + V2(- daySizeInPixels/2, -20.f);
                    DrawString(str, CenterStringX(str, strPos, strScale.x), strScale, 91, V4_Black());

                    day++;
                }
            }
            

            v2 buttonDim = V2(380.f, 64.f);
            v2 buttonPos = panelPos + V2(panelDim.x/2 - buttonDim.x/2, 15.f);

            // Map
            v2 mapDim = V2((barPos.y - 40.f)  - (buttonPos.y + buttonDim.y + 25.f));
            //v2 mapPos = V2(panelPos.x + panelDim.x/2, (buttonPos.y + buttonDim.y + barPos.y - 20.f)/2) - mapDim/2;
            v2 mapPos = V2(barPos.x + mapDim.x/2, (buttonPos.y + buttonDim.y + barPos.y - 20.f)/2) - mapDim/2;
            f32 pointSize = 1.f;
            f32 maxPointsPerAxis = 200.f;
            while(mapDim.x/pointSize > maxPointsPerAxis){
                pointSize += 1.f;
            }
            DrawRect(mapPos, mapDim, WATER_COLOR_DEEP, 90);
            DrawMap(mapPos, mapDim, 1.f, pointSize, (u16)MaxS32(finalSeaLevel, seaLevel), 90, false);
            
            // Player pos icon
            {
                v2 texPos = V2(0, gg->images[TEX_GUI].dim.y - 352);
                v2 texDim = V2(32);
                v2 windowPos = mapPos + V2(mapDim.x*Clamp(gg->playerPos.x/(TILES_X*TILE_SIZE), 0.f, 1.f),
                                           mapDim.y*Clamp(gg->playerPos.y/(TILES_Y*TILE_SIZE), 0.f, 1.f));
                v2 scale = V2(1.f);
                v2 drawPos = windowPos - V2(scale.x*texDim.x/2, 0);
                DrawSpriteRect(&gg->images[TEX_GUI], texPos, texDim, drawPos, scale, 90, V4_Red()); 
            }

            //
            // Bars
            //

            f32 stomachAfterSleep = Clamp01(gg->playerStomach - MaxS32(0, finalTime - gg->time)*hungerRate);
            f32 energyAfterSleep = Clamp01(gg->playerEnergy + MaxS32(0, finalTime - gg->time)*energyRecoveryRate);
            s32 healthAfterSleep = Clamp(gg->playerHealth + (s32)(MaxS32(0, finalTime - energyGoesFullTime)*healthRecoveryRate), gg->playerHealth, gg->playerHealthMax);
            

            // Draw bars
            {
                v2 frameDim = V2(4.f);
                f32 barWidth = 30.f;
                v2 hpBarDim = V2(350.f, barWidth);
                v2 hpBarPos = V2(Lerp(mapPos.x + mapDim.x, panelPos.x + panelDim.x, .5f) - hpBarDim.x/2 - frameDim.x,
                                 buttonPos.y + buttonDim.y + 10.f + frameDim.y);

                f32 barSep = 16.f;
                v2 energyBarDim = V2(barWidth, 250.f);
                v2 energyBarPos = V2(hpBarPos.x + hpBarDim.x - energyBarDim.x, hpBarPos.y + hpBarDim.y + barSep);
                energyBarDim.y = Min(energyBarDim.y, Max(50.f, barPos.y - energyBarPos.y - 50.f));

                v2 stomachBarDim = energyBarDim;
                v2 stomachBarPos = energyBarPos + V2(-barSep - stomachBarDim.x, 0);
            
                // Bar frame color
                f32 d = .5f;
                v4 hpBarColor      = V4(0, 0, 0);//V4(1.f, .1f, .1f);
                v4 energyBarColor  = V4(.7f*d, 1.f*d, .2f*d);
                v4 stomachBarColor = V4(1.f*d, .6f*d, 0.1f*d);
                // Bar inside color
                f32 a = .8f;
                v4 hpColor      = V4(1.f, .1f, .1f, a);
                v4 energyColor  = V4(.7f, 1.f, .2f, a);
                v4 stomachColor = V4(1.f,.7f, 0.13f, a);
                v4 emptyColor   = V4_Black(.65f);
                v4 emptyHpColor = V4(.2f, 0.08f, 0.08f, .7f);
                
                // Draw bar insides
                    // HP bar
                f32 hp = gg->playerHealth / (f32)gg->playerHealthMax;
                f32 hpAfterSleep = healthAfterSleep / (f32)gg->playerHealthMax;
                if (hpAfterSleep != 1.f)
                    DrawRect(hpBarPos + V2(hpBarDim.x*hpAfterSleep, 0), V2(hpBarDim.x*(1.f - hpAfterSleep), hpBarDim.y), emptyHpColor, 90);
                if (hpAfterSleep > hp)
                    DrawRect(hpBarPos + V2(hpBarDim.x*hp, 0), V2(hpBarDim.x*(hpAfterSleep - hp), hpBarDim.y), sleepColor, 90);
                if (hp)
                    DrawRect(hpBarPos, V2(hpBarDim.x*hp, hpBarDim.y), hpColor, 90);
                    // HP bar lines
                for(s32 i = 1; i <= 9; i++){
                    DrawRect(hpBarPos + V2(i*hpBarDim.x/10.f - 2.f, 0), V2(4.f, hpBarDim.y), V4_Black(.5f), 90);
                }

                    // Energy bar
                f32 energy = gg->playerEnergy;
                if (energy != 1.f)
                    DrawRect(energyBarPos + V2(0, energyBarDim.y*energy), V2(energyBarDim.x, energyBarDim.y*(1.f-energy)), emptyColor, 90);
                if (energy)
                    DrawRect(energyBarPos, V2(energyBarDim.x, energyBarDim.y*energy), energyColor, 90);
                if (finalTime > gg->time){
                    DrawRect(energyBarPos + V2(0, energyBarDim.y*energy), V2(energyBarDim.x, energyBarDim.y*(energyAfterSleep - energy)), sleepColor, 90);
                }
            
                    // Stomach bar
                f32 stomach = gg->playerStomach;
                if (stomach != 1.f)
                    DrawRect(stomachBarPos + V2(0, stomachBarDim.y*stomach), V2(stomachBarDim.x, stomachBarDim.y*(1.f-stomach)), emptyColor, 90);
                if (stomach){
                    if (stomachAfterSleep){
                        DrawRect(stomachBarPos, V2(stomachBarDim.x, stomachBarDim.y*stomachAfterSleep), stomachColor, 90);
                    }
                    if (stomach - stomachAfterSleep > 0){
                        DrawRect(stomachBarPos + V2(0, stomachBarDim.y*stomachAfterSleep), V2(stomachBarDim.x, stomachBarDim.y*(stomach - stomachAfterSleep)), V4_Grey(.4f), 90);
                    }
                }

                DrawRectFrame(energyBarPos, energyBarDim, frameDim, energyBarColor, 90);
                DrawRectFrame(stomachBarPos, stomachBarDim, frameDim, stomachBarColor, 90);
                DrawRectFrame(hpBarPos, hpBarDim, frameDim, hpBarColor, 90);
            }


            // Text
            {
                // Sleep time
                char timeStr[100];
                days_hours_minutes dhm = {};
                if (finalTime > gg->time){
                    s32 sleepTime = finalTime - gg->time;
                    dhm = TimeInFramesToDaysHoursMinutes(sleepTime);
                }
                if (dhm.days){
                    sprintf_s(timeStr, ArrayCount(timeStr), "%id %0ih %0im", dhm.days, dhm.hours, dhm.minutes);
                }else{
                    sprintf_s(timeStr, ArrayCount(timeStr), "%0ih %0im", dhm.hours, dhm.minutes);
                }
                //v2 timeStrPos = V2(barPos.x, barPos.y - 72.f);//(panelPos.y + barPos.y)/2);
                v2 timeStrPos = CenterStringXY(timeStr, barPos + barDim/2, V2(1.f));
                DrawString(timeStr, timeStrPos, V2(1.f), 91, V4_White());
            


                // Title (bed type)
                char *bedStr = (gg->sleepBedType == TileObject_HayBed ? "Hay Bed" :
                              (gg->sleepBedType == TileObject_SleepingBag ? "Sleeping Bag" :
                              (gg->sleepBedType == TileObject_Bed ? "Bed" : "?")));
                char titleStr[100]; 
                sprintf_s(titleStr, ArrayCount(titleStr), "%s:  Sleep quality = %i%%",
                          bedStr, (s32)Round((recoveryRate / (f32)BED_ENERGY_RECOVERY_RATE)*100.f));
                v2 titleStrPos = barPos + V2(0, barDim.y + (panelPos.y + panelDim.y - (barPos.y + barDim.y))/2 - 14.f);
                DrawString(titleStr, titleStrPos, V2(1.f), 90, V4_Black());
            }
            
            // Sleep button
            b32 canSleep = (gg->sleepWindowTimeAfterSleep > gg->time);
            if (DoButton(2201, buttonPos, buttonDim, "SLEEP", 91, !canSleep, V2(2.f))){
                gg->sleepWindowIsOpen = false;
                gg->time = gg->sleepWindowTimeAfterSleep;
                gg->playerEnergy = energyAfterSleep;
                gg->playerHealth = healthAfterSleep;
                gg->playerStomach = stomachAfterSleep;
                gg->sleepGradientEffectTimer = gg->sleepGradientEffectMaxTime = 60;
                gg->playerPos = gg->sleepBedPos - V2(0, TILE_SIZE/2);
            }
            // Cancel button
            buttonDim = V2(55.f, 46.f);
            buttonPos = panelPos + V2(15.f, 15.f);
            if (DoButton(2202, buttonPos, buttonDim, "<")){
                gg->sleepWindowIsOpen = false;
            }

        }else if (gg->noteIsOpen){
        //
        // Note window
        //
            // Blackness overlay.
            DrawRect(V2(0), gi->windowDim, V4_Black(.3f), 90);
            
            // Make text
            s32 noteIndex = ClampS32(gg->openNote, 0, ArrayCount(globalNotes) - 1);
            char str[700];
            sprintf_s(str, ArrayCount(str), "%s\n\n(Tip %i/%i)", globalNotes[noteIndex], noteIndex + 1, ArrayCount(globalNotes));
            v2 strDim = StringDim(str);

            // Panel
            f32 noteMargin = 50.f;
            f32 spaceBelowString = 100.f;
            v2 panelDim = strDim + V2(noteMargin*2, noteMargin*2 + spaceBelowString);
            v2 panelPos = (gi->windowDim - panelDim)/2;
            DrawRect(panelPos, panelDim, V4_Grey(.8f), 90);
            DrawRectFrame(panelPos, panelDim, V2(3.f), V4_Black(), 90);

            // String
            v2 strCenterPos = V2(panelPos.x + panelDim.x/2, panelPos.y + spaceBelowString + (panelDim.y - spaceBelowString)/2);
            v2 strPos = CenterStringXY(str, strCenterPos, V2(1.f));
            DrawString(str, strPos, V2(1.f), 90, V4_Black());

            // "Time has stopped"
            {
                char *str = "(time has stopped)";
                DrawString(str, CenterStringX(str, V2(panelPos.x + panelDim.x/2, panelPos.y - 40.f)), V2(1.f), 90, V4_White());
            }
            

            // Cancel button
            v2 buttonDim = V2(55.f, 46.f);
            v2 buttonPos = panelPos + V2(15.f, 15.f);
            if (DoButton(2202, buttonPos, buttonDim, "<")){
                gg->noteIsOpen = false;
            }
            if (ButtonWentDown(&gi->mouseButtons[0])){
                if (!PointInRect(gi->mousePos, panelPos, panelPos + panelDim)){
                    gg->noteIsOpen = false;
                }
            }
        }

        if (!dead && (!pauseGameplay || gg->pause)){
            if (ButtonWentDown(&gi->keyboard.letters['E' - 'A'])){
                gg->inventoryOpen = !gg->inventoryOpen;
            }
        }
        // Opening/closing inventory can be done even if paused, 
        if (!pauseGameplay){
            gg->animationTime++;

            if (dead){
                gg->playerIsRunning = false;
                gg->playerIsWalking = false;
                gg->playerSpeed *= .95f;
            }else{
                //
                // Input
                //
                if (ButtonWentDown(&gi->keyboard.numbers[0])){
                    gg->selectedItemSlot = 9;
                }
                for(s32 i = 1; i <= 9; i++){
                    if (ButtonWentDown(&gi->keyboard.numbers[i])){
                        gg->selectedItemSlot = i - 1;
                    }
                }
                // Mouse wheel moves selected item.
                if (gi->mouseWheel < 0){
                    gg->selectedItemSlot = (gg->selectedItemSlot - gi->mouseWheel) % 10;
                }else  if (gi->mouseWheel > 0){
                    gg->selectedItemSlot = (gg->selectedItemSlot + 10 + (-gi->mouseWheel % 10)) % 10;
                }


                static v2 inputMoveDir = {};
                b32 inputUp = (gi->keyboard.arrowUp.isDown       || gi->keyboard.letters['W' - 'A'].isDown);
                b32 inputLeft = (gi->keyboard.arrowLeft.isDown   || gi->keyboard.letters['A' - 'A'].isDown);
                b32 inputDown = (gi->keyboard.arrowDown.isDown   || gi->keyboard.letters['S' - 'A'].isDown);
                b32 inputRight = (gi->keyboard.arrowRight.isDown || gi->keyboard.letters['D' - 'A'].isDown);
                if (inputLeft && inputRight){
                    if (!inputMoveDir.x)
                        inputMoveDir.x = 1.f; // Press both at once: go right.
                }else{
                    inputMoveDir.x = (inputLeft ? -1.f : (inputRight ? 1.f : 0)); 
                }
                if (inputDown && inputUp){
                    if (!inputMoveDir.y)
                        inputMoveDir.y = 1.f; // Press both at once: go up.
                }else{
                    inputMoveDir.y = (inputDown ? -1.f : (inputUp ? 1.f : 0)); 
                }

                gg->playerIsRunning = false;
                gg->playerIsWalking = false;
                if (inputMoveDir.x || inputMoveDir.y){
                    if (gi->keyboard.shift.isDown){
                        gg->playerIsRunning = true;
                    }else{
                        gg->playerIsWalking = true;
                    }
                }

                v2 targetSpeed = {};
                f32 moveSpeed = 8.f * (gg->playerIsRunning ? 2.f : 1.f);
                if (gg->debug.runSpeedX5 && gg->playerIsRunning){
                    moveSpeed *= 5;
                }

                if (Length(inputMoveDir) > 1.f){
                    targetSpeed = moveSpeed*inputMoveDir / Length(inputMoveDir);
                }else{
                    targetSpeed = moveSpeed*inputMoveDir;
                }
                f32 movementControlFactor = .5f;
                if (gg->knockbackTimerMax > 0){
                    movementControlFactor = LerpClamp(0.f, movementControlFactor, 1.f - (gg->knockbackTimer / (f32)gg->knockbackTimerMax));
                }
                gg->playerSpeed = LerpV2(gg->playerSpeed, targetSpeed, movementControlFactor);
            }

            gg->playerPos += gg->playerSpeed;
            gg->knockbackTimer = MaxS32(0, gg->knockbackTimer -1);

            
            // Face left
            if (Abs(gg->playerSpeed.x) > 0.1f)
                gg->playerFacingLeft = gg->playerSpeed.x < 0;

        }
                    
        v2 playerDim = V2(100.f, 140.f);
        gg->camPos = gg->playerPos + V2(0, playerDim.y*.5f);
        v2 viewPos = gg->camPos - gi->windowDim/2;
        tile *playerTile = WorldPosGetTile(gg->playerPos);
        u16 playerTileH = playerTile->h;
        
        b32 inWater = false;
        if (playerTileH < gg->seaLevel){
            inWater = true;
        }

        //
        // Light map
        //
        f32 daylight = 1.f;
        f32 dayNightTransitionTime = 5*60.f/(f32)DAY_LENGTH;
        auto currentTime = TimeToDayAndDaytime(gg->time);
        if (currentTime.daytime < NIGHT_END || currentTime.daytime >= NIGHT_START){
            daylight = 0;
        }else if (currentTime.daytime < NIGHT_END + dayNightTransitionTime){
            daylight = Clamp01((currentTime.daytime - NIGHT_END) / dayNightTransitionTime);
        }else if (currentTime.daytime > NIGHT_START - dayNightTransitionTime){
            daylight = Clamp01(1.f - (currentTime.daytime - (NIGHT_START - dayNightTransitionTime))/dayNightTransitionTime);
        }
        {
            v2 lightMapMin = viewPos - V2(TILE_SIZE*4.f, TILE_SIZE*8.f);
            v2 lightMapMax = viewPos + gi->windowDim + V2(TILE_SIZE*4.f, TILE_SIZE*4.f);
            v2s lightMapMinTile = WorldPosGetTilePos(lightMapMin);
            v2s lightMapMaxTile = WorldPosGetTilePos(lightMapMax); // inclusive

            gg->lightMapSize = lightMapMaxTile - lightMapMinTile + V2S(1);
            gg->lightMapMinTile = lightMapMinTile;
            s32 lightMapNumTiles = gg->lightMapSize.x*gg->lightMapSize.y;

            // Clear lightmap with ambient light
            v4 lightCol = V4(daylight, daylight, daylight, 1.f);
            for(s32 i = 0; i < lightMapNumTiles; i++){
                gg->lightMap[i] = lightCol;
            }
            v4 fireCol = V4(1.f, .6f, 0.45f, 1.f);

            if (daylight < 1.f){ // Not fully daylight
                // Apply light sources
                s32 maxLightsourceRange = 20;
                v2s lightsourceCheckMinTile = V2S(MaxS32(0, lightMapMinTile.x - maxLightsourceRange), MaxS32(0, lightMapMinTile.y - maxLightsourceRange));
                v2s lightsourceCheckMaxTile = V2S(MinS32(TILES_X, lightMapMaxTile.x + maxLightsourceRange), MinS32(TILES_Y, lightMapMaxTile.y + maxLightsourceRange));
                v2 lightMapPos0 = V2(gg->lightMapMinTile)*TILE_SIZE + V2(TILE_SIZE/2.f);
                for(s32 ty = lightsourceCheckMinTile.y; ty <= lightsourceCheckMaxTile.y; ty++){
                    for(s32 tx = lightsourceCheckMinTile.x; tx <= lightsourceCheckMaxTile.x; tx++){
                        v2s tPos = {tx, ty};
                        tile t = globalGame.tiles[TILES_X*ty + tx];
                        if (t.object >= TileObject_Campfire1 && t.object <= TileObject_Campfire9){
                            f32 firePower = Clamp01((((s32)t.object - (s32)TileObject_Campfire0)*CAMPFIRE_SHRINK_TIME + t.objectData)/(10.f*CAMPFIRE_SHRINK_TIME)); 
                            f32 lightRadius = TILE_SIZE*15.f*firePower;
                            v2 lightsourcePos = V2(tPos)*TILE_SIZE + V2(TILE_SIZE/2.f);


                            // Apply fire light to lightmap
                            f32 maxDisSqr = lightRadius*lightRadius;
                            for(s32 lx = 0; lx < gg->lightMapSize.x; lx++){
                                for(s32 ly = 0; ly < gg->lightMapSize.y; ly++){
                                    v2 worldPos = lightMapPos0 + V2(lx*(f32)TILE_SIZE, ly*(f32)TILE_SIZE);
                                    f32 disSqr = LengthSqr(worldPos - lightsourcePos);
                                    if (disSqr < maxDisSqr){
                                        f32 light = Lerp(1.f, 0.f, disSqr/maxDisSqr);
                                        v4 *lightMapCol = &gg->lightMap[gg->lightMapSize.x*ly + lx];
                                        //lightMapCol->r = Min(1.f, lightMapCol->r + light*fireCol.r);
                                        //lightMapCol->g = Min(1.f, lightMapCol->g + light*fireCol.g);
                                        //lightMapCol->b = Min(1.f, lightMapCol->b + light*fireCol.b);
                                        lightMapCol->r = Max(lightMapCol->r, fireCol.r*light);
                                        lightMapCol->g = Max(lightMapCol->g, fireCol.g*light);
                                        lightMapCol->b = Max(lightMapCol->b, fireCol.b*light);
                                    }
                                }
                            }
                        }
                    }
                }

                // Apply player light source
                {
                    f32 lightRadius = TILE_SIZE*4.f;
                    f32 factor = .3f;
                    v4 lightCol = V4(fireCol.r*factor, fireCol.g*factor, fireCol.b*factor, 1.f);
                    v2 lightsourcePos = gg->playerPos;
                    f32 maxDisSqr = lightRadius*lightRadius;
                    for(s32 lx = 0; lx < gg->lightMapSize.x; lx++){
                        for(s32 ly = 0; ly < gg->lightMapSize.y; ly++){
                            v2 worldPos = lightMapPos0 + V2(lx*(f32)TILE_SIZE, ly*(f32)TILE_SIZE);
                            f32 disSqr = LengthSqr(worldPos - lightsourcePos);
                            if (disSqr < maxDisSqr){
                                f32 light = Lerp(1.f, 0.f, disSqr/maxDisSqr);
                                v4 *lightMapCol = &gg->lightMap[gg->lightMapSize.x*ly + lx];
                                lightMapCol->r = Min(1.f, lightMapCol->r + light*lightCol.r);
                                lightMapCol->g = Min(1.f, lightMapCol->g + light*lightCol.g);
                                lightMapCol->b = Min(1.f, lightMapCol->b + light*lightCol.b);
                            }
                        }
                    }
                }
            }
        }
                            
        // Render player
        if (dead){
            s32 subimage = 0;
            v2 subimageDim = V2(112, 192);
            v2 texPos = V2(subimageDim.x*(subimage % 2), gg->images[TEX_PLAYER].dim.y - (1 + (subimage / 2))*subimageDim.y);
            DrawSpriteRectRotated(&gg->images[TEX_PLAYER], texPos, subimageDim, texPos + subimageDim/2, gg->playerPos - viewPos, PI/2, V2(1.f), WindowYGetZ(gg->playerPos.y - viewPos.y), LightMapColor(gg->playerPos));
        }else{
            v2 subimageDim = V2(112, 192);
            s32 subimage = 0;
            if (inWater){
                s32 animationSpeed = (gg->playerIsRunning ? 6 : (gg->playerIsWalking ? 8 : 14));
                subimage = 4 + ((gg->animationTime / animationSpeed) % 2);
            }else if (gg->playerIsWalking || gg->playerIsRunning){
                s32 animationSpeed = (gg->playerIsRunning ? 5 : 10);
                subimage = 2 + ((gg->animationTime / animationSpeed) % 2);
            }
            v2 playerFeetOffset = V2(subimageDim.x/2, 16);
            v2 texPos = V2(subimageDim.x*(subimage % 2), gg->images[TEX_PLAYER].dim.y - (1 + (subimage / 2))*subimageDim.y);
            v2 finalTexPos = texPos;

            if (inWater){
                f32 deepness = Clamp01(((f32)gg->seaLevel - (f32)playerTileH)/1300.f);
                f32 cutPixels = Lerp(.41f, .70f, deepness)*subimageDim.y;
                finalTexPos.y += cutPixels;
                subimageDim.y -= cutPixels;
            }
            s32 z = WindowYGetZ(gg->playerPos.y - viewPos.y);
            v4 col = LightMapColor(gg->playerPos);
            v2 drawPos = gg->playerPos - viewPos - playerFeetOffset;
            DrawSpriteRect(&gg->images[TEX_PLAYER], finalTexPos, subimageDim, drawPos, V2(1.f), z, col, gg->playerFacingLeft); 
            // Armor
            f32 armorXOff = 224;
            if (gg->inventoryArmor[0].id == Item_LeatherHelmet){
                f32 helmetY0 = texPos.y + 144.f;
                f32 helmetY1 = texPos.y + 192.f;
                if (finalTexPos.y < helmetY1){
                    v2 texPos2 = V2(finalTexPos.x + armorXOff, Max(helmetY0, finalTexPos.y));
                    v2 subimageDim2 = V2(subimageDim.x, helmetY1 - texPos2.y);
                    v2 drawPos2 = drawPos + V2(0, Max(0, texPos2.y - finalTexPos.y));
                    DrawSpriteRect(&gg->images[TEX_PLAYER], texPos2, subimageDim2, drawPos2, V2(1.f), z, col, gg->playerFacingLeft); 
                }
            }
            if (gg->inventoryArmor[1].id == Item_LeatherJacket){
                f32 jacketY0 = texPos.y + 80.f;
                f32 jacketY1 = texPos.y + 144.f;
                if (finalTexPos.y < jacketY1){
                    v2 texPos2 = V2(finalTexPos.x + armorXOff, Max(jacketY0, finalTexPos.y));
                    v2 subimageDim2 = V2(subimageDim.x, jacketY1 - texPos2.y);
                    v2 drawPos2 = drawPos + V2(0, Max(0, texPos2.y - finalTexPos.y));
                    DrawSpriteRect(&gg->images[TEX_PLAYER], texPos2, subimageDim2, drawPos2, V2(1.f), z, col, gg->playerFacingLeft); 
                }
            }
            if (gg->inventoryArmor[2].id == Item_LeatherBoots){
                f32 bootsY0 = texPos.y + 0.f;
                f32 bootsY1 = texPos.y + 64.f;
                if (finalTexPos.y < bootsY1){
                    v2 texPos2 = V2(finalTexPos.x + armorXOff, Max(bootsY0, finalTexPos.y));
                    v2 subimageDim2 = V2(subimageDim.x, bootsY1 - texPos2.y);
                    v2 drawPos2 = drawPos + V2(0, Max(0, texPos2.y - finalTexPos.y));
                    DrawSpriteRect(&gg->images[TEX_PLAYER], texPos2, subimageDim2, drawPos2, V2(1.f), z, col, gg->playerFacingLeft); 
                }
            }
        }

        //
        // Temperature
        //
            // Cold/Hot limits
        f32 temperatureSafezoneSize = .4f;
        if (gg->inventoryArmor[0].id == Item_LeatherHelmet)
            temperatureSafezoneSize += .05f;
        if (gg->inventoryArmor[1].id == Item_LeatherJacket)
            temperatureSafezoneSize += .05f;
        if (gg->inventoryArmor[2].id == Item_LeatherBoots)
            temperatureSafezoneSize += .05f;
        temperatureSafezoneSize = Min(1.f, temperatureSafezoneSize);
        f32 heatLimit = .5f + temperatureSafezoneSize/2;
        f32 coldLimit = .5f - temperatureSafezoneSize/2;
        f32 extremeHeatLimit = Lerp(heatLimit, 1.f, .5f);
        f32 extremeColdLimit = Lerp(coldLimit, 0.f, .5f);

        // Campfire heat
        f32 playerCampfireHeat = 0.f;
        for(s32 i = 0; i < gg->numTilesWithBehaviour; i++){
            tile *t = gg->tilesWithBehaviour[i];
            if (t->object >= TileObject_Campfire1 && t->object <= TileObject_Campfire9){
                v2s tPos = TileGetGridPos(t);
                v2 pos = V2(tPos)*TILE_SIZE + V2(TILE_SIZE/2.f);

                f32 firePower = Clamp01((((s32)t->object - (s32)TileObject_Campfire0)*CAMPFIRE_SHRINK_TIME + t->objectData)/(10.f*CAMPFIRE_SHRINK_TIME)); 
                f32 heatRadius = TILE_SIZE*15.f*firePower;
                if (LengthSqr(gg->playerPos - pos) > Square(heatRadius))
                    continue;

                f32 playerDistanceNormalized = Clamp01(Length(gg->playerPos - pos)/heatRadius);
                f32 heat = Square(1.f - playerDistanceNormalized)*.4f + // Quadratic component
                            (1.f - playerDistanceNormalized)*.4f; // Linear component
                heat *= firePower;
                playerCampfireHeat += heat;
            }
        }

            // Exterior temperature
        f32 exteriorTemperature = LerpClamp(.68f, .1f, ((f32)playerTileH - INITIAL_SEA_LEVEL)/((f32)MAX_SEA_LEVEL - INITIAL_SEA_LEVEL));
        if (playerTile->biomeData == Biome_Dry) // Dry biome heat
            exteriorTemperature += .1f; 
        exteriorTemperature -= Clamp01(1.f - daylight)*.3f; // Night cold
        exteriorTemperature += playerCampfireHeat; // Campfire
        exteriorTemperature = Clamp01(exteriorTemperature);


            // Update body temperature and energy.
        if (!pauseGameplay && !dead){
            if (inWater)
                exteriorTemperature = Min(exteriorTemperature, .2f);
            //gg->playerTemperature = MoveValueTowards(gg->playerTemperature, exteriorTemperature, 0.2f/(60*60));
            gg->playerTemperature = Lerp(gg->playerTemperature, exteriorTemperature, 0.002f);

            // Energy
            if (inWater)
                gg->playerEnergy -= 1.f/(60*60*5);

            if (gg->playerIsWalking || gg->playerIsRunning)
                gg->playerEnergy -= 1.f/(60*60*5);

            if (gg->playerIsRunning)
                gg->playerEnergy -= 1.f/(60*60*5);

            if (gg->playerTemperature < coldLimit || gg->playerTemperature > heatLimit)
                gg->playerEnergy -= 1.f/(60*60*5);

            if (gg->playerStomach == 0.f)
                gg->playerEnergy -= 1.f/(60*60*5);

            gg->playerEnergy = Max(0.f, gg->playerEnergy);
        }
        temperature_state_type temperatureState = (gg->playerTemperature < extremeColdLimit ? Temperature_ExtremeCold :
                                                  (gg->playerTemperature < coldLimit ? Temperature_Cold :
                                                  (gg->playerTemperature <= heatLimit ? Temperature_Safe : 
                                                  (gg->playerTemperature <= extremeHeatLimit ? Temperature_Heat :
                                                   Temperature_ExtremeHeat))));
        // Hurt player by temperature
        if (!pauseGameplay && !dead){
            if (temperatureState == Temperature_ExtremeCold || temperatureState == Temperature_ExtremeHeat){
                gg->playerEnergy = Max(0.f, gg->playerEnergy - 1.f/(60*60*5)); // 5 minute total depletion
                if (gg->time % 60 == 0){
                    HurtPlayer(1);
                }
            }else if (temperatureState == Temperature_Cold || temperatureState == Temperature_Heat){
                gg->playerEnergy = Max(0.f, gg->playerEnergy - 1.f/(60*60*5)); // 5 minute total depletion
            }
        }
        // Hurt player by hunger
        if (gg->playerStomach == 0.f){
            if (gg->time % 60 == 0){
                HurtPlayer(1);
            }
        }



        // Draw tiles
        v2 roundedViewPos = RoundV2(viewPos);
        s32 minTileX = ClampS32((roundedViewPos.x - TILE_SIZE/2)/TILE_SIZE, 0, TILES_X - 1);
        s32 minTileY = ClampS32((roundedViewPos.y - TILE_SIZE/2)/TILE_SIZE, 0, TILES_Y - 1);
        s32 maxTileX = ClampS32((roundedViewPos.x + gi->windowDim.x)/TILE_SIZE, 0, TILES_X - 1);
        s32 maxTileY = ClampS32((roundedViewPos.y + gi->windowDim.y)/TILE_SIZE, 0, TILES_Y - 1);
        for(s32 ty = minTileY; ty <= maxTileY; ty++){
            for(s32 tx = minTileX; tx <= maxTileX; tx++){
                tile t = gg->tiles[TILES_X*ty + tx];
                v2 drawPos = V2(tx, ty)*TILE_SIZE - roundedViewPos;

                v4 lightMapCol = LightMapColor(V2S(tx, ty));

                f32 relativeLevelRange = 3000.f;
                s32 z = 10;
                v4 cols[4]; // Order: CW from 0,0
                s32 colsIndices[4] = {0, 3, 1, 2};

                v4 col = TileGetColor(t);
                if(t.h > gg->seaLevel){
                    DrawSpriteRect(&gg->images[TEX_TILES], V2((s32)t.biomeData*64, 64), V2(64), drawPos, V2(1.f), z, Hadamard(col, lightMapCol));
                }else{
                    DrawRect(drawPos, V2(TILE_SIZE), Hadamard(col, lightMapCol), z);
                }
                if (t.object){
                    if (t.object == TileObject_Tree){
                        if (t.h > gg->seaLevel){
                            v2 texDim = V2(64*3, 64*4);
                            v2 texPos = V2(448, 64);
                            DrawSpriteRect(&gg->images[TEX_TILES], texPos, texDim, drawPos + V2(32.f - texDim.x/2, 0), V2(1.f), WindowYGetZ(drawPos.y + 26.f), lightMapCol);
                        }
                    }else if (t.h > gg->seaLevel){
                        v2 texPos = V2(64.f*((s32)t.object % 10), gg->images[TEX_TILES].dim.y - 64 - 64.f*((s32)t.object / 10));
                        DrawSpriteRect(&gg->images[TEX_TILES], texPos, V2(64), drawPos, V2(1.f), WindowYGetZ(drawPos.y + 50.f), lightMapCol);
                    }
                }

                //if (gi->keyboard.letters['O' - 'A'].isDown){
                //  v4 col = TileGetColor(t);
                //  DrawColoredQuad(pos, pos + V2(0, TILE_SIZE), pos + V2(TILE_SIZE), pos + V2(TILE_SIZE, 0),
                //                  col, col, col, col, z);
                //}
            }
        }
        
        // Draw tall tile objects
        maxTileY = Max(0, minTileY - 1);
        minTileY = Max(0, minTileY - 3);
        for(s32 ty = minTileY; ty <= maxTileY; ty++){
            for(s32 tx = minTileX; tx <= maxTileX; tx++){
                tile t = gg->tiles[TILES_X*ty + tx];

                if (t.object == TileObject_Tree){
                    v2 pos = V2(tx, ty)*TILE_SIZE - viewPos;
                    if (t.h > gg->seaLevel){
                        v2 texDim = V2(64*3, 64*4);
                        v2 texPos = V2(448, 64);
                        v4 lightMapCol = LightMapColor(V2S(tx, ty));
                        DrawSpriteRect(&gg->images[TEX_TILES], texPos, texDim, pos + V2(32.f - texDim.x/2, 0), V2(1.f), WindowYGetZ(pos.y + 26.f), lightMapCol);
                    }
                }
            }
        }
        
        //
        // Update/Render entities
        //
        if (!pauseGameplay){
            for(s32 i = 0; i < gg->numEntities; i++){
                entity *e = &gg->entities[i];
                UpdateEntity(e);
                RenderEntity(e);
            }
            
            //
            // Update tiles with behaviour
            //
            for(s32 i = 0; i < gg->numTilesWithBehaviour;){
                s32 iNext = i + 1;
                tile *t = gg->tilesWithBehaviour[i];
                switch(t->object){
                case TileObject_Campfire9:
                case TileObject_Campfire8:
                case TileObject_Campfire7:
                case TileObject_Campfire6:
                case TileObject_Campfire5:
                case TileObject_Campfire4:
                case TileObject_Campfire3:
                case TileObject_Campfire2:
                case TileObject_Campfire1:
                {
                    // objectData acts as a timer to reduce the flame strength.
                    if (t->objectData > 0){
                        t->objectData--;
                    }else{ // Go to previous state
                        t->object = (tile_object_type)((s32)t->object - 1);
                        if (t->object == TileObject_Campfire0){
                            t->objectData = 0;
                        }else{
                            t->objectData = CAMPFIRE_SHRINK_TIME;
                        }
                    }
                    if (gg->seaLevel >= t->h){
                        t->object = (tile_object_type)0;
                        t->objectData = 0;
                        RemoveTileWithBehaviour(t);
                        iNext = i;
                    }
                } break;
                }
                i = iNext;
            }
        }else{
            for(s32 i = 0; i < gg->numEntities; i++){
                entity *e = &gg->entities[i];
                RenderEntity(e);
            }
        }



        // Draw Inventory
        s32 itemFrameSize = 52.f;
        v2 borderMargin = V2(20.f, 20.f);
        f32 frameMargin = 10.f;

        b32 mouseOnGui = false;
        v2 topInventoryPos = V2(borderMargin.x, gi->windowDim.y - borderMargin.y - itemFrameSize);
        v2 topInventoryDim = V2(10*itemFrameSize + 9*frameMargin, itemFrameSize);
        if (!gg->inventoryOpen){
            // Drop holding item
            if (gg->holdingItem.id){
                if (gg->holdingItem.numItems){
                    DropItem(gg->holdingItem);
                }
                gg->holdingItem.id = (item_id)0;
                gg->holdingItem.numItems = 0;
            }

            // Draw top inventory
            for(s32 i = 0; i < 10; i++){
                v2 pos = topInventoryPos + V2(i*(itemFrameSize + frameMargin), 0);
                // Select slot by click
                if (ButtonWentDown(&gi->mouseButtons[0]) && PointInRect(gi->mousePos, pos, pos + V2(itemFrameSize))){
                    gg->selectedItemSlot = i;
                }
                // Draw slot
                if (gg->selectedItemSlot == i){
                    f32 extraMargin = 4.f;
                    DrawRect(pos, V2(itemFrameSize), V4_Grey(.8f), 85);
                    DrawRectFrame(pos, V2(itemFrameSize), V2(extraMargin), V4_Black(), 85);
                
                }else{
                    DrawRect(pos, V2(itemFrameSize), V4_Grey(.6f), 85);
                }

                DrawItem(pos + V2(itemFrameSize/2), gg->inventoryTop[i].id, gg->inventoryTop[i].numItems, 85);
            }
        }else{
            // Inventory open
            v2 selectedSlotPos = topInventoryPos + V2(gg->selectedItemSlot*(itemFrameSize + frameMargin), 0);
            DrawRect(selectedSlotPos - V2(4.f), V2(itemFrameSize) + V2(4.f*2), V4_Black(), 85);
            
            f32 inventoryTopSep = 20.f;
            for(s32 i = 0; i < ArrayCount(gg->inventoryAll); i++){
                s32 row    = i / 10;
                s32 column = i % 10;
                v2 pos = topInventoryPos + V2(column*(itemFrameSize + frameMargin), -row*(itemFrameSize + frameMargin));
                if (row > 0){
                    pos.y -= inventoryTopSep;
                }
                
                if (!pauseGameplay && PointInRect(gi->mousePos, pos, pos + V2(itemFrameSize))){
                    if (!gg->holdingItem.id){
                        // Grab item.
                        if (ButtonWentDown(&gi->mouseButtons[0])){
                            gg->holdingItem.id       = gg->inventoryAll[i].id;
                            gg->holdingItem.numItems = gg->inventoryAll[i].numItems;

                            gg->inventoryAll[i].id = (item_id)0;
                            gg->inventoryAll[i].numItems = 0;

                        }else if (ButtonWentDown(&gi->mouseButtons[2])){
                            gg->holdingItem.id = gg->inventoryAll[i].id;
                            gg->holdingItem.numItems = (gg->inventoryAll[i].numItems /2) + (gg->inventoryAll[i].numItems % 2); // Ceil(numItems/2)

                            gg->inventoryAll[i].numItems /= 2;
                            if (gg->inventoryAll[i].numItems == 0){
                                gg->inventoryAll[i].id = (item_id)0;
                            }
                        }
                    }else{
                        // Put item.
                        b32 lmb = ButtonWentDown(&gi->mouseButtons[0]);
                        b32 rmb = ButtonWentDown(&gi->mouseButtons[2]);
                        if (lmb || rmb){
                            s32 numItemsToPut = (lmb ? gg->holdingItem.numItems : 1);
                            if (!gg->inventoryAll[i].id){
                                // No item: put.
                                gg->inventoryAll[i].id       = gg->holdingItem.id;
                                gg->inventoryAll[i].numItems = numItemsToPut;

                                gg->holdingItem.numItems -= numItemsToPut;
                                if (gg->holdingItem.numItems == 0)
                                    gg->holdingItem.id = (item_id)0;

                            }else if (gg->inventoryAll[i].id == gg->holdingItem.id){
                                // Same item: stack
                                auto itemProperties = globalItemProperties[gg->holdingItem.id];
                                if (!(itemProperties.flags & ItemFlag_Unstackable)){
                                    gg->inventoryAll[i].numItems += numItemsToPut;
                                
                                    gg->holdingItem.numItems -= numItemsToPut;
                                    if (gg->holdingItem.numItems == 0)
                                        gg->holdingItem.id = (item_id)0;
                                }
                            }else if (lmb){
                                // Different items: swap.
                                auto newHoldingItem = gg->inventoryAll[i];
                                gg->inventoryAll[i] = gg->holdingItem;
                                gg->holdingItem = newHoldingItem;
                            }

                        }
                    }
                }

                
                if (gg->selectedItemSlot == i){
                    f32 extraMargin = 4.f;
                    DrawRect(pos - V2(extraMargin), V2(itemFrameSize) + V2(extraMargin*2), V4_Black(), 85);
                
                    DrawRect(pos, V2(itemFrameSize), V4_Grey(.8f), 85);
                }else{
                    DrawRect(pos, V2(itemFrameSize), V4_Grey(.6f), 85);
                }

                DrawItem(pos + V2(itemFrameSize/2), gg->inventoryAll[i].id, gg->inventoryAll[i].numItems, 85);

            }
            //
            // Armor slots
            //
            for(s32 i = 0; i < 3; i++){
                s32 row    = i + 1;
                s32 column = 11;
                v2 pos = topInventoryPos + V2(column*(itemFrameSize + frameMargin), -row*(itemFrameSize + frameMargin));
                if (row > 0){
                    pos.y -= inventoryTopSep;
                }
                
                if (!pauseGameplay && PointInRect(gi->mousePos, pos, pos + V2(itemFrameSize))){
                    if (!gg->holdingItem.id){
                        // Grab item.
                        if (ButtonWentDown(&gi->mouseButtons[0])){
                            gg->holdingItem.id       = gg->inventoryArmor[i].id;
                            gg->holdingItem.numItems = gg->inventoryArmor[i].numItems;

                            gg->inventoryArmor[i].id = (item_id)0;
                            gg->inventoryArmor[i].numItems = 0;

                        }else if (ButtonWentDown(&gi->mouseButtons[2])){
                            gg->holdingItem.id = gg->inventoryArmor[i].id;
                            gg->holdingItem.numItems = (gg->inventoryArmor[i].numItems /2) + (gg->inventoryArmor[i].numItems % 2); // Ceil(numItems/2)

                            gg->inventoryArmor[i].numItems /= 2;
                            if (gg->inventoryArmor[i].numItems == 0){
                                gg->inventoryArmor[i].id = (item_id)0;
                            }
                        }
                    }else{
                        // Put item.
                        b32 lmb = ButtonWentDown(&gi->mouseButtons[0]);
                        b32 rmb = ButtonWentDown(&gi->mouseButtons[2]);
                        if (lmb || rmb){
                            s32 numItemsToPut = (lmb ? gg->holdingItem.numItems : 1);
                            b32 holdingItemIsAllowedInSlot = false;
                            if (i == 0){
                                holdingItemIsAllowedInSlot = (gg->holdingItem.id == Item_LeatherHelmet);
                            }else if (i == 1){
                                holdingItemIsAllowedInSlot = (gg->holdingItem.id == Item_LeatherJacket);
                            }else if (i == 2){
                                holdingItemIsAllowedInSlot = (gg->holdingItem.id == Item_LeatherBoots);
                            }
                            if (!gg->inventoryArmor[i].id){
                                // No item: put.
                                if (holdingItemIsAllowedInSlot){
                                    gg->inventoryArmor[i].id       = gg->holdingItem.id;
                                    gg->inventoryArmor[i].numItems = numItemsToPut;

                                    gg->holdingItem.numItems -= numItemsToPut;
                                    if (gg->holdingItem.numItems == 0)
                                        gg->holdingItem.id = (item_id)0;
                                }

                            }else if (gg->inventoryAll[i].id == gg->holdingItem.id){
                                // Same item: stack.
                                auto itemProperties = globalItemProperties[gg->holdingItem.id];
                                if (!(itemProperties.flags & ItemFlag_Unstackable)){
                                    gg->inventoryAll[i].numItems += numItemsToPut;
                                
                                    gg->holdingItem.numItems -= numItemsToPut;
                                    if (gg->holdingItem.numItems == 0)
                                        gg->holdingItem.id = (item_id)0;
                                }
                            }else if (lmb){
                                // Different item: swap.
                                auto newHoldingItem = gg->inventoryAll[i];
                                gg->inventoryAll[i] = gg->holdingItem;
                                gg->holdingItem = newHoldingItem;
                            }

                        }
                    }
                }

                DrawRect(pos, V2(itemFrameSize), V4_Grey(.6f), 85);
                // Draw armor icon
                if (gg->inventoryArmor[i].id == 0){
                    v2 texDim = V2(48);
                    v2 texPos = V2(texDim.x*i, gg->images[TEX_GUI].dim.y - 192.f - 48.f);
                    v2 drawPos = pos + V2(itemFrameSize/2) - texDim/2;
                    DrawSpriteRect(&gg->images[TEX_GUI], texPos, texDim, drawPos, V2(1.f), 85, V4_Grey(.3f));
                }

                DrawItem(pos + V2(itemFrameSize/2), gg->inventoryArmor[i].id, gg->inventoryArmor[i].numItems, 85);
            }
            
            s32 numColumns = ArrayCount(gg->inventoryAll)/10;
            v2 inventoryPanelDim = V2(12*itemFrameSize + 11*frameMargin,
                                        numColumns*itemFrameSize + (numColumns - 1)*frameMargin + inventoryTopSep); 
            v2 inventoryPanel0 = V2(borderMargin.x, gi->windowDim.y - borderMargin.y - inventoryPanelDim.y);
            v2 inventoryPanel1 = inventoryPanel0 + inventoryPanelDim;
            if (!pauseGameplay)
                mouseOnGui = PointInRect(gi->mousePos, inventoryPanel0, inventoryPanel1);


            b32 craftingTableIsClose[CraftingTable_Count] = {};
            tile_object_type craftingTableObjects[CraftingTable_Count] = {};
            craftingTableObjects[CraftingTable_Bench] = TileObject_Bench;
            craftingTableObjects[CraftingTable_Anvil] = TileObject_Anvil;
            craftingTableObjects[CraftingTable_Campfire] = TileObject_Campfire9;
            s32 maxDistance = TILE_SIZE*4;
            for(f32 y = -maxDistance; y < maxDistance + 0.1f; y += TILE_SIZE){
                for(f32 x = -maxDistance; x < maxDistance + 0.1f; x+= TILE_SIZE){
                    v2 probePos = gg->playerPos + V2(x, y);
                    tile *t = WorldPosGetTile(probePos);
                    for(s32 i = 1; i < CraftingTable_Count; i++){
                        if (t->object == craftingTableObjects[i]){
                            craftingTableIsClose[i] = true;
                        }
                    }
                    if (t->object >= TileObject_Campfire1 && t->object <= TileObject_Campfire9){
                        craftingTableIsClose[CraftingTable_Campfire] = true;
                    }
                }
            }
            craftingTableIsClose[CraftingTable_None] = true;



            //
            // Crafting recipes
            //
            v2 recipesPos = inventoryPanel0 + V2(0, -70.f);
            DrawString("Craft", recipesPos + V2(0, 15.f), V2(1.f), 85, V4_Black());

            f32 recipeSep = frameMargin + 8.f;
            s32 recipesPerColumn = (s32)Max(1.f, (recipesPos.y - 0.f)/(itemFrameSize + recipeSep));

            b32 clickedOnRecipe = false;
            for(s32 i = 0; i < ArrayCount(globalRecipes); i++){
                auto recipe = globalRecipes[i];
                b32 hasIngredients = true;
                s32 numIngredients = 0;
                for(s32 j = 0; j < ArrayCount(recipe.ingredients); j++){
                    if (recipe.ingredients[j].id){
                        numIngredients++;
                        if (!HasItem(recipe.ingredients[j])){
                            hasIngredients = false;
                        }
                    }
                }
                f32 row = i % recipesPerColumn;
                f32 column = i / recipesPerColumn;
                v2 pos = recipesPos + V2(column*(itemFrameSize + recipeSep), - itemFrameSize - row*(itemFrameSize + recipeSep));
                v4 col;
                if (!hasIngredients){
                    col = V4(.9f, .4f, .4f, 1.f); // Red 
                }else if (craftingTableIsClose[recipe.craftingTable]){
                    col = V4_Grey(.85f);
                }else{
                    col = V4_Grey(.5f);
                }
                DrawRect(pos, V2(itemFrameSize), col, 85);

                // Draw frame
                f32 frameSize = 3.f;
                v4 frameCol = (hasIngredients ? V4_White(): V4_Red());
                //DrawRectFrame(pos + V2(frameSize), V2(itemFrameSize - frameSize*2), V2(frameSize), frameCol, 85);
                DrawRectFrame(pos, V2(itemFrameSize), V2(frameSize), V4_Black(), 85);

                v4 itemTintCol = V4_White();
                if (!hasIngredients || !craftingTableIsClose[recipe.craftingTable])
                    itemTintCol = V4_Grey(.5f);
                DrawItem(pos + V2(itemFrameSize/2), recipe.result.id, recipe.result.numItems, 85, itemTintCol);

                // Draw tooltip when hovering
                if (PointInRect(gi->mousePos, pos, pos + V2(itemFrameSize))){
                    mouseOnGui = true;

                    if (ButtonWentDown(&gi->mouseButtons[0]) && hasIngredients &&
                        craftingTableIsClose[recipe.craftingTable])
                    {
                        // Craft
                        for(s32 j = 0; j < ArrayCount(recipe.ingredients); j++){
                            auto ingredient = recipe.ingredients[j];
                            if (ingredient.id){
                                if (recipe.ingredients[j].id == gg->holdingItem.id){
                                    s32 taken = Min(ingredient.numItems, gg->holdingItem.numItems);
                                    gg->holdingItem.numItems = gg->holdingItem.numItems - taken;
                                    if (gg->holdingItem.numItems == 0)
                                        gg->holdingItem.id = (item_id)0;
                                    ingredient.numItems -= taken;
                                    if (ingredient.numItems == 0)
                                        continue;
                                }
                                RemoveItem(ingredient);
                            }
                        }
                        auto resultProperties = globalItemProperties[recipe.result.id];
                        if (gg->holdingItem.id == 0){
                            gg->holdingItem = recipe.result;
                        }else if (gg->holdingItem.id == recipe.result.id && !(resultProperties.flags & ItemFlag_Unstackable)){
                            // Can stack on hand
                            gg->holdingItem.numItems += recipe.result.numItems;
                        }else{
                            TakeItemOrDrop(recipe.result);
                        }

                    }

                    //
                    // Hovering recipe tooltip
                    //
                        // Text
                    char craftingTableStr[100] = "";
                    if (recipe.craftingTable){
                        sprintf_s(craftingTableStr, ArrayCount(craftingTableStr), "needs %s",
                                  globalCraftingTableNames[(s32)recipe.craftingTable]);
                    }
                    f32 textWidth = StringWidth(craftingTableStr);

                        // Rect
                    v2 tooltipPos = gi->mousePos;
                    f32 borderMargin = 10.f;
                    v2 tooltipDim = V2(Max(textWidth + 2*borderMargin, numIngredients*itemFrameSize + Max(0, numIngredients - 1)*frameMargin + 2*borderMargin),
                                        itemFrameSize + borderMargin + 40.f);
                    DrawRect(tooltipPos - V2(3.f), tooltipDim + V2(6.f), V4_Black(), 86);
                    DrawRect(tooltipPos, tooltipDim, V4_Grey(.6f), 86);

                        // Ingredients
                    v2 ingredientPos = tooltipPos + V2(borderMargin, tooltipDim.y - borderMargin - itemFrameSize);
                    for(s32 j = 0; j < ArrayCount(recipe.ingredients); j++){
                        if (recipe.ingredients[j].id){
                            v4 col = (HasItem(recipe.ingredients[j]) ? V4_Grey(.8f) : V4(.9f, .4f, .4f, 1.f));
                            DrawRect(ingredientPos, V2(itemFrameSize), col, 86);
                            DrawItem(ingredientPos + V2(itemFrameSize/2), recipe.ingredients[j].id, recipe.ingredients[j].numItems, 86);
                            ingredientPos.x += itemFrameSize + frameMargin;
                        }
                    }

                    v2 strScale = V2(1.f);
                    v4 col = V4(1.f, .1f, .1f, 1.f);
                    if (craftingTableIsClose[recipe.craftingTable]){
                        col = V4_White();
                    }
                    DrawString(craftingTableStr, AlignStringRight(craftingTableStr, tooltipPos + V2(tooltipDim.x - borderMargin, borderMargin), strScale.x),
                               strScale, 87, col);
                }
            }
            
            if (gg->holdingItem.id){
                b32 lmb = ButtonWentDown(&gi->mouseButtons[0]);
                b32 rmb = ButtonWentDown(&gi->mouseButtons[2]);
                if (!pauseGameplay && (lmb || rmb) && !mouseOnGui){
                    auto itemProperties = globalItemProperties[gg->holdingItem.id];

                    // Why drop items if you'll never fill the inventory? 
                    //if (lmb){// || (rmb && !(itemProperties.flags & ItemFlag_Placeable))){
                    //  s32 numItemsToDrop = (lmb ? gg->holdingItem.numItems : 1);

                    //  entity *e = CreateEntity(Entity_Item);
                    //  e->pos = gg->playerPos;
                    //  e->speed = V2LengthDir(14.f, AngleOf(gi->mousePos - gi->windowDim/2));
                    //  e->data.item.itemId       = gg->holdingItem.id;
                    //  e->data.item.numItems     = numItemsToDrop;
                    //  e->data.item.creationTime = gg->time;
    
                    //  gg->holdingItem.numItems -= numItemsToDrop;
                    //  if (gg->holdingItem.numItems == 0)
                    //      gg->holdingItem.id = (item_id)0;
                    //}
                }
                if (gg->holdingItem.id){
                    DrawItem(gi->mousePos, gg->holdingItem.id, gg->holdingItem.numItems, 89);
                }
            }
        }

        if (!pauseGameplay && !mouseOnGui && !dead){
            v2 mouseWorldPos = viewPos + gi->mousePos;
            
            s32 mouseTileX = Clamp(mouseWorldPos.x / TILE_SIZE, 0, TILES_X - 1);
            s32 mouseTileY = Clamp(mouseWorldPos.y / TILE_SIZE, 0, TILES_Y - 1);
            tile *mouseTile = &gg->tiles[TILES_X*mouseTileY + mouseTileX];
        
            v2 mouseTileCenter = V2(((f32)mouseTileX + .5f)*TILE_SIZE, ((f32)mouseTileY + .5f)*TILE_SIZE);
            f32 tilePlayerDistance = Length(gg->playerPos - mouseTileCenter);

            //
            // Use/Pick up objects
            //
            inventory_item *handItem;
            if (gg->holdingItem.id && gg->holdingItem.numItems){
                handItem = &gg->holdingItem;
            }else{
                handItem = &gg->inventoryTop[gg->selectedItemSlot];
            }
            auto handItemProperties = globalItemProperties[(s32)handItem->id];

            if (tilePlayerDistance < TILE_SIZE*5 && mouseTile->h > gg->seaLevel){
                if (mouseTile->object == TileObject_Stick ||
                    mouseTile->object == TileObject_Stone ||
                    mouseTile->object == TileObject_Flower1 ||
                    mouseTile->object == TileObject_Plant1 ||
                    mouseTile->object == TileObject_Mushroom ||
                    mouseTile->object == TileObject_Hay ||
                    mouseTile->object == TileObject_Bed ||
                    mouseTile->object == TileObject_SleepingBag ||
                    mouseTile->object == TileObject_HayBed ||
                    mouseTile->object == TileObject_Note)
                {
                    // Draw object outline
                    v2 tileDrawPos = V2(mouseTileX, mouseTileY)*TILE_SIZE - viewPos;
                    v2 texPos = V2(64.f*((s32)mouseTile->object % 10), gg->images[TEX_TILES].dim.y - 64.f*(1.f + (f32)((s32)mouseTile->object / 10)));
                    v2 offsets[8] = {{2.f, 0}, {1.f, 1.f}, {0, 2.f}, {-1.f, 1.f}, {-2.f, 0}, {-1.f, -1.f}, {0, -2.f}, {1.f, -1.f}};
                    f32 z = WindowYGetZ(tileDrawPos.y + 50);
                    for(s32 i = 0; i < ArrayCount(offsets); i++){
                        offsets[i]*=2.f;
                        DrawSpriteRect(&gg->images[TEX_TILES], texPos, V2(64), tileDrawPos + offsets[i], V2(1.f), z, V4_Black());
                    }
                    DrawSpriteRect(&gg->images[TEX_TILES], texPos, V2(64), tileDrawPos, V2(1.f), z, V4_White());

                    if (gi->mouseButtons[2].isDown){
                        if (mouseTile->object == TileObject_Bed||
                            mouseTile->object == TileObject_SleepingBag ||
                            mouseTile->object == TileObject_HayBed )
                        {
                            if (ButtonWentDown(&gi->mouseButtons[2])){
                                // Right-click bed: show sleep window
                                gg->sleepWindowIsOpen = true;
                                gg->sleepWindowTimeAfterSleep = 0;
                                gg->sleepBedType = mouseTile->object;
                                gg->sleepBedPos = mouseTileCenter;
                            }
                        }else if (mouseTile->object == TileObject_Note){
                            if (ButtonWentDown(&gi->mouseButtons[2])){
                                // Show note
                                gg->noteIsOpen = true;
                                gg->openNote = mouseTile->objectData;
                            }
                        }else{
                            inventory_item item = {};
                            item.numItems = 1;
                            if      (mouseTile->object == TileObject_Stick)    item.id = Item_Stick;
                            else if (mouseTile->object == TileObject_Stone)    item.id = Item_Stone;
                            else if (mouseTile->object == TileObject_Flower1)  item.id = Item_Flower;
                            else if (mouseTile->object == TileObject_Mushroom) item.id = Item_Mushroom;
                            else if (mouseTile->object == TileObject_Hay)      item.id = Item_Hay;
                            else if (mouseTile->object == TileObject_Plant1)   item.id = Item_Leaf;

                            TakeItemOrDrop(item);

                            mouseTile->object = TileObject_None;
                        }
                    }

                }else if (handItemProperties.flags & ItemFlag_Placeable ||
                          handItem->id == Item_Axe || handItem->id == Item_Pickaxe)
                {
                    DrawRectFrame(V2(mouseTileX, mouseTileY)*TILE_SIZE - viewPos, V2(TILE_SIZE), V2(3), V4_Black(.5f), 45);

                    if (handItemProperties.flags & ItemFlag_Placeable){
                        // Place items
                        if (ButtonWentDown(&gi->mouseButtons[2])){
                            if (!mouseTile->object){
                                if (handItemProperties.placeableObject == TileObject_Campfire){
                                    mouseTile->object = TileObject_Campfire5;
                                    mouseTile->objectData = CAMPFIRE_SHRINK_TIME;
                                    AddTileWithBehaviour(mouseTile);
                                }else{
                                    mouseTile->object = handItemProperties.placeableObject;
                                }
                                // Subtract item
                                Assert(handItemProperties.placeableObject);
                                Assert(handItem->numItems > 0);
                                handItem->numItems--;
                                if (handItem->numItems == 0){
                                    handItem->id = (item_id)0;
                                }
                            }
                        }
                    }else if (handItem->id == Item_Axe && mouseTile->object == TileObject_Tree){
                        // Draw tree outline
                        v2 texDim = V2(64*3, 64*4);
                        v2 texPos = V2(448, 64);
                        v2 drawPos = V2(mouseTileX*TILE_SIZE + 32.f - texDim.x/2, mouseTileY*TILE_SIZE) - viewPos;
                        f32 z = WindowYGetZ(drawPos.y + 50);
                        v2 offsets[8] = {{2.f, 0}, {1.f, 1.f}, {0, 2.f}, {-1.f, 1.f}, {-2.f, 0}, {-1.f, -1.f}, {0, -2.f}, {1.f, -1.f}};
                        for(s32 i = 0; i < ArrayCount(offsets); i++){
                            offsets[i]*=2.f;
                            DrawSpriteRect(&gg->images[TEX_TILES], texPos, texDim, drawPos + offsets[i], V2(1.f), z, V4_Black());
                        }
                        DrawSpriteRect(&gg->images[TEX_TILES], texPos, texDim, drawPos, V2(1.f), z, V4_White());
                    }
                }else if (handItemProperties.flamability &&
                          mouseTile->object >= TileObject_Campfire0 &&
                          mouseTile->object < TileObject_Campfire9)
                {
                    //
                    // About adding fuel to campfire
                    //
                        // Draw outline
                    v2 tileDrawPos = V2(mouseTileX, mouseTileY)*TILE_SIZE - viewPos;
                    v2 texPos = V2(64.f*((s32)mouseTile->object % 10), gg->images[TEX_TILES].dim.y - 64.f*(1.f + (f32)((s32)mouseTile->object / 10)));
                    v2 offsets[8] = {{2.f, 0}, {1.f, 1.f}, {0, 2.f}, {-1.f, 1.f}, {-2.f, 0}, {-1.f, -1.f}, {0, -2.f}, {1.f, -1.f}};
                    for(s32 i = 0; i < ArrayCount(offsets); i++){
                        offsets[i]*=2.f;
                        DrawSpriteRect(&gg->images[TEX_TILES], texPos, V2(64), tileDrawPos + offsets[i], V2(1.f), 20, V4_Black());
                    }
                    DrawSpriteRect(&gg->images[TEX_TILES], texPos, V2(64), tileDrawPos, V2(1.f), 20, V4_White());

                    // Helper text on hover
                    char *str = "ADD FUEL";
                    v2 strScale = V2(1.f);
                    DrawString(str, CenterStringX(str, tileDrawPos + V2(TILE_SIZE/2, TILE_SIZE*1.1f), strScale.x), strScale, 80, V4_Black());

                    if (ButtonWentDown(&gi->mouseButtons[2])){
                        // Add power to campfire.
                        u16 power = CAMPFIRE_SHRINK_TIME*handItemProperties.flamability;
                        mouseTile->objectData += power;
                        while(mouseTile->objectData > CAMPFIRE_SHRINK_TIME){
                            if (mouseTile->object == TileObject_Campfire9){
                                mouseTile->objectData = CAMPFIRE_SHRINK_TIME;
                                break;
                            }
                            mouseTile->object = (tile_object_type)((s32)mouseTile->object + 1);
                            mouseTile->objectData -= CAMPFIRE_SHRINK_TIME;
                        }

                        // Subtract item
                        Assert(handItem->numItems > 0);
                        handItem->numItems--;
                        if (handItem->numItems == 0){
                            handItem->id = (item_id)0;
                        }
                    }

                }
            }else if (handItem->id == Item_Raft && mouseTile->h <= gg->seaLevel){
                // Draw placeable tile outline
                DrawRectFrame(V2(mouseTileX, mouseTileY)*TILE_SIZE - viewPos, V2(TILE_SIZE), V2(3), V4_Black(.5f), 45);
            }


            // Finish action
            if (gg->itemActionTimer > gg->itemActionResetTime){
                gg->itemActionTimer = 0; // Stop action
                gg->itemActionMaxTime = 0;
                gg->itemActionResetTime = 0;
                gg->actioningItem = (item_id)0;
            }
            if (gg->actioningItem != handItem->id){
                gg->actioningItem = (item_id)0;
            }

            if (!gg->itemActionTimer && gi->mouseButtons[0].isDown){
                if (handItemProperties.flags & ItemFlag_Actionable){
                    gg->itemActionHitEntitiesCount = 0;
                    switch(handItem->id){
                    case Item_Sword:
                    {
                        gg->actioningItem = handItem->id;
                        gg->itemActionMaxTime = 18;
                        gg->itemActionResetTime = 24;
                        gg->itemActionTimer = 1; // 0 for no action.
                        gg->itemActionDirection = AngleOf(gi->mousePos - gi->windowDim/2);
                    } break;

                    case Item_Spear:
                    {
                        gg->actioningItem = handItem->id;
                        gg->itemActionMaxTime = 16;
                        gg->itemActionResetTime = 40;
                        gg->itemActionTimer = 1; // 0 for no action.
                        gg->itemActionDirection = AngleOf(gi->mousePos - gi->windowDim/2);
                    } break;

                    case Item_Axe:
                    {
                        gg->actioningItem = handItem->id;
                        gg->itemActionMaxTime = 20;
                        gg->itemActionResetTime = 40;
                        gg->itemActionTimer = 1; // 0 for no action.
                        gg->itemActionDirection = AngleOf(gi->mousePos - gi->windowDim/2);
                        
                        if (mouseTile->object == TileObject_Log){
                            for(s32 i = (s32)RandomRange(2, 4); i > 0; i--){
                                v2 speed = V2LengthDir(RandomRange(4.f, 15.f), Random(2*PI));
                                item_id item = (i == 1 ? Item_Log : (RandomChance(.5f) ? Item_Log : Item_Stick));
                                CreateItem(item, mouseTileCenter, speed);
                            }

                            mouseTile->object = (tile_object_type)0;
                            mouseTile->objectData = 0;

                        }else if (mouseTile->object == TileObject_Bed){
                            CreateItem(Item_Bed, mouseTileCenter);
                            mouseTile->object = (tile_object_type)0;
                            mouseTile->objectData = 0;
                        }else if (mouseTile->object == TileObject_Bench){
                            CreateItem(Item_Bench, mouseTileCenter);
                            mouseTile->object = (tile_object_type)0;
                            mouseTile->objectData = 0;
                        }else if (mouseTile->object == TileObject_SleepingBag){
                            CreateItem(Item_SleepingBag, mouseTileCenter);
                            mouseTile->object = (tile_object_type)0;
                            mouseTile->objectData = 0;
                        }else if (mouseTile->object == TileObject_HayBed){
                            CreateItem(Item_HayBed, mouseTileCenter);
                            mouseTile->object = (tile_object_type)0;
                            mouseTile->objectData = 0;

                        }else if (mouseTile->object == TileObject_Tree){
                            // Create leaf particles
                            
                            // Leaf particles 2
                            s32 numParticles = RandomRangeS32(20, 25);
                            for(s32 i = 0; i < numParticles; i++){
                                entity *e = CreateEntity(Entity_TexParticle);
                                auto t = &e->data.texParticle;

                                e->speed = V2LengthDir(RandomRange(2.f, 5.f), Random(2*PI));
                                e->speed.x += RandomRange(-6.f, 6.f);
                                e->pos = mouseTileCenter;

                                t->imageIndex = TEX_EFFECTS;
                                t->texPos = V2(16.f*(RandomChance(.33f) ? 10 : (RandomChance(.5f) ? 12 : 14)), 0.f);
                                t->texDim = V2(16.f);
                                t->scale = V2(RandomRange(.7f, 1.4f));
                                t->z = 0;

                                t->h = RandomRange(50.f, 170.f); // height from ground.
                                t->hSpeed = RandomRange(-5.f, 6.f);
                                t->hGravity = RandomRange(.2f, .4f);

                                t->speedMultiplier = RandomRange(.9f, .98f);
                                t->maxLifetime = RandomRangeS32(30, 50);
                                t->creationTime = gg->time;

                                t->color = V4(RandomRange(0.f, .2f), RandomRange(.45f, .7f), RandomRange(0.f, .2f), RandomRange(.9f, 1.f));
                            }
                            
                            // Drop leaf items
                            for(s32 i = (s32)RandomRange(1, 2); i > 0; i--){
                                v2 speed = V2LengthDir(RandomRange(15.f, 20.f), Random(2*PI));
                                CreateItem(Item_Leaf, mouseTileCenter, speed);
                            }


                            if (mouseTile->objectData > 1){
                                mouseTile->objectData--;
                            }else{ // Cut tree
                                v2 pos0 = V2(mouseTileCenter.x - 64, mouseTileCenter.y);
                                v2 pos1 = V2(mouseTileCenter.x + 64, mouseTileCenter.y + 64*2.8f);
                                for(s32 i = RandomRangeS32(20, 25); i > 0; i--){
                                    v2 pos = V2(RandomRange(pos0.x, pos1.x), RandomRange(pos0.y, pos1.y));
                                    CreateTexParticles(4, V2(448, 64), V2(448 + 64*3, 64 + 64*4), pos, 1, 0, .6f, 1.2f,
                                                       RandomRangeS32(40, 70), .10f, .32f);
                                }
                                
                                // Create items
                                for(s32 i = (s32)RandomRange(3, 5); i > 0; i--){
                                    v2 speed = V2LengthDir(RandomRange(4.f, 15.f), Random(2*PI));
                                    CreateItem(Item_Log, mouseTileCenter, speed);
                                }
                                for(s32 i = (s32)RandomRange(0, 3); i > 0; i--){
                                    v2 speed = V2LengthDir(RandomRange(4.f, 15.f), Random(2*PI));
                                    CreateItem(Item_Stick, mouseTileCenter, speed);
                                }

                                // Remove tree
                                mouseTile->object = TileObject_TreeTrunk;
                                mouseTile->objectData = 0;

                            }
                        }
                    } break;

                    case Item_Pickaxe:
                    {
                        gg->actioningItem = handItem->id;
                        gg->itemActionMaxTime = 20;
                        gg->itemActionResetTime = 33;
                        gg->itemActionTimer = 1; // 0 for no action.
                        gg->itemActionDirection = AngleOf(gi->mousePos - gi->windowDim/2);

                        if (mouseTile->object == TileObject_Rock){
                            for(s32 i = (s32)RandomRange(3, 5); i > 0; i--){
                                v2 speed = V2LengthDir(RandomRange(3.f, 13.f), Random(2*PI));
                                item_id item = Item_Stone;
                                if (RandomChance(.1f))
                                    item = Item_Flint;
                                CreateItem(item, mouseTileCenter, speed);
                            }

                            mouseTile->object = (tile_object_type)0;
                        }
                    } break;
                    
                    case Item_Stone:
                    {
                        gg->actioningItem = handItem->id;
                        gg->itemActionMaxTime = 40;
                        gg->itemActionResetTime = 40;
                        gg->itemActionTimer = 1; // 0 for no action.
                        v2 throwPos = gg->playerPos;
                        f32 throwDir = AngleOf(gi->mousePos - (throwPos - viewPos));
                        gg->itemActionDirection = throwDir;
                        
                        handItem->numItems--;
                        if (handItem->numItems == 0){
                            handItem->id = (item_id)0;
                        }

                        entity *e = CreateEntity(Entity_ThrownStone);
                        e->pos = gg->playerPos;
                        e->speed = V2LengthDir(40.f, throwDir);
                        e->data.thrownStone.creationTime = gg->time;


                    } break;
                    }
                }
            }
            
            if (!gg->itemActionTimer && gi->mouseButtons[2].isDown){
                gg->itemActionHitEntitiesCount = 0;
                if (ButtonWentDown(&gi->mouseButtons[2])){
                    if (handItemProperties.flags & ItemFlag_Consumable){
                        // Eat
                        switch(handItem->id){
                        case Item_RawRedMeat:
                        case Item_RawWhiteMeat:
                        case Item_CookedRedMeat:
                        case Item_CookedWhiteMeat:
                        case Item_Mushroom:
                        case Item_Omelet:
                        {
                            if (gg->playerStomach < .95f){
                                // Fill stomach
                                f32 stomach = 0.f;
                                if (handItem->id == Item_CookedRedMeat || handItem->id == Item_CookedWhiteMeat){
                                    stomach = .15f;
                                }else if (handItem->id == Item_Omelet){
                                    stomach = .1f;
                                }else if (handItem->id == Item_RawRedMeat || handItem->id == Item_RawWhiteMeat){
                                    stomach = .07f;
                                }else if (handItem->id == Item_Mushroom){
                                    stomach = .05f;
                                }
                                gg->playerStomach = Min(1.f, gg->playerStomach + stomach);

                                // Subtract Item
                                handItem->numItems--;
                                if (handItem->numItems == 0){
                                    handItem->id = (item_id)0;
                                }
                            }

                        } break;
                        case Item_Medicine:
                        {
                            if (gg->playerHealth < gg->playerHealthMax){
                                gg->playerHealth = Min(gg->playerHealthMax, gg->playerHealth + 30);
                        
                                // Subtract Item
                                handItem->numItems--;
                                if (handItem->numItems == 0){
                                    handItem->id = (item_id)0;
                                }
                            }
                        } break;
                        }
                    }

                    if (handItem->id == Item_Raft && mouseTile->h <= gg->seaLevel){
                        gg->won = true;
                        gg->activeSection = PS_FinalScreen;
                    }
                }
                if (handItemProperties.flags & ItemFlag_Actionable){
                    switch(handItem->id){
                    }
                }
            }

            if (gg->itemActionTimer){
                f32 progress = gg->itemActionTimer / (f32)gg->itemActionMaxTime;
                if (gg->itemActionTimer <= gg->itemActionMaxTime){
                    switch(handItem->id){
                    case Item_Sword:
                    case Item_Axe:
                    case Item_Pickaxe:
                    {
                        item_id itemId = handItem->id;
                        v2 texPos = { (f32)((s32)itemId % 10)*48, gg->images[TEX_ITEMS].dim.y - (1 + ((s32)itemId / 10))*48.f};

                        f32 armLength = (itemId == Item_Sword ? 90.f : 70.f);
                        v2 pivotOffset = V2(0, 50.f);
                        f32 texAngle = PI/4;
                        v2 texPivotPos = texPos + V2(48/2) + V2LengthDir(armLength, texAngle + PI);

                        f32 angle = gg->itemActionDirection + Lerp(PI*.35f, -PI*.35f, progress);
                        DrawSpriteRectRotated(&gg->images[TEX_ITEMS], texPos, V2(48), texPivotPos, gg->playerPos + pivotOffset - viewPos,
                                              angle - texAngle, V2(1.f), 60, LightMapColor(gg->playerPos));

                        v2 swordPos = gg->playerPos + pivotOffset + V2LengthDir(armLength, angle);
                        // Entity collision
                        f32 r = 24.f;
                        b32 collision = false;
                        for(s32 i = 0; i < gg->numEntities; i++){
                            entity *e = &gg->entities[i];

                            if (e->type == Entity_Rabbit || e->type == Entity_Goose || e->type == Entity_Boar){
                                if (CircleEntityCollision(e, swordPos, r)){
                                    b32 alreadyHit = false;
                                    for (s32 i = 0; i < gg->itemActionHitEntitiesCount; i++){
                                        if (gg->itemActionHitEntities[i] == e->id)
                                            alreadyHit = true;
                                    }
                                    if (!alreadyHit && gg->itemActionHitEntitiesCount < ArrayCount(gg->itemActionHitEntities)){
                                        gg->itemActionHitEntities[gg->itemActionHitEntitiesCount++] = e->id;

                                        f32 hitDirection = AngleOf(e->pos - (gg->playerPos + pivotOffset));
                                        e->speed = V2LengthDir(30.f, hitDirection);
                                        e->stateTimer = 0;
                                        e->state = ENTITY_STATE_HIT;
                                        collision = true;

                                        s32 damage = 0;
                                        if      (handItem->id == Item_Sword)   damage = 6;
                                        else if (handItem->id == Item_Axe)     damage = 4;
                                        else if (handItem->id == Item_Pickaxe) damage = 3;
                                        HurtEntity(e, damage, hitDirection); 
                                    }
                                }
                            }
                        }
                        if (gg->debugOn){
                            DrawCircleLine(swordPos - viewPos, r, V4_Red(), 90);
                            if (collision)
                                DrawCircle(swordPos - viewPos, r, V4_Red(.3f), 90);
                        }
    
                    } break;
                    case Item_Spear:
                    {
                        item_id itemId = handItem->id;
                        //v2 texPos = { (f32)((s32)itemId % 10)*48, gg->images[TEX_ITEMS].dim.y - (1 + ((s32)itemId / 10))*48.f};
                        v2 texPos = {80.f, gg->images[TEX_EFFECTS].dim.y - 32.f};
                        v2 texPivotPos = texPos + V2(16.f);
                        v2 texDim = V2(128.f, 32.f);

                        f32 armLength = Lerp(-30.f, 60.f, 1.f - Square(1.f - progress));//progress*progress);

                        // Draw
                        v2 pivotOffset = V2(0, 40.f);
                        f32 drawAngle = gg->itemActionDirection;
                        v2 handPos = gg->playerPos + pivotOffset + V2LengthDir(armLength, gg->itemActionDirection); 
                        DrawSpriteRectRotated(&gg->images[TEX_EFFECTS], texPos, texDim, texPivotPos, handPos - viewPos, drawAngle, V2(1.f), 60, LightMapColor(gg->playerPos));

                        v2 tipPos = handPos + V2LengthDir(84.f, gg->itemActionDirection);
                        f32 r = 16.f;

                        // Entity collision
                        b32 collision = false;
                        for(s32 i = 0; i < gg->numEntities; i++){
                            entity *e = &gg->entities[i];

                            if (e->type == Entity_Rabbit || e->type == Entity_Goose || e->type == Entity_Boar){
                                if (CircleEntityCollision(e, tipPos, r)){
                                    b32 alreadyHit = false;
                                    for (s32 i = 0; i < gg->itemActionHitEntitiesCount; i++){
                                        if (gg->itemActionHitEntities[i] == e->id)
                                            alreadyHit = true;
                                    }
                                    if (!alreadyHit && gg->itemActionHitEntitiesCount < ArrayCount(gg->itemActionHitEntities)){
                                        gg->itemActionHitEntities[gg->itemActionHitEntitiesCount++] = e->id;

                                        f32 hitDirection = AngleOf(e->pos - (gg->playerPos + pivotOffset));
                                        e->speed = V2LengthDir(30.f, hitDirection);
                                        e->stateTimer = 0;
                                        e->state = ENTITY_STATE_HIT;
                                        collision = true;

                                        s32 damage = 5;
                                        HurtEntity(e, damage, hitDirection); 
                                    }
                                }
                            }
                        }
                        if (gg->debugOn){
                            DrawCircleLine(tipPos - viewPos, r, V4_Red(), 90);
                            if (collision)
                                DrawCircle(tipPos - viewPos, r, V4_Red(.3f), 90);
                        }
    
                    } break;
                    }
                }
                gg->itemActionTimer++;
            }
        }


        //
        // Draw bars
        //
        {
            v2 borderMargin = V2(24.f);
            f32 barWidth = 30.f;
            f32 barSep = 16.f;
            v2 hpBarDim = V2(350.f, barWidth);
            v2 energyBarDim, stomachBarDim, temperatureBarDim;
            energyBarDim = stomachBarDim = temperatureBarDim = V2(barWidth, 250.f);

            v2 barFrameSize = V2(4.f);

            v2 hpBarPos = V2(gi->windowDim.x - borderMargin.x - hpBarDim.x, borderMargin.y);

            v2 energyBarPos = V2(gi->windowDim.x - borderMargin.x - energyBarDim.x, hpBarPos.y + hpBarDim.y + barSep);
            v2 stomachBarPos = energyBarPos + V2(-barSep - stomachBarDim.x, 0);
            v2 temperatureBarPos = stomachBarPos + V2(-barSep - temperatureBarDim.x, 0);
            

            // Bar frame color
            f32 d = .5f;
            v4 hpBarColor          = V4(0, 0, 0);//V4(1.f, .1f, .1f);
            v4 energyBarColor      = V4(.7f*d, 1.f*d, .2f*d);
            v4 stomachBarColor     = V4(1.f*d, .6f*d, 0.1f*d);
            v4 temperatureBarColor = V4_Grey(.1f); //V4(.3f*d, .9f*d, 1.f*d);
            // Bar inside color
            f32 a = .8f;
            v4 hpColor      = V4(1.f, .1f, .1f, a);
            v4 energyColor  = V4(.7f, 1.f, .2f, a);
            v4 stomachColor = V4(1.f,.7f, 0.13f, a);
            v4 emptyColor   = V4_Black(.65f);
            v4 emptyHpColor = V4(.2f, 0.08f, 0.08f, .7f);

            // Draw bar insides
            f32 hp = gg->playerHealth / (f32)gg->playerHealthMax;
            if (hp != 1.f)
                DrawRect(hpBarPos + V2(hpBarDim.x*hp, 0), V2(hpBarDim.x*(1.f-hp), hpBarDim.y), emptyHpColor, 80);
            if (hp)
                DrawRect(hpBarPos, V2(hpBarDim.x*hp, hpBarDim.y), hpColor, 80);
            // HP bar lines
            for(s32 i = 1; i <= 9; i++){
                DrawRect(hpBarPos + V2(i*hpBarDim.x/10.f - 2.f, 0), V2(4.f, hpBarDim.y), V4_Black(.5f), 80);
            }
            
            f32 energy = gg->playerEnergy;
            if (energy != 1.f)
                DrawRect(energyBarPos + V2(0, energyBarDim.y*energy), V2(energyBarDim.x, energyBarDim.y*(1.f-energy)), emptyColor, 80);
            if (energy)
                DrawRect(energyBarPos, V2(energyBarDim.x, energyBarDim.y*energy), energyColor, 80);
            
            f32 stomach = gg->playerStomach;
            if (stomach != 1.f)
                DrawRect(stomachBarPos + V2(0, stomachBarDim.y*stomach), V2(stomachBarDim.x, stomachBarDim.y*(1.f-stomach)), emptyColor, 80);
            if (stomach)
                DrawRect(stomachBarPos, V2(stomachBarDim.x, stomachBarDim.y*stomach), stomachColor, 80);
            
            // Temperature bar
            f32 temperature = gg->playerTemperature;
            f32 y0, y1, y2, y3, y4, y5;
            y0 = temperatureBarPos.y;
            y1 = y0 + temperatureBarDim.y*extremeColdLimit;
            y2 = y0 + temperatureBarDim.y*coldLimit;
            y3 = y0 + temperatureBarDim.y*heatLimit;
            y4 = y0 + temperatureBarDim.y*extremeHeatLimit;
            y5 = y0 + temperatureBarDim.y;
            {
                f32 x0 = temperatureBarPos.x;
                f32 x1 = temperatureBarPos.x + temperatureBarDim.x;
            
                v4 col0 = V4(0, 1.f, 1.f); // (extreme cold)
                v4 col2 = V4_Grey(.8f); // (cold)
                v4 col4 = V4(1.f, 0.f, 0.f); // (safe)
                v4 col1 = LerpV4(col0, col2, .5f); // (heat)
                v4 col3 = LerpV4(col2, col4, .5f); // (extreme heat)

                DrawRect(V2(x0, y0), V2(temperatureBarDim.x, y1 - y0), col0, 80);
                DrawRect(V2(x0, y1), V2(temperatureBarDim.x, y2 - y1), col1, 80);
                DrawRect(V2(x0, y2), V2(temperatureBarDim.x, y3 - y2), col2, 80);
                DrawRect(V2(x0, y3), V2(temperatureBarDim.x, y4 - y3), col3, 80);
                DrawRect(V2(x0, y4), V2(temperatureBarDim.x, y5 - y4), col4, 80);

                // Limit lines.
                DrawRect(V2(x0, y1), V2(temperatureBarDim.x, 1.f), V4_Black(), 80);
                DrawRect(V2(x0, y2), V2(temperatureBarDim.x, 1.f), V4_Black(), 80);
                DrawRect(V2(x0, y3), V2(temperatureBarDim.x, 1.f), V4_Black(), 80);
                DrawRect(V2(x0, y4), V2(temperatureBarDim.x, 1.f), V4_Black(), 80);

                // @DELETEME We used to do a gradient.
                //DrawRect(temperatureBarPos, temperatureBarDim, col1, 80);
                //y1 = y0 + temperatureBarDim.y*.333f;
                //DrawColoredQuad(V2(x0, y0), V2(x0, y1), V2(x1, y1), V2(x1, y0),
                //              col0, col1, col1, col0, 80);
                //y1 = y0 + temperatureBarDim.y*.666f;
                //DrawColoredQuad(V2(x0, y1), V2(x0, y2), V2(x1, y2), V2(x1, y1),
                //              col1, col2, col2, col1, 80);


                // Exterior temperature mark
                {
                    f32 y = temperatureBarPos.y + temperatureBarDim.y*exteriorTemperature;
                    v2 dim      = V2(.7f, .53f)*temperatureBarDim.x;
                    v2 innerDim = dim - V2(2.f);
                    v4 col       = V4_White();
                    v4 marginCol = V4_Black();
                    f32 innerTriangleYOff = 1.f;
                    f32 marginSize = 2.f;
                    s32 z = 80;
                    if (exteriorTemperature > heatLimit || exteriorTemperature < coldLimit){
                        col = (exteriorTemperature < extremeColdLimit ? col0 :
                              (exteriorTemperature < coldLimit        ? col1 :
                              (exteriorTemperature < extremeHeatLimit ? col3 :
                               col4)));
                        z = 81;
                        dim *= 1.1f;//V2(4.f);
                        marginSize = 4.f;
                    }
                    if (Abs(exteriorTemperature - temperature) < .02f){ // Exterior is about the same as body temperature: Draw a rect.
                        dim = V2(temperatureBarDim.x*.66f, temperatureBarDim.x*.45f);
                        v2 pos = V2(Round(temperatureBarPos.x + temperatureBarDim.x/2), Round(y)) - dim/2;
                        DrawRect(pos, dim, col, z);

                        DrawLine(pos, V2(pos.x, pos.y + dim.y), marginCol, z, marginSize);
                        DrawLine(V2(pos.x, pos.y + dim.y), pos + dim, marginCol, z, marginSize);
                        DrawLine(pos + dim, V2(pos.x + dim.x, pos.y), marginCol, z, marginSize);
                        DrawLine(V2(pos.x + dim.x, pos.y), pos, marginCol, z, marginSize);

                    }else{ 
                        v2 p0, p1, p2;
                        f32 dir = (exteriorTemperature < temperature ? -1.f : 1.f); // traingle points up or down
                        p0 = V2(temperatureBarPos.x + temperatureBarDim.x/2, y + dir*dim.y/2);
                        p1 = V2(temperatureBarPos.x + temperatureBarDim.x/2 - dim.x/2, y - dir*dim.y/2);
                        p2 = V2(temperatureBarPos.x + temperatureBarDim.x/2 + dim.x/2, y - dir*dim.y/2);
                        DrawTriangle(p0, p1, p2, col, z);

                        DrawLine(p0, p1, marginCol, z, marginSize);
                        DrawLine(p2, p0, marginCol, z, marginSize);
                        DrawLine(p1, p2, marginCol, z, marginSize);
                    }
                }

                DrawRectFrame(temperatureBarPos, temperatureBarDim, barFrameSize, temperatureBarColor, 80);

                // Body temperature
                v2 lineDim = {temperatureBarDim.x + (barFrameSize.x + 4.f)*2, 6.f};
                f32 lineY = temperatureBarPos.y + temperatureBarDim.y*temperature;
                DrawRect(V2(temperatureBarPos.x + (temperatureBarDim.x - lineDim.x)/2, lineY - lineDim.y/2), lineDim, V4_Black(), 80);

            }


            // Bar tooltips
            if (!gg->guiHoveredId && PointInRect(gi->mousePos, energyBarPos, energyBarPos + energyBarDim)){
                char text[100];
                sprintf_s(text, ArrayCount(text), "E=%i%%", (s32)Round(energy*100));
                DrawTextTooltip(text, gi->mousePos, 81);
            }
            if (!gg->guiHoveredId && PointInRect(gi->mousePos, stomachBarPos, stomachBarPos + stomachBarDim)){
                char text[100];
                sprintf_s(text, ArrayCount(text), "Stomach=%i%%", (s32)Round(stomach*100));
                DrawTextTooltip(text, gi->mousePos, 81);
            }
            if (!gg->guiHoveredId && PointInRect(gi->mousePos, temperatureBarPos, temperatureBarPos + temperatureBarDim)){
                char text[100];
                sprintf_s(text, ArrayCount(text), "T=%i%%", (s32)Round(temperature*100));
                DrawTextTooltip(text, gi->mousePos, 81);

                // Temperature notes
                v4 c0 = V4_White(.98f);
                v4 c1 = V4_White(.5f);
                f32 x = temperatureBarPos.x - 18.f;
                f32 yOff = -10.f;
                v2 scale = V2(.75f);
                char *str = "LOSE E & HP";
                DrawString(str, AlignStringRight(str, V2(x, (y0 + y1)/2 + yOff), scale.x), scale, 82, (temperatureState == Temperature_ExtremeCold ? c0 : c1));
                str = "LOSE E";
                DrawString(str, AlignStringRight(str, V2(x, (y1 + y2)/2 + yOff), scale.x), scale, 82, (temperatureState == Temperature_Cold        ? c0 : c1));
                str = "SAFE";
                DrawString(str, AlignStringRight(str, V2(x, (y2 + y3)/2 + yOff), scale.x), scale, 82, (temperatureState == Temperature_Safe        ? c0 : c1));
                str = "LOSE E";
                DrawString(str, AlignStringRight(str, V2(x, (y3 + y4)/2 + yOff), scale.x), scale, 82, (temperatureState == Temperature_Heat        ? c0 : c1));
                str = "LOSE E & HP";
                DrawString(str, AlignStringRight(str, V2(x, (y4 + y5)/2 + yOff), scale.x), scale, 82, (temperatureState == Temperature_ExtremeHeat ? c0 : c1));
            }
            if (!gg->guiHoveredId && PointInRect(gi->mousePos, hpBarPos, hpBarPos + hpBarDim)){
                char text[100];
                sprintf_s(text, ArrayCount(text), "Health=%i/%i", gg->playerHealth, gg->playerHealthMax);
                DrawTextTooltip(text, gi->mousePos, 81);
            }


            // Bar buff icons (arrows).
            {
                v2 texDim = V2(30.f, 28.f);
                v2 texPos = V2(16.f, gg->images[TEX_GUI].dim.y - 272.f + 16.f) - texDim/2;

                // Energy buffs
                v2 scale = V2(1.f);
                v2 iconPos = energyBarPos + V2(energyBarDim.x/2 - texDim.x*scale.x/2, energyBarDim.y + 5.f);
                v2 iconDim = Hadamard(texDim, scale) - V2(0, -2.f); // used for hitbox

                f32 iconSep = 0.f;
                f32 a = .85f;

                u64 guiId = 3067; // random num
                // Hunger
                if (gg->playerStomach == 0){
                    DrawSpriteRect(&gg->images[TEX_GUI], texPos, texDim, iconPos, scale, 80, V4(1.f, .66f, .3f, a));
                    // Tooltip
                    if (!gg->guiHoveredId && PointInRect(gi->mousePos, iconPos, iconPos + iconDim)){
                        DrawTextTooltip("Starving.\nLowers your energy.", gi->mousePos, 81);
                        gg->guiHoveredId = guiId;
                    }
                    iconPos.y += texDim.y + iconSep;
                }
                // Water
                if (inWater){
                    DrawSpriteRect(&gg->images[TEX_GUI], texPos, texDim, iconPos, scale, 80, V4_Blue(a));
                    // Tooltip
                    if (!gg->guiHoveredId && PointInRect(gi->mousePos, iconPos, iconPos + iconDim)){
                        DrawTextTooltip("In water.\nLowers your energy.", gi->mousePos, 81);
                        gg->guiHoveredId = guiId;
                    }
                    iconPos.y += texDim.y + iconSep;
                }
                // Cold
                if (gg->playerTemperature < coldLimit){
                    DrawSpriteRect(&gg->images[TEX_GUI], texPos, texDim, iconPos, scale, 80, V4_Cyan(a));
                    // Tooltip
                    if (!gg->guiHoveredId && PointInRect(gi->mousePos, iconPos, iconPos + iconDim)){
                        DrawTextTooltip("Cold.\nLowers your energy.", gi->mousePos, 81);
                        gg->guiHoveredId = guiId;
                    }
                    iconPos.y += texDim.y + iconSep;
                }
                // Heat
                if (gg->playerTemperature > heatLimit){
                    DrawSpriteRect(&gg->images[TEX_GUI], texPos, texDim, iconPos, scale, 80, V4_Red(a));
                    // Tooltip
                    if (!gg->guiHoveredId && PointInRect(gi->mousePos, iconPos, iconPos + iconDim)){
                        DrawTextTooltip("Heat.\nLowers your energy.", gi->mousePos, 81);
                        gg->guiHoveredId = guiId;
                    }
                    iconPos.y += texDim.y + iconSep;
                }
                // Moving
                if (gg->playerIsWalking || gg->playerIsRunning){
                    DrawSpriteRect(&gg->images[TEX_GUI], texPos, texDim, iconPos, scale, 80, V4_Grey(.5f, a));
                    // Tooltip
                    if (!gg->guiHoveredId && PointInRect(gi->mousePos, iconPos, iconPos + iconDim)){
                        DrawTextTooltip("Moving.\nLowers your energy.", gi->mousePos, 81);
                        gg->guiHoveredId = guiId;
                    }
                    iconPos.y += texDim.y + iconSep;
                }
                // Running
                if (gg->playerIsRunning){
                    DrawSpriteRect(&gg->images[TEX_GUI], texPos, texDim, iconPos, scale, 80, V4_White(a));
                    // Tooltip
                    if (!gg->guiHoveredId && PointInRect(gi->mousePos, iconPos, iconPos + iconDim)){
                        DrawTextTooltip("Sprinting.\nLowers your energy.", gi->mousePos, 81);
                        gg->guiHoveredId = guiId;
                    }
                    iconPos.y += texDim.y + iconSep;
                }
            }


            // Emergency icons
            v2 scale = V2(1.f);
            if (Min(gi->windowDim.x, gi->windowDim.y) < 640)
                scale = V2(.75f);
            v2 emergencyDim = V2(96.f);
            v2 emergencyPos = temperatureBarPos - V2(15.f + emergencyDim.x*scale.x, 0);
            f32 emergencyMargin = 10.f;
            f32 circleRadius = emergencyDim.x*scale.x/2;
            s32 emergencyIndex = 0; 
            v4 shineCol = V4_White();
            if (energy < .2f){
                DrawSpriteRect(&gg->images[TEX_GUI], V2(3*emergencyDim.x, gg->images[TEX_GUI].dim.y - emergencyDim.y), emergencyDim, emergencyPos, scale, 80, V4_White()); 
                // Shine
                f32 shineAlpha = 1.f-Abs(Sin((gg->animationTime*2*PI/60.f)*.5f - 0.3f*emergencyIndex));
                shineAlpha = Square(shineAlpha)*.5f;
                shineCol.a = shineAlpha;
                DrawSpriteRect(&gg->images[TEX_GUI], V2(4*emergencyDim.x, gg->images[TEX_GUI].dim.y - emergencyDim.y), emergencyDim, emergencyPos, scale, 80, shineCol); 
                // Tooltip
                if (!gg->guiHoveredId && PointInCircle(gi->mousePos, emergencyPos + V2(circleRadius), circleRadius)){
                    char text[100];
                    sprintf_s(text, ArrayCount(text), "LOW ENERGY: E=%i%%\nSleep to recover energy.\nIf E reaches 0%% you will pass out.",
                              (s32)Round(energy*100));
                    DrawTextTooltip(text, gi->mousePos, 81);
                }
                emergencyPos.x += -emergencyMargin - emergencyDim.x*scale.x;
                emergencyIndex++;
            }
            if (stomach <= 0){
                DrawSpriteRect(&gg->images[TEX_GUI], V2(5*emergencyDim.x, gg->images[TEX_GUI].dim.y - emergencyDim.y), emergencyDim, emergencyPos, scale, 80, V4_White()); 
                // Shine
                f32 shineAlpha = 1.f-Abs(Sin((gg->animationTime*2*PI/60.f)*.5f - 0.3f*emergencyIndex));
                shineAlpha = Square(shineAlpha)*.5f;
                shineCol.a = shineAlpha;
                DrawSpriteRect(&gg->images[TEX_GUI], V2(4*emergencyDim.x, gg->images[TEX_GUI].dim.y - emergencyDim.y), emergencyDim, emergencyPos, scale, 80, shineCol); 
                // Tooltip
                if (!gg->guiHoveredId && PointInCircle(gi->mousePos, emergencyPos + V2(circleRadius), circleRadius)){
                    char text[100];
                    sprintf_s(text, ArrayCount(text), "STARVING: Stomach=%i%%\nEat something!\nYou are losing energy and health.",
                              (s32)Round(stomach*100));
                    DrawTextTooltip(text, gi->mousePos, 81);
                }
                emergencyPos.x += -emergencyMargin - emergencyDim.x*scale.x;
                emergencyIndex++;
            }
            if (temperature < coldLimit){
                b32 xtreme = (temperature < extremeColdLimit);
                v2 texPos = V2(0, gg->images[TEX_GUI].dim.y - emergencyDim.y*(xtreme ? 2 : 1));
                DrawSpriteRect(&gg->images[TEX_GUI], texPos, emergencyDim, emergencyPos, scale, 80, V4_White()); 
                // Shine
                f32 shineAlpha = 1.f-Abs(Sin((gg->animationTime*2*PI/60.f)*.5f - 0.3f*emergencyIndex));
                shineAlpha = Square(shineAlpha)*.5f;
                shineCol.a = shineAlpha;
                DrawSpriteRect(&gg->images[TEX_GUI], V2(4*emergencyDim.x, gg->images[TEX_GUI].dim.y - emergencyDim.y), emergencyDim, emergencyPos, scale, 80, shineCol); 
                // Tooltip
                if (!gg->guiHoveredId && PointInCircle(gi->mousePos, emergencyPos + V2(circleRadius), circleRadius)){
                    char text[100];
                    sprintf_s(text, ArrayCount(text), (xtreme ? "EXTREME COLD: T=%i%%\nYou are losing energy and health!" : "COLD: T=%i%%\nYou are losing energy."),
                              (s32)Round(temperature*100));
                    DrawTextTooltip(text, gi->mousePos, 81);
                }
                emergencyPos.x += -emergencyMargin - emergencyDim.x*scale.x;
                emergencyIndex++;
            }
            if (temperature > heatLimit){
                b32 xtreme = (temperature > extremeHeatLimit);
                v2 texPos = V2(emergencyDim.x, gg->images[TEX_GUI].dim.y - emergencyDim.y*(xtreme ? 2 : 1));
                DrawSpriteRect(&gg->images[TEX_GUI], texPos, emergencyDim, emergencyPos, scale, 80, V4_White()); 
                // Shine
                f32 shineAlpha = 1.f-Abs(Sin((gg->animationTime*2*PI/60.f)*.5f - 0.3f*emergencyIndex));
                shineAlpha = Square(shineAlpha)*.5f;
                shineCol.a = shineAlpha;
                DrawSpriteRect(&gg->images[TEX_GUI], V2(4*emergencyDim.x, gg->images[TEX_GUI].dim.y - emergencyDim.y), emergencyDim, emergencyPos, scale, 80, shineCol); 
                // Tooltip
                if (!gg->guiHoveredId && PointInCircle(gi->mousePos, emergencyPos + V2(circleRadius), circleRadius)){
                    char text[100];
                    sprintf_s(text, ArrayCount(text), (xtreme ? "EXTREME HEAT: T=%i%%\nYou are losing energy and health!" : "HEAT: T=%i%%\nYou are losing energy."),
                              (s32)Round(temperature*100));
                    DrawTextTooltip(text, gi->mousePos, 81);
                }
                emergencyPos.x += -emergencyMargin - emergencyDim.x*scale.x;
                emergencyIndex++;
            }

            // Draw bar frames
            DrawRectFrame(hpBarPos, hpBarDim, barFrameSize, hpBarColor, 80);
            DrawRectFrame(energyBarPos, energyBarDim, barFrameSize, energyBarColor, 80);
            DrawRectFrame(stomachBarPos, stomachBarDim, barFrameSize, stomachBarColor, 80);
            // (temperature frame drawn above)
        }

        // Minimap
        {
            // Map
            v2 borderMargin = V2(20.f);
            v2 mapDim = V2(150.f);
            v2 mapPos =  gi->windowDim - borderMargin - mapDim;
            DrawMap(mapPos, mapDim, .6f, 1.f, gg->seaLevel, 80);
            
            // Player pos icon
            {
                v2 texPos = V2(0, gg->images[TEX_GUI].dim.y - 352);
                v2 texDim = V2(32);
                v2 windowPos = mapPos + V2(mapDim.x*Clamp(gg->playerPos.x/(TILES_X*TILE_SIZE), 0.f, 1.f),
                                           mapDim.y*Clamp(gg->playerPos.y/(TILES_Y*TILE_SIZE), 0.f, 1.f));
                v2 scale = V2(.5f);
                v2 drawPos = windowPos - V2(scale.x*texDim.x/2, 0);
                DrawSpriteRect(&gg->images[TEX_GUI], texPos, texDim, drawPos, scale, 80, V4_Red()); 
            }

            // Black border
            DrawRectFrame(mapPos, mapDim, V2(3.f), V4_Black(), 80);


            // Clock (time window)
            v2 clockDim = V2(mapDim.x, 22.f);
            v2 clockPos = V2(mapPos.x, mapPos.y - clockDim.y - 3.f - 10.f);
            v4 dayCol = V4(.51f, .47f, .4f, .7f);
            v4 nightCol = V4(.51f*.3f, .47f*.3f, .4f*.3f, .7f);
            s32 clockTimeWindow = DAY_LENGTH; // in frames
            f32 clockTimeWindowInDays = clockTimeWindow/(f32)DAY_LENGTH;
            f32 pixelsPerDay = clockDim.x / clockTimeWindowInDays;

            s32 time0 = gg->time - clockTimeWindow/2;
            s32 time1 = gg->time + clockTimeWindow/2;
            day_and_daytime timeItInDays = TimeToDayAndDaytime(time0);
            v2 drawPos = clockPos;
            while(DayAndDaytimeToTime(timeItInDays) < time1){
                if (timeItInDays.daytime >= NIGHT_END && timeItInDays.daytime < NIGHT_START){
                    // Day rect
                    day_and_daytime timeNightStart = timeItInDays;
                    timeNightStart.daytime = NIGHT_START;
                    v2 drawPos1 = drawPos + V2((timeNightStart.daytime - timeItInDays.daytime)*pixelsPerDay, clockDim.y);
                    if (drawPos1.x > clockPos.x + clockDim.x)
                        drawPos1.x = clockPos.x + clockDim.x;
                    DrawRect(drawPos, drawPos1 - drawPos, dayCol, 80);
                    // Advance
                    drawPos.x = drawPos1.x;
                    timeItInDays = timeNightStart;
                }else{
                    // Night rect
                    day_and_daytime newTimeItInDays = timeItInDays;
                    newTimeItInDays.daytime = NIGHT_END;
                    f32 daysTillSunrise;
                    b32 drawMidnight = false;
                    f32 daysTillMidnight = 0;
                    if (timeItInDays.daytime > NIGHT_END){ // advance to next day.
                        daysTillSunrise = 1.f - timeItInDays.daytime + NIGHT_END;
                        newTimeItInDays.day++;
                        daysTillMidnight = 1.f - timeItInDays.daytime;
                        drawMidnight = true;
                    }else{ // already in the next day.
                        daysTillSunrise = NIGHT_END - timeItInDays.daytime;
                    }
                    v2 drawPos1 = drawPos + V2(daysTillSunrise*pixelsPerDay, clockDim.y);
                    if (drawPos1.x > clockPos.x + clockDim.x)
                        drawPos1.x = clockPos.x + clockDim.x;
                    DrawRect(drawPos, drawPos1 - drawPos, nightCol, 80);
                    if (drawMidnight){
                        v2 pos = drawPos + V2(daysTillMidnight*pixelsPerDay, 0);
                        f32 widthRight = 2.f;
                        f32 widthLeft = 2.f;
                        f32 mapOutline = 3.f;
                        if (pos.x > mapPos.x && pos.x < mapPos.x + mapDim.x + mapOutline){
                            DrawRect(pos - V2(widthLeft, 0), V2(widthLeft, clockDim.y), V4_Black(), 80);
                        }
                        if (pos.x < mapPos.x + mapDim.x && pos.x > mapPos.x - mapOutline){
                            DrawRect(pos, V2(widthRight, clockDim.y), V4_Black(), 80);
                        }
                    }
                    // Advance
                    drawPos.x = drawPos1.x;
                    timeItInDays = newTimeItInDays;
                }
            }

                // Center mark
            //DrawRect(clockPos + V2(clockDim.x/2 - 2.f, 0), V2(4.f, clockDim.y), V4_White(), 80);
            v2 triDim = V2(.6f, .3f)*clockDim.y;
            DrawTriangle(clockPos + V2(clockDim.x/2, clockDim.y - triDim.y), clockPos + V2(clockDim.x/2 - triDim.x/2, clockDim.y), clockPos + V2(clockDim.x/2 + triDim.x/2, clockDim.y), V4_Black(), 80);
            DrawTriangle(clockPos + V2(clockDim.x/2, triDim.y), clockPos + V2(clockDim.x/2 - triDim.x/2, 0), clockPos + V2(clockDim.x/2 + triDim.x/2, 0), V4_Black(), 80);
                // Black frame
            DrawRectFrame(clockPos, clockDim, V2(3.f), V4_Black(), 80);



            // Time string
            s32 timeSinceDay0 = GameFrameTimeToFrameTimeSinceDay0(gg->time);
            days_hours_minutes dhm = TimeInFramesToDaysHoursMinutes(timeSinceDay0);
            char str[64];
            sprintf_s(str, ArrayCount(str), "day%i %02i:%02i", dhm.days, dhm.hours, dhm.minutes);
            v2 strScale = V2(.82f);
            v2 strPos = AlignStringRight(str, V2(clockPos.x + clockDim.x, clockPos.y - 27.f), strScale.x);
            DrawString(str, strPos, strScale, 80, V4_Black());

        }

        //
        // Delete entities marked as deleted.
        //
        for(s32 i = 0; i < gg->numEntities; i++){
            entity *e = gg->entities + i;
            if (e->deleted){
                // copy last entity into the emptied slot.
                if (i != gg->numEntities - 1){
                    entity *lastEntity = gg->entities + gg->numEntities - 1;
                    memcpy(e, lastEntity, sizeof(entity));
                }
                gg->numEntities--;
            }
        }

        
        // Death rect
        if (dead && !gg->passedOutTimer){
            s32 timeDead = gg->time - gg->timeOfDeath;
            f32 alpha = LerpClamp(0.f, 1.f, timeDead/(60.f*3.f));
            DrawRect(V2(0), gi->windowDim, V4_Black(alpha), 98);
            if (timeDead >= 60*3){
                gg->activeSection = PS_FinalScreen;
                gg->worldIsValid = false;
            }
            gg->pause = false;
        }


        // Sleep effect
        if (gg->sleepGradientEffectTimer){
            f32 a = Clamp01(gg->sleepGradientEffectTimer / (f32)gg->sleepGradientEffectMaxTime);
            DrawRect(V2(0), gi->windowDim, V4_Black(a), 75);
            gg->sleepGradientEffectTimer--;
        }

        //
        // Pause
        //
        if (gg->pause){
            v2 buttonDim = V2(200.f, 46.f);
            v2 buttonPos = gi->windowDim/2 - buttonDim/2;

            DrawRect(V2(0), gi->windowDim, V4_Black(.5f), 80);

            char *str = "PAUSE";
            v2 strScale = V2(2.f);
            v2 strPos = V2(gi->windowDim.x/2.f, Min(gi->windowDim.y*.75f, buttonPos.y + buttonDim.y + 80.f));
            DrawString(str, CenterStringXY(str, strPos, strScale), strScale, 98, V4_White());
        
            if (DoButton(1201, buttonPos, buttonDim, "Continue")){
                gg->pause = false;
            }
                                 
            if (DoButton(1202, gi->windowDim/2 - V2(0, 60.f) - buttonDim/2, buttonDim, "Abandon")){
                gg->activeSection = PS_MainMenu;
                gg->pause = false;
            }
        
            if (ButtonWentDown(&gi->keyboard.escape)){
                gg->pause = false;
            }
        }else{
            //
            // Program State change.
            //
            if (ButtonWentDown(&gi->keyboard.escape)){
                if (gg->sleepWindowIsOpen){
                    gg->sleepWindowIsOpen = false;
                }else if (gg->noteIsOpen){
                    gg->noteIsOpen = false;
                }else if (!dead){
                    gg->pause = !gg->pause;
                }
            }
            if (gg->sleepWindowIsOpen && ButtonWentDown(&gi->keyboard.letters['E' - 'A'])){
                gg->sleepWindowIsOpen = false;
            }
            if (gg->noteIsOpen && ButtonWentDown(&gi->keyboard.letters['E' - 'A'])){
                gg->noteIsOpen = false;
            }
        }

        if (ButtonWentDown(&gi->keyboard.letters['M' - 'A']) || ButtonWentDown(&gi->keyboard.tab)){
            gg->activeSection = PS_Map;
        }

    }else if (gg->activeSection == PS_Map){
        clearCol = LerpV4(V4_Black(), WATER_COLOR_DEEP, 1.f);

        
        char text[500] = "Map";
        v2 scale = V2(1.f);
        DrawString(text, CenterStringX(text, V2(gi->windowDim.x/2, gi->windowDim.y - 40.f), scale.x), scale, 50, V4_White());

        f32 size = Min(gi->windowDim.x, gi->windowDim.y);
        size -= 190.f;
        v2 mapDim = V2(size, size);
        v2 mapPos = gi->windowDim/2 - mapDim/2 + V2(0, -35.f);

        // Map border
        DrawRectFrame(mapPos, mapDim, V2(3.f), V4_Black(.1f), 70);

        
        // player sink...
        //{
        //  tile *playerTile = WorldPosGetTile(gg->playerPos);
        //
        //  s32 secondsLeft = ((f32)playerTile->h - (f32)gg->seaLevel)/60;
        //  char str[100];
        //  if (secondsLeft > 0){
        //      if (secondsLeft < 60.f)
        //          sprintf_s(str, "Your location will flood in %i\"", (s32)secondsLeft);
        //      else
        //          sprintf_s(str, "Your location will flood in %i'%02i\"",
        //                    (s32)(secondsLeft/60), ((s32)secondsLeft) % 60);
        //  }else{
        //      sprintf_s(str, "You are in the waters!");
        //  }
        //  DrawString(str, V2(90.f, 26.f), V2(1.f), 90, V4_White());
        //}

        // Time has stopped
        char *str = "(time has stopped)";
        DrawString(str, CenterStringX(str, V2(gi->windowDim.x/2, 26.f)), V2(1.f), 90, V4_White());


        // Sea level bar
        u16 seaLevel = gg->seaLevel;

            // Draw inside
        v2 barDim = V2(gi->windowDim.x*.8f, 40.f);
        v2 barPos = V2(gi->windowDim.x/2, gi->windowDim.y - 80.f) - barDim/2;
        DrawRect(barPos, barDim, V4(.51f, .47f, .4f), 95);
        f32 currentSeaLevelFactor = Clamp((seaLevel - INITIAL_SEA_LEVEL)/(f32)(MAX_SEA_LEVEL - INITIAL_SEA_LEVEL), 0.f, 1.f);
        
        f32 seaLevelFactor = currentSeaLevelFactor;
        if (PointInRect(gi->mousePos, barPos, V2(barPos.x + barDim.x, gi->windowDim.y))){
            seaLevelFactor = Clamp((gi->mousePos.x - barPos.x)/barDim.x, 0.f, 1.f);
            seaLevel = INITIAL_SEA_LEVEL + (s32)((MAX_SEA_LEVEL - INITIAL_SEA_LEVEL)*seaLevelFactor);
        }

        // Flooded part
        DrawRect(barPos, V2(seaLevelFactor*barDim.x, barDim.y), V4_Blue(), 95);

        // Black Bar Outline
        DrawRectFrame(barPos, barDim, V2(3.f), V4_Black(), 94);

        char levelString[50];
        sprintf(levelString, "Sea level: %i%%", (s32)(101*seaLevelFactor));
        DrawString(levelString, CenterStringXY(levelString, barPos + barDim/2), V2(1.f), 97, V4_White());

        // Draw Day/Night Marks
        {
            s32 day = 1;
            while(true){
                day_and_daytime dad = {day, 0.f};
                s32 time = DayAndDaytimeToTime(dad);
                if (time > MAX_TIME)
                    break;
                v2 pos = barPos + V2(Clamp01(time/(f32)MAX_TIME)*barDim.x, 0);
                // Day start marker
                DrawRect(pos, V2(3.f, barDim.y), V4_Black(), 96);

                // Night highlight
                f32 maxTimeInDays = MAX_TIME/(f32)DAY_LENGTH;
                f32 daySizeInPixels = (barDim.x/maxTimeInDays);
                f32 nightSizeInPixels = daySizeInPixels*((1.f - NIGHT_START) + NIGHT_END);
                v2 nightStartPos = pos - V2(daySizeInPixels*(1.f - NIGHT_START), 0);
                if (nightStartPos.x + nightSizeInPixels > barPos.x && nightStartPos.x < barPos.x + barDim.x){
                    nightStartPos.x = Max(barPos.x, nightStartPos.x);
                    f32 nightSize = Min(nightSizeInPixels, barPos.x + barDim.x - nightStartPos.x);
                    DrawRect(nightStartPos, V2(nightSize, barDim.y), V4_Black(.4f), 96);
                }

                // Day number
                char str[24];
                sprintf_s(str, ArrayCount(str), "day %i", day);
                v2 strScale = V2(.5f);
                v2 strPos = pos + V2(- daySizeInPixels/2, -20.f);
                DrawString(str, CenterStringX(str, strPos, strScale.x), strScale, 96, V4_Black());

                day++;
            }
        }
            // Current time mark
        DrawRect(barPos + V2(currentSeaLevelFactor*barDim.x - 1.5f, 0), V2(3.f, barDim.y), V4_White(), 96);


        // Usage text
        {
            char *originalText = "Hover the time bar to see how the island will look as the sea level rises.";

            v2 strPos = V2(28.f, barPos.y - 64.f);

            char str[120];
            f32 strMaxWidth = Max(230.f, mapPos.x - 17.f - strPos.x);
            ChopStringToFitWidth(str, ArrayCount(str), originalText, strMaxWidth);

            DrawString(str, strPos, V2(1.f), 90, V4_White());

        }

        // Draw map
        s32 mapSize = 100;
        v2 cellDim = mapDim/mapSize;
        //for(s32 y = 0; y < mapSize; y++){
        //  for(s32 x = 0; x < mapSize; x++){
        //      s32 tileX = TILES_X*x/mapSize;
        //      s32 tileY = TILES_Y*y/mapSize;
        //      tile t = gg->tiles[TILES_X*tileY + tileX];
        //      v4 col = TileGetColor(t, seaLevel);
        //      v2 cellPos = mapPos + V2(x*cellDim.x, y*cellDim.y);
        //      DrawRect(cellPos, cellDim, col, 60 + (y*20/mapSize));
        //  }
        //}
        f32 pointSize = 1.f;
        f32 maxPointsPerAxis = 300.f;
        while(mapDim.x/pointSize > maxPointsPerAxis){
            pointSize += 1.f;
        }
        DrawMap(mapPos, mapDim, 1.f, pointSize, seaLevel, 60, false);

        //
        // DEBUG: draw biome points.
        //
        if (gg->debugOn){
            for(s32 i = 0; i < ArrayCount(biomePoints); i++){
                v2 point = biomePoints[i].pos;
                biome_id biome = biomePoints[i].biome;
                f32 a = .9f;
                v4 colors[4] = {V4_Green(a), V4(0.f, .5f, .2f, a), V4(1.f, .4f, 0.f, a), V4_Grey(.2f, a)};
                v2 scale = V2(1.f, 1.f);
                v2 pos = mapPos + V2(mapDim.x*point.x/(TILES_X*TILE_SIZE), mapDim.y*point.y/(TILES_Y*TILE_SIZE));
                DrawCircle(pos, 6.f, colors[(s32)biome], 95);

                char str[20];
                itoa(i, str, 10);
                DrawString(str, pos, V2(.5f), 95, V4_Black());
            }

            // Peak
            char *str = "P";
            v2 strPos = mapPos + V2(mapDim.x*DEBUG_peakPos.x/(TILES_X*TILE_SIZE), mapDim.y*DEBUG_peakPos.y/(TILES_Y*TILE_SIZE));
            DrawString(str, CenterStringXY(str, strPos, V2(.5f)), V2(.5f), 95, V4_Black());
        }
            
        
        // "Map state in n seconds"
        //if (seaLevel != gg->seaLevel){
        //  s32 seconds = ((f32)seaLevel - (f32)gg->seaLevel)/60;
        //  char str[100];
        //  if (seconds > 0){
        //      if (seconds < 60.f)
        //          sprintf_s(str, "Water level after %i\"", (s32)seconds);
        //      else
        //          sprintf_s(str, "Water level after %i'%02i\"",
        //                    (s32)(seconds/60), ((s32)seconds) % 60);
        //      DrawString(str, V2(gi->windowDim.x - 20.f - StringWidth(str), gi->windowDim.y - 140.f), V2(1.f), 90, V4_White());
        //  }else{
        //      if (seconds > -60.f)
        //          sprintf_s(str, "Water level %i\" ago", (s32)-seconds);
        //      else
        //          sprintf_s(str, "Water level %i'%02i\" ago",
        //                    (s32)(-seconds/60), ((s32)-seconds) % 60);
        //      DrawString(str, V2(20.f, gi->windowDim.y - 140.f), V2(1.f), 90, V4_White());
        //  }
        //}

        { // Player Pos Icon
            v2 texPos = V2(0, gg->images[TEX_GUI].dim.y - 352);
            v2 texDim = V2(32);
            v2 windowPos = mapPos + V2(mapDim.x*Clamp01(gg->playerPos.x/(TILES_X*TILE_SIZE)),
                                       mapDim.y*Clamp01(gg->playerPos.y/(TILES_Y*TILE_SIZE)));
            v2 scale = V2(1.f);
            v2 drawPos = windowPos - V2(scale.x*texDim.x/2, 0);
            DrawSpriteRect(&gg->images[TEX_GUI], texPos, texDim, drawPos, scale, 90, V4_Red()); 

            // We used to draw a red X instead...
            //v2 scale = V2(2.f, 1.4f);
            //v2 pos = CenterStringXY("X",
            //                      mapPos + V2(mapDim.x*Clamp(gg->playerPos.x/(TILES_X*TILE_SIZE), 0.f, 1.f),
            //                                  mapDim.y*Clamp(gg->playerPos.y/(TILES_Y*TILE_SIZE), 0.f, 1.f)),
            //                      scale);
            //DrawString("X", pos, scale, 90, V4_Red());
        }
        
        if (ButtonWentDown(&gi->keyboard.escape) || ButtonWentDown(&gi->keyboard.letters['E' - 'A'])){
            gg->activeSection = PS_World;
        }
        if (ButtonWentDown(&gi->keyboard.letters['M' - 'A']) || ButtonWentDown(&gi->keyboard.tab)){
            gg->activeSection = PS_World;
        }
        v2 buttonDim = V2(55.f, 46.f);
        if (DoButton(1101, V2(20.f, 20.f), buttonDim, "<")){
            gg->activeSection = PS_World;
        }

    }else if (gg->activeSection == PS_Tutorial){
        clearCol = V4(.12f, .26f, .09f, 1.f);

        s32 numPages = ArrayCount(globalNotes) + 1;

        if (gg->tutorialMenuSelectedPage == 0){
            char *title = "HOW TO PLAY:";
            char *tutorialText = "\
Your goal is to survive in the island for as long as you can.\n\
The sea level rises constantly, forcing you to move uphill.\n\
But the higher you go, the colder it is, so you'll have to\n\
make a fire. Moving (WASD) consumes energy, so plan your\n\
route carefully. You can access the map with M to see how the\n\
island will evolve as it floods. Press E to access the\n\
\"inventory\" and see all the crafting recipes. You can also\n\
run by holding SHIFT, but it consumes more energy than walking.\n\
\n\
Only sleep can recover energy. You can sleep at any time if\n\
you have a bed, a sleeping bag, or a hay bed. Sleeping will\n\
also recover health once your energy is full. If you don't\n\
sleep and your energy reaches 0, you'll pass out for some\n\
hours, during which you'll die if the tide reaches you.\n\
\n\
Other things that deplete energy are temperature and hunger.\n\
Hunger and extreme temperature will also deplete your health.\n\
But above all, escape the rising tide: getting in the water\n\
will make you waste heat and energy.\n\
\n\
Good luck!";
            v2 scale = V2(1.f);
            v2 textPos = CenterStringXY(tutorialText, gi->windowDim/2, scale);
            DrawString(tutorialText, textPos, scale, 50, V4_White());

            scale = V2(2.f);
            v2 titlePos = V2(gi->windowDim.x/2, textPos.y + 45.f);
            DrawString(title, CenterStringX(title, titlePos, scale.y), scale, 50, V4_White());
        }else{
            // Note string
            s32 noteIndex = Clamp(gg->tutorialMenuSelectedPage - 1, 0, ArrayCount(globalNotes) - 1);
            char *str = globalNotes[noteIndex];
            //char str[400];
            //sprintf_s(str, ArrayCount(str), "%s\n\n(Tip %i/%i)", globalNotes[noteIndex], noteIndex + 1, ArrayCount(globalNotes));
            v2 scale = V2(1.f);
            f32 bottomMargin = 46.f*2 + 20.f + 10.f + 10.f;
            v2 textCenterPos = V2(gi->windowDim.x/2, bottomMargin + (gi->windowDim.y - bottomMargin)/2);
            v2 textPos = CenterStringXY(str, textCenterPos, scale);
            DrawString(str, textPos, scale, 50, V4_Black());

            // Note rectangle
            f32 noteMargin = 50.f;
            v2 strDim = StringDim(str);
            v2 rectPos = textCenterPos - strDim/2 - V2(noteMargin);
            strDim.y += 30.f;
            rectPos.y -= 30.f;
            DrawRect(rectPos, strDim + 2*V2(noteMargin), V4_Grey(.8f), 40);
            DrawRectFrame(rectPos, strDim + 2*V2(noteMargin), V2(3.f), V4_Black(), 40);

            // Tip index string
            char indexStr[50];
            sprintf_s(indexStr, ArrayCount(indexStr), "(Tip %i/%i)", noteIndex + 1, ArrayCount(globalNotes));
            DrawString(indexStr, CenterStringXY(indexStr, V2(gi->windowDim.x, bottomMargin)/2, V2(1.f)), scale, 40, V4_White());
        }


        // Buttons
        v2 buttonDim = V2(100.f, 46.f);
        if (DoButton(1101, V2(20.f, 20.f), buttonDim, "Back")){
            gg->activeSection = PS_MainMenu;
        }
        buttonDim = V2(200.f, 46.f);
        if (gg->tutorialMenuSelectedPage > 0 ){
            if (DoButton(1102, V2(20.f, 20.f + buttonDim.y + 10.f), buttonDim, "Previous")){
                gg->tutorialMenuSelectedPage -= 1;
            }
        }
        if (gg->tutorialMenuSelectedPage < numPages - 1){
            if (DoButton(1103, V2(gi->windowDim.x - 20.f - buttonDim.x, 20.f + buttonDim.y + 10.f), buttonDim, (gg->tutorialMenuSelectedPage == 0 ? "See tips" : "Next"))){
                gg->tutorialMenuSelectedPage += 1;
            }
        }
        if (ButtonWentDown(&gi->keyboard.escape)){
            gg->activeSection = PS_MainMenu;
        }

    }else if (gg->activeSection == PS_FinalScreen){
        clearCol = V4(.06f, 0.1f, .04f, 1.f);

        v2 buttonDim = V2(200.f, 46.f);
        v2 buttonPos = gi->windowDim/2 - buttonDim/2;


        s32 daysSurvived = gg->time / DAY_LENGTH;

        v2 strScale = V2(2.f);
        v2 strPos = V2(gi->windowDim.x/2.f, Min(gi->windowDim.y*.75f, buttonPos.y + buttonDim.y + 80.f));

        char str[100];
        if (gg->won){
            clearCol = WATER_COLOR_DEEP;
            sprintf_s(str, ArrayCount(str), "YOU WON!");
            strPos.y += gi->windowDim.y*.2f;

            char str2[100];
            sprintf_s(str2, ArrayCount(str2), "You escaped the island after %i day%s.", daysSurvived, (daysSurvived == 1 ? "" : "s"));
            DrawString(str2, CenterStringXY(str2, strPos - V2(0,90.f)), V2(1.f), 50, V4_White());

            // Draw raft
            v2 rotationPoint = {130.f, 35.f};
            v2 drawPos = {gi->windowDim.x/2 - 10.f, Lerp(gi->windowDim.y*.3f, 100.f, .5f)};
            f32 t0 = (f32)(gg->realTime % (60*6))/(f32)(60*6);
            f32 t1 = (f32)((gg->realTime + 20) % (60*6))/(f32)(60*6);
            f32 yOff = Cos(t0*2*PI)*40.f;
            if (yOff < 0){
                yOff *= .5f;
            }
            drawPos.y += yOff;
            f32 angle  = Cos(t1*2*PI)*PI*.03f;
            DrawSpriteRectRotated(&gg->images[TEX_RAFT], V2(0), gg->images[TEX_RAFT].dim, rotationPoint, drawPos, angle, V2(2.f), 30);

            // Draw player on raft
            v2 playerTexDim = {112, 192};
            v2 playerTexPos = V2(0, gg->images[TEX_PLAYER].dim.y - playerTexDim.y);
            v2 playerRotationPos = playerTexPos + V2(playerTexDim.x/2 + 60.f, 16.f);
            f32 playerAngle = Cos(t1*2*PI - .3f)*PI*.03f;
            DrawSpriteRectRotated(&gg->images[TEX_PLAYER], playerTexPos, playerTexDim, playerRotationPos,
                                  drawPos, playerAngle, V2(1.5f), 30);

        }else{
            sprintf_s(str, ArrayCount(str), "YOU SURVIVED FOR %i DAY%s", daysSurvived, (daysSurvived == 1 ? "" : "S"));
        }

        DrawString(str, CenterStringXY(str, strPos, strScale), strScale, 50, V4_White());
        
        //if (DoButton(1201, buttonPos, buttonDim, "Main Menu")){
        if (DoButton(1201, V2(20.f, 20.f), buttonDim, "Main Menu")){
            gg->activeSection = PS_MainMenu;
        }
                                 
        //if (DoButton(1202, gi->windowDim/2 - V2(0, 60.f) - buttonDim/2, buttonDim, "Main Menu")){
        //  gg->activeSection = PS_MainMenu;
        //}
        
        //if (ButtonWentDown(&gi->keyboard.escape)){
        //  gg->activeSection = PS_World;
        //}

    }

    // 
    // Debug
    //
    if (gg->debugOn){
        v2 buttonDim = V2(180.f, 42.f);
        v2 buttonPos = V2(gi->windowDim.x - buttonDim.x - 12.f, 40.f - buttonDim.y/2);
        f32 buttonSep = 8.f;
        char str[50] = "Stop time: ";
        strcat(str, (gg->debug.stopTime ? "ON" : "OFF"));
        if (DoButton(3001, buttonPos, buttonDim, str, 96, false, V2(.5f))){
            gg->debug.stopTime = !gg->debug.stopTime;
        }
        buttonPos.y += buttonDim.y + buttonSep;
        
        strcpy(str, "Time x10: ");
        strcat(str, (gg->debug.timeX10 ? "ON" : "OFF"));
        if (DoButton(3002, buttonPos, buttonDim, str, 96, false, V2(.5f))){
            gg->debug.timeX10 = !gg->debug.timeX10;
        }
        buttonPos.y += buttonDim.y + buttonSep;

        strcpy(str, "Run Speed x5: ");
        strcat(str, (gg->debug.runSpeedX5 ? "ON" : "OFF"));
        if (DoButton(3003, buttonPos, buttonDim, str, 96, false, V2(.5f))){
            gg->debug.runSpeedX5 = !gg->debug.runSpeedX5;
        }
        buttonPos.y += buttonDim.y + buttonSep;
        
        strcpy(str, "God mode: ");
        strcat(str, (gg->debug.godMode ? "ON" : "OFF"));
        if (DoButton(3004, buttonPos, buttonDim, str, 96, false, V2(.5f))){
            gg->debug.godMode = !gg->debug.godMode;
        }
        buttonPos.y += buttonDim.y + buttonSep;
        
        strcpy(str, "Give all items");
        if (DoButton(3005, buttonPos, buttonDim, str, 96, false, V2(.5f))){
            for (s32 i = 1; i < Item_Count; i++){
                if (i - 1 >= ArrayCount(gg->inventory))
                    break;
                gg->inventory[i - 1].id = (item_id)i;
                gg->inventory[i - 1].numItems = (globalItemProperties[i].flags & ItemFlag_Unstackable ? 1 : 100);
            }
        }
        buttonPos.y += buttonDim.y + buttonSep;

        strcpy(str, "Deplete Bars");
        if (DoButton(3006, buttonPos, buttonDim, str, 96, false, V2(.5f))){
            f32 amount = Clamp01((gi->mousePos.x - buttonPos.x) / buttonDim.x);
            gg->playerEnergy = amount;
            gg->playerTemperature = amount;
            gg->playerStomach = amount;
            gg->playerHealth = amount*gg->playerHealthMax;
        }
        buttonPos.y += buttonDim.y + buttonSep;


        // Data
        {
            char str[400];
            sprintf_s(str, ArrayCount(str), "num entities: %i", gg->numEntities);
            v2 pos = V2(gi->windowDim.x - buttonDim.x - 30.f, 30.f);
            v2 scale = V2(.5f);
            DrawString(str, AlignStringRight(str, pos, scale.x), scale, 96, V4_White());
        }

    }

    gg->realTime++;

    DrawClear(clearCol, 1);

    GuiEndOfFrameReset();
}
