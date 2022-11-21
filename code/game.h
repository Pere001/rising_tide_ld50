

#ifndef GAME_H
#define GAME_H

#define TILES_X 2048//2024
#define TILES_Y TILES_X
#define TILE_SIZE 64.f
#define INITIAL_SEA_LEVEL 30000
#define MAX_HEIGHT 65000
#define MAX_SEA_LEVEL 65500
#define WATER_COLOR_DEEP V4(.09f, .24f, .8f)
#define WATER_COLOR_SHALLOW V4(.2f, .6f, .8f)
#define DEEP_WATER_DEPTH 1000
#define MAX_ENTITIES 4096
#define MAX_TILES_WITH_BEHAVIOUR 2048

#define MAX_LIGHT_MAP_SIZE 128




// "Daytime" means the time in a day, from 0 (start) to 1.f (end).
#define DAY_LENGTH (60*60*4) // in frames
#define NIGHT_START .9f
#define NIGHT_END .22f
#define INITIAL_DAYTIME (NIGHT_END + .02f)
#define MAX_TIME (DAY_LENGTH*6.97f)

#define BED_ENERGY_RECOVERY_RATE         ((1.f/(DAY_LENGTH/2.f))*1.f)
#define SLEEPINGBAG_ENERGY_RECOVERY_RATE ((1.f/(DAY_LENGTH/2.f))*.666f)
#define HAYBED_ENERGY_RECOVERY_RATE      ((1.f/(DAY_LENGTH/2.f))*.333f)
#define PASSED_OUT_ENERGY_RECOVERY_RATE  ((1.f/(DAY_LENGTH/2.f))*.22f)
#define CAMPFIRE_SHRINK_TIME (60*5)
#define HUNGER_RATE (1.f/(60*60*3)) // 3 minutes to deplete

#define ENTITY_Z_MIN 20
#define ENTITY_Z_MAX 70


#define TEX_RAFT 0
#define TEX_FONT 1
#define TEX_RABBIT 2
#define TEX_ITEMS 3
#define TEX_TILES 4
#define TEX_GOOSE 5
#define TEX_PLAYER 6
#define TEX_BOAR 7
#define TEX_GUI 8
#define TEX_EFFECTS 9
#define NUM_TEXTURES 10

enum temperature_state_type{
    Temperature_ExtremeHeat,
    Temperature_Heat,
    Temperature_Safe,
    Temperature_Cold,
    Temperature_ExtremeCold
};



static char *globalNotes[] = {
    "SLEEP\n\n\
Sleeping raises your energy until it's full, and then it starts\n\
raising your health. Sleep also empties your stomach, but at a\n\
slower rate than when awake. You can't sleep with an empty\n\
stomach.\n\
There are three tiers of beds, with different recovery rates.\n\
The normal bed is the best, sleeping bag is second, and the hay\n\
bed is the worst, i.e. it requires more hours to recover the same\n\
amount of energy and health.\n\
You can sleep at any time of the day, but try to sleep through the\n\
night to avoid its cold and dangers.", 

    "ENERGY\n\n\
Staying still doesn't consume energy. Walking consumes some energy.\n\
If you run you move 2X as fast, but you also consume 2X the energy\n\
as walking for the same distance.",

    "TREES\n\n\
Trees are only found in the forest biome.",
    
    "CAMPFIRES\n\n\
Campfires give you heat and light, but they lose power over time.\n\
You can add fuel to a campfire to make it last longer. It will also\n\
grow and provide more heat and light.\n\
The campfire gives more heat the closer you are to it, but be careful,\n\
if the campfire is big and you get too close you might get burned!",

    "FOOD\n\n\
You can cook meat via crafting when near a campfire. You can also\n\
cook eggs and make an omelet.",

    "WATER\n\n\
Avoid getting into the water. You will lose energy\n\
and your temperature will drop.",

    "HEALING\n\n\
You can recover health by sleeping when your energy is full or by\n\
eating medicine.",

    "BIOMES\n\n\
There are 4 biomes: prairie, forest, dry biome, and rock biome.\n\
- In the prairie you can find flowers and bunnies.\n\
- The forest is the best for wood and mushrooms, but there are also\n\
plenty of boars, which will attack you during the night.\n\
- In the dry biome the temperature is always higher, and it's also the\n\
best place to find hay.\n\
- Rocky biome is the only place to find big rocks.",//, but you don't wana pass the nigth there...",that will attack you during the night.\n\

    "TEMPERATURE\n\n\
The temperature bar shows your current body temperature with a black\n\
bar, and the exterior temperature with an arrow, indicating where your\n\
body temperature will move over time.\n\
The bar is broken up into 5 sections. The grey area is the safe zone.\n\
The light blue and light red indicate moderate cold and heat. If your\n\
body temperature goes there you will start losing energy. The strong\n\
blue and red sections at the extremes indicate extreme cold and heat.\n\
Staying there will make you lose health as well as energy.\n\
Clothing extends the area of the safe zone and reduces the cold/heat\n\
zones.",

    "TOOLS\n\n\
The axe can chop trees and logs, and remove placed furniture,\n\
including beds.\n\
The pickaxe can break rocks.\n\
Both also serve as weapons.",

    "CLOTHING\n\n\
The leather armor protects from animal attacks but it also\n\
provides resistance to both hot and cold.",

    "COMBAT\n\n\
Running (by pressing shift) in combats is very useful to dodge\n\
charging animals.",
    
    "WEAPONS\n\n\
The spear is the best weapon, and the stone the worst.",

    "PAUSE\n\n\
You can pause the game at any time by pressing escape.\n\
Use the pause to look at your situation, the inventory, or\n\
the recipes without having to worry about the rising tide.\n\
It's totally legal!",

    "THE MAP\n\n\
Press M or TAB to open the map. Put the mouse over the top bar to\n\
see how the island's shape will evolve as the sea level rises, and\n\
use that knowledge to plan your route.\n\
The four biomes are represented by four colors: dark green (forest),\n\
light green (prairie), brown (dry biome), and grey (rocks). On top\n\
of that, each of these colors becomes slightly lighter the higher it\n\
is and darker the lower it is.",

    "PICKING UP STUFF\n\n\
Instead of right-clicking each stick or stone to pick it up,\n\
hold the right mouse button to pick up multiple stuff quiker.",

    "RAFT\n\n\
You can escape the island by making a raft, but getting the\n\
materials won't be easy..."
    };


enum biome_id{
    Biome_Prairie = 0,
    Biome_Forest,
    Biome_Dry,
    Biome_Rocky,

    Biome_Count,
};

enum item_id{
    Item_None = 0,
    Item_Stone,
    Item_Flower,
    Item_Stick,
    Item_String,
    Item_Leather,
    Item_Leaf,
    Item_Feather,
    Item_Egg,

    Item_RawRedMeat,
    Item_RawWhiteMeat,
    Item_CookedRedMeat,
    Item_CookedWhiteMeat,

    Item_Sword,
    Item_Axe,
    Item_Pickaxe,
    Item_Spear,
    Item_LeatherHelmet,
    Item_LeatherJacket,
    Item_LeatherBoots, 

    Item_SleepingBag,
    Item_Bed,
    Item_Campfire,
    Item_Bench,
    Item_Mushroom,
    Item_Anvil,
    Item_Log,
    Item_Omelet,
    Item_Fiber,
    Item_Hay,

    Item_HayBed,
    Item_Medicine,
    Item_Flint,
    Item_Raft,
    
    Item_Count,
};

enum tile_object_type : u8{
    TileObject_None = 0,
    TileObject_Stone,
    TileObject_Stick,
    TileObject_Rock,
    TileObject_Log,
    TileObject_Flower1,
    TileObject_Mushroom,
    TileObject_TreeTrunk,
    TileObject_Hay,
    TileObject_Tree,
    TileObject_CampfireOff = 10,
    TileObject_Campfire,
    TileObject_Bench,
    TileObject_Bed,
    TileObject_SleepingBag,
    TileObject_Anvil,
    TileObject_HayBed,
    TileObject_Plant1 = 20,
    TileObject_Grass1,
    TileObject_Grass2,
    TileObject_Grass3,
    TileObject_Grass4,
    TileObject_Grass5,
    TileObject_GrassAndRocks1,
    TileObject_Rocks1,
    TileObject_Rocks2,
    TileObject_Rocks3,
    TileObject_Campfire0 = 30,
    TileObject_Campfire1,
    TileObject_Campfire2,
    TileObject_Campfire3,
    TileObject_Campfire4,
    TileObject_Campfire5,
    TileObject_Campfire6,
    TileObject_Campfire7,
    TileObject_Campfire8,
    TileObject_Campfire9,
    TileObject_Note = 40, 

    TileObject_Count
};
enum item_flags{
    ItemFlag_Placeable   = 0x01,
    ItemFlag_Unstackable = 0x02,
    ItemFlag_Consumable  = 0x04, // This flag might be unnecessary
    ItemFlag_Actionable  = 0x08, // This flag is totally unnecessary
};
struct item_property{
    b32 flags;
    tile_object_type placeableObject; // Only applied when ItemFlag_Placeable.
    s32 flamability; // range 0-9
};
static item_property globalItemProperties[Item_Count] = {};
void InitGlobalItemProperties(){
    globalItemProperties[Item_Stick].flamability = 1;
    globalItemProperties[Item_Hay].flamability = 1;
    globalItemProperties[Item_Log].flamability = 2;

    globalItemProperties[Item_Stone].flags   = ItemFlag_Actionable;
    globalItemProperties[Item_Flower].flags  = 0;
    globalItemProperties[Item_Stick].flags   = 0;
    globalItemProperties[Item_String].flags  = 0;
    globalItemProperties[Item_Leather].flags = 0;
    globalItemProperties[Item_Leaf].flags    = 0;
    globalItemProperties[Item_Feather].flags = 0;
    globalItemProperties[Item_Egg].flags     = 0;
    
    globalItemProperties[Item_RawRedMeat].flags      = ItemFlag_Consumable;
    globalItemProperties[Item_RawWhiteMeat].flags    = ItemFlag_Consumable;
    globalItemProperties[Item_CookedRedMeat].flags   = ItemFlag_Consumable;
    globalItemProperties[Item_CookedWhiteMeat].flags = ItemFlag_Consumable;
    globalItemProperties[Item_Mushroom].flags        = ItemFlag_Consumable;
    globalItemProperties[Item_Medicine].flags        = ItemFlag_Consumable;
    globalItemProperties[Item_Omelet].flags          = ItemFlag_Consumable;
    
    globalItemProperties[Item_Sword].flags         = ItemFlag_Actionable | ItemFlag_Unstackable;
    globalItemProperties[Item_Axe].flags           = ItemFlag_Actionable | ItemFlag_Unstackable;
    globalItemProperties[Item_Pickaxe].flags       = ItemFlag_Actionable | ItemFlag_Unstackable;
    globalItemProperties[Item_Spear].flags         = ItemFlag_Actionable | ItemFlag_Unstackable;
    globalItemProperties[Item_LeatherHelmet].flags = ItemFlag_Unstackable;
    globalItemProperties[Item_LeatherJacket].flags = ItemFlag_Unstackable;
    globalItemProperties[Item_LeatherBoots].flags  = ItemFlag_Unstackable;
    
    globalItemProperties[Item_SleepingBag].flags           = ItemFlag_Placeable;
    globalItemProperties[Item_SleepingBag].placeableObject = TileObject_SleepingBag;
    globalItemProperties[Item_Bed].flags           = ItemFlag_Placeable;
    globalItemProperties[Item_Bed].placeableObject = TileObject_Bed;
    globalItemProperties[Item_HayBed].flags           = ItemFlag_Placeable;
    globalItemProperties[Item_HayBed].placeableObject = TileObject_HayBed;
    globalItemProperties[Item_Campfire].flags           = ItemFlag_Placeable;
    globalItemProperties[Item_Campfire].placeableObject = TileObject_Campfire;
    globalItemProperties[Item_Bench].flags           = ItemFlag_Placeable; 
    globalItemProperties[Item_Bench].placeableObject = TileObject_Bench;
    globalItemProperties[Item_Anvil].flags           = ItemFlag_Placeable; 
    globalItemProperties[Item_Anvil].placeableObject = TileObject_Anvil;

}

enum entity_type{
    Entity_Item,

    Entity_ThrownStone,

    Entity_Boar,
    //Entity_Duck,
    Entity_Goose,
    Entity_Rabbit,
    Entity_Wolf,
    Entity_Spider,

    Entity_Bed,
    Entity_Campfire,
    Entity_TexParticle,
};

struct entity_item{
    item_id itemId;
    s32 numItems;
    s32 creationTime;
};
struct entity_thrown_stone{
    s32 creationTime;
};

struct entity_boar{
    s32 attackCooldown;
    s32 lastHurtPlayerTime; // last time that we hurt the player.
};

struct entity_goose{
    s32 layEggCooldown;
    s32 attackCooldown;
    s32 lastHurtPlayerTime; // last time that we hurt the player.
    s32 angryCooldown;
};
struct entity_rabbit{
    s32 scaredTimer;
};
struct entity_wolf{
};
struct entity_spider{
};
  
struct entity_bed{
};
  
struct entity_campfire{
    b32 isOn;
    item_id holdingItem;
};
struct entity_tex_particle{
    s32 imageIndex;
    v2 texPos;
    v2 texDim;
    v2 scale;
    s32 z;

    f32 h; // height from ground.
    f32 hSpeed;
    f32 hGravity;

    f32 speedMultiplier;
    s32 maxLifetime;
    s32 creationTime;

    v4 color;
};

struct entity{
    v2 pos;
    v2 speed;
    s32 id;
    s32 state;
    s32 stateTimer;
    entity_type type;
    b32 deleted;
    s32 health;
    b32 cantHurtPlayer; // while this is true the entity can't hurt the player.
    b32 facingLeft;
    union{
        entity_item item;
        entity_thrown_stone thrownStone;
        entity_boar boar;
        entity_goose goose;
        entity_rabbit rabbit;
        entity_wolf wolf;
        entity_spider spider;
        entity_bed bed;
        entity_campfire campfire;
        entity_tex_particle texParticle;
    }data;
};




struct loaded_image{
    u32 *base;
    v2 dim;
    s32 texId; // OpenGl texture id.
};

enum program_section{
    PS_MainMenu,
    //PS_Pause,
    PS_Map,
    PS_CreatingWorld,
    PS_World,
    PS_FinalScreen,
    PS_Tutorial
};

struct tile{
    u16 h; // height
    u8 biomeData; // biome_id
    tile_object_type object;
    u16 objectData;
};

struct worker_thread_state{
    HANDLE handle;
    DWORD id;

    volatile b32 lock;
    //b32 stopThread;
    
    struct{
        volatile b32 startGenerating;
        volatile b32 cancelWork;
    } input;

    struct{
        volatile b32 sleeping;
        volatile b32 finishedGenerating;
        volatile f32 progress; // 0-1
    } output;
};

struct inventory_item{
    item_id id;
    s32 numItems;
};

enum crafting_table_type{
    CraftingTable_None = 0,
    CraftingTable_Bench,
    CraftingTable_Anvil,
    CraftingTable_Campfire,

    CraftingTable_Count
};

static char *globalCraftingTableNames[CraftingTable_Count] = {
        "", "Bench", "Anvil", "Campfire"};

struct crafting_recipe{
    inventory_item result;
    inventory_item ingredients[5];
    crafting_table_type craftingTable;
};

static crafting_recipe globalRecipes[] = {
            {{Item_Bench, 1},       {{Item_Stick, 4}, {}, {}, {}, {}}, CraftingTable_None},
            {{Item_Campfire, 1},    {{Item_Stick, 5}, {}, {}, {}, {}}, CraftingTable_None},

            {{Item_Sword, 1},       {{Item_Stone, 3}, {Item_Stick, 2}, {}, {}, {}}, CraftingTable_None},
            {{Item_Axe, 1},         {{Item_Stone, 3}, {Item_Stick, 3}, {}, {}, {}}, CraftingTable_None},
            {{Item_Pickaxe, 1},     {{Item_Stone, 3}, {Item_Stick, 3}, {}, {}, {}}, CraftingTable_None},
            {{Item_Spear, 1},       {{Item_Flint, 1}, {Item_Stick, 5}, {Item_Leaf, 3}, {}, {}}, CraftingTable_None},

            {{Item_LeatherHelmet, 1}, {{Item_Leather, 5}, {}, {}, {}, {}}, CraftingTable_Bench},
            {{Item_LeatherJacket, 1}, {{Item_Leather, 5}, {}, {}, {}, {}}, CraftingTable_Bench},
            {{Item_LeatherBoots, 1},  {{Item_Leather, 5}, {}, {}, {}, {}}, CraftingTable_Bench},
            {{Item_Bed, 1},           {{Item_Log, 10}, {Item_Feather, 10}, {}, {}, {}}, CraftingTable_Bench},
            {{Item_SleepingBag, 1},   {{Item_Leather, 6}, {}, {}, {}, {}}, CraftingTable_Bench},
            {{Item_HayBed, 1},        {{Item_Hay, 10}, {}, {}, {}, {}}, CraftingTable_None},
            {{Item_HayBed, 1},        {{Item_Leaf, 20}, {}, {}, {}, {}}, CraftingTable_None},

            {{Item_Medicine, 1},      {{Item_Leaf, 10}, {Item_Flower, 5}, {}, {}, {}}, CraftingTable_Campfire},
            {{Item_CookedRedMeat, 1}, {{Item_RawRedMeat, 1}, {}, {}, {}, {}}, CraftingTable_Campfire},
            {{Item_CookedWhiteMeat, 1}, {{Item_RawWhiteMeat, 1}, {}, {}, {}, {}}, CraftingTable_Campfire},
            {{Item_Omelet, 1},        {{Item_Egg, 1}, {}, {}, {}, {}}, CraftingTable_Campfire},

            {{Item_Raft, 1},        {{Item_Log, 40}, {Item_Stick, 40}, {Item_Hay, 40}, {Item_Leather, 20}, {}}, CraftingTable_None}

            //{{Item_Anvil, 1},       {{Item_, }, {}, {}, {}, {}}, CraftingTable_None},
            //{{Item_, 1},            {{Item_, }, {}, {}, {}, {}}, CraftingTable_None},

        };
                            

struct game_state{
    memory_stack permMem;
    memory_stack frameMem;
    b32 init;

    program_section activeSection;

    worker_thread_state workerThread;

    loaded_image images[NUM_TEXTURES];
    loaded_image *defaultFontTex;

    b32 debugOn; // NOTE: when debugOn is false most cheats still work if they are activated.
    struct{
        b32 stopTime;
        b32 timeX10;
        b32 runSpeedX5;
        b32 godMode;
    }debug;

    u64 guiActiveId;
    u64 guiHoveredId;
    b32 guiKeepActive;

    b32 worldIsValid;

    tile *tiles;
    
    //
    //
    //
    u8 ___stuffBelowThisMemberWillGetZeroedEveryNewGame___;
    //
    //
    //

    b32 pause;
    b32 won; // Whether the player won the game by escaping with raft. Used to display different final message.

    v2 sleepBedPos;
    b32 sleepWindowIsOpen;
    s32 sleepWindowTimeAfterSleep; // 0 for unset.
    tile_object_type sleepBedType;
    s32 sleepGradientEffectMaxTime;
    s32 sleepGradientEffectTimer;

    s32 passedOutTimer; // counts up
    s32 passedOutTimerMaxTime;
    b32 passedOutAndDrowned;

    b32 noteIsOpen;
    s32 openNote;

    s32 tutorialMenuSelectedPage;


    // Inventory
    b32 inventoryOpen;
    s32 selectedItemSlot;
    union{
        inventory_item inventoryAll[40];
        struct{
            inventory_item inventoryTop[10];
            inventory_item inventory[30];
        };
    };
    inventory_item inventoryArmor[3]; // 0: head, 1: torso, 2: feet

    inventory_item holdingItem;

    // Item action.
    item_id actioningItem;
    s32 itemActionMaxTime; // after this the action "stops" (but you still can't start a new action)
    s32 itemActionResetTime; // after this the timer is reset to 0 and you can start a new action.
    s32 itemActionTimer; // 0 for no action.
    f32 itemActionDirection;
    s32 itemActionHitEntities[20];
    s32 itemActionHitEntitiesCount;


    u16 seaLevel;

    s32 time; // World time for gameplay stuff. In frames.
    s32 animationTime; // World time for animation. When in debug stop-time mode this advances anyway. In frames.
    s32 realTime; // Times that the main game function has run. In frames.
    s32 timeOfDeath;


    s32 playerHealthMax;
    s32 playerHealth;
    f32 playerTemperature;
    f32 playerStomach;
    f32 playerEnergy;
    v2 playerPos;
    v2 playerSpeed;
    v2 camPos;
    b32 playerIsRunning;
    b32 playerIsWalking;
    b32 playerFacingLeft;
    
    // When knockbackTimer == 0 player has full movement control.
    // When knockbackTimer >= knockbackTimerMax movement control is 0.
    // When 0 < knockbackTimer < knockbackTimerMax movement control is interpolated.
    s32 knockbackTimerMax; // Enemy attacks can change this.
    s32 knockbackTimer; // Goes down. 0 for no knockback.


    entity entities[MAX_ENTITIES];
    s32 numEntities;
    s32 lastUsedEntityId;

    tile *tilesWithBehaviour[MAX_TILES_WITH_BEHAVIOUR];
    s32 numTilesWithBehaviour;

    // light map is only computed for the visible section of the map.
    v4 lightMap[MAX_LIGHT_MAP_SIZE*MAX_LIGHT_MAP_SIZE];
    v2s lightMapSize; // num of tiles x and y occupied by map
    v2s lightMapMinTile; // position of the map in the tile grid.
};

#endif
