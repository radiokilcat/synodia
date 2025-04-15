
### How to create Entites:

Entity newEntity = registry->CreateEntity();

registry->TagEntity(newEntity, tag);
registry->GroupEntity(newEntity, group);
entity.AddComponent<SpriteComponent>( "sprite_name", 64, 64, 1, true, 0, 0);