#ifndef SERIAL_H_
#define SERIAL_H_

// Toggle between Serial debugging and Xbee communcations

#ifdef SERIAL_DEBUG
  #define Debug Serial
  #define XBee  if(0) Serial
#else
  #define Debug if(0) Serial
  #define XBee  Serial
#endif

#endif
