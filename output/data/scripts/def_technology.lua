Data.Tech = {}



Data.Tech[ "Chopper" ] =
{
    difficulty = 1,
    require_tech = {},
    require_item = { { "Stone", 2 } },
    crafted_item = "Chopper",
    crafted_time = 10,
}

Data.Tech[ "StoneAxe" ] =
{
    difficulty = 3,
    require_tech = { "Chopper" },
    require_item = { { "Stone", 2 }, { "Wood", 2 } },
    crafted_item = "StoneAxe",
    crafted_time = 20,
}

Data.Tech[ "House" ] =
{
    difficulty = 10,
    require_tech = { "Chopper" }, -- this tech learned only after this techs
    require_item = { { "Stone", 20 }, { "Wood", 20 } },
    crafted_building = "House", -- this tech produce building
    crafted_time = 1000, -- this amount of time unit will craft this
}

Data.Tech[ "CastleWall" ] =
{
    difficulty = 100,
    require_tech = { "House" },
    require_item = { { "Stone", 2000 } },
    crafted_building = "CastleWall",
    crafted_time = 5000,
}
