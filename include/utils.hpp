#pragma once

#include <string>

namespace efesx::queue {

/**
 * @brief Trasform raw data pointer to desired type pointer
*/
template<typename T>
const T* queue_raw_data_cast(const std::string& rd){
    return reinterpret_cast<const T*>(rd.c_str());
}


}; // namespace efesx::queue
