#ifndef PLY_hpp
#define PLY_hpp

#include "Tools.hpp"
#include "Render.hpp"

Render::DrawElementArray *ReadPLY(const char *fileName, const bool &reverseWinding=false, const bool &flipNormals=false);

#endif /* PLY_hpp */
