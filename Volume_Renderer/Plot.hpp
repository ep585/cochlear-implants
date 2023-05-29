#ifndef Plot_hpp
#define Plot_hpp

#include "Header.hpp"

namespace Plot {

void InitMemry();
void UpdateMemry(const float &newVal);
void DrawMemry();

const float &Latest();

}

#endif /* Plot_hpp */
