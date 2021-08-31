//
// Created by aleksastevic on 8/30/21.
//

#ifndef MATF_RG_PROJEKAT_COMMON_HPP
#define MATF_RG_PROJEKAT_COMMON_HPP

#include <iostream>

#define LOG(stream) stream << "[" << __FILE__ << ", " << __func__ << ", " << __LINE__ << "] "
#define BREAK_IF_FALSE(x) if (!(x)) __builtin_trap()
#define ASSERT(x, msg) do { if (!(x)) { std::cerr << msg << '\n'; BREAK_IF_FALSE(false); } } while(0)
#define GLCALL(x) \
do{ rg::clearAllOpenGlErrors(); x; BREAK_IF_FALSE(rg::wasPreviousOpenGLCallSuccessful(__FILE__, __LINE__, #x)); } while (0)

namespace rg {
    void clearAllOpenGlErrors();

    const char *openGLErrorToString(GLenum error);

    bool wasPreviousOpenGLCallSuccessful(const char *file, int line, const char *call);
}

#endif //MATF_RG_PROJEKAT_COMMON_HPP
