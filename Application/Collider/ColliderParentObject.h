#pragma once
#include <variant>

class Player;
class Block;
class Magnet;
using ColliderParentObject = std::variant<Player*,Block*,Magnet*>;
