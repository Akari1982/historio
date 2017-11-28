Data.Item = {}



Data.Item[ "Chopper" ] =
{
    stack = 10, -- this is size of stack in inventory
    damage = 1, -- if you hit something this is damage
    durability = 10, -- one hit reduce one durability. After it reaches 0 it destroyed.
    material = { { "Stone" }, },
}

Data.Item[ "StoneAxe" ] =
{
    stack = 10,
    damage = 5,
    durability = 50,
    material = { { "Stone" }, },
}
