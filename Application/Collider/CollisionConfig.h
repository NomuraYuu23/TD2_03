#pragma once
#include <cstdInt>

//プレイヤー
const uint32_t kCollisionAttributePlayer = 0b1;
//ブロック
const uint32_t kCollisionAttributeBlock = 0b1 << 1;
//ネジ
const uint32_t kCollisionAttributeScrew = 0b1 << 2;
//磁力エリア
const uint32_t kCollisionAttributeMagnet = 0b1 << 3;
//UFO
const uint32_t kCollisionAttributeUFO = 0b1 << 4;
//UFO吸引エリア
const uint32_t kCollisionAttributeUFOAttract = 0b1 << 5;