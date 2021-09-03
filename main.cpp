
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include<sstream>
using namespace std;
#include "ResourcePath.hpp"
using namespace sf;

const int num_branches = 7;
Sprite branches[num_branches];
enum class side {left, right, none};
side branchPos[num_branches];

void updateBranches(int seed);

void updateBranches(int seed){
    for(int i=num_branches-1; i>0; i--){
        branchPos[i] = branchPos[i-1];
    }
//spawn a new branch at position left, right or none
    srand((int)time(0) + seed);
    int r = (rand() % 5);
    switch(r){
        case 0:
            branchPos[0] = side::left;
            break;
        case 1:
            branchPos[0] = side::right;
            break;
        default:
            branchPos[0] = side::none;
            break;
    }
    
}

int main(int, char const**)
{
    //create a video mode object
    VideoMode vm(800,600);
    //system("pwd");
    //Create and open a window for the game
    RenderWindow window(vm,"Game !!!",Style::Fullscreen);
    Event event;
    
    Font font;
    font.loadFromFile("fonts/font_1.TTF");
    
    Text messageText;
    messageText.setFont(font);
    messageText.setString("Press enter to start! \nUse the left and right arrow keys to cut the branches.");
    messageText.setCharacterSize(50);
    messageText.setFillColor(Color::White);
    
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score = 0");
    scoreText.setCharacterSize(100);
    scoreText.setFillColor(Color::White);
    
    //position the message text
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1920/2.0f, 1080/2.0f);
    scoreText.setPosition(30, 20);
    
    int score = 0;
    
    Texture textureBg;
    textureBg.loadFromFile("graphics/bg2.jpg");
    Vector2u textureSize;  //Added to store texture size.
    textureSize = textureBg.getSize();
    Vector2u windowSize;   //Added to store window size
    windowSize = window.getSize();
    Sprite spriteBg;
    spriteBg.setTexture(textureBg);
    
    float ScaleX = (float) windowSize.x / textureSize.x;
    float ScaleY = (float) windowSize.y / textureSize.y;     //Calculate scale.
    
    spriteBg.setScale(ScaleX, ScaleY);
    
    //tree
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree_c.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(1350,50);
    
    //bee
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(2800,500);
    bool beeActive = false; //if the bee is currently moving
    float beeSpeed = 0.0f;   //set the bee speed
    
    //make clouds
    Texture textureClouds;
    textureClouds.loadFromFile("graphics/cloud.png");
    Sprite spriteCloud_1;
    Sprite spriteCloud_2;
    Sprite spriteCloud_3;
    spriteCloud_1.setTexture(textureClouds);
    spriteCloud_1.setPosition(300,200);
    spriteCloud_2.setTexture(textureClouds);
    spriteCloud_2.setPosition(1100,300);
    spriteCloud_3.setTexture(textureClouds);
    spriteCloud_3.setPosition(1900,150);
    
    bool cloud1active = false;
    bool cloud2active = false;
    bool cloud3active = false;
    
    float cloud1speed = 0.0f;
    float cloud2speed = 0.0f;
    float cloud3speed = 0.0f;
    
    //varable to control time
    Clock clock;
    
    //time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 50;
    timeBar.setSize(Vector2f(timeBarStartWidth,timeBarHeight));
    timeBar.setFillColor(Color::Yellow);
    timeBar.setPosition((1920/2) - timeBarStartWidth/2, 980);
    
    Time gameTotalTime;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;
    
    bool paused = true;
    
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");
    for(int i=0;i<num_branches;i++){
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000,-2000);
        branches[i].setOrigin(220,0);
    }
    
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(1125,1100);
    
    //the player starts on the left
    side playerSide = side::left;
    
    Texture textureRip;
    textureRip.loadFromFile("graphics/rip.png");
    Sprite spriteRip;
    spriteRip.setTexture(textureRip);
    //hide the rip sign
    spriteRip.setPosition(3500, 3500);
    
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(1250, 1215);
    
    //line the axe up with the tree
    const float axe_pos_left = 1250;
    const float axe_pos_right = 1620;
    
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(1345, 1170);
    
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;
  
    //control player's input
    bool acceptInput = false;
    
    //sound
    SoundBuffer bgMusicBuffer;
    bgMusicBuffer.loadFromFile("sound/bgmusic.wav");
    Sound bgmusic;
    bgmusic.setBuffer(bgMusicBuffer);
    
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);
    
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);
    
    SoundBuffer out_of_timeBuffer;
    out_of_timeBuffer.loadFromFile("sound/out_of_time.wav");
    Sound out_of_time;
    out_of_time.setBuffer(out_of_timeBuffer);
    
    
    
    while(window.isOpen()){
       
        while(window.pollEvent(event)){
            
            if(event.type == Event::KeyReleased && !paused){
                //listen for keypress again
                acceptInput = true;
                //hide the axe;
                spriteAxe.setPosition(3500, spriteAxe.getPosition().y);
            }
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Escape){
                    window.close();
                }
                
                //start the game
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Enter){
                    paused = false;
                    score = 0;
                    timeRemaining = 5;
                    //make all the branches disappear
                    for(int i=0;i<num_branches;i++){
                        branchPos[i] = side::none;
                    }
                    //make sure that the axe is at position
                    spriteAxe.setPosition(1250, 1215);
                    //make sure that the gravestone is hidden
                    spriteRip.setPosition(3500,3500);
                    //move the player into position
                    spritePlayer.setPosition(1125, 1100);
                    acceptInput = true;
                    bgmusic.play();
            }
            if(acceptInput){
                //first handle pressing the right arrow key
                if(event.type == Event::KeyPressed && event.key.code == Keyboard::Right){
                    //make sure that the player is on the right
                    playerSide = side::right;
                    score++;
                            
                    //add to the amount of time remaining
                    timeRemaining += (2 / score) + .15;
                    spriteAxe.setPosition(axe_pos_right,spriteAxe.getPosition().y);
                            
                    spritePlayer.setPosition(1745,1100);
                            
                    //upadte the branches
                    updateBranches(score);
                            
                    //set the log flying ot the left
                    spriteLog.setPosition(1345, 1170);
                    logSpeedX = -5000;
                    logActive = true;
                    acceptInput = false;
                    chop.play();        //chop sound
                    }
                            
                if(event.type == Event::KeyPressed && event.key.code == Keyboard::Left){
                    //make sure that the player is on the right
                    playerSide = side::left;
                    score++;
                            
                    //add to the amount of time remaining
                    timeRemaining += (2 / score) + .15;
                    spriteAxe.setPosition(axe_pos_left,spriteAxe.getPosition().y);
                            
                    spritePlayer.setPosition(1125,1100);
                            
                    //upadte the branches
                    updateBranches(score);
                            
                    //set the log flying ot the right
                    spriteLog.setPosition(1345, 1170);
                    logSpeedX = 5000;
                    logActive = true;
                    acceptInput = false;
                    chop.play();
                }
            }
        }
        if(!paused){
            Time dt = clock.restart();      //measure clock
            
            //subtract from the amount of time remaining
            timeRemaining -= dt.asSeconds();
            //size up the timebar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
            
            if(timeRemaining < 0.0f){
                paused = true;
                messageText.setString("Out of time!! \nPress Enter to play again.");
                
                //reposition the message text
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920/2.0f, 1080/2.0f);
                out_of_time.play();
            }
        
            if(beeActive == false){         //if bee is not active
            
                //how fast is the bee
                srand((int)time(0) * 10);
                beeSpeed = (rand() % 300) + 300;
            
                //how high is the bee
                srand((int)time(0)*10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2800,height);
                beeActive = true;
            }
        
            else{                           //if bee is active, move bee
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
            
                //if the bee reached the right end of the screen
                if(spriteBee.getPosition().x < -100){
                    beeActive = false;
                }
            }
        
        //manage cloud 1
        if(cloud1active == false){         //if cloud is not active
            
                //how fast is the cloud
                srand((int)time(0) * 10);
                cloud1speed = (rand() % 200);
            
                //how high is the cloud
                srand((int)time(0)*10);
                float height = (rand() % 150);
                spriteCloud_1.setPosition(-200,height);
                cloud1active = true;
            }
        
            else{                           //if cloud is active, move bee
                spriteCloud_1.setPosition(spriteCloud_1.getPosition().x + (cloud1speed * dt.asSeconds()), spriteCloud_1.getPosition().y);
            
                //if the bee reached the right end of the screen
                if(spriteCloud_1.getPosition().x > 2500){
                    cloud1active = false;
                }
            }
        
        //manage cloud 2
        if(cloud2active == false){         //if cloud is not active
            
                //how fast is the cloud
                srand((int)time(0) * 50);
                cloud2speed = (rand() % 200);
            
                //how high is the cloud
                srand((int)time(0) * 50);
                float height = (rand() % 300) - 150;
                spriteCloud_2.setPosition(-200,height);
                cloud2active = true;
            }
        
            else{                           //if cloud is active, move bee
                spriteCloud_2.setPosition(spriteCloud_2.getPosition().x + (cloud2speed * dt.asSeconds()), spriteCloud_2.getPosition().y);
            
                //if the bee reached the right end of the screen
                if(spriteCloud_2.getPosition().x > 2500){
                    cloud2active = false;
                }
            }
        
         //manage cloud 3
        if(cloud3active == false){         //if cloud is not active
            
                //how fast is the cloud
                srand((int)time(0) * 30);
                cloud3speed = (rand() % 200);
            
                //how high is the cloud
                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                spriteCloud_3.setPosition(-200,height);
                cloud3active = true;
            }
        
            else{                           //if cloud is active, move bee
                spriteCloud_3.setPosition(spriteCloud_3.getPosition().x + (cloud3speed * dt.asSeconds()), spriteCloud_3.getPosition().y);
            
                //if the bee reached the right end of the screen
                if(spriteCloud_3.getPosition().x > 2500){
                    cloud3active = false;
                }
            }
            
            //update the score
            stringstream ss;
            ss<<"Score = "<<score;
            scoreText.setString(ss.str());
            
            //update the branches
            for(int i=0;i<num_branches;i++){
                float height = i*150;
                if(branchPos[i] == side::left){
                    //move the sprite to the left side
                    branches[i].setPosition(1130,height);
                    branches[i].setRotation(180);
                }
                else if(branchPos[i] == side::right){
                    //move the sprite to the right side
                    branches[i].setPosition(1865,height);
                    branches[i].setRotation(0);
                }
                else{
                    //hide the branch
                    branches[i].setPosition(3500, height);
                }
            }
            
            //handle a flying log
            if(logActive){
                spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
                
                //has the log reached the right hand edge
                if(spriteLog.getPosition().x < -100 || spriteLog.getPosition().y > 3500){
                    logActive = false;
                    spriteLog.setPosition(1345, 1170);
                }
            }
            //has the player squished by a branch?
            if(branchPos[6] == playerSide){
                //death
                paused = true;
                acceptInput = false;
                
                //draw the gravestone
                spriteRip.setPosition(1130, 1200);
                
                //hide the player
                spritePlayer.setPosition(3500,3500);
                
                //hide the axe
                spriteAxe.setPosition(3500,3500);
                
                //hide the log
                spriteLog.setPosition(3500,3500);
                
                //Change the text of the message
                messageText.setString("Squished! Press Enter to play again!");
                
                //Center the above message on the screen
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
                death.play();
                bgmusic.stop();
                
            }
        }
            window.clear();                 //close previos frame of animation
            window.draw(spriteBg);
            window.draw(spriteBee);
            window.draw(spriteCloud_1);
            window.draw(spriteCloud_2);
            window.draw(spriteCloud_3);
            window.draw(spriteTree);
        for(int i=0;i<num_branches;i++) window.draw(branches[i]);
            window.draw(spritePlayer);
            window.draw(spriteAxe);
            window.draw(spriteLog);
            window.draw(spriteRip);
            window.draw(scoreText);
            window.draw(timeBar);
            if(paused) window.draw(messageText);
            
            window.display();   //show everything we just drew
    }
    return EXIT_SUCCESS;
}
