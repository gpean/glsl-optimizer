#pragma once

#include <nan.h>

#include <glsl_optimizer.h>

class Compiler : public Nan::ObjectWrap
{
public:
	static void Init(v8::Local<v8::Object> exports);

	inline glslopt_ctx* getBinding() const { return _binding; }

	void release();

private:
	Compiler(glslopt_target target);
	~Compiler();

	glslopt_ctx* _binding;

	static NAN_METHOD(New);
	static NAN_METHOD(Dispose);

	static Nan::Persistent<v8::Function> constructor;
};
