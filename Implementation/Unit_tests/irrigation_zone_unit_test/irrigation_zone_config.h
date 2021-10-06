/**
 * Irrigation Zone configuration file
*/
#ifndef IRRIGATION_ZONE_CONFIG
#define IRRIGATION_ZONE_CONFIG
#include <cstdint>

constexpr uint32_t HEALTHY_OFFSET = 1000; /*> Sets the max difference between measure on pump sensor and electrovalve sensor
                                           *  to consider irrigation zone healthy (only applies to analog sensors)
                                           */

#endif //IRRIGATION_ZONE_CONFIG