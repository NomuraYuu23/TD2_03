#pragma once
#include <variant>

class Player;
class Box;
using ColliderParentObject = std::variant<Player*,Box*>;
