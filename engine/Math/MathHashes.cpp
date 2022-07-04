#include "EngineMath.hpp"

namespace wfe {
    // Hashing functions
    template<>
    void GetHashCode(Vector2 vec, size_t& hash) {
        GetHashCode(vec.x, hash);
        CombineHash(hash, vec.y);
    }
    template<>
    void GetHashCode(Vector3 vec, size_t& hash) {
        GetHashCode(vec.x, hash);
        CombineHash(hash, vec.y);
        CombineHash(hash, vec.z);
    }
    template<>
    void GetHashCode(Vector4 vec, size_t& hash) {
        GetHashCode(vec.x, hash);
        CombineHash(hash, vec.y);
        CombineHash(hash, vec.z);
        CombineHash(hash, vec.w);
    }
    template<>
    void GetHashCode(Quaternion quat, size_t& hash) {
        GetHashCode(quat.x, hash);
        CombineHash(hash, quat.y);
        CombineHash(hash, quat.z);
        CombineHash(hash, quat.w);
    }
    template<>
    void GetHashCode(Matrix4x4 mat, size_t& hash) {
        GetHashCode(mat.data[0], hash);

        for(size_t i = 1; i < 16; i++)
            CombineHash(hash, mat.data[i]);
    }
}