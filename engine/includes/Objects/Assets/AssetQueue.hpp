// AssetQueue.cpp
#ifndef ASSETQUEUE_H
#define ASSETQUEUE_H

const std::string MODEL_PATH = "/home/samurai/dark-engine/engine/source/render/models/chalet.obj";
const std::string TEXTURE_PATH = "/home/samurai/dark-engine/engine/source/render/textures/chalet.jpg";

#ifndef TEST_VERT_FILE
#define TEST_VERT_FILE "/home/samurai/dark-engine/engine/source/render/shaders/vert.spv"
#endif

#ifndef TEST_FRAG_FILE
#define TEST_FRAG_FILE "/home/samurai/dark-engine/engine/source/render/shaders/frag.spv"
#endif

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

}

#endif // ASSETQUEUE_H