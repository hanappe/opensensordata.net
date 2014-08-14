#include "photostream.h"

#include "db.h"



// Photostream

Photostream::Photostream() :
        id(0), 
 	ownerId(0) {
        memset(name, 0, sizeof(name));
        memset(description, 0, sizeof(description));
}
	
Photostream::~Photostream() {
        //clear();
}
	

bool Photostream::load(int id) {
        clear();
        return DB::GetInstance()->loadPhotostream(this, id);  
}


void Photostream::print() const {
        std::cout << "photoinfos size: " << photoinfos.size()
                  << std::endl;
}

void Photostream::clear() {
        id = 0;
        ownerId = 0;
        memset(name, 0, sizeof(name));
        memset(description, 0, sizeof(description));
}

void Photostream::append(const PhotoInfo& photo) {
        photoinfos.push_back(photo);
}

// Photostream::JS

Handle<ObjectTemplate> Photostream::JS::GetNewTemplate(Isolate * i) {

        Handle<ObjectTemplate> ps_template = ObjectTemplate::New();
        ps_template->SetInternalFieldCount(1);
        
        ps_template->Set(String::NewFromUtf8(i, "load"), FunctionTemplate::New(i, Load));
        return ps_template;
}
/*
Local<Object> Photostream::JS::GetNewInstance(Isolate * i) {
        
        Local<Object> obj = ps_template->NewInstance();
        return obj;
}
*/
void Photostream::JS::SetupObject(Local<Object> obj, const Photostream * ps, Isolate* i) {

        if (!ps) {
                fprintf(stderr, "Photostream::JS::SetupObject error, Photostream is null");
                return;
        }

        obj->Set(String::NewFromUtf8(i, "id"), Number::New(i, ps->id));
        obj->Set(String::NewFromUtf8(i, "ownerId"), Number::New(i, ps->ownerId));
        obj->Set(String::NewFromUtf8(i, "name"), String::NewFromUtf8(i, ps->name));

        //Handle<Array> array = d->toV8Array(i);
        //obj->Set(String::NewFromUtf8(i, "datapoints"), array);

        // Alias
        const std::vector<PhotoInfo>& photoinfos = ps->photoinfos;

        Handle<Array> photoinfos_array = Array::New(i, photoinfos.size());

        //std::cout << "Photostream::JS::SetupObject photosinfos: " << photoinfos.size() << std::endl;
        
        int photoinfos_count = 0;
        for (std::vector<PhotoInfo>::const_iterator it = photoinfos.begin(); it != photoinfos.end(); ++it, photoinfos_count++) {                
                const PhotoInfo& pi = (*it);    
                
                Handle<Object> obj = Object::New(i);
                
                obj->Set(String::NewFromUtf8(i, "datetime"), Date::New(i, pi.datetime));
                obj->Set(String::NewFromUtf8(i, "filename"), String::NewFromUtf8(i, pi.filename));
                
                photoinfos_array->Set(photoinfos_count, obj);
        }
        
        //std::cout << "toV8Array datapoints count: " << datapoints_count << " " << datapoints.size() << std::endl;
        obj->Set(String::NewFromUtf8(i, "photoinfos"), photoinfos_array);
}

void Photostream::JS::Register(Handle<ObjectTemplate> global, Isolate* i) {
        global->Set(String::NewFromUtf8(i, "Photostream"), FunctionTemplate::New(i, Constructor));
}


void Photostream::JS::Constructor(const FunctionCallbackInfo<Value>& info) {
        v8::Isolate* i = v8::Isolate::GetCurrent();

        const Handle<Value>& arg1 = info[0]; 
        
        const int photostream_id = !arg1.IsEmpty() && arg1->IsNumber() ? arg1->Int32Value() : 0;  
        Photostream * photostream = new Photostream();
        
        if (photostream) {

                Handle<ObjectTemplate> ps_template = GetNewTemplate(i);
                Local<Object> obj = ps_template->NewInstance();

                obj->SetInternalField(0, External::New(i, photostream));
                // Create and Return this newly created object
                info.GetReturnValue().Set(obj);
                
                //datastream->print();

        } else {
                fprintf(stderr, "datastream not found with id %d \n", photostream_id);
        }
}

void Photostream::JS::Load(const FunctionCallbackInfo<Value>& info ) {

        Isolate * isolate = info.GetIsolate();
        
        Local<Object> self = info.Holder();
        Local<External> wrap = Local<External>::Cast(self->GetInternalField(0));
        void* ptr = wrap->Value();
        
        Photostream * photostream = static_cast<Photostream*>(ptr);
        
        const Handle<Value>& arg1 = info[0]; 
        
        //const int photostream_id = !arg1.IsEmpty() && arg1->IsNumber() ? arg1->Int32Value() : 0;
        const int photostream_id = !arg1.IsEmpty() ? arg1->Int32Value() : 0;

        //std::cout << "Photostream Load: !arg1.IsEmpty() " << !arg1.IsEmpty() << std::endl;
        //std::cout << "Photostream Load: arg1->IsNumber() " << arg1->IsNumber() << std::endl;
        //std::cout << "Photostream Load: photostream_id: " << photostream_id << std::endl;
        //std::cout << "Photostream Load: photostream: " << photostream << std::endl;
        //std::cout << "Photostream->load(photostream_id): " << photostream->load(photostream_id) << std::endl;
        //std::cout << "Photostream->loadContiguous(photostream_id): " << photostream->loadContiguous(photostream_id) << std::endl;

        photostream->load(photostream_id);


        //Fill this "Photostream" javascript object With properties

        SetupObject(info.This(), photostream, isolate);
}

std::vector<Photostream*> Photostream::JS::references;

void Photostream::JS::AddToRef(Photostream * photostream) {
        references.push_back(photostream);
}

void Photostream::JS::DeleteAllRef() {
        
        for (std::vector<Photostream*>::iterator it = references.begin(); it != references.end(); ++it) {
                delete (*it);
        }

        references.clear();        
}

