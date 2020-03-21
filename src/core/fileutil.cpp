//
// Created by LEI XU on 3/20/20.
//

/*
    pbrt source code is Copyright(c) 1998-2016
                        Matt Pharr, Greg Humphreys, and Wenzel Jakob.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#include "fileutil.h"
#include <cstdlib>
#include <climits>
#include <libgen.h>

namespace hikari {

    static std::string searchDirectory;

    bool IsAbsolutePath(const std::string &filename) {
        return (filename.size() > 0) && filename[0] == '/';
    }

    std::string AbsolutePath(const std::string &filename) {
        char full[PATH_MAX];
        if (realpath(filename.c_str(), full))
            return std::string(full);
        else
            return filename;
    }

    std::string ResolveFilename(const std::string &filename) {
        if (searchDirectory.empty() || filename.empty())
            return filename;
        else if (IsAbsolutePath(filename))
            return filename;
        else if (searchDirectory[searchDirectory.size() - 1] == '/')
            return searchDirectory + filename;
        else
            return searchDirectory + "/" + filename;
    }

    std::string DirectoryContaining(const std::string &filename) {
        char *t = strdup(filename.c_str());
        std::string result = dirname(t);
        free(t);
        return result;
    }
    void SetSearchDirectory(const std::string &dirname) {
        searchDirectory = dirname;
    }
}