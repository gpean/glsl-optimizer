#pragma once

#include "compiler.h"

#include <nan.h>

#include <glsl_optimizer.h>

class Shader : public Nan::ObjectWrap 
{
public:
	static void Init(v8::Local<v8::Object> exports);

	inline bool isCompiled() const { return _compiled; }
	const char* getOutput() const;
	const char* getRawOutput() const;
	const char* getLog() const;

	void release();

private:
	Shader(Compiler* compiler, int type, const char* source);
	~Shader();

	glslopt_shader* _binding;
	bool _compiled;

	static NAN_METHOD(New);
	static NAN_METHOD(Dispose);
	static NAN_METHOD(Compiled);
	static NAN_METHOD(Output);
	static NAN_METHOD(RawOutput);
	static NAN_METHOD(Log);

	static Nan::Persistent<v8::Function> constructor;
};
