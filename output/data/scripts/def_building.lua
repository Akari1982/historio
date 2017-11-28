Data.Building = {}



Data.Building[ "Tree" ] =
{
    passable = true,
    health = 10, -- after this amount of damage builfing destroyed
    material = { { "Wood" } },
    gather_items = { { "Wood", 5 } },
}

Data.Building[ "House" ] =
{
    passable = false, -- player and people cant pass
    health = 1000, -- after this amount of damage builfing destroyed
    living_area = true, -- means your people can live here. You cant get any more people without this.
    material = { { "Wood" }, { "Stone" } },
    gather_items = { { "Stone", 10 } },
}

Data.Building[ "CastleWall" ] =
{
    passable = false,
    health = 10000,
    material = { { "Stone" } },
    gather_items = { { "Stone", 200 } },
}