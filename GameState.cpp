/**
 * Copyright (C) David Wolfe, 1999.  All rights reserved.
 * Ported to Qt and adapted for TDDD86, 2015.
 */

#include "GameState.h"
#include "utilities.h"
#include "constants.h"

GameState::GameState(){}

GameState::GameState(int numberOfRobots) {
    for (int i = 0; i < numberOfRobots; ++i) {
        Robot* robot = new Robot();

        while (!isEmpty(*robot)){
            delete robot;
            robot = new Robot;
        }
        robots.push_back(robot);
    }
    teleportHero();
}

GameState::GameState(const GameState& gameState){  // copy constructor
    copy(gameState);
}

GameState::~GameState(){  // destructor
    for (Robot* robot : robots){
        delete robot;
    }
    robots.clear();
}

void GameState::copy(const GameState& other){
    hero = other.hero; // copy hero
    for (auto robot : other.robots){ //
           robots.push_back(robot->robClone()); // push copied robots (robot-clones)
    }
}

GameState& GameState::operator=(const GameState& other){

    for(auto robot : robots){
            delete robot;
        }
    robots.clear();

    copy(other);

    return *this;
}

void GameState::draw(QGraphicsScene *scene) const {
    scene->clear();
    hero.draw(scene);
    for (Robot* robot : robots)
        robot->draw(scene);
}

void GameState::teleportHero() {
    do hero.teleport();
    while (!isEmpty(hero));
}

void GameState::moveRobots() {
    for (unsigned int i = 0; i < robots.size(); ++i)
        robots[i]->moveTowards(hero);
}

int GameState::countCollisions() {
    int numberDestroyed = 0;
    for(unsigned int i = 0; i < robots.size(); ++i) {

        //true if more than 1 robot at same position (collision)
        bool collision = (countRobotsAt(*robots[i]) > 1);

        if(!robots[i]->isJunk()){
            //robot
            if (collision){
                Junk* junk = new Junk(*robots[i]);
                robots[i] = junk;
                numberDestroyed++;
            }
        }
    }
    return numberDestroyed;
}

bool GameState::anyRobotsLeft() const {
    for(unsigned int i = 0; i < robots.size(); ++i){
        if(!robots[i]->isJunk()){
            return true;
        }
    }
    return false;
}

bool GameState::heroDead() const {
    return !isEmpty(hero);
}

bool GameState::isSafe(const Unit& unit) const {
    for (unsigned int i = 0; i < robots.size(); ++i)
        if (robots[i]->attacks(unit)) return false;
    if (!isEmpty(unit)) return false;
    return true;
}

void GameState::moveHeroTowards(const Unit& dir) {
    hero.moveTowards(dir);
}

Hero GameState::getHero() const {return hero;}

/*
 * Free of robots and junk only
 */
bool GameState::isEmpty(const Unit& unit) const {
    return (countRobotsAt(unit) == 0);
}

/*
 * How many robots are there at unit?
 */
int GameState::countRobotsAt(const Unit& unit) const {
    int count = 0;
    for (size_t i = 0; i < robots.size(); ++i) {
        if (robots[i]->at(unit))
            count++;
    }
    return count;
}




