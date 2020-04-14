//
// Created by LEI XU on 3/7/20.
//

#include "sampler.h"
#include "Camera.h"

namespace hikari {
    Sampler::~Sampler() {}

    Sampler::Sampler(int samplesPerPixel): samplesPerPixel(samplesPerPixel){}
//    CameraSample Sampler::GetCameraSample(const Point2i &pRaster) {
//        CameraSample cs;
//        cs.pFilm = (Point2f)pRaster + Get2D();
//        cs.time = Get1D();
//        cs.pLens = Get2D();
//        return cs;
//    }
    void Sampler::StartPixel(const Vector2 &p) {
        currentPixel = p;
        currentPixelSampleIndex = 0;
        // Reset array offsets for next pixel sample
        array1DOffset = array2DOffset = 0;
    }

    bool Sampler::StartNextSample() {
        // Reset array offsets for next pixel sample
        array1DOffset = array2DOffset = 0;
        return ++currentPixelSampleIndex < samplesPerPixel;
    }

    bool Sampler::SetSampleNumber(int64_t sampleNum) {
        // Reset array offsets for next pixel sample
        array1DOffset = array2DOffset = 0;
        currentPixelSampleIndex = sampleNum;
        return currentPixelSampleIndex < samplesPerPixel;
    }

    void Sampler::Request1DArray(int n) {
//        CHECK_EQ(RoundCount(n), n);
        samples1DArraySizes.push_back(n);
        sampleArray1D.push_back(std::vector<float >(n * samplesPerPixel));
    }

    void Sampler::Request2DArray(int n) {
//        CHECK_EQ(RoundCount(n), n);
        samples2DArraySizes.push_back(n);
        sampleArray2D.push_back(std::vector<Vector2>(n * samplesPerPixel));
    }

    const float *Sampler::Get1DArray(int n) {
        if (array1DOffset == sampleArray1D.size()) return nullptr;
//        CHECK_EQ(samples1DArraySizes[array1DOffset], n);
//        CHECK_LT(currentPixelSampleIndex, samplesPerPixel);
        return &sampleArray1D[array1DOffset++][currentPixelSampleIndex * n];
    }

    const Vector2 *Sampler::Get2DArray(int n) {
        if (array2DOffset == sampleArray2D.size()) return nullptr;
//        CHECK_EQ(samples2DArraySizes[array2DOffset], n);
//        CHECK_LT(currentPixelSampleIndex, samplesPerPixel);
        return &sampleArray2D[array2DOffset++][currentPixelSampleIndex * n];
    }

    PixelSampler::PixelSampler(int64_t samplesPerPixel, int nSampledDimensions)
            : Sampler(samplesPerPixel) {
        for (int i = 0; i < nSampledDimensions; ++i) {
            samples1D.push_back(std::vector<float>(samplesPerPixel));
            samples2D.push_back(std::vector<Vector2>(samplesPerPixel));
        }
    }

    bool PixelSampler::StartNextSample() {
        current1DDimension = current2DDimension = 0;
        return Sampler::StartNextSample();
    }

    bool PixelSampler::SetSampleNumber(int64_t sampleNum) {
        current1DDimension = current2DDimension = 0;
        return Sampler::SetSampleNumber(sampleNum);
    }

    float PixelSampler::Get1D() {
//        ProfilePhase _(Prof::GetSample);
//        CHECK_LT(currentPixelSampleIndex, samplesPerPixel);
        if (current1DDimension < samples1D.size())
            return samples1D[current1DDimension++][currentPixelSampleIndex];
        else
//            return rng.UniformFloat();
            return drand48();
    }

    Vector2 PixelSampler::Get2D() {
//        ProfilePhase _(Prof::GetSample);
//        CHECK_LT(currentPixelSampleIndex, samplesPerPixel);
        if (current2DDimension < samples2D.size())
            return samples2D[current2DDimension++][currentPixelSampleIndex];
        else
//            return Point2f(rng.UniformFloat(), rng.UniformFloat());
            return Vector2(drand48(), drand48());
    }
}