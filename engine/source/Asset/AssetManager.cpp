bool AssetManager::load(const std::string path) {
	if path in "assets/" {
		with open path as file {
			// rapid xml http://rapidxml.sourceforge.net/
			for each node {
				for each attr {
					if asset type is audio {
						load audio asset type
					}
				}
			}
		}
	}

	return FAILURE; // path does not exist...
}