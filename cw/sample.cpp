#include "sample.hpp"

std::ostream& operator<<(std::ostream& os, const WaterSample& sample) {
    return << "ID: " << sample.getId()
        << "\nSampling Point: " << sample.getSamplingPoint()
        << "\nSampling DateTime: " << sample.getSamplingDateTime()
        << 
}
