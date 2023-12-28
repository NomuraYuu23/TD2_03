#pragma once
#include <variant>

class Player;
class Block;
using ColliderParentObject = std::variant<Player*,Block*>;
