#include "../src/systems/ScreenTree.h"
#include <cassert>

int main() {
  ScreenTree tree;
  tree.entries = {{"a", "Widgets", "Slider", 0}, {"b", "Components", "Button", 1},
                  {"b", "Components", "Alias", 1}, {"c", "Tools", "Inspector", 2}};
  tree.rebuild();
  assert(tree.rows.size() == 2);
  tree.reveal(1);
  assert(tree.rows[tree.cursor].header());
  tree.right();
  tree.toggle(3);
  assert(tree.rows.size() == 5);
  tree.query = "component";
  tree.rebuild();
  assert(tree.rows.size() == 3);
  tree.query.clear();
  tree.rebuild();
  tree.toggle(0);
  assert(tree.rows.size() == 3);
  tree.query = "slider";
  tree.rebuild();
  assert(tree.rows.size() == 2);
  tree.query = "no match";
  tree.rebuild();
  assert(tree.rows.empty());
  tree.left(); tree.right(); tree.move(1);
  tree.reveal(1);
  assert(tree.rows[tree.cursor].header());
  tree.left();
  assert(tree.rows[tree.cursor].header());
  tree.right();
  tree.right();
  assert(!tree.rows[tree.cursor].header());
}
