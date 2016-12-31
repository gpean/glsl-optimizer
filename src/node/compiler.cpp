#include "compiler.h"

Nan::Persistent<v8::Function> Compiler::constructor;

//----------------------------------------------------------------------

Compiler::Compiler(glslopt_target target)
{
	_binding = glslopt_initialize(target);
}

//----------------------------------------------------------------------

Compiler::~Compiler()
{
	release();
}

//----------------------------------------------------------------------

void Compiler::release()
{
	if (_binding)
	{
		glslopt_cleanup(_binding);

		_binding = 0;
	}
}

//----------------------------------------------------------------------

void Compiler::Init(v8::Local<v8::Object> exports) 
{
	using namespace v8;
	Nan::HandleScope scope;

    // Prepare constructor template
	Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
	tpl->SetClassName(Nan::New("Compiler").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype
	Nan::SetPrototypeMethod(tpl, "dispose", Dispose);

	// Export the class
    constructor.Reset(tpl->GetFunction());
	exports->Set(Nan::New("Compiler").ToLocalChecked(), tpl->GetFunction());
}

//----------------------------------------------------------------------

void Compiler::New(const Nan::FunctionCallbackInfo<v8::Value>& info)
{
    using namespace v8;
	Nan::HandleScope scope;
    if (info.IsConstructCall()) { // Invoked as constructor: `new Compiler(...)`
        glslopt_target target = kGlslTargetOpenGL;
		if (info[0]->IsInt32()) 
			target = (glslopt_target)info[0]->Int32Value();
		else if (info[0]->IsBoolean())
			target = (glslopt_target)( (int)info[0]->BooleanValue() );
        auto obj = new Compiler(target);
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else { // Invoked as plain function `Compiler(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { info[0] };
        v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
        info.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
}

//----------------------------------------------------------------------

NAN_METHOD(Compiler::Dispose)
{
    using namespace v8;
    Nan::HandleScope scope;
    auto obj = ObjectWrap::Unwrap<Compiler>(info.Holder());
    obj->release();
}
