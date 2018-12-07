#include "../includes/topo.hpp"

int main(int argc, char ** argv) {
	Topo topo;

	topo = new Topo();
	if (topo._Start() == FAILURE)
		return (EXIT_FAILURE);
	if (topo._Run() == FAILURE)
		return (EXIT_FAILURE);
	if (topo._Destroy() == FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}