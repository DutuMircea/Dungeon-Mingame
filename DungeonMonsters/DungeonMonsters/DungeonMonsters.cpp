#include <iostream>
#include <string>
#include <array>
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()
#include "Random.h"


class Potion {
public:
    enum class PotionType { // all types of potions
        health,
        strength,
        poison,
        max_types
    };

    enum class PotionSize { // and their size
        small,
        medium,
        large,
        max_size
    };

    PotionType m_type{};
    PotionSize m_size{};

public:
    Potion(PotionType type, PotionSize size) : m_type{ type }, m_size{ size } {}

    static Potion getRandomPotion() {

        int randType{ Random::get(0, static_cast<int>(PotionType::max_types) - 1) };
        int randSize{ Random::get(0, static_cast<int>(PotionSize::max_size) - 1) };

        return Potion{ static_cast<PotionType>(randType), static_cast<PotionSize>(randSize) };
    }
    // set of getters for the potion class
    const std::string getType() const {
        switch (m_type) {
        case PotionType::health: return "Health";
            break;
        case PotionType::strength: return "Strength";
            break;
        case PotionType::poison: return "Poison";
            break;
        }
    }

    const std::string getSize() const {
        switch (m_size) {
        case PotionSize::small: return "Small";
            break;
        case PotionSize::medium: return "Medium";
            break;
        case PotionSize::large: return "Large";
            break;
        }
    }
};
class Creature {   // the class creature contains all the attributes
protected:
    std::string m_name{};
    char m_symbol{};
    int m_health{};
    int m_damage{};
    int m_gold{};

public:
    //Creature() = default;
    Creature(std::string name, char symbol, int health, int damage, int gold) :
        m_name{ name }, m_symbol{ symbol }, m_health{ health }, m_damage{ damage }, m_gold{ gold }
    {}

    //getters
    const std::string& getName() const { return m_name; }
    char getSymbol() { return m_symbol; }
    int getHealth() const { return m_health; }
    int getDamage() const { return m_damage; }
    int getGold() const { return m_gold; }

    void reduceHealth(int value) { m_health -= value; }
    bool isDead() { return (m_health <= 0); }
    void addGold(int value) { m_gold += value; }

};

class Player : public Creature { // the class player inherits from the creature class all the attribts
private:
    int m_level{ 1 };

public:
    Player(std::string name) : Creature{ name, '@', 10, 1, 0 } {}

    int getLevel() const { return m_level; }

    void levelUp() {
        m_level++;
        m_damage++;

        std::cout << "You are now level " << m_level << " (Damage = " << m_damage << ")\n";
    }

    bool hasWon() { return getLevel() >= 20; }

    void drinkPotion(Potion potion) {

        switch (potion.m_type) {
        case Potion::PotionType::health:
            switch (potion.m_size) {
            case Potion::PotionSize::small: m_health++;
                break;
            case Potion::PotionSize::medium: m_health += 3;
                break;
            case Potion::PotionSize::large: m_health += 5;
                break;
            }
            break;

        case Potion::PotionType::strength:
            switch (potion.m_size) {
            case Potion::PotionSize::small: m_damage++;
                break;
            case Potion::PotionSize::medium: m_damage += 3;
                break;
            case Potion::PotionSize::large: m_damage += 5;
                break;
            }
            break;

        case Potion::PotionType::poison:
            switch (potion.m_size) {
            case Potion::PotionSize::small: m_health--;
                break;
            case Potion::PotionSize::medium: m_health -= 3;
                break;
            case Potion::PotionSize::large: m_health -= 5;
                break;
            }
            break;
        }
    }

    void showStats() {
        std::cout << "---- " << m_name << "'s STATS ----" << "\n";
        std::cout << "\nHEALTH: " << m_health << "\n";
        std::cout << "DAMAGE: " << m_damage << "\n";
        std::cout << "GOLD: " << m_gold << "\n\n";
    }
};

class Monster : public Creature {
public:
    enum class Type {
        dragon, // 0
        orc,
        slime,
        max_types // 3
    };
    // could add more monster types
    Monster(Type type) : Creature{ getDefaultCreature(type) } {}
    // look up table - map for the creature, it reaturns a creature based on the given type
    static const Creature& getDefaultCreature(Type type) {
        static const std::array <Creature, static_cast<std::size_t>(Type::max_types)> monsterData{

            Creature{"dragon", 'D', 20, 4, 100},
            Creature{"orc", 'o', 4, 2, 25},
            Creature{"slime", 's', 1, 1, 10}
        };
    
        return monsterData[static_cast<std::size_t>(type)]; // cast in tip int
    }


    static Monster getRandomMonster() {

        // generare nr random intre 0 si 3 (max_type - 1) 
        int random{ Random::get(0, static_cast<int>(Type::max_types) - 1) };

        return Monster{ static_cast<Type>(random) };
    }
};

static bool escapeEncounter() {  // 50% chance to escape the monster

    int rand{ Random::get(1,100) };
    return (rand <= 50);
}

static bool chancePotion() { // 30% chance to encounter a potion

    int rand{ Random::get(1,100) };
    return (rand <= 30);
}

void encounterPotion(Player& player) { /

    if (chancePotion()) {
        Potion potion{ Potion::getRandomPotion() };
        char choice{};

        std::cout << "You found a mythical potion! Do you want to drink it? [y/n]: ";
        std::cin >> choice;

        if (choice == 'y') {
            std::cout << "You drank a " << potion.getSize() << " potion of " << potion.getType() << ".\n";
            player.drinkPotion(potion);
        }
        else
            return;
    }
}
void fightMonster(Player& player) {

    char choice{};
    Monster monster{ Monster::getRandomMonster() };
    std::cout << "You have encountered a " << monster.getName() << ".\n";

    while (true) {
        std::cout << "(R)un or (F)ight: ";
        std::cin >> choice;

        switch (choice) {
        case 'f':  // the player choses to fight
            std::cout << "You hit the " << monster.getName() << " for " << player.getDamage() << " damage.\n";
            monster.reduceHealth(player.getDamage());

            if (monster.isDead()) {
                player.addGold(monster.getGold());
                player.levelUp();
                std::cout << "You found " << monster.getGold() << " gold.\n";
                encounterPotion(player);
                player.showStats();
                return;
            }
            else {
                std::cout << "The " << monster.getName() << " hit you for " << monster.getDamage() << " damage.\n";
                player.reduceHealth(monster.getDamage());
                player.showStats();
                break;
            }

        case 'r': // the player chooses to run
            if (escapeEncounter()) {
                std::cout << "You successfully fled.\n";
                return;
            }
            else {
                std::cout << "The " << monster.getName() << " hit you for " << monster.getDamage() << " damage.\n";
                player.reduceHealth(monster.getDamage());
                player.showStats();
                break;
            }

        default: std::cout << "Invalid command! \n";
            break;
        }

        if (player.isDead()) {
            std::cout << "You died at level " << player.getLevel() << " and with " << player.getGold() << " gold.\n";
            return;
        }
    }
}

void playGame() {

    std::cout << "-----WELCOME TO DUNGEON MONSTERS!-----\n";

    std::string name{};
    std::cout << "Enter your name: ";
    std::cin >> name;

    Player player{ name };
    std::cout << "Welcome, " << player.getName() << ".\n";
    std::cout << "You have " << player.getHealth() << " health and are carrying " << player.getGold() << " gold.\n";

    while (!player.isDead()) {
        fightMonster(player);

        if (player.hasWon()) {
            std::cout << "\n\nCongratulations you reached level 20!\nYou win!\n\n";
            return;
        }
    }
}

int main()
{
    playGame();
}
