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
#include "parser.h"
//#include "stats.h"

namespace hikari {
  // Parsing Global Interface
  void loadScene(Scene *scene)
  {
    scene->add( dynamic_cast<Shape*>(new Sphere(1000,Vector3(0,0,-1000), Vector3(), Vector3(1.0,1.0,1.0),new MatteMaterial(Vector3(1.0, 0.0, 0.0)))));
    scene->add( dynamic_cast<Shape*>(new Sphere(1000,Vector3(-1004,0,0), Vector3(), Vector3(0.85,0.4,0.4),DIFF)));
    scene->add( dynamic_cast<Shape*>(new Sphere(1000,Vector3(1004,0,0),  Vector3(), Vector3(0.4,0.4,0.85),DIFF)));
    scene->add( dynamic_cast<Shape*>(new Sphere(1000,Vector3(0,1006,0),  Vector3(), Vector3(1.0,1.0,1.0),DIFF)));
    scene->add( dynamic_cast<Shape*>(new Sphere(1000,Vector3(0,1006,0),  Vector3(), Vector3(1.0,1.0,1.0),DIFF)));
    scene->add( dynamic_cast<Shape*>(new Sphere(100, Vector3(0,0,110),   Vector3(1,1,1)*.999,Vector3(1.0,1.0,1.0)*.999,DIFF))); //light
    scene->addMesh( new GeometricPrimitive(new Mesh(Vector3(0,0,2.5), "../scene/dragon2.scene"), new MatteMaterial(Vector3(0.99, 0.84, 0))));
  }

    
}  // namespace hikari
