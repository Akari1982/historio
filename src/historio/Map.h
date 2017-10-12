#ifndef MAP_H
#define MAP_H

#include <OgreVector3.h>
#include <vector>



struct WalkmeshTriangle
{
    WalkmeshTriangle():
        a( 0, 0, 0 ),
        b( 0, 0, 0 ),
        c( 0, 0, 0 ),
        locked( false )
    {
        access_side[ 0 ] = -1;
        access_side[ 1 ] = -1;
        access_side[ 2 ] = -1;
    }

    Ogre::Vector3 a;
    Ogre::Vector3 b;
    Ogre::Vector3 c;

    int           access_side[ 3 ];
    bool          locked;
};



struct MapBlock
{
    bool pass[ 1000 ][ 1000 ];
};



class Map
{
public:
    Map();
    virtual ~Map();

    void DrawDebug();

private:
    MapBlock block;
};



#endif // WALKMESH_H
