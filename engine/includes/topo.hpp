#ifndef TOPO_HPP
#define TOPO_HPP

#include <cstring>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <stack>
#include <vector>
#include <deque>
#include <functional>

#include "./defined.hpp"
#include "./engine/Environ.hpp"
#include "./utils/utils.hpp"

#include "./threads/JobQueue.hpp"
JobQueue                *g_job_q;

#endif