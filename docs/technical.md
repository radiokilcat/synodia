## Архитектура проекта

проект предположительно состоит из движка Anvil - динамической библиотеки, линкующей SDL и клиента (игры). В будущем было бы круто добавить поддержку Lua или AngelScript и переписать клиентский код на него. 

SDL включен в проект в качестве git submodules. 


Набор подсистем, которые необходимо реализовать:
- [ ] GUI System
- [ ] Window System
- [ ] Game System
- [ ] Resourse System
- [ ] Level System
- [ ] Items System
- [ ] state machine


Полезные ссылки:
гайд по миграции с SDL2 на SDL3 https://github.com/libsdl-org/SDL/blob/main/docs/README-migration.md
