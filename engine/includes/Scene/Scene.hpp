class Scene {
public:
	/*
	When the scene is triggered to load/prepped for
	transition, it will traverse its configuration
	file to collect pointers to each asset file.

	Each Asset should submit itself to an init process
	that returns a pointer to the scene, asset list

	debate whether or not to hand this to a threadpool...

	consider, on asset load, loading that asset once
	for the first time and then signaling to all scenes
	with a pointer to that asset, that it had in fact
	been loaded and they do not need to reload it
	*/

private:
	// Assets *assets
}