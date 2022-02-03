#include "vectors/main.h"

LevelData level0{"Qualification", 3, 20, 2000};
LevelData level1{"1/16 final", 5, 30, 3000};
LevelData level2{"1/8 final", 8, 40, 4500};
LevelData level3{"1/4 final", 10, 40, 5000};
LevelData level4{"Semi-final", 10, 40, 5500};
LevelData level5{"Final", 10, 40, 5500, true};
LevelData level6{"Grand Prix", 15, 50, 6000, true};
LevelData level7{"End", 0, 30, 1000};

//LevelData level0{"Qualification", 0, 30, 1000};
//LevelData level1{"1/16 final", 1, 30, 1000};
//LevelData level2{"1/8 final", 1, 30, 1000};
//LevelData level3{"1/4 final", 1, 30, 1000};
//LevelData level4{"Semi-final", 1, 30, 1000};
//LevelData level5{"Final", 1, 30, 1000, true};
//LevelData level6{"Grand Prix", 1, 30, 1000, true};
//LevelData level7{"End", 0, 0, 1000};

std::vector<LevelData> levels = {level0, level1, level2, level3, level4, level5, level6, level7};

void setLevels(Game *game)
{
    game->levels = levels;
}