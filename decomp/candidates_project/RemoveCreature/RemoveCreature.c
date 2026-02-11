#include "gamecode/creature.h"

void RemoveCreature(struct creature_s *c) {
  RemoveGameObject(&c->obj);
  c->used = 0;
  return;
}
