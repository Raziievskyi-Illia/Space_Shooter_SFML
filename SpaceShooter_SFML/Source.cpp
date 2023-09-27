#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<string>
#include<vector>
#include<stack>


bool checkCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2) {
    return sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds());
}




class Bullet {
public:
    sf::Sprite sprite;
    float speed = -10.0f;

    Bullet(sf::Texture& texture) {
        sprite.setTexture(texture);
        sprite.setScale(0.3f, 0.3f);
    }

    void update() {
        
        sprite.move(0, speed);
    }

    bool isOutOfBounds(const sf::RenderWindow& window) {
        sf::FloatRect bounds = sprite.getGlobalBounds();
        return bounds.top + bounds.height < 0 ;
    }

 
};


class Enemy {
public:
    sf::Sprite sprite;
     float speed;

    Enemy(sf::Texture& texture , const sf::RenderWindow& window , float&&newSpeed) {
        sprite.setTexture(texture);       
        sprite.setPosition(rand() % (window.getSize().x -20), -100); 
        speed = std::move(newSpeed);
    }

    void update() {
        sprite.move(0, speed);
    }
    
    
};






std::vector<sf::Sprite> setupHPbar(std::vector<sf::Sprite>& hp)
{     
    hp[0].setScale(0.15f, 0.15f);
    hp[1].setScale(0.15f, 0.15f);
    hp[2].setScale(0.15f, 0.15f);
    hp[0].setPosition(5.f, 45.f);
    hp[1].setPosition(5.f + hp[1].getGlobalBounds().width, 45.f);
    hp[2].setPosition(5.f + 2.f * hp[2].getGlobalBounds().width, 45.f);
    return hp;
}










int main() {
    srand(time(NULL)); 
    sf::RenderWindow window(sf::VideoMode(800, 600), "Space Shooter" , 1);
    window.setFramerateLimit(60);
    window.setMouseCursorGrabbed(true);
    window.setMouseCursorVisible(false);

    int Score = 0;

    std::vector<sf::Sprite> hp;

    sf::Clock clock;
    float timeSinceLastSpeedChange = 0.0f;
    const float speedChangeInterval = 15.0f;

    


  

    sf::Texture hp_texture;
    hp_texture.loadFromFile("heart.png");
    hp.emplace_back(sf::Sprite(hp_texture)); 
    hp.emplace_back(sf::Sprite(hp_texture));
    hp.emplace_back(sf::Sprite(hp_texture));
    setupHPbar(hp);


    sf::Music music; 
    music.openFromFile("music.mp3"); 
    music.play(); 




    sf::SoundBuffer bf_dead; 
    bf_dead.loadFromFile("dead.wav");

    sf::Sound sound_dead;
    sound_dead.setBuffer(bf_dead);
  
  
    sf::SoundBuffer buffer;
    buffer.loadFromFile("gunsound.wav");

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.setVolume(15.f);

   
    sf::SoundBuffer bf_hploss; 
    bf_hploss.loadFromFile("hp_loss.wav"); 

    sf::Sound sound_hploss; 
    sound_hploss.setBuffer(bf_hploss); 


   

    

    sf::Font font;
    font.loadFromFile("font.TTF"); 
   
    sf::Text text;
    sf::Text final_screen; 

    sf::Font fs_font;
    fs_font.loadFromFile("font.TTF");

    final_screen.setFont(fs_font); 
    final_screen.setFillColor(sf::Color::White); 
  //  final_screen.setPosition((window.getSize().x / 2.0f) - final_screen.getGlobalBounds().width, 300.0f); 

    text.setFont(font);
    text.setFillColor(sf::Color::White);
    
    text.setString("Score : " + std::to_string(Score));

    std::vector<Bullet> bullets; 
    std::vector<Enemy> enemies; 
    int HP= 3; 
    

    float enemy_speed = 0.7f;

    sf::Texture explosion_t;
    explosion_t.loadFromFile("explosion.png"); 
    sf::Sprite explosion_sprite; 
    explosion_sprite.setTexture(explosion_t); 


    sf::Sprite player_sprite;
    sf::Texture player_texture; 
    player_texture.loadFromFile("ship2.png");
    player_sprite.setTexture(player_texture); 
    player_sprite.setScale(0.05f, 0.05f);

    sf::Texture bullet_texture;
    bullet_texture.loadFromFile("bullet_blue.png");
    int maxBullets = 4; 
    

    std::vector<Bullet> bulletsToRemove;
    std::vector<Enemy> enemiesToRemove;

    sf::Texture enemy_texture; 
    enemy_texture.loadFromFile("enemy.png"); 
    int maxEnemy = 4; 



    bool splaying = false; 
    float n = window.getSize().y - player_sprite.getGlobalBounds().height;
    
    player_sprite.setPosition((window.getSize().x / 2) - player_sprite.getGlobalBounds().width /2, window.getSize().y - player_sprite.getGlobalBounds().height);
    bool mouse_movement = false; 
   
    while (window.isOpen()) {
        
        explosion_sprite.setPosition(-300 , -300);
        if (music.getStatus() != sf::SoundSource::Playing)
            {
                music.openFromFile("music.mp3");
                music.play();
            }
        if (HP == 0)
        {
         
           
            sf::Event event1;
            window.pollEvent(event1);
            
            window.clear();

           

            final_screen.setString("Game Over. Your Score : " + std::to_string(Score) + "\n\nPress ' F ' to play again!" + "\nPress ' Escape ' to close the game");
            final_screen.setPosition((window.getSize().x / 2.0f) - (final_screen.getGlobalBounds().width /2.f), (window.getSize().y / 2.0f) - (final_screen.getGlobalBounds().height / 2.f));
            if (splaying == false)
            {
                sound_dead.play();
                splaying = true;
            }

            

            if (event1.type == sf::Event::KeyPressed)
            { 
              
                if (event1.key.scancode == sf::Keyboard::Scan::F)
                {
                    HP = 3; 
                    bullets.clear();
                    enemies.clear();
                    hp.emplace_back(sf::Sprite(hp_texture));
                    hp.emplace_back(sf::Sprite(hp_texture));
                    hp.emplace_back(sf::Sprite(hp_texture));
                    setupHPbar(hp);
                    Score = 0; 
                }
                else if (event1.key.scancode == sf::Keyboard::Scan::Escape)
                {
                    window.close(); 
                }

            }
            window.draw(final_screen);
            window.display();

        }
        else {

            sf::Event event;
            text.setString(" Score : " + std::to_string(Score));

            sf::Time elapsed = clock.getElapsedTime();
            float deltaTime = elapsed.asSeconds() - timeSinceLastSpeedChange;

            if (deltaTime >= speedChangeInterval)
            {
               
                enemy_speed += 0.3f; 
                timeSinceLastSpeedChange = elapsed.asSeconds(); // Обновляем время последнего изменения
            }
            


           /* switch (Score)
            {
            case 160:
            {
                enemy_speed = 1.0f;
                break; 

            }
            case 340:
            {
                enemy_speed = 1.7f;
                maxEnemy += 1;
                break; 
            }
            case 800:
            {
                enemy_speed = 1.7f;
                maxEnemy += 1;
                maxBullets += 1;
                break; 
            }

            case 1200:
            {
                enemy_speed = 3.0f;
                maxEnemy += 1;
                break; 
            }
            default:
                break;
            }*/


            if (std::rand() % 100 < 3) {
                if (enemies.size() < maxEnemy)
                {
                    enemies.emplace_back(Enemy(enemy_texture, window, std::move(enemy_speed)));
                }
            }
            while (window.pollEvent(event)) {
                switch (event.type)
                {
                case  sf::Event::KeyPressed:
                {
                    if (event.key.scancode == sf::Keyboard::Scan::Escape)
                    {
                        window.close();
                        break;
                    }
                }

                case sf::Event::MouseMoved:
                {
                   
                    if (event.mouseMove.x < 0) event.mouseMove.x = 0;
                    else if (event.mouseMove.x > window.getSize().x - player_sprite.getGlobalBounds().width) {
                        event.mouseMove.x = window.getSize().x - player_sprite.getGlobalBounds().width;
                    }
                    player_sprite.setPosition(event.mouseMove.x, n);
                 
                    break;
                }
                case sf::Event::MouseButtonPressed:
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        if (bullets.size() < maxBullets)
                        {
                            sound.play();
                            bullets.emplace_back(Bullet(bullet_texture));
                            bullets.back().sprite.setPosition(-4 + player_sprite.getGlobalBounds().getPosition().x + player_sprite.getGlobalBounds().width / 2, -3 + player_sprite.getGlobalBounds().getPosition().y - bullets.back().sprite.getGlobalBounds().height);
                        }

                    }
                    break;
                }

                default:
                    break;
                }


            }


            for (auto it = bullets.begin(); it < bullets.end();)
            {
                it->update();
                if (it->isOutOfBounds(window))
                {
                    it = bullets.erase(it);
                }
                else
                {
                    ++it;
                }
            }

            for (auto it = enemies.begin(); it != enemies.end();) {
                it->update();


                if (it->sprite.getPosition().y > window.getSize().y) {
                    hp.erase(hp.end()-1);
                    --HP;
                    it = enemies.erase(it);
                    sound_hploss.play(); 
                }
                else {
                    ++it;
                }
            }


            for (auto& i : enemies)
            {
                i.update();
            }
            //

            for (auto it = bullets.begin(); it != bullets.end();) {

                bool bulletRemoved = false;

                for (auto it2 = enemies.begin(); it2 != enemies.end();) {
                    if (checkCollision(it->sprite, it2->sprite)) {
                      //  explosion_sound.play(); 
                        explosion_sprite.setPosition(it2->sprite.getPosition().x , it2->sprite.getPosition().y);
                        window.draw(explosion_sprite);
                        Score += 20;
                        it2 = enemies.erase(it2);
                        bulletRemoved = true;
                    }
                    else {
                        ++it2;
                    }
                }

                if (bulletRemoved) {
                    it = bullets.erase(it);
                }
                else {
                    ++it;
                }
            }



            //
            
            window.clear();
            
            window.draw(explosion_sprite);
            window.draw(player_sprite);
            window.draw(explosion_sprite);
            for (const auto& bullet : bullets) {
                window.draw(bullet.sprite);
            }
            for (const auto& el : hp)
            {
                window.draw(el);
            }



            for (const auto& en : enemies)
            {
                window.draw(en.sprite);
            }
           /* for (const auto& b : background)
            {
                window.draw(b);
            }*/
           
            window.draw(text);
           

            window.display();
        }
    }

}
