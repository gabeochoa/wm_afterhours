# Entity Pool Design

**Date:** 2026-03-01
**Goal:** Eliminate entity alloc/dealloc overhead for smooth frame-to-frame interactions.

## Problem

Each `Entity` is ~1KB (128-slot `ComponentArray` of `unique_ptr`). Creating and destroying entities via `make_shared`/destructor dominates frame cost during screen transitions and dynamic content changes. Profiling shows 73% of a flat-1000 layout benchmark is entity alloc/dealloc.

## Approach

Add an optional entity pool to `EntityCollection`. Pre-allocated entities are recycled instead of destroyed, avoiding heap allocation for the Entity struct. Existing API is unchanged -- users who don't call `reserve_entities()` get exactly today's behavior.

## Design

### 1. Entity Pool in EntityCollection

```cpp
struct EntityCollection {
  // ... existing members ...

  Entities entity_pool_;
  size_t max_pool_size_ = 0;       // 0 = no cap (grows with cleanup)
  std::vector<EntityID> free_ids_; // reusable IDs from recycled entities

  void reserve_entities(size_t count) {
    max_pool_size_ = std::max(max_pool_size_, count * 2);
    entity_pool_.reserve(count);
    for (size_t i = 0; i < count; ++i) {
      entity_pool_.push_back(std::make_shared<Entity>());
    }
  }
};
```

### 2. createEntity() Checks Pool First

```cpp
Entity &createEntityWithOptions(const CreationOptions &options) {
  if (temp_entities.capacity() == 0) [[unlikely]]
    reserve_temp_space();

  EntityType e;
  if (!entity_pool_.empty()) {
    e = std::move(entity_pool_.back());
    entity_pool_.pop_back();
    e->recycle(alloc_entity_id());
  } else {
    e = std::make_shared<Entity>(alloc_entity_id());
  }
  temp_entities.push_back(e);

  if (options.is_permanent) {
    permanant_ids.insert(e->id);
  }
  return *e;
}

EntityID alloc_entity_id() {
  if (!free_ids_.empty()) {
    EntityID id = free_ids_.back();
    free_ids_.pop_back();
    return id;
  }
  return ENTITY_ID_GEN++;
}
```

### 3. Entity::recycle()

Resets state without deallocating the struct. Only touches occupied component slots using the bitset.

```cpp
void recycle(EntityID new_id) {
  // Only reset slots that actually have components
  for (size_t i = 0; i < max_num_components; ++i) {
    if (componentSet.test(i)) {
      componentArray[i].reset();
    }
  }
  componentSet.reset();
  tags.reset();
  cleanup = false;
  entity_type = 0;
  ah_slot_index = EntityHandle::INVALID_SLOT;
  id = new_id;
}
```

### 4. cleanup() Returns to Pool

```cpp
// In the cleanup loop, instead of just popping:
invalidate_entity_slot_if_any(entities[i]);
EntityID old_id = entities[i]->id;
free_ids_.push_back(old_id);

if (max_pool_size_ == 0 || entity_pool_.size() < max_pool_size_) {
#ifdef AFTER_HOURS_DEBUG
  assert(entities[i].use_count() == 1 &&
         "Entity returned to pool with external references");
#endif
  entities[i]->recycle(0); // ID assigned on next createEntity
  entity_pool_.push_back(std::move(entities[i]));
} else {
  // Pool is full, let shared_ptr die normally
}
// ... swap-remove from active list as before
```

### 5. Entity Constructor Change

Entity constructor needs to accept an ID parameter so pooled entities don't burn IDs on construction.

```cpp
Entity() : id(ENTITY_ID_GEN++) {}           // existing (backward compat)
explicit Entity(EntityID id_) : id(id_) {}  // new: pool-assigned ID
```

## Goblin Review Fixes

| # | Issue | Fix |
|---|-------|-----|
| 1 | recycle() iterates all 128 slots | Use componentSet to iterate only occupied slots |
| 2 | Unbounded ID growth | ID free list (`free_ids_`) reuses IDs from recycled entities |
| 3 | Thread safety of pool | Document: pool is not thread-safe, same as existing createEntity |
| 4 | Pool grows without bound | `max_pool_size_` caps pool, excess entities freed normally |
| 5 | shared_ptr overhead remains | Accepted for now; future optimization to unique_ptr/indices |
| 6 | Stale external references | Debug assert on `use_count() == 1` before recycling |

## Tests (entity_pool_test.cpp)

1. No component leakage across recycle
2. Fresh ID after recycle (from free list)
3. Component data isolation (no stale data)
4. Handle invalidation after recycle (stale generation)
5. componentSet is clean after recycle
6. Tags are clean after recycle
7. Mixed pool and non-pool (fallback to make_shared when pool exhausted)
8. Singleton entities never pooled
9. Stress test: 1000 create/cleanup/create cycles with data verification
10. Pool size respects max cap
11. ID reuse: IDs stay bounded after many create/cleanup cycles

## What This Doesn't Solve

- Component allocations (`make_unique<T>()` per `addComponent`) still hit the heap
- `shared_ptr` refcount overhead on every vector operation
- These are future optimizations that can layer on top of this design

## API Surface

```cpp
// Optional warm-up. Existing code unaffected.
collection.reserve_entities(500);

// Everything else unchanged:
// createEntity(), addComponent(), cleanup(), get(), has()
```
