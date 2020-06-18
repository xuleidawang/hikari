//
// Created by LEI XU on 3/9/20.
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

// core/parser.cpp*
#include <memory>
#include "parser.h"
//#include "stats.h"

namespace hikari {

// Parsing Global Interface
static void parse(std::unique_ptr<Tokenizer> t) {
    std::vector<std::unique_ptr<Tokenizer>> fileStack;
    fileStack.push_back(std::move(t));
    parserLoc = &fileStack.back()->loc;

    bool ungetTokenSet = false;
    std::string ungetTokenValue;

    // nextToken is a little helper function that handles the file stack,
    // returning the next token from the current file until reaching EOF,
    // at which point it switches to the next file (if any).
    std::function<string_view(int)> nextToken;
    nextToken = [&](int flags) -> string_view {
        if (ungetTokenSet) {
            ungetTokenSet = false;
            return string_view(ungetTokenValue.data(), ungetTokenValue.size());
        }

        if (fileStack.empty()) {
            if (flags & TokenRequired) {
                Error("premature EOF");
                exit(1);
            }
            parserLoc = nullptr;
            return {};
        }

        string_view tok = fileStack.back()->Next();

        if (tok.empty()) {
            // We've reached EOF in the current file. Anything more to parse?
            fileStack.pop_back();
            if (!fileStack.empty()) parserLoc = &fileStack.back()->loc;
            return nextToken(flags);
        } else if (tok == "Include") {
            // Switch to the given file.
            std::string filename =
                toString(dequoteString(nextToken(TokenRequired)));
            filename = AbsolutePath(ResolveFilename(filename));
            auto tokError = [](const char *msg) { Error("%s", msg); };
            std::unique_ptr<Tokenizer> tinc =
                Tokenizer::CreateFromFile(filename, tokError);
            if (tinc) {
                fileStack.push_back(std::move(tinc));
                parserLoc = &fileStack.back()->loc;
            }
            return nextToken(flags);
        } else if (tok[0] == '#') {
            // Swallow comments, unless --cat or --toply was given, in
            // which case they're printed to stdout.
            if (PbrtOptions.cat || PbrtOptions.toPly)
                printf("%*s%s\n", catIndentCount, "", toString(tok).c_str());
            return nextToken(flags);
        } else
            // Regular token; success.
            return tok;
    };

    auto ungetToken = [&](string_view s) {
        CHECK(!ungetTokenSet);
        ungetTokenValue = std::string(s.data(), s.size());
        ungetTokenSet = true;
    };

    MemoryArena arena;

    // Helper function for pbrt API entrypoints that take a single string
    // parameter and a ParamSet (e.g. pbrtShape()).
    auto basicParamListEntrypoint = [&](
        SpectrumType spectrumType,
        std::function<void(const std::string &n, ParamSet p)> apiFunc) {
        string_view token = nextToken(TokenRequired);
        string_view dequoted = dequoteString(token);
        std::string n = toString(dequoted);
        ParamSet params =
            parseParams(nextToken, ungetToken, arena, spectrumType);
        apiFunc(n, std::move(params));
    };

    auto syntaxError = [&](string_view tok) {
        Error("Unexpected token: %s", toString(tok).c_str());
        exit(1);
    };

    while (true) {
        string_view tok = nextToken(TokenOptional);
        if (tok.empty()) break;

        switch (tok[0]) {
        case 'A':
            if (tok == "AttributeBegin")
                pbrtAttributeBegin();
            else if (tok == "AttributeEnd")
                pbrtAttributeEnd();
            else if (tok == "ActiveTransform") {
                string_view a = nextToken(TokenRequired);
                if (a == "All")
                    pbrtActiveTransformAll();
                else if (a == "EndTime")
                    pbrtActiveTransformEndTime();
                else if (a == "StartTime")
                    pbrtActiveTransformStartTime();
                else
                    syntaxError(tok);
            } else if (tok == "AreaLightSource")
                basicParamListEntrypoint(SpectrumType::Illuminant,
                                         pbrtAreaLightSource);
            else if (tok == "Accelerator")
                basicParamListEntrypoint(SpectrumType::Reflectance,
                                         pbrtAccelerator);
            else
                syntaxError(tok);
            break;

        case 'C':
            if (tok == "ConcatTransform") {
                if (nextToken(TokenRequired) != "[") syntaxError(tok);
                Float m[16];
                for (int i = 0; i < 16; ++i)
                    m[i] = parseNumber(nextToken(TokenRequired));
                if (nextToken(TokenRequired) != "]") syntaxError(tok);
                pbrtConcatTransform(m);
            } else if (tok == "CoordinateSystem") {
                string_view n = dequoteString(nextToken(TokenRequired));
                pbrtCoordinateSystem(toString(n));
            } else if (tok == "CoordSysTransform") {
                string_view n = dequoteString(nextToken(TokenRequired));
                pbrtCoordSysTransform(toString(n));
            } else if (tok == "Camera")
                basicParamListEntrypoint(SpectrumType::Reflectance, pbrtCamera);
            else
                syntaxError(tok);
            break;

        case 'F':
            if (tok == "Film")
                basicParamListEntrypoint(SpectrumType::Reflectance, pbrtFilm);
            else
                syntaxError(tok);
            break;

        case 'I':
            if (tok == "Integrator")
                basicParamListEntrypoint(SpectrumType::Reflectance,
                                         pbrtIntegrator);
            else if (tok == "Identity")
                pbrtIdentity();
            else
                syntaxError(tok);
            break;

        case 'L':
            if (tok == "LightSource")
                basicParamListEntrypoint(SpectrumType::Illuminant,
                                         pbrtLightSource);
            else if (tok == "LookAt") {
                Float v[9];
                for (int i = 0; i < 9; ++i)
                    v[i] = parseNumber(nextToken(TokenRequired));
                pbrtLookAt(v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7],
                           v[8]);
            } else
                syntaxError(tok);
            break;

        case 'M':
            if (tok == "MakeNamedMaterial")
                basicParamListEntrypoint(SpectrumType::Reflectance,
                                         pbrtMakeNamedMaterial);
            else if (tok == "MakeNamedMedium")
                basicParamListEntrypoint(SpectrumType::Reflectance,
                                         pbrtMakeNamedMedium);
            else if (tok == "Material")
                basicParamListEntrypoint(SpectrumType::Reflectance,
                                         pbrtMaterial);
            else if (tok == "MediumInterface") {
                string_view n = dequoteString(nextToken(TokenRequired));
                std::string names[2];
                names[0] = toString(n);

                // Check for optional second parameter
                string_view second = nextToken(TokenOptional);
                if (!second.empty()) {
                    if (isQuotedString(second))
                        names[1] = toString(dequoteString(second));
                    else {
                        ungetToken(second);
                        names[1] = names[0];
                    }
                } else
                    names[1] = names[0];

                pbrtMediumInterface(names[0], names[1]);
            } else
                syntaxError(tok);
            break;

        case 'N':
            if (tok == "NamedMaterial") {
                string_view n = dequoteString(nextToken(TokenRequired));
                pbrtNamedMaterial(toString(n));
            } else
                syntaxError(tok);
            break;

        case 'O':
            if (tok == "ObjectBegin") {
                string_view n = dequoteString(nextToken(TokenRequired));
                pbrtObjectBegin(toString(n));
            } else if (tok == "ObjectEnd")
                pbrtObjectEnd();
            else if (tok == "ObjectInstance") {
                string_view n = dequoteString(nextToken(TokenRequired));
                pbrtObjectInstance(toString(n));
            } else
                syntaxError(tok);
            break;

        case 'P':
            if (tok == "PixelFilter")
                basicParamListEntrypoint(SpectrumType::Reflectance,
                                         pbrtPixelFilter);
            else
                syntaxError(tok);
            break;

        case 'R':
            if (tok == "ReverseOrientation")
                pbrtReverseOrientation();
            else if (tok == "Rotate") {
                Float v[4];
                for (int i = 0; i < 4; ++i)
                    v[i] = parseNumber(nextToken(TokenRequired));
                pbrtRotate(v[0], v[1], v[2], v[3]);
            } else
                syntaxError(tok);
            break;

        case 'S':
            if (tok == "Shape")
                basicParamListEntrypoint(SpectrumType::Reflectance, pbrtShape);
            else if (tok == "Sampler")
                basicParamListEntrypoint(SpectrumType::Reflectance,
                                         pbrtSampler);
            else if (tok == "Scale") {
                Float v[3];
                for (int i = 0; i < 3; ++i)
                    v[i] = parseNumber(nextToken(TokenRequired));
                pbrtScale(v[0], v[1], v[2]);
            } else
                syntaxError(tok);
            break;

        case 'T':
            if (tok == "TransformBegin")
                pbrtTransformBegin();
            else if (tok == "TransformEnd")
                pbrtTransformEnd();
            else if (tok == "Transform") {
                if (nextToken(TokenRequired) != "[") syntaxError(tok);
                Float m[16];
                for (int i = 0; i < 16; ++i)
                    m[i] = parseNumber(nextToken(TokenRequired));
                if (nextToken(TokenRequired) != "]") syntaxError(tok);
                pbrtTransform(m);
            } else if (tok == "Translate") {
                Float v[3];
                for (int i = 0; i < 3; ++i)
                    v[i] = parseNumber(nextToken(TokenRequired));
                pbrtTranslate(v[0], v[1], v[2]);
            } else if (tok == "TransformTimes") {
                Float v[2];
                for (int i = 0; i < 2; ++i)
                    v[i] = parseNumber(nextToken(TokenRequired));
                pbrtTransformTimes(v[0], v[1]);
            } else if (tok == "Texture") {
                string_view n = dequoteString(nextToken(TokenRequired));
                std::string name = toString(n);
                n = dequoteString(nextToken(TokenRequired));
                std::string type = toString(n);

                basicParamListEntrypoint(
                    SpectrumType::Reflectance,
                    [&](const std::string &texName, const ParamSet &params) {
                        pbrtTexture(name, type, texName, params);
                    });
            } else
                syntaxError(tok);
            break;

        case 'W':
            if (tok == "WorldBegin")
                pbrtWorldBegin();
            else if (tok == "WorldEnd")
                pbrtWorldEnd();
            else
                syntaxError(tok);
            break;

        default:
            syntaxError(tok);
        }
    }
}


void parsePBRT(std::string filename) {
    if (filename != "-") SetSearchDirectory(DirectoryContaining(filename));

    std::unique_ptr<Tokenizer> t = Tokenizer::CreateFromFile(filename);
    if (!t) return;
    parse(std::move(t));
}
    
}  // namespace hikari
