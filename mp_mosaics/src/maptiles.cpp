/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>

#include "maptiles.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    MosaicCanvas* mosaic = new MosaicCanvas(theSource.getRows(), theSource.getColumns());

    map<Point<3>, TileImage*> tiles_map;
    vector<Point<3>> points;

    for (size_t i = 0; i < theTiles.size(); i++) {
        Point<3> point = convertToXYZ(theTiles[i].getAverageColor());
        points.push_back(point);
        tiles_map.insert(pair<Point<3>, TileImage*>(point, new TileImage(theTiles[i])));
    }

    KDTree<3>* kd_tree = new KDTree<3>(points);
    for (int y = 0; y < theSource.getRows(); y++) {
        for (int x = 0; x < theSource.getColumns(); x++) {
            LUVAPixel region_color = theSource.getRegionColor(y, x);
            Point<3> nearest = kd_tree->findNearestNeighbor(convertToXYZ(region_color));
            mosaic->setTile(y, x, tiles_map[nearest]);
        }
    }
    
    delete kd_tree;
    return mosaic;
}

