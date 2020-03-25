//
// Created by LEI XU on 3/20/20.
//

#ifndef PATHTRACER_API_H
#define PATHTRACER_API_H

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


// core/api.h*
#include "hikari.h"

namespace hikari {

// API Function Declarations
    void pbrtInit();
    void pbrtCleanup();
    void pbrtIdentity();
    void pbrtTranslate(float dx, float dy, float dz);
    void pbrtRotate(float angle, float ax, float ay, float az);
    void pbrtScale(float sx, float sy, float sz);
    void pbrtLookAt(float ex, float ey, float ez, float lx, float ly, float lz,
                    float ux, float uy, float uz);
    void pbrtConcatTransform(float transform[16]);
    void pbrtTransform(float transform[16]);
    void pbrtCoordinateSystem(const std::string &);
    void pbrtCoordSysTransform(const std::string &);
    void pbrtActiveTransformAll();
    void pbrtActiveTransformEndTime();
    void pbrtActiveTransformStartTime();
    void pbrtTransformTimes(float start, float end);
    void pbrtPixelFilter(const std::string &name, const ParamSet &params);
    void pbrtFilm(const std::string &type, const ParamSet &params);
    void pbrtSampler(const std::string &name, const ParamSet &params);
    void pbrtAccelerator(const std::string &name, const ParamSet &params);
    void pbrtIntegrator(const std::string &name, const ParamSet &params);
    void pbrtCamera(const std::string &, const ParamSet &cameraParams);
    void pbrtMakeNamedMedium(const std::string &name, const ParamSet &params);
    void pbrtMediumInterface(const std::string &insideName,
                             const std::string &outsideName);
    void pbrtWorldBegin();
    void pbrtAttributeBegin();
    void pbrtAttributeEnd();
    void pbrtTransformBegin();
    void pbrtTransformEnd();
    void pbrtTexture(const std::string &name, const std::string &type,
                     const std::string &texname, const ParamSet &params);
    void pbrtMaterial(const std::string &name, const ParamSet &params);
    void pbrtMakeNamedMaterial(const std::string &name, const ParamSet &params);
    void pbrtNamedMaterial(const std::string &name);
    void pbrtLightSource(const std::string &name, const ParamSet &params);
    void pbrtAreaLightSource(const std::string &name, const ParamSet &params);
    void pbrtShape(const std::string &name, const ParamSet &params);
    void pbrtReverseOrientation();
    void pbrtObjectBegin(const std::string &name);
    void pbrtObjectEnd();
    void pbrtObjectInstance(const std::string &name);
    void pbrtWorldEnd();

    void pbrtParseFile(std::string filename);
    void pbrtParseString(std::string str);

}  // namespace pbrt
#endif //PATHTRACER_API_H
