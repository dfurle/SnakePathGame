#include "snake.h"
#include <stdio.h>

int main(){
    srand(time(NULL));
    Game game;
    return 0;
}

const bool PLAY_MANUAL = false;
size_t FRAMES = 60;
const size_t COLS = 30;
const size_t ROWS = 20;
size_t Block::size = 30;

const size_t WIDTH = Block::size * COLS;
const size_t HEIGHT = Block::size * ROWS;
const sf::Vector2f UP(0,-1);
const sf::Vector2f DOWN(0,1);
const sf::Vector2f RIGHT(1,0);
const sf::Vector2f LEFT(-1,0);
std::vector<std::vector<Cell>> cells;


Game::Game():
snake(sf::Vector2f(int(WIDTH/2/Block::size)*Block::size, int(HEIGHT/2/Block::size)*Block::size))
{
    window = new sf::RenderWindow(sf::VideoMode(WIDTH,HEIGHT),"Game", sf::Style::Close | sf::Style::Titlebar);
    if(PLAY_MANUAL)
        FRAMES = 10;
    window->setFramerateLimit(FRAMES);
    running = true;
    size_t counter = 0;
    for(int x = 0; x < WIDTH; x += Block::size){
        std::vector<Cell> tmp;
        for(int y = 0; y < HEIGHT; y += Block::size){
            tmp.push_back(Cell(sf::Vector2f(x,y),++counter));
        }
        cells.push_back(tmp);
    }

    run();
}

inline size_t Game::testSide(int x, int y, int a){
    bool test = true;
    test = test && x < cells.size();
    test = test && x >= 0;
    test = test && y < cells[0].size();
    test = test && y >= 0;
    if(test && cells[x][y].selected == -1){
        bool flag = false;
        for (int q = 0; q < snake.body.size() && !flag; q++) {
            if (snake.body[q].pos == cells[x][y].pos) {
                if (a >= snake.length - snake.body[q].count)
                    continue;
                flag = true;
            }
        }
        if(!flag){
            cells[x][y].selected = a + 1;
            if(cells[x][y].pos == snake.food)
                return cells[x][y].selected;
        }
    }
    return -1;
}
inline size_t Game::testReturnSide(int x, int y, size_t& currSel){
    bool test = true;
    test = test && x < cells.size();
    test = test && x >= 0;
    test = test && y < cells[0].size();
    test = test && y >= 0;
    if(test && cells[x][y].selected == currSel - 1) {
        path.push_back(cells[x][y].pos);
        currSel--;
        return true;
    }
    return false;
}


size_t Game::calcPath(){
    for (int a = 0; a < 100; a++) {
        for (int x = 0; x < cells.size(); x++) {
            for (int y = 0; y < cells[0].size(); y++){
                if (cells[x][y].selected == a){
                    size_t retVal = -1;
                    retVal = testSide(x-1,y,a);
                    if(retVal != -1)
                        return retVal;
                    retVal = testSide(x+1,y,a);
                    if(retVal != -1)
                        return retVal;
                    retVal = testSide(x,y-1,a);
                    if(retVal != -1)
                        return retVal;
                    retVal = testSide(x,y+1,a);
                    if(retVal != -1)
                        return retVal;
                }
            }
        }
    }
    return -1;
}

bool Game::updatePath(){
    for (int x = 0; x < cells.size(); x++){
        for (int y = 0; y < cells[0].size(); y++){
            cells[x][y].selected = -1;
            if(cells[x][y].pos == snake.body[0].pos){
                cells[x][y].selected = 0;
            }
        }
    }
    size_t currSel = calcPath();
    if(currSel == -1)
        return false;
    path.clear();
    path.push_back(snake.food);
    sf::Vector2f pos = snake.food;
    while (currSel > 1) {
        pos = sf::Vector2f(path[path.size()-1].x/Block::size,path[path.size()-1].y/Block::size);
        if (testReturnSide(int(pos.x)-1,int(pos.y),currSel))
            continue;
        if (testReturnSide(int(pos.x)+1,int(pos.y),currSel))
            continue;
        if (testReturnSide(int(pos.x),int(pos.y)-1,currSel))
            continue;
        if (testReturnSide(int(pos.x),int(pos.y)+1,currSel))
            continue;
    }
    // for (int i = path.size() - 1; i >= 0; i--)
    //     printf("path: %d:%d\n", int(path[i].x/Block::size), int(path[i].y/Block::size));
    std::reverse(path.begin(),path.end());
    return true;
}


void Game::run(){
    bool pathSuccess = false;
    bool stop = false;
    while (window->isOpen() && running) {
        sf::Event event;
        while(window->pollEvent(event)){
            switch(event.type){
            case sf::Event::Closed:
                window->close();
                running = false;
                break;
            case sf::Event::KeyPressed:
                switch(event.key.code){
                case sf::Keyboard::Key::W:
                    snake.dir = UP;
                    break;
                case sf::Keyboard::Key::S:
                    snake.dir = DOWN;
                    break;
                case sf::Keyboard::Key::A:
                    snake.dir = LEFT;
                    break;
                case sf::Keyboard::Key::D:
                    snake.dir = RIGHT;
                    break;
                case sf::Keyboard::Key::Space:
                    if(stop)
                        running = false;
                    stop = false;
                    break;
                }
                break;
            }
        }
        if(!stop){
            if(!PLAY_MANUAL && path.size() > 0){
                snake.dir = snake.findDir(path[0]);
                path.erase(path.begin());
            }
            snake.move();
            if(!pathSuccess)
                pathSuccess = updatePath();

            if(snake.checkCollision())
                stop = true;
            if(snake.checkFood())
                pathSuccess = updatePath();

            window->clear(sf::Color::White);
            for(int i = 0; i < path.size(); i++){
                sf::RectangleShape p;
                p.setSize(sf::Vector2f(Block::size/2., Block::size/2.));
                p.setFillColor(sf::Color::Blue);
                p.setPosition(sf::Vector2f(path[i].x+Block::size/4.,path[i].y+Block::size/4.));
                window->draw(p);
            }
            // for(int i = 0; i < cells.size(); i++){
            //     sf::RectangleShape p;
            //     p.setSize(sf::Vector2f(Block::size, Block::size));
            //     size_t alpha = (cells[i].count/(30.*20.)*255);
            //     printf("alph: %d\n",alpha);
            //     p.setFillColor(sf::Color(255,0,0,alpha));
            //     p.setPosition(cells[i].pos);
            //     window->draw(p);
            // }

            // for(int y = 0; y < cells[0].size(); y++){
            //     printf("\n");
            //     for(int x = 0; x < cells.size(); x++){
            //         printf("%3d",cells[x][y].selected);
            //     }
            //     printf("\n");
            // }
            snake.draw(window);
            for(int x = Block::size; x < WIDTH; x+=Block::size){
                sf::Vertex line[2];
                line[0].position = sf::Vector2f(x, 0);
                line[0].color = sf::Color::Black;
                line[1].position = sf::Vector2f(x, HEIGHT);
                line[1].color = sf::Color::Black;
                window->draw(line, 2, sf::Lines);
            }
            for(int y = Block::size; y < HEIGHT; y+=Block::size){
                sf::Vertex line[2];
                line[0].position = sf::Vector2f(0, y);
                line[0].color = sf::Color::Black;
                line[1].position = sf::Vector2f(WIDTH, y);
                line[1].color = sf::Color::Black;
                window->draw(line, 2, sf::Lines);
            }
            window->display();
        }
    }
}

Snake::Snake(sf::Vector2f pos){
    while(true){
        sf::Vector2f pos;
        pos.x = (rand() % COLS) * Block::size;
        pos.y = (rand() % ROWS) * Block::size;
        bool flag = false;
        for(int i = 0; i < body.size() && !flag; i++){
            if(pos == body[i].pos)
                flag = true;
        }
        if(!flag){
            food = pos;
            break;  
        }
    }
    length = 0;
    dir = UP;
    body.push_back(Block(pos,++length));
}

Block::Block(sf::Vector2f pos, size_t count){
    body.setSize(sf::Vector2f(Block::size,Block::size));
    body.setPosition(pos);
    body.setFillColor(sf::Color::Red);
    this->pos = pos;
    prevPos = pos;
    this->count = count;
}

void Snake::move(){
    int velocity = Block::size;
    body[0].prevPos = body[0].pos;
    body[0].pos.x += dir.x * velocity;
    body[0].pos.y += dir.y * velocity;
    body[0].body.setPosition(body[0].pos);
    for(int i = 1; i < body.size(); i++){
        body[i].move(body[i-1]);
    }

}

sf::Vector2f Snake::findDir(sf::Vector2f path){
    sf::Vector2f pos = body[0].pos;

    sf::Vector2f ddir = UP;
    sf::Vector2f checkDir = sf::Vector2f(pos.x+ddir.x * Block::size, pos.y+ddir.y * Block::size);
    if(checkDir == path)
        return ddir;
    ddir = DOWN;
    checkDir = sf::Vector2f(pos.x+ddir.x * Block::size, pos.y+ddir.y * Block::size);
    if(checkDir == path)
        return ddir;
    ddir = RIGHT;
    checkDir = sf::Vector2f(pos.x+ddir.x * Block::size, pos.y+ddir.y * Block::size);
    if(checkDir == path)
        return ddir;
    ddir = LEFT;
    checkDir = sf::Vector2f(pos.x+ddir.x * Block::size, pos.y+ddir.y * Block::size);
    if(checkDir == path)
        return ddir;
    return sf::Vector2f(0,-2);
}

bool Snake::checkCollision(){
    for(int i = 1; i < body.size(); i++){
        if(body[0].pos == body[i].pos)
        return true;
    }
    sf::Vector2f pos = body[0].pos;
    if(pos.x >= WIDTH || pos.x < 0 || pos.y < 0 || pos.y >= HEIGHT)
        return true;
    return false;
}
bool Snake::checkFood(){
    if(body[0].pos == food){
        body.push_back(Block(body[body.size()-1].prevPos,++length));
        while(true){
            sf::Vector2f pos;
            pos.x = (rand() % COLS) * Block::size;
            pos.y = (rand() % ROWS) * Block::size;
            bool flag = false;
            for(int i = 0; i < body.size() && !flag; i++){
                if(pos == body[i].pos)
                    flag = true;
            }
            if(!flag){
                food = pos;
                break;
            }
                
        }
        return true;
    }
    return false;
}

void Block::move(Block nextBlock){
    prevPos = pos;
    pos = nextBlock.prevPos;
    // body.setPosition(pos);
}

void Snake::draw(sf::RenderWindow* window){
    sf::RectangleShape fd;
    fd.setSize(sf::Vector2f(Block::size,Block::size));
    fd.setFillColor(sf::Color::Green);
    fd.setPosition(food);
    window->draw(fd);

    window->draw(body[0].body);
    for(int i = 1; i < body.size(); i++){
        body[i].body.setPosition(body[i].pos);
        window->draw(body[i].body);
    }
    body[0].body.setPosition(body[0].pos);
    body[0].body.setFillColor(sf::Color(128,0,128));
}