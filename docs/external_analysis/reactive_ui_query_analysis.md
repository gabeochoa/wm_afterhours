# ReactiveUI / DynamicData → EntityQuery Analysis

What ReactiveUI and DynamicData do for reactive collections, compared to
afterhours' `EntityQuery`, and what ideas we can adopt to make queries
easier to use and faster.

Sources:
- https://www.reactiveui.net/docs/getting-started/
- https://github.com/reactivemarbles/DynamicData
- EnTT (C++ ECS) and Flecs (C ECS) for ECS-specific comparison

---

## 1. What ReactiveUI / DynamicData Actually Is

ReactiveUI is a **.NET MVVM framework** built on Reactive Extensions (Rx).
It's not an ECS or a query builder in the traditional sense — it's a
reactive data binding system.

**DynamicData** is the more relevant piece: a reactive collection library
that provides LINQ-like operators over observable collections. When the
source data changes, derived queries automatically re-evaluate.

The key concepts:
- `SourceCache<T, TKey>` — a keyed mutable collection (like an entity store)
- `IObservable<IChangeSet>` — a stream of diffs (adds, removes, updates)
- Operators: `Filter`, `Sort`, `Transform`, `Group`, `Page`, `Bind`
- Queries are **declarative pipelines** that react to changes

---

## 2. Current EntityQuery API

```cpp
// Current afterhours EntityQuery usage:
auto enemies = EntityQuery()
    .whereHasComponent<Enemy>()
    .whereHasComponent<Health>()
    .whereMissingComponent<Dead>()
    .whereLambda([](const Entity& e) {
        return e.get<Health>().value > 0;
    })
    .orderByLambda([](const Entity& a, const Entity& b) {
        return a.get<Health>().value < b.get<Health>().value;
    })
    .take(5)
    .gen();
```

**Current strengths:**
- Fluent builder pattern (already good)
- `SKIP_ENTITY_QUERY_MODIFICATIONS` fast path avoids virtual dispatch
- Short-circuit for `gen_first()` / `has_values()`
- Result caching (`ran_query` flag)
- Multiple output modes: `gen()`, `gen_first()`, `gen_count()`, `gen_ids()`,
  `gen_handles()`, `gen_as<T>()`, `gen_random()`

**Current pain points:**
1. Every query scans **all entities** linearly — no index acceleration
2. `whereHasComponent<T>()` checks the component map per entity per filter
3. No way to compose queries or reuse partial query definitions
4. `orderByLambda` only supports one sort key
5. No `select` / `transform` / `map` — results are always entities
6. No aggregation (`sum`, `min`, `max`, `count_where`)
7. Component access after query requires a second lookup (`entity.get<T>()`)
8. No caching across frames — query re-runs every time `gen()` is called

---

## 3. What DynamicData Does That's Interesting

### 3.1 Change-Set Propagation (Reactive Queries)

DynamicData's core idea: instead of re-querying from scratch every frame,
propagate **deltas** through a pipeline.

```csharp
// DynamicData: define once, auto-updates when source changes
var liveEnemies = entityCache
    .Connect()
    .Filter(e => e.Has<Enemy>() && !e.Has<Dead>())
    .Sort(SortExpressionComparer<Entity>.Ascending(e => e.Get<Health>().Value))
    .Top(5)
    .Bind(out var topEnemies)  // topEnemies auto-updates
    .Subscribe();
```

When an entity is added/removed/modified, only the diff flows through
the pipeline. The `Filter` only re-evaluates for changed entities, `Sort`
does incremental insertion, and `Bind` outputs the final list.

**afterhours applicability:** This is the most impactful idea, but also the
hardest to implement. It requires:
- Entity change tracking (know which entities had components added/removed)
- Incremental filter evaluation (only check changed entities)
- Stable result caching across frames

See "Proposed: Cached Queries" below.

### 3.2 Composable Operators

DynamicData has ~60 operators that compose into pipelines:

| Operator | What It Does | EntityQuery Equivalent |
|---|---|---|
| `Filter(predicate)` | Keep matching items | `whereHasComponent<T>()`, `whereLambda()` |
| `Sort(comparer)` | Order results | `orderByLambda()` |
| `Top(n)` / `Page(request)` | Limit/paginate | `take(n)` |
| `Transform(selector)` | Project to different type | Missing |
| `Group(keySelector)` | Group by key | Missing |
| `DistinctValues(selector)` | Unique values of a field | Missing |
| `And(other)` / `Or(other)` | Set intersection/union of queries | Missing |
| `Except(other)` | Set difference | Missing |
| `MergeMany(selector)` | Flatten nested observables | Missing |
| `TrueForAll(predicate)` | All-match check | Missing |
| `TrueForAny(predicate)` | Any-match check | `has_values()` (limited) |
| `QueryWhenChanged()` | Re-query on any change | Missing |
| `WatchValue(key)` | Watch a single entity | Missing |
| `AutoRefresh(property)` | Re-filter when property changes | Missing |
| `Bind(collection)` | Output to observable collection | Missing |
| `ToCollection()` | Snapshot to list | `gen()` |
| `Count()` | Live count | `gen_count()` |

### 3.3 Projection / Transform

```csharp
// DynamicData: project entities to a simpler view
var healthBars = entityCache
    .Connect()
    .Filter(e => e.Has<Health>())
    .Transform(e => new HealthBarVM(e.Get<Health>().Value, e.Get<Health>().Max))
    .Bind(out var bars)
    .Subscribe();
```

This avoids carrying the full entity when you only need a few fields.

### 3.4 Grouping

```csharp
// Group entities by their team
var teams = entityCache
    .Connect()
    .Group(e => e.Get<Team>().Id)
    .Transform(group => new TeamVM(group.Key, group.Cache.Items))
    .Bind(out var teamList)
    .Subscribe();
```

### 3.5 Set Operations (And / Or / Except)

```csharp
// Intersection: entities that are in BOTH visible and damageable
var targets = visible.And(damageable);

// Union: entities that are in EITHER group
var all = groupA.Or(groupB);

// Difference: visible but NOT in cover
var exposed = visible.Except(inCover);
```

---

## 4. What EnTT / Flecs Do (ECS-Specific Patterns)

### 4.1 Compile-Time Multi-Component Views (EnTT)

EnTT's `view<A, B, C>` iterates only over entities that have **all** of
the specified components, using the smallest component pool as the
driving iterator:

```cpp
// EnTT: compile-time typed, zero overhead
auto view = registry.view<Position, Velocity>(entt::exclude<Dead>);
for (auto [entity, pos, vel] : view.each()) {
    pos.x += vel.dx;
}
```

Key optimization: EnTT picks the **smallest pool** among A, B, C to
iterate, then checks containment in the other pools. This means a query
for `<Rare, Common>` only iterates the Rare pool, not all entities.

**afterhours equivalent:** This would require component-indexed storage
(an index from component type → set of entity IDs that have it). Current
afterhours stores components on entities, so there's no fast "give me all
entities with component X" lookup.

### 4.2 Cached Queries (Flecs)

Flecs caches query results in "tables" (archetypes). When a query is
created, it finds all matching tables. When entities are
created/destroyed/change components, only the affected tables are updated.

```cpp
// Flecs: cached query, evaluated once, updated incrementally
auto q = world.query<Position, Velocity>();
q.each([](Position& p, Velocity& v) {
    p.x += v.x;
});
```

The query never re-scans all entities — it maintains a list of matching
archetypes that is updated when archetypes are created or destroyed.

### 4.3 Structured Bindings with Component Access

Both EnTT and Flecs return components directly in the iteration, not
entities. No second lookup needed:

```cpp
// EnTT: components returned directly
for (auto [entity, pos, vel] : view.each()) {
    // pos and vel are references, no entity.get<>() needed
}

// Flecs: same pattern
q.each([](Position& p, Velocity& v) {
    // direct references
});
```

---

## 5. Proposed Improvements for afterhours EntityQuery

### 5.1 Multi-Component Query with Direct Access (High Priority)

**Problem:** Every `whereHasComponent<T>()` is a separate filter that checks
the entity's component map. Then after matching, you call `entity.get<T>()`
which does another map lookup.

**Proposal:** A `with<A, B, C>()` method that both filters AND provides
direct access, eliminating the double lookup:

```cpp
// Proposed: query + access in one step
EntityQuery()
    .with<Enemy, Health, Position>()
    .whereMissingComponent<Dead>()
    .each([](Entity& e, Enemy& enemy, Health& hp, Position& pos) {
        // Components are already resolved — no get<>() needed
    });
```

Implementation:

```cpp
template <typename... Components>
TReturn& with() {
    return add_filter([](const Entity& e) {
        return (e.has<Components>() && ...);
    });
}

template <typename Fn, typename... Components>
void each(Fn&& fn) {
    for (Entity& e : gen()) {
        fn(e, e.get<Components>()...);
    }
}
```

This is syntactic sugar but saves significant boilerplate and makes the
intent clearer.

### 5.2 Transform / Select (High Priority)

**Problem:** `gen()` always returns entities. When you only need one
component, you still carry the full entity reference.

**Proposal:** `gen_map<T>()` and `gen_select()`:

```cpp
// Get just the Health components from matching entities
auto healths = EntityQuery()
    .whereHasComponent<Health>()
    .gen_map<Health>();
// Returns: std::vector<std::reference_wrapper<Health>>

// Project to a custom struct
auto bars = EntityQuery()
    .whereHasComponent<Health>()
    .gen_select([](Entity& e) -> HealthBar {
        auto& hp = e.get<Health>();
        return { hp.current, hp.max, e.id };
    });
// Returns: std::vector<HealthBar>
```

Implementation:

```cpp
template <typename T>
[[nodiscard]] std::vector<std::reference_wrapper<T>> gen_map() const {
    // Note: gen_as<T>() already exists! This is just an alias.
    return gen_as<T>();
}

template <typename Fn>
[[nodiscard]] auto gen_select(Fn&& fn) const {
    using R = std::invoke_result_t<Fn, Entity&>;
    auto results = gen();
    std::vector<R> out;
    out.reserve(results.size());
    for (Entity& e : results) {
        out.push_back(fn(e));
    }
    return out;
}
```

Note: `gen_as<T>()` already exists and does exactly `gen_map<T>()`.
Just adding `gen_select()` for arbitrary projections.

### 5.3 Aggregation Methods (Medium Priority)

**Problem:** Computing sum/min/max/avg requires materializing the full
result then manually iterating.

**Proposal:** Direct aggregation on the query:

```cpp
// Sum all health values
float total = EntityQuery()
    .whereHasComponent<Health>()
    .gen_sum([](Entity& e) { return e.get<Health>().current; });

// Find entity with minimum health
auto weakest = EntityQuery()
    .whereHasComponent<Health>()
    .gen_min_by([](const Entity& a, const Entity& b) {
        return a.get<Health>().current < b.get<Health>().current;
    });

// Count entities matching additional predicate
size_t critical = EntityQuery()
    .whereHasComponent<Health>()
    .gen_count_where([](Entity& e) {
        return e.get<Health>().current < 10.0f;
    });

// Check if all matching entities satisfy a condition
bool all_full = EntityQuery()
    .whereHasComponent<Health>()
    .gen_all([](Entity& e) {
        return e.get<Health>().current >= e.get<Health>().max;
    });

// Check if any matching entity satisfies a condition
bool any_dead = EntityQuery()
    .whereHasComponent<Health>()
    .gen_any([](Entity& e) {
        return e.get<Health>().current <= 0;
    });
```

Implementation:

```cpp
template <typename Fn>
[[nodiscard]] auto gen_sum(Fn&& fn) const {
    using R = std::invoke_result_t<Fn, Entity&>;
    R total{};
    for (Entity& e : gen()) total += fn(e);
    return total;
}

template <typename Fn>
[[nodiscard]] OptEntity gen_min_by(Fn&& cmp) const {
    auto results = gen();
    if (results.empty()) return {};
    auto it = std::min_element(results.begin(), results.end(),
        [&](const Entity& a, const Entity& b) { return cmp(a, b); });
    return *it;
}

template <typename Fn>
[[nodiscard]] size_t gen_count_where(Fn&& pred) const {
    size_t count = 0;
    for (Entity& e : gen()) if (pred(e)) ++count;
    return count;
}

template <typename Fn>
[[nodiscard]] bool gen_all(Fn&& pred) const {
    for (Entity& e : gen()) if (!pred(e)) return false;
    return true;
}

template <typename Fn>
[[nodiscard]] bool gen_any(Fn&& pred) const {
    for (Entity& e : gen()) if (pred(e)) return true;
    return false;
}
```

### 5.4 Chained / Multi-Key Sorting (Medium Priority)

**Problem:** `orderByLambda` only supports one sort key and errors on
multiple calls.

**Proposal:** `thenBy()` for secondary sort keys:

```cpp
auto sorted = EntityQuery()
    .whereHasComponent<Enemy>()
    .orderBy<Health>([](const Health& hp) { return hp.current; })
    .thenBy<Position>([](const Position& p) { return p.x; })
    .gen();
```

Implementation: store multiple sort keys, compose them:

```cpp
template <typename Component, typename KeyFn>
TReturn& orderBy(KeyFn&& key_fn, bool descending = false) {
    auto fn = [key_fn, descending](const Entity& a, const Entity& b) {
        auto ka = key_fn(a.get<Component>());
        auto kb = key_fn(b.get<Component>());
        return descending ? kb < ka : ka < kb;
    };
    if (!orderby) {
        orderby = fn;
    } else {
        auto prev = *orderby;
        orderby = [prev, fn](const Entity& a, const Entity& b) {
            if (prev(a, b)) return true;
            if (prev(b, a)) return false;
            return fn(a, b);  // Tie-break with secondary key
        };
    }
    return static_cast<TReturn&>(*this);
}

template <typename Component, typename KeyFn>
TReturn& thenBy(KeyFn&& key_fn, bool descending = false) {
    return orderBy<Component>(std::forward<KeyFn>(key_fn), descending);
}
```

### 5.5 Query Presets / Reusable Definitions (Medium Priority)

**Problem:** Common queries are rebuilt from scratch every time.

**Proposal:** Allow saving a query definition and re-executing it:

```cpp
// Define once (could be static or member):
auto alive_enemies = EntityQuery()
    .whereHasComponent<Enemy>()
    .whereHasComponent<Health>()
    .whereMissingComponent<Dead>();

// Reuse with different terminators:
auto all = alive_enemies.gen();
auto count = alive_enemies.gen_count();
auto first = alive_enemies.gen_first();

// Chain additional filters on top:
auto nearby = EntityQuery(alive_enemies)  // copy the filter chain
    .whereLambda([&](const Entity& e) {
        return distance(player_pos, e.get<Position>()) < 100.f;
    })
    .gen();
```

This already mostly works because `EntityQuery` stores filters in a
`std::vector<FilterFn>`. The issue is the `ran_query` cache. Adding a
copy constructor that resets the cache would enable this:

```cpp
EntityQuery(const EntityQuery& other)
    : entities(other.entities)
    , orderby(other.orderby)
    , mods(other.mods)  // Copy filter chain
    , ran_query(false)   // Reset cache!
{}
```

### 5.6 Component Index (Low Priority, High Impact)

**Problem:** Every query does a linear scan of all entities. For a game
with 10,000 entities and a query matching 50, we check all 10,000.

**Proposal:** A per-component-type index mapping component type → entity set:

```cpp
// Internal to EntityCollection:
std::unordered_map<ComponentTypeId, std::vector<EntityID>> component_index;

// When entity.addComponent<T>() is called:
component_index[type_id<T>()].push_back(entity.id);

// When entity.removeComponent<T>() is called:
auto& vec = component_index[type_id<T>()];
vec.erase(std::remove(vec.begin(), vec.end(), entity.id), vec.end());
```

Then `whereHasComponent<T>()` can use the index to start with a smaller
set instead of scanning all entities:

```cpp
// Instead of scanning 10,000 entities, start with the smallest pool:
template <typename T> auto& whereHasComponent() {
    // Tell the query to use component T's index as the driving set
    if (component_index_for<T>().size() < driving_set.size()) {
        driving_set = component_index_for<T>();
    }
    return add_filter([](const Entity& e) { return e.has<T>(); });
}
```

This is EnTT's key optimization. It's a significant refactor but would
make queries O(smallest_pool) instead of O(all_entities).

### 5.7 Partition / Group By (Low Priority)

**Problem:** No way to group query results by a key.

**Proposal:**

```cpp
// Group enemies by team
auto teams = EntityQuery()
    .whereHasComponent<Enemy>()
    .gen_group_by<Team>([](Entity& e) { return e.get<Team>().id; });
// Returns: std::unordered_map<int, RefEntities>
```

Implementation:

```cpp
template <typename KeyFn>
[[nodiscard]] auto gen_group_by(KeyFn&& key_fn) const {
    using K = std::invoke_result_t<KeyFn, Entity&>;
    std::unordered_map<K, RefEntities> groups;
    for (Entity& e : gen()) {
        groups[key_fn(e)].push_back(e);
    }
    return groups;
}
```

---

## 6. What NOT to Adopt

### 6.1 Full Reactive / Observable Pipeline

DynamicData's core value is incremental updates via change-set
propagation. This requires:
- An event system for component add/remove/modify
- Subscription management
- Memory for cached intermediate results

For a game running at 60fps where entity sets change constantly, the
overhead of maintaining subscriptions and change-set machinery may exceed
the cost of just re-querying. The simpler approach (fast linear scan +
optional component index) is likely better for afterhours.

**Skip:** Observable change-sets, `AutoRefresh`, `WhenAnyValue`,
subscription lifecycle management.

### 6.2 Complex Set Operations (And / Or / Except)

While these look nice, in practice ECS queries express set operations
through component presence:
- **And** = `whereHasComponent<A>().whereHasComponent<B>()` (already works)
- **Or** = would require a union of two query results (rare need)
- **Except** = `whereMissingComponent<X>()` (already works)

**Skip:** Formal set algebra operators. The component filter system
already covers this.

### 6.3 MVVM Binding System

ReactiveUI's `OneWayBind`, `Bind`, `BindCommand` patterns are
specific to UI data binding in a retained-mode architecture. afterhours
uses immediate-mode UI where the "binding" is just reading the data
directly in the render function.

**Skip:** All binding infrastructure.

---

## 7. Implementation Priority

| Improvement | Effort | Impact | Priority |
|---|---|---|---|
| `gen_select()` (projection) | Small | Medium | **High** — reduces boilerplate |
| `each()` with component access | Small | High | **High** — eliminates double lookup |
| Aggregation methods (sum/min/max/all/any) | Small | Medium | **High** — common operations |
| Copy constructor for query reuse | Tiny | Medium | **High** — enables presets |
| Multi-key sorting (`thenBy`) | Small | Low | **Medium** — nice ergonomics |
| `gen_group_by()` | Small | Low | **Medium** — useful for team/faction logic |
| Component index | Large | Very High | **Low** priority, high impact — major refactor |

The first four items are all small additions (< 50 lines each) that
immediately improve the day-to-day ergonomics of using EntityQuery.

---

## 8. Concrete Code Additions

All of the High priority items can be added to `entity_query.h` without
modifying any other files:

```cpp
// === In EntityQuery struct, after gen_random() ===

// Project results to a custom type
template <typename Fn>
[[nodiscard]] auto gen_select(Fn&& fn) const {
    using R = std::invoke_result_t<Fn, Entity&>;
    auto results = gen();
    std::vector<R> out;
    out.reserve(results.size());
    for (Entity& e : results) out.push_back(fn(e));
    return out;
}

// Iterate with component access (avoids double lookup)
template <typename... Components, typename Fn>
void each(Fn&& fn) const {
    for (Entity& e : gen()) {
        fn(e, e.template get<Components>()...);
    }
}

// Aggregations
template <typename Fn>
[[nodiscard]] auto gen_sum(Fn&& fn) const {
    using R = std::invoke_result_t<Fn, Entity&>;
    R total{};
    for (Entity& e : gen()) total += fn(e);
    return total;
}

template <typename Fn>
[[nodiscard]] OptEntity gen_min_by(Fn&& cmp) const {
    auto results = gen();
    if (results.empty()) return {};
    return *std::min_element(results.begin(), results.end(),
        [&](const Entity& a, const Entity& b) { return cmp(a, b); });
}

template <typename Fn>
[[nodiscard]] OptEntity gen_max_by(Fn&& cmp) const {
    auto results = gen();
    if (results.empty()) return {};
    return *std::max_element(results.begin(), results.end(),
        [&](const Entity& a, const Entity& b) { return cmp(a, b); });
}

template <typename Fn>
[[nodiscard]] size_t gen_count_where(Fn&& pred) const {
    size_t c = 0;
    for (Entity& e : gen()) if (pred(e)) ++c;
    return c;
}

template <typename Fn>
[[nodiscard]] bool gen_all(Fn&& pred) const {
    for (Entity& e : gen()) if (!pred(e)) return false;
    return true;
}

template <typename Fn>
[[nodiscard]] bool gen_any(Fn&& pred) const {
    for (Entity& e : gen()) if (pred(e)) return true;
    return false;
}

// Group by key
template <typename KeyFn>
[[nodiscard]] auto gen_group_by(KeyFn&& key_fn) const {
    using K = std::invoke_result_t<KeyFn, Entity&>;
    std::unordered_map<K, RefEntities> groups;
    for (Entity& e : gen()) groups[key_fn(e)].push_back(e);
    return groups;
}
```

---

## Summary

ReactiveUI/DynamicData's biggest idea — **incremental change-set
propagation** — is architecturally interesting but overkill for a
game ECS running at 60fps. The simpler improvements that transfer well:

1. **Projection** (`gen_select()`) — avoid carrying full entities when
   you only need specific data
2. **Direct component access** (`each<A,B>()`) — eliminate the pattern
   of filter-then-get which does redundant lookups
3. **Aggregation** (`gen_sum`, `gen_min_by`, `gen_all`, `gen_any`) —
   first-class support for common aggregate operations
4. **Query reuse** — copy constructor that preserves filters but resets
   cache, enabling saved query definitions
5. **Multi-key sort** (`thenBy`) — compose sort keys instead of writing
   manual tie-breaking lambdas

The long-term win is a **component index** (EnTT-style), which would
make `whereHasComponent<T>()` O(pool_size) instead of O(all_entities).
This is the single biggest performance improvement possible, but requires
changes to entity storage, not just the query interface.
