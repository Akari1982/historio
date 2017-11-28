Data.Material = {}



Data.Material[ "Flesh" ] =
{
    damage_material =
    {
        { "Flesh", 1, },
        { "Wood", 0, },
        { "Stone", 0, },
        { "Iron", 0, },
    },
}

Data.Material[ "Wood" ] =
{
    damage_material =
    {
        { "Flesh", 2, },
        { "Wood", 0.25, },
        { "Stone", 0.1, },
        { "Iron", 0, },
    },
}

Data.Material[ "Stone" ] =
{
    damage_material =
    {
        { "Flesh", 10, },
        { "Wood", 2, },
        { "Stone", 1, },
        { "Iron", 0.5, },
    },
}

Data.Material[ "Iron" ] =
{
    damage_material =
    {
        { "Flesh", 100, },
        { "Wood", 10, },
        { "Stone", 0.5, },
        { "Iron", 0.25, },
    },
}
