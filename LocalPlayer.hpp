#pragma once
struct LocalPlayer {
    MyDisplay *display;
    uint64_t base;
    bool dead;
    bool knocked;
    bool inAttack;
    bool inZoom;
    bool inJump;
    bool grippingGrenade;
    Vector3D localOrigin;
    Vector3D CameraPosition;
    Vector2D viewAngles;
    Vector2D punchAngles;
    Vector2D punchAnglesPrev;
    Vector2D punchAnglesDiff;
    long actWeaponID;
    long currentWeapon;
    long actWeaponIDMasked;
    long highlightSettingsPtr;
    long weaponEntity;
    long weaponHandle;
    long weaponHandleMasked;
    int weaponIndex;
    int frameCount; 
    int grenadeID;
    int ammoInClip;
    int teamNumber;
    int squadNumber;
    int currentHealth;
    float WeaponProjectileSpeed;
    float WeaponProjectileScale;
    float worldtime;
    float traversalStartTime;
    float traversalProgress;
    float traversalReleaseTime;
    float onWall;

    void reset() {
        base = 0;
    }

    void readFromMemory(Level* map) {
        base = mem::Read<long>(OFF_REGION + OFF_LOCAL_PLAYER, "LocalPlayer base");
        if (base == 0) return;
        dead = mem::Read<short>(base + OFF_LIFE_STATE, "LocalPlayer base") > 0;
        knocked = mem::Read<short>(base + OFF_BLEEDOUT_STATE, "LocalPlayer base") > 0;
        inZoom = mem::Read<short>(base + OFF_ZOOMING, "LocalPlayer inZoom") > 0;
        teamNumber = mem::Read<int>(base + OFF_TEAM_NUMBER, "LocalPlayer teamNumber");

        squadNumber = mem::Read<int>(base + OFF_SQUAD_ID, "LocalPlayer squadNumber");
        inAttack = mem::Read<bool>(OFF_REGION + OFF_IN_ATTACK, "LocalPlayer inAttack") > 0;
        inJump = mem::Read<bool>(OFF_REGION + OFF_IN_JUMP, "LocalPlayer inJump") > 0;
        highlightSettingsPtr = mem::Read<long>( OFF_REGION + OFF_GLOW_HIGHLIGHTS, "LocalPlayer HiglightsSettingPtr");
        localOrigin = mem::Read<Vector3D>(base + OFF_LOCAL_ORIGIN, "LocalPlayer localOrigin");
        
        frameCount = mem::Read<int>(OFF_REGION + OFF_GLOBAL_VARS + sizeof(double), "LocalPlayer frameCount");
        worldtime = mem::Read<float>(base + OFFSET_TIME_BASE, "LocalPlayer worldTime");
        traversalStartTime = mem::Read<float>(base + OFFSET_TRAVERSAL_START_TIME, "LocalPlayer traversalStartTime");
        traversalProgress = mem::Read<float>(base + OFFSET_TRAVERSAL_PROGRESS, "LocalPlayer traversalProgress"); 
        traversalReleaseTime = mem::Read<float>(base + OFFSET_TRAVERSAL_RELEASE_TIME, "LocalPlayer traversalReleaseTime"); 
        onWall = mem::Read<float>(base + OFFSET_WALL_RUN_START_TIME, "LocalPlayer wallRunStartTime");

        currentHealth = mem::Read<int>(base + OFF_CURRENT_HEALTH, "LocalPlayer currentHealth");
        CameraPosition = mem::Read<Vector3D>(base + OFF_CAMERAORIGIN, "LocalPlayer CameraPosition");
        viewAngles = mem::Read<Vector2D>(base + OFF_VIEW_ANGLES, "LocalPlayer viewAngles");
        punchAngles = mem::Read<Vector2D>(base + OFF_PUNCH_ANGLES, "LocalPlayer punchAngles");
        punchAnglesDiff = punchAnglesPrev.Subtract(punchAngles);
        punchAnglesPrev = punchAngles;
        if (!dead && !knocked && map->playable) {
            weaponHandle = mem::Read<long>(base + OFF_WEAPON_HANDLE, "LocalPlayer weaponHandle");
            weaponHandleMasked = weaponHandle & 0xffff;
           
            weaponEntity = mem::Read<long>(OFF_REGION + OFF_ENTITY_LIST + (weaponHandleMasked << 5), "LocalPlayer weaponEntity");
            grenadeID = mem::Read<int>(base + OFF_GRENADE_HANDLE, "LocalPlayer grenadeID");
            grippingGrenade = grenadeID == -251 ? true : false;
            ammoInClip = mem::Read<int>(weaponEntity + OFFSET_AMMO, "LocalPlayer ammoInClip"); 
            weaponIndex = mem::Read<int>(weaponEntity + OFF_WEAPON_INDEX, "LocalPlayer weaponIndex");
            actWeaponID = mem::Read<long>((base + OFF_VIEW_MODELS), "LocalPlayer active Weapon ID");
            actWeaponIDMasked = actWeaponID & 0xFFFF;
            currentWeapon = mem::Read<long>(OFF_REGION + OFF_ENTITY_LIST + (actWeaponIDMasked << 5), "LocalPlayer current Weapon");
            WeaponProjectileSpeed = mem::Read<float>(weaponEntity + OFF_PROJECTILESPEED, "LocalPlayer WeaponProjectileSpeed");
            WeaponProjectileScale = mem::Read<float>(weaponEntity + OFF_PROJECTILESCALE, "LocalPlayer WeaponProjectileScale");   
        }
    }
    bool isValid(){
        return base != 0;
    }
    bool isCombatReady(){
        if (base == 0) return false;
        if (dead) return false;
        if (knocked) return false;
        return true;
    }
    void setYaw(float angle){
        long ptrLong = base + OFF_VIEW_ANGLES + sizeof(float);
        mem::Write<float>(ptrLong, angle);
    }
    float getPunchPitch(){
        long ptrLong = base + OFF_PUNCH_ANGLES;
        float result = mem::Read<float>(ptrLong,"PunchPitch");
        return result;
    }
    float getPunchYaw(){
        long ptrLong = base + OFF_PUNCH_ANGLES + sizeof(float);
        float result = mem::Read<float>(ptrLong, "PunchYaw");
        return result;
    }
    float getPitch(){
        long ptrLong = base + OFF_VIEW_ANGLES;
        float result = mem::Read<float>(ptrLong, "getPitch");
        return result;
    }
    void setPitch(float angle){
        long ptrLong = base + OFF_VIEW_ANGLES;
        mem::Write<float>(ptrLong, angle);
    }
    float getYaw() {
        long ptrLong = base + OFF_VIEW_ANGLES + sizeof(float);
        float result = mem::Read<float>(ptrLong,"getYaw");
        return result;
    }
};
