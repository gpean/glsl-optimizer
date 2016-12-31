#include <node.h>
#include <nan.h>
#include "shader.h"
#include <glsl_optimizer.h>

using namespace v8;

void InitAll(Handle<Object> exports)
{
	// Export constants
    Nan::Set(exports, Nan::New<String>("VERTEX_SHADER").ToLocalChecked(), Nan::New<Integer>(kGlslOptShaderVertex));
    Nan::Set(exports, Nan::New<String>("FRAGMENT_SHADER").ToLocalChecked(), Nan::New<Integer>(kGlslOptShaderFragment));
    Nan::Set(exports, Nan::New<String>("TARGET_OPENGL").ToLocalChecked(), Nan::New<Integer>(kGlslTargetOpenGL));
    Nan::Set(exports, Nan::New<String>("TARGET_OPENGLES20").ToLocalChecked(), Nan::New<Integer>(kGlslTargetOpenGLES20));
    Nan::Set(exports, Nan::New<String>("TARGET_OPENGLES30").ToLocalChecked(), Nan::New<Integer>(kGlslTargetOpenGLES30));
	
	// Export classes
	Compiler::Init(exports);
	Shader::Init(exports);
}

NODE_MODULE(glslOptimizer, InitAll);
