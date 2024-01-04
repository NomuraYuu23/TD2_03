#pragma once
#include <variant>

class Player;
class Block;
class Screw;
class Magnet;
using ColliderParentObject = std::variant<Player*,Block*,Magnet*,Screw*>;
