#pragma once
#include <variant>

class Player;
class Block;
class Screw;
class Magnet;
class UFO;
class UFOAttract;
class Energy;
using ColliderParentObject = std::variant<Player*,Block*,Magnet*,Screw*,UFO*,UFOAttract*,Energy*>;
