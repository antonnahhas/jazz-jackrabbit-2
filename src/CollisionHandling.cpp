//----------------------------------include section-----------------------------------
#include "CollisionHandling.h"
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <typeindex>

#include "Direction.h"
#include "Macros.h"
#include "Jazz.h"
#include "AmmoCon.h"
#include "BombCon.h"
#include "Ruby.h"
#include "CheckPoint.h"
#include "LifeCon.h"
#include "Floor.h"
#include "Wall.h"
#include "Rope.h"
#include "Weapon.h"
#include "Enemy.h"
#include "MeleeEnemy.h"
#include "RangedEnemy.h"
#include "SentryEnemy.h"
#include "BombWeapon.h"
#include "BulletWeapon.h"
#include "CoffeeWeapon.h"
#include "SmokeWeapon.h"
#include "FinishLine.h"

//----------------------------------const section-----------------------------------
const int JAZ_OFFSET = 10;

//----------------------------------namespace section-----------------------------------
namespace // anonymous namespace — the standard way to make function "static"
{
    // primary collision-processing functions
    void jazzAmmoCon(GameObject& jazz, GameObject& ammoCon)
    {
        Jazz& jaz = dynamic_cast<Jazz&>(jazz);
        AmmoCon& ac = dynamic_cast<AmmoCon&>(ammoCon);

        Resources::instance().playSound(Resources::s_collectWeapon);
        jaz.updateInfo(SCORE, INC);
        jaz.updateInfo(AMMO, INC);
        ac.setDisposed();
    }
    void ammoConJazz(GameObject& ammoCon, GameObject& jazz) 
    {
        jazzAmmoCon(jazz, ammoCon);
    }

    void jazzBombCon(GameObject& jazz, GameObject& bombCon)
    {
        Jazz& jaz = dynamic_cast<Jazz&>(jazz);
        BombCon& bc = dynamic_cast<BombCon&>(bombCon);

        Resources::instance().playSound(Resources::s_collectWeapon);
        jaz.updateInfo(SCORE, INC);
        jaz.updateInfo(BOMBS, INC);
        bc.setDisposed();
    }
    void bombConJazz(GameObject& bombCon, GameObject& jazz)
    {
        jazzBombCon(jazz, bombCon);
    }

    void jazzRuby(GameObject& jazz, GameObject& ruby)
    {
        Jazz& jaz = dynamic_cast<Jazz&>(jazz);
        Ruby& rub = dynamic_cast<Ruby&>(ruby);

        Resources::instance().playSound(Resources::s_collectRuby);
        jaz.updateInfo(SCORE, INC_RUBY);
        rub.setDisposed();
    }
    void rubyJazz(GameObject& ruby, GameObject& jazz)
    {
        jazzRuby(jazz, ruby);
    }

    void jazzLifeCon(GameObject& jazz, GameObject& lifeCon)
    {
        Jazz& jaz = dynamic_cast<Jazz&>(jazz);
        LifeCon& lc = dynamic_cast<LifeCon&>(lifeCon);

        Resources::instance().playSound(Resources::s_extraLife);
        jaz.updateInfo(LIVES, INC);
        lc.setDisposed();
    }
    void lifeConJazz(GameObject& lifeCon, GameObject& jazz)
    {
        jazzLifeCon(jazz, lifeCon);
    }

    void jazzWall(GameObject& jazz, GameObject& wall)
    {
        Jazz& jaz = dynamic_cast<Jazz&>(jazz);
        jaz.hitAwall();
        jaz.setIdleAction();
    }
    void wallJazz(GameObject& wall, GameObject& jazz)
    {
        jazzWall(jazz, wall);
    }

    void jazzFloor(GameObject& jazz, GameObject& floor)
    {
        Jazz& jaz = dynamic_cast<Jazz&>(jazz);

        if (jaz.getPosition().y < (floor.getPosition().y - (floor.getGlobalBounds().height / 2) + 1))
            jaz.setOnGround(true);
    }
    void floorJazz(GameObject& floor, GameObject& jazz)
    {
        jazzFloor(jazz, floor);
    }

    void jazzRope(GameObject& jazz, GameObject& rope)
    {
        Jazz& jaz = dynamic_cast<Jazz&>(jazz);
        Rope& rop = dynamic_cast<Rope&>(rope);

        if (rop.getGlobalBounds().contains(jaz.getPosition()))
        {
            jaz.setOnRope(true);

            if (rop.getGlobalBounds().top - JAZ_OFFSET < jaz.getGlobalBounds().top)
                jaz.centerOnRope(rop.getPosition());
            else
                jaz.reachedEndOfRope();
        }
    }
    void ropeJazz(GameObject& jazz, GameObject& rope)
    {
        jazzRope(rope, jazz);
    }

    void jazzCoffeeWeapon(GameObject& jazz, GameObject& coffeeWeapon)
    {
        Jazz& jaz = dynamic_cast<Jazz&>(jazz);
        CoffeeWeapon& cw = dynamic_cast<CoffeeWeapon&>(coffeeWeapon);
        if (jaz.getAction() != Jazz::PlayerAction::Shoot)
        {
            jaz.updateInfo(LIVES, DEC);
            Resources::instance().playSound(Resources::s_resetToCheckpoint);
            jaz.setJazzToDefaultPosition();
        }
        cw.setDisposed();
    }
    void coffeeWeaponJazz(GameObject& smokeWeapon, GameObject& jazz)
    {
        jazzCoffeeWeapon(jazz, smokeWeapon);
    }

    void jazzSmokeWeapon(GameObject& jazz, GameObject& smokeWeapon)
    {
        Jazz& jaz = dynamic_cast<Jazz&>(jazz);
        SmokeWeapon& sw = dynamic_cast<SmokeWeapon&>(smokeWeapon);
        if (jaz.getAction() != Jazz::PlayerAction::Shoot)
        {
            jaz.updateInfo(LIVES, DEC);
            Resources::instance().playSound(Resources::s_resetToCheckpoint);
            jaz.setJazzToDefaultPosition();
        }
        sw.setDisposed();
    }
    void smokeWeaponJazz(GameObject& smokeWeapon, GameObject& jazz)
    {
        jazzSmokeWeapon(jazz, smokeWeapon);
    }

    void jazzEnemy(GameObject& jazz, GameObject& enemy)
    {
        Jazz& jaz = dynamic_cast<Jazz&>(jazz);
        Enemy& en = dynamic_cast<Enemy&>(enemy);

        Resources::instance().playSound(Resources::s_resetToCheckpoint);
        jaz.setJazzToDefaultPosition();
        jaz.updateInfo(LIVES, DEC);
    }
    void enemyJazz(GameObject& enemy, GameObject& jazz)
    {
        jazzEnemy(jazz, enemy);
    }

    void jazzCheckPoint(GameObject& jazz, GameObject& checkPoint)
    {
        Jazz& jaz = dynamic_cast<Jazz&>(jazz);
        CheckPoint& ge = dynamic_cast<CheckPoint&>(checkPoint);

        Resources::instance().playSound(Resources::s_reachedCheckpoint);
        //ge.setDisposed();
        jaz.setDefaultPosition(jaz.getJazzPos());
    }
    void checkPointJazz(GameObject& checkPoint, GameObject& jazz)
    {
        jazzCheckPoint(jazz, checkPoint);
    }

    void jazzFinishLine(GameObject& jazz, GameObject& finishLine)
    {
        Jazz& jaz = dynamic_cast<Jazz&>(jazz);
        FinishLine& fl = dynamic_cast<FinishLine&>(finishLine);
        Resources::instance().playSound(Resources::s_levelComplete);
        //endl.setAnimation(true);
        //std::cout << "col jaz end level\n ";
        fl.setDisposed();
    }
    void FinishLineJazz(GameObject& jazz, GameObject& FinishLine)
    {
        jazzFinishLine(FinishLine, jazz);
    }

    void enemyBombWeapon(GameObject& enemy, GameObject& bombWeapon)
    {
        Enemy& en = dynamic_cast<Enemy&>(enemy);
        BombWeapon& bw = dynamic_cast<BombWeapon&>(bombWeapon);
        Resources::instance().playSound(Resources::s_enemyDis);
        en.setDisposed();
        bw.setDirection(Direction::Stay);
    }
    void bombWeaponEnemy(GameObject& bombWeapon, GameObject& enemy)
    {
        enemyBombWeapon(enemy, bombWeapon);
    }

    void enemyBulletWeapon(GameObject& enemy, GameObject& bulletWeapon)
    {
        Enemy& en = dynamic_cast<Enemy&>(enemy);
        BulletWeapon& bw = dynamic_cast<BulletWeapon&>(bulletWeapon);
        Resources::instance().playSound(Resources::s_enemyDis);
        en.setDisposed();
        bw.setDisposed();
    }
    void bulletWeaponEnemy(GameObject& bulletWeapon, GameObject& enemy)
    {
        enemyBulletWeapon(enemy, bulletWeapon);
    }

    void weaponWall(GameObject& weapon, GameObject& wall)
    {
        Weapon& we = dynamic_cast<Weapon&>(weapon);
        we.setDirection(Direction::Stay);
        we.setDisposed();
    }
    void wallWeapon(GameObject& wall, GameObject& weapon)
    {
        weaponWall(weapon, wall);
    }

    void weaponFloor(GameObject& weapon, GameObject& floor)
    {
        Weapon& we = dynamic_cast<Weapon&>(weapon);
        we.setDirection(Direction::Stay);

        auto& p = typeid(weapon);
        if (p == typeid(BombWeapon));
        else
            we.setDisposed();
        
    }
    void floorWeapon(GameObject& floor, GameObject& weapon)
    {
        weaponWall(weapon, floor);
    }

    void weaponWeapon(GameObject& weapon1, GameObject& weapon2)
    {
        Weapon& we1 = dynamic_cast<Weapon&>(weapon1);
        Weapon& we2 = dynamic_cast<Weapon&>(weapon2);
        we1.setDisposed();
        we2.setDisposed();
    }

    using HitFunctionPtr = void (*)(GameObject&, GameObject&);
    using Key = std::pair<std::type_index, std::type_index>;
    using HitMap = std::map<Key, HitFunctionPtr>;

    //all possible collisions
    HitMap initializeCollisionMap()
    {
        HitMap phm;
        phm[Key(typeid(Jazz), typeid(AmmoCon))] = &jazzAmmoCon;
        phm[Key(typeid(AmmoCon), typeid(Jazz))] = &ammoConJazz;

        phm[Key(typeid(Jazz), typeid(BombCon))] = &jazzBombCon;
        phm[Key(typeid(BombCon), typeid(Jazz))] = &bombConJazz;

        phm[Key(typeid(Jazz), typeid(Ruby))] = &jazzRuby;
        phm[Key(typeid(Ruby), typeid(Jazz))] = &rubyJazz;

        phm[Key(typeid(Jazz), typeid(LifeCon))] = &jazzLifeCon;
        phm[Key(typeid(LifeCon), typeid(Jazz))] = &lifeConJazz;

        phm[Key(typeid(Jazz), typeid(Wall))] = &jazzWall;
        phm[Key(typeid(Wall), typeid(Jazz))] = &wallJazz;

        phm[Key(typeid(Jazz), typeid(Floor))] = &jazzFloor;
        phm[Key(typeid(Floor), typeid(Jazz))] = &floorJazz;

        phm[Key(typeid(Jazz), typeid(Rope))] = &jazzRope;
        phm[Key(typeid(Rope), typeid(Jazz))] = &ropeJazz;

        phm[Key(typeid(Jazz), typeid(CoffeeWeapon))] = &jazzCoffeeWeapon;
        phm[Key(typeid(CoffeeWeapon), typeid(Jazz))] = &coffeeWeaponJazz;

        phm[Key(typeid(Jazz), typeid(SmokeWeapon))] = &jazzSmokeWeapon;
        phm[Key(typeid(SmokeWeapon), typeid(Jazz))] = &smokeWeaponJazz;

        phm[Key(typeid(Jazz), typeid(CheckPoint))] = &jazzCheckPoint;
        phm[Key(typeid(CheckPoint), typeid(Jazz))] = &checkPointJazz;

        phm[Key(typeid(Jazz), typeid(Enemy))] = &jazzEnemy;
        phm[Key(typeid(Enemy), typeid(Jazz))] = &enemyJazz;

        phm[Key(typeid(Jazz), typeid(FinishLine))] = &jazzFinishLine;
        phm[Key(typeid(FinishLine), typeid(Jazz))] = &FinishLineJazz;

        phm[Key(typeid(Enemy), typeid(BombWeapon))] = &enemyBombWeapon;
        phm[Key(typeid(BombWeapon), typeid(Enemy))] = &bombWeaponEnemy;
        
        phm[Key(typeid(Enemy), typeid(BulletWeapon))] = &enemyBulletWeapon;
        phm[Key(typeid(BulletWeapon), typeid(Enemy))] = &bulletWeaponEnemy;

        phm[Key(typeid(Jazz), typeid(MeleeEnemy))] = &jazzEnemy;
        phm[Key(typeid(MeleeEnemy), typeid(Jazz))] = &enemyJazz;
        phm[Key(typeid(MeleeEnemy), typeid(BombWeapon))] = &enemyBombWeapon;
        phm[Key(typeid(BombWeapon), typeid(MeleeEnemy))] = &bombWeaponEnemy;
        phm[Key(typeid(MeleeEnemy), typeid(BulletWeapon))] = &enemyBulletWeapon;
        phm[Key(typeid(BulletWeapon), typeid(MeleeEnemy))] = &bulletWeaponEnemy;

        phm[Key(typeid(Jazz), typeid(RangedEnemy))] = &jazzEnemy;
        phm[Key(typeid(RangedEnemy), typeid(Jazz))] = &enemyJazz;
        phm[Key(typeid(RangedEnemy), typeid(BombWeapon))] = &enemyBombWeapon;
        phm[Key(typeid(BombWeapon), typeid(RangedEnemy))] = &bombWeaponEnemy;
        phm[Key(typeid(RangedEnemy), typeid(BulletWeapon))] = &enemyBulletWeapon;
        phm[Key(typeid(BulletWeapon), typeid(RangedEnemy))] = &bulletWeaponEnemy;

        phm[Key(typeid(Jazz), typeid(SentryEnemy))] = &jazzEnemy;
        phm[Key(typeid(SentryEnemy), typeid(Jazz))] = &enemyJazz;
        phm[Key(typeid(SentryEnemy), typeid(BombWeapon))] = &enemyBombWeapon;
        phm[Key(typeid(BombWeapon), typeid(SentryEnemy))] = &bombWeaponEnemy;
        phm[Key(typeid(SentryEnemy), typeid(BulletWeapon))] = &enemyBulletWeapon;
        phm[Key(typeid(BulletWeapon), typeid(SentryEnemy))] = &bulletWeaponEnemy;

        phm[Key(typeid(Weapon), typeid(Wall))] = &weaponWall;
        phm[Key(typeid(Wall), typeid(Weapon))] = &wallWeapon;

        phm[Key(typeid(BombWeapon), typeid(Wall))] = &weaponWall;
        phm[Key(typeid(Wall), typeid(BombWeapon))] = &wallWeapon;
        phm[Key(typeid(BulletWeapon), typeid(Wall))] = &weaponWall;
        phm[Key(typeid(Wall), typeid(BulletWeapon))] = &wallWeapon;
        phm[Key(typeid(CoffeeWeapon), typeid(Wall))] = &weaponWall;
        phm[Key(typeid(Wall), typeid(CoffeeWeapon))] = &wallWeapon;
        phm[Key(typeid(SmokeWeapon), typeid(Wall))] = &weaponWall;
        phm[Key(typeid(Wall), typeid(SmokeWeapon))] = &wallWeapon;
        phm[Key(typeid(BombWeapon), typeid(Floor))] = &weaponFloor;
        phm[Key(typeid(Floor), typeid(BombWeapon))] = &floorWeapon;
        phm[Key(typeid(BulletWeapon), typeid(Floor))] = &weaponFloor;
        phm[Key(typeid(Floor), typeid(BulletWeapon))] = &floorWeapon;
        phm[Key(typeid(CoffeeWeapon), typeid(Floor))] = &weaponFloor;
        phm[Key(typeid(Floor), typeid(CoffeeWeapon))] = &floorWeapon;
        phm[Key(typeid(SmokeWeapon), typeid(Floor))] = &weaponFloor;
        phm[Key(typeid(Floor), typeid(SmokeWeapon))] = &floorWeapon;

        phm[Key(typeid(Weapon), typeid(Weapon))] = &weaponWeapon;
        phm[Key(typeid(BombWeapon), typeid(CoffeeWeapon))] = &weaponWeapon;
        phm[Key(typeid(CoffeeWeapon), typeid(BombWeapon))] = &weaponWeapon;
        phm[Key(typeid(BombWeapon), typeid(SmokeWeapon))] = &weaponWeapon;
        phm[Key(typeid(SmokeWeapon), typeid(BombWeapon))] = &weaponWeapon;
        phm[Key(typeid(BulletWeapon), typeid(CoffeeWeapon))] = &weaponWeapon;
        phm[Key(typeid(CoffeeWeapon), typeid(BulletWeapon))] = &weaponWeapon;
        phm[Key(typeid(BulletWeapon), typeid(SmokeWeapon))] = &weaponWeapon;
        phm[Key(typeid(SmokeWeapon), typeid(BulletWeapon))] = &weaponWeapon;

        return phm;
    }

    //find the given collision in collision map
    HitFunctionPtr lookup(const std::type_index& class1, const std::type_index& class2)
    {
        static HitMap collisionMap = initializeCollisionMap();
        auto mapEntry = collisionMap.find(std::make_pair(class1, class2));
        if (mapEntry == collisionMap.end())
        {
            return nullptr;
        }
        return mapEntry->second;
    }
}// end namespace

//----------------------------------function section-----------------------------------

//
void processCollision(GameObject& object1, GameObject& object2)
{
    auto phf = lookup(typeid(object1), typeid(object2));
    if (!phf)
        return;
    phf(object1, object2);
}