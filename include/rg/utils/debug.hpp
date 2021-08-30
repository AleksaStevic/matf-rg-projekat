//
// Created by aleksastevic on 8/30/21.
//

#ifndef MATF_RG_PROJEKAT_COMMON_HPP
#define MATF_RG_PROJEKAT_COMMON_HPP

#define ASSERT(x) do { if (!(x)) __builtin_trap(); } while(0)
#define GLCALL(x) { rg::glClearError(); x; ASSERT(rg::glLogCall(__FILE__, __LINE__, #x)); }

namespace rg {
    void glClearError();

    const char *glErrorToString(GLenum error);

    bool glLogCall(const char *file, int line, const char *call);
}

#endif //MATF_RG_PROJEKAT_COMMON_HPP
