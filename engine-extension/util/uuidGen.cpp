#define UUID_TIME_GENERATOR
#ifdef _WIN32
#include "windows.h"
#endif
#include "uuidGen.h"
#undef UUID_TIME_GENERATOR

namespace XD::UUID
{
    uuids::uuid gen()
    {
        static std::unique_ptr<uuids::uuid_time_generator> generator = nullptr;
        if (!generator)
        {
            // std::random_device rd;
            // auto seed_data = std::array<int, std::mt19937::state_size> {};
            // std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
            // std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
            // std::mt19937 engine(seq);
            // generator = std::make_unique<uuids::uuid_random_generator>(&engine);
            generator = std::make_unique<uuids::uuid_time_generator>(/*&engine*/);
        }
        return generator->operator()();
    }
} // namespace XD