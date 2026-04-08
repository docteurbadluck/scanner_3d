#include "../Camera_Driver.hpp"

bool        Camera_Driver::hasPhoto() const { return _hasPhoto; }
std::string Camera_Driver::getData()  const { return _data; }
