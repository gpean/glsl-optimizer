#include "shader.h"

#include <nan.h>

#include <iostream>

Nan::Persistent<v8::Function> Shader::constructor;

//----------------------------------------------------------------------

Shader::Shader(Compiler* compiler, int type, const char* source)
{
    //std::cerr << "compiler:" << compiler << std::endl;
    //std::cerr << "type:" << type << std::endl;
    //std::cerr << "source:" << source << std::endl;
	if (compiler)
	{
		_binding = glslopt_optimize(compiler->getBinding(), (glslopt_shader_type)type, source, 0);
		_compiled = glslopt_get_status(_binding);
	}
	else
	{
		_binding = 0;
		_compiled = false;
	}
}

//----------------------------------------------------------------------

Shader::~Shader()
{
	release();
}

//----------------------------------------------------------------------

void Shader::release()
{
	if (_binding)
	{
		glslopt_shader_delete(_binding);
		_binding = 0;
		_compiled = false;
	}
}

//----------------------------------------------------------------------

const char* Shader::getOutput() const
{
	return (_compiled) ? glslopt_get_output(_binding) : "";
}

//----------------------------------------------------------------------

const char* Shader::getRawOutput() const
{
	return (_compiled) ? glslopt_get_raw_output(_binding) : "";
}

//----------------------------------------------------------------------

const char* Shader::getLog() const
{
	auto s = glslopt_get_log(_binding);
    if (!s) return "";
    return s;
}

//----------------------------------------------------------------------

void Shader::Init(v8::Local<v8::Object> exports) 
{
    using namespace v8;
	Nan::HandleScope scope;

	// Prepare constructor template
	Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
	tpl->SetClassName(Nan::New("Shader").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	Nan::SetPrototypeMethod(tpl, "dispose", Dispose);
	Nan::SetPrototypeMethod(tpl, "compiled", Compiled);
	Nan::SetPrototypeMethod(tpl, "output", Output);
	Nan::SetPrototypeMethod(tpl, "rawOutput", RawOutput);
	Nan::SetPrototypeMethod(tpl, "log", Log);

	// Export the class
    constructor.Reset(tpl->GetFunction());
	exports->Set(Nan::New("Shader").ToLocalChecked(), tpl->GetFunction());
}

//----------------------------------------------------------------------

void Shader::New(const Nan::FunctionCallbackInfo<v8::Value>& info) 
{
    using namespace v8;
	Nan::HandleScope scope;
    if (info.IsConstructCall()) { // Invoked as constructor: `new Shader(...)`
        if (info.Length() != 3) {
            Nan::ThrowError("Invalid arguments");
            return;
        }
        if (!info[0]->IsObject()) {
            Nan::ThrowError("Invalid arguments");
            return;
        }
        if (!info[1]->IsInt32()) {
            Nan::ThrowError("Invalid arguments");
            return;
        }
        if (!info[2]->IsString()) {
            Nan::ThrowError("Invalid arguments");
            return;
        }
        auto obj = new Shader(ObjectWrap::Unwrap<Compiler>(info[0]->ToObject()), 
            info[1]->Int32Value(), *Nan::Utf8String(info[2]->ToString()));
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else { // Invoked as plain function `Shader(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { info[0] };
        v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
        info.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
}

//----------------------------------------------------------------------

NAN_METHOD(Shader::Dispose) 
{
    using namespace v8;
    Nan::HandleScope scope;
    auto obj = ObjectWrap::Unwrap<Shader>(info.Holder());
    obj->release();
}

//----------------------------------------------------------------------

NAN_METHOD(Shader::Compiled) 
{
    using namespace v8;
    Nan::HandleScope scope;
    auto obj = ObjectWrap::Unwrap<Shader>(info.Holder());
    info.GetReturnValue().Set(Nan::New(obj->isCompiled()));
}

//----------------------------------------------------------------------

NAN_METHOD(Shader::Output) 
{
    using namespace v8;
    Nan::HandleScope scope;
    auto obj = ObjectWrap::Unwrap<Shader>(info.Holder());
    info.GetReturnValue().Set(Nan::New(obj->getOutput()).ToLocalChecked());
}

//----------------------------------------------------------------------

NAN_METHOD(Shader::RawOutput) 
{
    using namespace v8;
    Nan::HandleScope scope;
    auto obj = ObjectWrap::Unwrap<Shader>(info.Holder());
    info.GetReturnValue().Set(Nan::New(obj->getRawOutput()).ToLocalChecked());
}

//----------------------------------------------------------------------

NAN_METHOD(Shader::Log) 
{
    using namespace v8;
    Nan::HandleScope scope;
    auto obj = ObjectWrap::Unwrap<Shader>(info.Holder());
    info.GetReturnValue().Set(Nan::New(obj->getLog()).ToLocalChecked());
}

/*
wrapMethod(fn)
{
    std::function<void(Nan::FunctionCallbackInfo<v8::Value> const*)> wfn = []() {
        Nan::HandleScope scope;
        auto obj = ObjectWrap::Unwrap<Shader>(info.Holder());
        info.GetReturnValue().Set(Nan::New((*obj).*fn()).ToLocalChecked());
    };
}
*/