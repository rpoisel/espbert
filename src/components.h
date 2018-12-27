#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <cstddef>
#include <cstdint>

struct StreamFrame
{
  uint8_t buf[64];
  size_t numBytes;
};

#endif /* COMPONENTS_H */
