#ifndef OBJ_LOADER_HPP
#define OBJ_LOADER_HPP

#include <Vector3.hpp>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<float> loadObjFile(const std::string& path);
#endif