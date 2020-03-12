#include "util/io.h"

namespace vgatext{

   static inline void writechar(int loc, uint8_t c, uint8_t bg, uint8_t fg, addr_t base){
     uint16_t info = ((fg << 4)|bg);                    // merge background and foreground info
     mmio::write16(base,loc*2,(info<<8)|(uint16_t)c);   //((fg)(bg))(ASCII)
   }

}//namespace vgatext
