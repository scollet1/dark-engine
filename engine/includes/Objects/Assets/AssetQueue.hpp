// AssetQueue.cpp
#ifndef ASSETQUEUE_H
#define ASSETQUEUE_H

class AssetQueue { // extends Queue?
/*
each scene will have a scene file with
dependent assets

on scene load, pull assets

should scenes be partitioned by user or
by engine?

should users be incentivised to optimize
asset loading or do we do the heavy lifting?

*/

public:
	/*
	load asset breakdown : file paths
	for i in assets:
		...
	*/

private:
	std::vector<Vertex> vertices;
}

#endif // ASSETQUEUE_H