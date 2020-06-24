#include <SFML/Graphics.hpp>
#include <random>
#include <time.h>


class Cell{
public:
    int selected;
    const sf::Vector2f pos;
    const size_t count;
public:
    Cell(sf::Vector2f m_pos, size_t m_count):
    pos(m_pos),
    count(m_count)
    {
        selected = -1;
    }
};

class Block{
public:
    size_t count;
    sf::RectangleShape body;
    sf::Vector2f pos;
    sf::Vector2f prevPos;
    static size_t size;
    Block(sf::Vector2f pos, size_t count);
    void move(Block nextBlock);
};

class Snake{
public:
    std::vector<Block> body;
    size_t length;
    sf::Vector2f dir;
    sf::Vector2f food;
public:
    Snake(sf::Vector2f pos);
    void move();
    sf::Vector2f findDir(sf::Vector2f path);
    bool checkCollision();
    bool checkFood();
    void draw(sf::RenderWindow* window);


};

class Game{
public:
    sf::RenderWindow* window;
    bool running;
    Snake snake;
    std::vector<sf::Vector2f> path;

public:
    Game();
    size_t calcPath();
    bool updatePath();
    inline size_t testSide(int x, int y, int a);
    inline size_t testReturnSide(int x, int y, size_t& currSel);
    void run();
    

};
